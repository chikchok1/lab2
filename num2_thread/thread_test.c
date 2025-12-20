#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* thread_func(void* arg) {
    int num = *(int*)arg;
    for (int i = 0; i < 5; i++) {
        printf("▶ Thread %d running... (%d)\n", num, i);
        sleep(1);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t t1, t2;
    int id1 = 1, id2 = 2;

    // 쓰레드 생성
    pthread_create(&t1, NULL, thread_func, &id1);
    pthread_create(&t2, NULL, thread_func, &id2);

    // 쓰레드 종료 대기
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("▶ Main thread finished.\n");
    return 0;
}
