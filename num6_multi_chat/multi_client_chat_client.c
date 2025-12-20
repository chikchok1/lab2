#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define PORT 9000
#define BUF_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    fd_set read_fds;

    char buf[BUF_SIZE];

    // 1) 소켓 생성
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) { perror("socket"); exit(1); }

    // 2) 서버 주소 설정
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 3) 서버 연결
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect"); exit(1);
    }

    printf("🟢 Connected to server!\n");

    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(0, &read_fds);     // 키보드 입력
        FD_SET(sock, &read_fds);  // 서버 메시지

        if (select(sock + 1, &read_fds, NULL, NULL, NULL) < 0) {
            perror("select"); exit(1);
        }

        // 1) 서버 메시지 수신
        if (FD_ISSET(sock, &read_fds)) {
            int len = read(sock, buf, BUF_SIZE);
            if (len <= 0) {
                printf("🔴 Server disconnected\n");
                break;
            }
            buf[len] = '\0';
            printf("💬 %s", buf);
        }

        // 2) 사용자 입력
        if (FD_ISSET(0, &read_fds)) {
            fgets(buf, BUF_SIZE, stdin);
            write(sock, buf, strlen(buf));
        }
    }

    close(sock);
    return 0;
}
