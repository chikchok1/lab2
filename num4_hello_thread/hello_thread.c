#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// 반복 횟수
#define LOOP_COUNT 10

// 0이면 부모 차례, 1이면 자식 차례
int turn = 0;  // 이진 플래그

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* child_thread(void* arg) {
    for (int i = 0; i < LOOP_COUNT; i++) {
        pthread_mutex_lock(&mutex);

        // 자식 차례가 올 때까지 대기
        while (turn != 1) {
            pthread_cond_wait(&cond, &mutex);
        }

        // 자식 출력
        printf("hello child (%d)\n", i);
        fflush(stdout);

        // 다음은 부모 차례
        turn = 0;
        pthread_cond_signal(&cond);  // 부모에게 알림

        pthread_mutex_unlock(&mutex);

        // 약간 딜레이
        usleep(100 * 1000);
    }

    return NULL;
}

int main(void) {
    pthread_t tid;

    // 자식 쓰레드 생성
    pthread_create(&tid, NULL, child_thread, NULL);

    for (int i = 0; i < LOOP_COUNT; i++) {
        pthread_mutex_lock(&mutex);

        // 부모 차례가 올 때까지 대기
        while (turn != 0) {
            pthread_cond_wait(&cond, &mutex);
        }

        // 부모 출력
        printf("hello parent (%d)\n", i);
        fflush(stdout);

        // 다음은 자식 차례
        turn = 1;
        pthread_cond_signal(&cond);  // 자식에게 알림

        pthread_mutex_unlock(&mutex);

        usleep(100 * 1000);
    }

    // 부모 루프가 끝난 뒤, 자식도 끝날 때까지 대기
    pthread_join(tid, NULL);

    // 자원 정리 (보고서용으로 써주면 좋음)
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
