//
// Created by yyancy on 2019/11/10.
//

//
// Created by yyancy on 2019/11/10.
//

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define NUM_THREAD 100

int num = 0;
pthread_mutex_t mutex;

void *read2(void *argc);

void *accu(void *argc);

sem_t sem_has_val;
sem_t sem_empty;


int main() {
    pthread_t id_t1, id_t2;
    sem_init(&sem_has_val, 0, 0);
    sem_init(&sem_empty, 0, 1);

    pthread_create(&id_t1, NULL, read2, NULL);
    pthread_create(&id_t2, NULL, accu, NULL);

    pthread_join(id_t1, NULL);
    pthread_join(id_t2, NULL);

    sem_destroy(&sem_has_val);
    sem_destroy(&sem_empty);
    return 0;
}

#define EACH_COUNT 50,000,000

void *read2(void *argc) {
    for (int i = 0; i < 5; ++i) {
        fputs("input number :", stdout);
        sem_wait(&sem_empty);
        scanf("%d", &num);
        sem_post(&sem_has_val);
    }
    return NULL;
}

void *accu(void *argc) {
    int sum = 0;
    for (int i = 0; i < 5; ++i) {
        sem_wait(&sem_has_val);
        sum += num;
        sem_post(&sem_empty);
    }
    printf("Result %d\n", sum);
    return NULL;
}