//
// Created by yyancy on 2019/11/10.
//

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *thread_main(void *argc);

int main() {
    pthread_t p_id, p_id2;
    int range1[] = {1, 5};
    int range2[] = {6, 10};
    void *thr_ret, *thr_ret2;

    if (pthread_create(&p_id, NULL, thread_main, (void *) range1) != 0) {
        puts("failed to create thread!");
        return -1;
    }
    if (pthread_create(&p_id2, NULL, thread_main, (void *) range2) != 0) {
        puts("failed to create thread!");
        return -1;
    }

    if (pthread_join(p_id, &thr_ret) != 0) {
        puts("pthread_join error()");
        return -1;
    }
    if (pthread_join(p_id2, &thr_ret2) != 0) {
        puts("pthread_join error()");
        return -1;
    }
    printf("end thread,return value is %d\n", (int) thr_ret + (int) thr_ret2);
    return 0;
}

void *thread_main(void *argc) {
    int start = ((int *) argc)[0];
    int end = ((int *) argc)[1];
    int sum = 0;
    while (start <= end) {
        sum += start;
        start++;
    }
    return (void *) sum;
}