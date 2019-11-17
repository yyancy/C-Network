//
// Created by yyancy on 2019/11/10.
//

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREAD 100

long long num = 0;
pthread_mutex_t mutex;

void *thread_inc(void *argc);

void *thread_des(void *argc);

int main() {
    pthread_t thread_id[NUM_THREAD];
    pthread_mutex_init(&mutex, NULL);
    void *thr_ret, *thr_ret2;

    for (int i = 0; i < NUM_THREAD; ++i) {
        if (i % 2)
            pthread_create(&(thread_id[i]), NULL, thread_inc, NULL);
        else
            pthread_create(&(thread_id[i]), NULL, thread_des, NULL);
    }
    for (int j = 0; j < NUM_THREAD; ++j) {
        pthread_join(thread_id[j], NULL);
    }
    printf("end thread,return value is %lld\n", num);
    return 0;
}

#define EACH_COUNT 50,000,000

void *thread_inc(void *argc) {
    for (int i = 0; i < 50000000; ++i) {
        pthread_mutex_lock(&mutex);
        num++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *thread_des(void *argc) {
    for (int i = 0; i < 50000000; ++i) {
        pthread_mutex_lock(&mutex);
        num--;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}