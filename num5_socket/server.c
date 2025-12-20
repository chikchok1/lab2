// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9000

int main() {
    int server_sock, client_sock;
    char buffer[1024];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;

    // 1) 소켓 생성
    server_sock = socket(AF_INET, SOCK_STREAM, 0);

    // 2) 서버 주소 설정
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // 3) 바인딩
    bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // 4) 클라이언트 접속 대기
    listen(server_sock, 5);
    printf("Server Ready. Waiting for Client...\n");

    addr_size = sizeof(client_addr);
    client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);

    // 5) 클라이언트 메시지 수신
    int n = read(client_sock, buffer, sizeof(buffer) - 1);
    buffer[n] = '\0';

    printf("Client: %s\n", buffer);

    // 6) 서버 → 클라이언트 메시지 전송
    char msg[] = "Hello Client!";
    write(client_sock, msg, strlen(msg));

    // 7) 소켓 종료
    close(client_sock);
    close(server_sock);

    return 0;
}
