// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9000

int main() {
    int sock;
    char buffer[1024];
    struct sockaddr_in server_addr;

    // 1) 소켓 생성
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // 2) 서버 주소 설정
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 3) 서버와 연결
    connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // 4) 서버에 메시지 전송
    char msg[] = "Hello Server!";
    write(sock, msg, strlen(msg));

    // 5) 서버 메시지 수신
    int n = read(sock, buffer, sizeof(buffer) - 1);
    buffer[n] = '\0';

    printf("Server: %s\n", buffer);

    // 6) 종료
    close(sock);

    return 0;
}
