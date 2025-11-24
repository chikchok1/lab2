#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define BUF_SIZE 256

// 메시지 구조체
struct msgbuf {
    long mtype;
    char mtext[BUF_SIZE];
};

int main() {
    key_t key;
    int msgid;
    struct msgbuf message;
    char input[BUF_SIZE];
    int role;

    // 동일한 키 생성
    key = ftok(".", 'A');
    if (key == -1) {
        perror("ftok error");
        exit(1);
    }

    // 메시지 큐 생성 (없으면 생성, 있으면 접근)
    msgid = msgget(key, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("msgget error");
        exit(1);
    }

    // 역할 선택
    printf("=== 메시지 큐 채팅 ===\n");
    printf("1. 사용자 A\n");
    printf("2. 사용자 B\n");
    printf("역할 선택: ");
    scanf("%d", &role);
    getchar();  // 엔터 제거

    long sendType = (role == 1 ? 1 : 2);  // 내가 보낼 타입
    long recvType = (role == 1 ? 2 : 1);  // 내가 받을 타입

    printf("\n--- 채팅 시작! ('exit' 입력 시 종료) ---\n");

    while (1) {
        // ① 내가 입력 → 상대에게 전송
        printf(role == 1 ? "A 입력: " : "B 입력: ");
        fgets(input, BUF_SIZE, stdin);

        // 종료
        if (strncmp(input, "exit", 4) == 0) {
            message.mtype = sendType;
            strcpy(message.mtext, input);
            msgsnd(msgid, &message, strlen(input) + 1, 0);
            break;
        }

        // 메시지 전송
        message.mtype = sendType;
        strcpy(message.mtext, input);
        msgsnd(msgid, &message, strlen(message.mtext) + 1, 0);

        // ② 상대방 메시지 수신
        msgrcv(msgid, &message, BUF_SIZE, recvType, 0);
        printf(role == 1 ? "B → A: %s" : "A → B: %s", message.mtext);

        // 상대가 exit 입력하면 종료
        if (strncmp(message.mtext, "exit", 4) == 0)
            break;
    }

    // A가 끝낼 때 메시지 큐 삭제
    if (role == 1) {
        msgctl(msgid, IPC_RMID, NULL);
        printf("메시지 큐 삭제 완료.\n");
    }

    return 0;
}
