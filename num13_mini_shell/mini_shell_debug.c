#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

#define MAX_LINE 1024
#define MAX_ARG 64

void sigint_handler(int signum) {
    printf("\n[DEBUG] SIGINT(Ctrl-C) received\n");
}

void sigquit_handler(int signum) {
    printf("\n[DEBUG] SIGQUIT(Ctrl-Z) received\n");
}

void sigtstp_handler(int signum) {
    printf("\n[DEBUG] SIGTSTP(Ctrl-Z) received\n");
}


void parse(char *cmd, char **args, int *background) {
    printf("[DEBUG] Parsing command: \"%s\"\n", cmd);


    int i = 0;
    *background = 0;

    while ((args[i] = strsep(&cmd, " ")) != NULL) {
        if (*args[i] == '\0') continue;

        if (strcmp(args[i], "&") == 0) {
            *background = 1;
            args[i] = NULL;
            printf("[DEBUG] '&' detected → background mode ON\n");
            break;
        }
        i++;
    }
}

int main() {
    char cmdline[MAX_LINE];
    char *args[MAX_ARG];

    // 시그널 설정
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, sigquit_handler);
    signal(SIGTSTP, sigtstp_handler);  // Ctrl+Z 핸들링

    while (1) {
        printf("mini-shell > ");
        fflush(stdout);

        if (fgets(cmdline, MAX_LINE, stdin) == NULL)
            continue;

        cmdline[strcspn(cmdline, "\n")] = 0;

        printf("[DEBUG] 입력된 명령어: \"%s\"\n", cmdline);

        // exit 처리
        if (strcmp(cmdline, "exit") == 0) {
            printf("[DEBUG] exit 명령어 입력됨 → 프로그램 종료\n");
            break;
        }

        // 파이프 존재 여부 확인
        char *pipe_pos = strchr(cmdline, '|');

        if (pipe_pos != NULL) {
            printf("[DEBUG] 파이프(|) 발견 → 파이프 실행 모드\n");

            *pipe_pos = '\0';
            char *cmd1 = cmdline;
            char *cmd2 = pipe_pos + 1;

            printf("[DEBUG] LEFT CMD: \"%s\"\n", cmd1);
            printf("[DEBUG] RIGHT CMD: \"%s\"\n", cmd2);

            char *args1[MAX_ARG], *args2[MAX_ARG];
            int bg1 = 0, bg2 = 0;

            parse(cmd1, args1, &bg1);
            parse(cmd2, args2, &bg2);

            printf("[DEBUG] pipe() 호출\n");
            int fd[2];
            pipe(fd);

            pid_t pid1 = fork();
            if (pid1 == 0) {
                printf("[DEBUG] 자식1 실행: %s\n", args1[0]);
                dup2(fd[1], STDOUT_FILENO);
                close(fd[0]);
                execvp(args1[0], args1);
                perror("exec1 error");
                exit(1);
            }

            pid_t pid2 = fork();
            if (pid2 == 0) {
                printf("[DEBUG] 자식2 실행: %s\n", args2[0]);
                dup2(fd[0], STDIN_FILENO);
                close(fd[1]);
                execvp(args2[0], args2);
                perror("exec2 error");
                exit(1);
            }

            close(fd[0]);
            close(fd[1]);

            printf("[DEBUG] 부모: 두 자식의 실행을 기다립니다...\n");
            waitpid(pid1, NULL, 0);
            waitpid(pid2, NULL, 0);
            continue;
        }

        // 일반 명령 처리 (리다이렉션 포함)
        int background = 0;
        char *cmd = strdup(cmdline);
        parse(cmd, args, &background);

        if (args[0] == NULL) continue;

        printf("[DEBUG] 실행할 명령어: %s\n", args[0]);
        printf("[DEBUG] background = %d\n", background);

        int redirect_in = -1, redirect_out = -1;

for (int i = 0; args[i] != NULL; i++) {

    // args[i] null 이면 strcmp 하면 안 됨
    if (args[i] == NULL) break;

    if (strcmp(args[i], "<") == 0) {
        if (args[i + 1] == NULL) {
            printf("[ERROR] '<' 뒤에는 파일명이 필요합니다.\n");
            break;
        }
        printf("[DEBUG] 입력 리다이렉션 '<' 발견: %s\n", args[i + 1]);
        redirect_in = open(args[i + 1], O_RDONLY);
        args[i] = NULL;
        continue;  // 다음 i로 넘어가면서 충돌 방지
    }

    if (strcmp(args[i], ">") == 0) {
        if (args[i + 1] == NULL) {
            printf("[ERROR] '>' 뒤에는 파일명이 필요합니다.\n");
            break;
        }
        printf("[DEBUG] 출력 리다이렉션 '>' 발견: %s\n", args[i + 1]);
        redirect_out = open(args[i + 1],
                            O_WRONLY | O_CREAT | O_TRUNC, 0644);
        args[i] = NULL;
        continue;
    }
}

        pid_t pid = fork();
        if (pid == 0) {
            printf("[DEBUG] 자식 프로세스 실행: %s\n", args[0]);

            if (redirect_in != -1) {
                printf("[DEBUG] 입력 파일로 STDIN 변경\n");
                dup2(redirect_in, STDIN_FILENO);
            }
            if (redirect_out != -1) {
                printf("[DEBUG] 출력 파일로 STDOUT 변경\n");
                dup2(redirect_out, STDOUT_FILENO);
            }

            execvp(args[0], args);
            perror("[DEBUG] exec error");
            exit(1);
        } else {
            if (!background) {
                printf("[DEBUG] 부모: 자식 PID %d 대기 중...\n", pid);
                waitpid(pid, NULL, 0);
            } else {
                printf("[DEBUG] 백그라운드 실행 PID: %d\n", pid);
            }
        }

        free(cmd);
    }

    return 0;
}
