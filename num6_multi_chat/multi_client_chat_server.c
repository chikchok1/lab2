#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define PORT 9000
#define BUF_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);

    fd_set read_fds, tmp_fds;
    int fd_max;

    char buf[BUF_SIZE];

    // 1) 소켓 생성
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket"); exit(1); }

    // 2) 서버 주소 설정
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // 3) 바인드
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind"); exit(1);
    }

    // 4) 리슨
    if (listen(server_fd, 10) < 0) {
        perror("listen"); exit(1);
    }

    printf("📡 Chat Server Started on port %d...\n", PORT);

    FD_ZERO(&read_fds);
    FD_SET(server_fd, &read_fds);
    fd_max = server_fd;

    while (1) {
        tmp_fds = read_fds; // select는 파괴적이라 복사본 사용

        if (select(fd_max + 1, &tmp_fds, NULL, NULL, NULL) < 0) {
            perror("select"); exit(1);
        }

        // 5) 다중 소켓 검사
        for (int i = 0; i <= fd_max; i++) {
            if (FD_ISSET(i, &tmp_fds)) {

                // (1) 새로운 클라이언트 접속
                if (i == server_fd) {
                    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_size);
                    FD_SET(client_fd, &read_fds);
                    if (client_fd > fd_max) fd_max = client_fd;

                    printf("🟢 Client connected: %d\n", client_fd);
                    sprintf(buf, "Welcome! Your ID is %d\n", client_fd);
                    write(client_fd, buf, strlen(buf));
                }

                // (2) 기존 클라이언트 메시지
                else {
                    int len = read(i, buf, BUF_SIZE);
                    if (len <= 0) {
                        printf("🔴 Client %d disconnected\n", i);
                        close(i);
                        FD_CLR(i, &read_fds);
                    } else {
                        buf[len] = '\0';
                        printf("📩 Client %d: %s", i, buf);

                        // 모든 클라이언트에게 브로드캐스트
                        for (int j = 0; j <= fd_max; j++) {
                            if (j != server_fd && j != i && FD_ISSET(j, &read_fds)) {
                                write(j, buf, len);
                            }
                        }
                    }
                }
            }
        }
    }
}
