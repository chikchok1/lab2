#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <fcntl.h>

#define BUF_SIZE 8192

void error(const char *msg) {
    perror(msg);
    exit(1);
}

void send_header(int client, int status) {
    if (status == 200)
        write(client, "HTTP/1.1 200 OK\r\n", strlen("HTTP/1.1 200 OK\r\n"));
    else
        write(client, "HTTP/1.1 404 Not Found\r\n", strlen("HTTP/1.1 404 Not Found\r\n"));

    write(client, "Content-Type: text/html; charset=UTF-8\r\n\r\n",
          strlen("Content-Type: text/html; charset=UTF-8\r\n\r\n"));
}

void serve_file(int client, const char *path) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        send_header(client, 404);
        write(client, "<h1>404 Not Found</h1>", strlen("<h1>404 Not Found</h1>"));
        return;
    }

    send_header(client, 200);

    char buf[BUF_SIZE];
    ssize_t n;
    while ((n = read(fd, buf, BUF_SIZE)) > 0) {
        write(client, buf, n);
    }

    close(fd);
}

void execute_cgi(int client, const char *path, const char *method, const char *body) {
    int pipe_out[2], pipe_in[2];
    pipe(pipe_out);
    pipe(pipe_in);

    pid_t pid = fork();
    if (pid == 0) {
        dup2(pipe_out[1], STDOUT_FILENO);
        dup2(pipe_in[0], STDIN_FILENO);

        close(pipe_out[0]);
        close(pipe_in[1]);

        setenv("REQUEST_METHOD", method, 1);

        char fullpath[256];
        snprintf(fullpath, sizeof(fullpath), "./%s", path);

        execl(fullpath, fullpath, NULL);
        perror("execl failed");
        exit(1);
    }

    close(pipe_out[1]);
    close(pipe_in[0]);

    // POST body → CGI STDIN 전달
    if (body) write(pipe_in[1], body, strlen(body));
    close(pipe_in[1]);

    // 서버 헤더
    write(client, "HTTP/1.1 200 OK\r\n", strlen("HTTP/1.1 200 OK\r\n"));
    write(client, "Content-Type: text/html\r\n\r\n",
          strlen("Content-Type: text/html\r\n\r\n"));

    char buf[BUF_SIZE];
    ssize_t n;

    while ((n = read(pipe_out[0], buf, BUF_SIZE)) > 0) {
        write(client, buf, n);
    }

    close(pipe_out[0]);
    waitpid(pid, NULL, 0);
}

int main() {

    int server = socket(AF_INET, SOCK_STREAM, 0);
    if (server < 0) error("socket");

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(8080)
    };

    if (bind(server, (struct sockaddr*)&addr, sizeof(addr)) < 0)
        error("bind");

    listen(server, 5);

    printf("🚀 Simple Web Server started on port 8080...\n");

    while (1) {
        int client = accept(server, NULL, NULL);
        if (client < 0) continue;

        char buf[BUF_SIZE];
        ssize_t n = read(client, buf, BUF_SIZE);
        if (n <= 0) {
            close(client);
            continue;
        }

        buf[n] = '\0';

        char method[10], path[1024];
        char *body = strstr(buf, "\r\n\r\n");
        if (body) body += 4;

        sscanf(buf, "%s %s", method, path);

        printf("📥 Request: %s %s\n", method, path);

        // CGI
        if (strncmp(path, "/cgi-bin/", 9) == 0) {
            execute_cgi(client, path + 1, method, body);
        }
        // GET
        else if (strcmp(method, "GET") == 0) {
            char file_path[1024] = ".";
            strcat(file_path, path);

            if (strcmp(path, "/") == 0)
                serve_file(client, "./index.html");
            else
                serve_file(client, file_path);
        }
        // POST
        else if (strcmp(method, "POST") == 0) {
            send_header(client, 200);
            write(client, "<h1>POST DATA</h1><pre>", strlen("<h1>POST DATA</h1><pre>"));
            write(client, body, strlen(body));
            write(client, "</pre>", strlen("</pre>"));
        }

        close(client);
    }

    close(server);
    return 0;
}
