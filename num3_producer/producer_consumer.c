#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5   // 제한 버퍼 크기
int buffer[BUFFER_SIZE];
int count = 0;          // 현재 버퍼에 들어있는 아이템 개수
int in = 0;             // 생산자가 데이터를 넣는 위치
int out = 0;            // 소비자가 데이터를 꺼내는 위치

pthread_mutex_t mutex;          // 상호 배제 락(Mutex)
pthread_cond_t not_full;        // 버퍼가 가득 찼을 때 대기하는 조건 변수
pthread_cond_t not_empty;       // 버퍼가 비었을 때 대기하는 조건 변수

// ----------------------------
// 생산자 함수
// ----------------------------
void* producer(void* arg) {
    int id = *(int*)arg;

    while (1) {
        sleep(rand() % 2); // 생산 시간 랜덤

        pthread_mutex_lock(&mutex);

        while (count == BUFFER_SIZE) {
            printf("🚫 Producer %d: Buffer FULL, waiting...\n", id);
            pthread_cond_wait(&not_full, &mutex);     // 버퍼가 가득 차면 대기
        }

        int item = rand() % 100;
        buffer[in] = item;
        printf("➕ Producer %d: Produced %d at %d\n", id, item, in);

        in = (in + 1) % BUFFER_SIZE;
        count++;

        pthread_cond_signal(&not_empty); // 소비자에게 신호
        pthread_mutex_unlock(&mutex);
    }
}

// ----------------------------
// 소비자 함수
// ----------------------------
void* consumer(void* arg) {
    int id = *(int*)arg;

    while (1) {
        sleep(rand() % 3); // 소비 시간 랜덤

        pthread_mutex_lock(&mutex);

        while (count == 0) {
            printf("⚠️ Consumer %d: Buffer EMPTY, waiting...\n", id);
            pthread_cond_wait(&not_empty, &mutex);     // 버퍼가 비면 대기
        }

        int item = buffer[out];
        printf("➖ Consumer %d: Consumed %d from %d\n", id, item, out);

        out = (out + 1) % BUFFER_SIZE;
        count--;

        pthread_cond_signal(&not_full); // 생산자에게 신호
        pthread_mutex_unlock(&mutex);
    }
}

// ----------------------------
// main 함수
// ----------------------------
int main() {
    pthread_t prod[2], cons[2];
    int prod_id[2] = {1, 2};
    int cons_id[2] = {1, 2};

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&not_full, NULL);
    pthread_cond_init(&not_empty, NULL);

    // 생산자 2명 생성
    for (int i = 0; i < 2; i++)
        pthread_create(&prod[i], NULL, producer, &prod_id[i]);

    // 소비자 2명 생성
    for (int i = 0; i < 2; i++)
        pthread_create(&cons[i], NULL, consumer, &cons_id[i]);

    // 쓰레드 종료 대기(끝나지 않음)
    for (int i = 0; i < 2; i++)
        pthread_join(prod[i], NULL);

    for (int i = 0; i < 2; i++)
        pthread_join(cons[i], NULL);

    return 0;
}
