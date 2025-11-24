#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int p2c[2];  // 부모 → 자식 pipe
    int c2p[2];  // 자식 → 부모 pipe

    if (pipe(p2c) == -1 || pipe(c2p) == -1) {
        perror("pipe 생성 실패");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork 실패");
        return 1;
    }

    if (pid == 0) {
        // ================================
        // 자식 프로세스
        // ================================
        close(p2c[1]); // 부모 → 자식 pipe 쓰기 닫기
        close(c2p[0]); // 자식 → 부모 pipe 읽기 닫기

        char buffer[100];
        read(p2c[0], buffer, sizeof(buffer));  // 부모 메시지 읽기
        printf("[child] 부모로부터 받은 메시지: %s\n", buffer);

        // 자식이 부모에게 응답
        char reply[] = "자식이 메시지 수신 완료!";
        write(c2p[1], reply, strlen(reply) + 1);
        close(p2c[0]);
        close(c2p[1]);
    } 
    else {
        // ================================
        // 부모 프로세스
        // ================================
        close(p2c[0]); // 부모 → 자식 pipe 읽기 닫기
        close(c2p[1]); // 자식 → 부모 pipe 쓰기 닫기

        // 부모가 자식에게 메시지 전송
        char msg[] = "안녕 자식아, 메시지 받았니?";
        write(p2c[1], msg, strlen(msg) + 1);
        close(p2c[1]);

        // 자식 응답 읽기
        char buffer[100];
        read(c2p[0], buffer, sizeof(buffer));
        printf("[parent] 자식으로부터 응답: %s\n", buffer);

        close(c2p[0]);
        wait(NULL);
    }

    return 0;
}
