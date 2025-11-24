#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define BUF_SIZE 256

int main() {
    int pipeAtoB[2];   // A → B
    int pipeBtoA[2];   // B → A
    pid_t pid;
    char buffer[BUF_SIZE];

    // 파이프 생성
    if (pipe(pipeAtoB) == -1 || pipe(pipeBtoA) == -1) {
        perror("pipe error");
        exit(1);
    }

    pid = fork();

    if (pid < 0) {
        perror("fork error");
        exit(1);
    }

    // 📌 부모 프로세스: 사용자 A
    if (pid > 0) {
        close(pipeAtoB[0]); // A→B 읽기 끝 닫기
        close(pipeBtoA[1]); // B→A 쓰기 끝 닫기

        printf("=== A 채팅창 ===\n");

        while (1) {
            printf("A 입력: ");
            fflush(stdout);

            fgets(buffer, BUF_SIZE, stdin);

            // 종료 명령
            if (strncmp(buffer, "exit", 4) == 0) {
                write(pipeAtoB[1], buffer, strlen(buffer));
                break;
            }

            // A → B 전달
            write(pipeAtoB[1], buffer, strlen(buffer));

            // B에게서 메시지 읽기
            int n = read(pipeBtoA[0], buffer, BUF_SIZE);
            buffer[n] = '\0';

            printf("B → A: %s", buffer);
        }

        close(pipeAtoB[1]);
        close(pipeBtoA[0]);
    }

    // 📌 자식 프로세스: 사용자 B
    else {
        close(pipeAtoB[1]); // A→B 쓰기 끝 닫기
        close(pipeBtoA[0]); // B→A 읽기 끝 닫기

        printf("=== B 채팅창 ===\n");

        while (1) {
            // A → B 메시지 읽기
            int n = read(pipeAtoB[0], buffer, BUF_SIZE);
            buffer[n] = '\0';
            printf("A → B: %s", buffer);

            // 종료 명령
            if (strncmp(buffer, "exit", 4) == 0)
                break;

            // B 입력
            printf("B 입력: ");
            fflush(stdout);
            fgets(buffer, BUF_SIZE, stdin);

            // B → A 전달
            write(pipeBtoA[1], buffer, strlen(buffer));
        }

        close(pipeAtoB[0]);
        close(pipeBtoA[1]);
    }

    return 0;
}
