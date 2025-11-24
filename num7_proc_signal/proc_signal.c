#include <stdio.h>
#include <unistd.h>     // fork, getpid, sleep
#include <signal.h>     // signal, sigaction, kill
#include <sys/wait.h>   // waitpid
#include <stdlib.h>

volatile sig_atomic_t got_usr1 = 0;

// 자식 프로세스에서 사용할 시그널 핸들러
void child_handler(int sig) {
    if (sig == SIGUSR1) {
        got_usr1 = 1;   // 플래그만 세팅
    } else if (sig == SIGTERM) {
        printf("[child] SIGTERM 수신, 종료합니다.\n");
        _exit(0);       // 시그널 핸들러 안에서는 exit 대신 _exit 권장
    }
}

int main(void) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork 실패");
        return 1;
    }

    if (pid == 0) {
        // 자식 프로세스
        struct sigaction sa;
        sa.sa_handler = child_handler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;

        // SIGUSR1, SIGTERM 핸들러 등록
        sigaction(SIGUSR1, &sa, NULL);
        sigaction(SIGTERM, &sa, NULL);

        printf("[child] PID=%d, PPID=%d\n", getpid(), getppid());

        while (1) {
            if (got_usr1) {
                printf("[child] SIGUSR1 받음! (특별 작업 수행)\n");
                got_usr1 = 0;
            }

            printf("[child] 동작 중... PID=%d\n", getpid());
            sleep(1);
        }
    } else {
        // 부모 프로세스
        printf("[parent] PID=%d, child PID=%d\n", getpid(), pid);

        sleep(3);
        printf("[parent] 자식에게 SIGUSR1 전송\n");
        kill(pid, SIGUSR1);      // 자식에게 사용자 정의 시그널 전송

        sleep(3);
        printf("[parent] 자식에게 SIGTERM 전송 (종료 요청)\n");
        kill(pid, SIGTERM);      // 자식 종료 요청

        // 자식이 종료될 때까지 대기
        waitpid(pid, NULL, 0);
        printf("[parent] 자식 종료 확인, 부모도 종료.\n");

        return 0;
    }
}
