//
// Created by yyancy on 2019/11/10.
//

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *thread_main(void *argc);

int main() {
    pthread_t p_id;
    int thread_param = 5;
    void *thr_ret;

    if (pthread_create(&p_id, NULL, thread_main, &thread_param) != 0) {
        puts("failed to create thread!");
        return -1;
    }

    if (pthread_join(p_id, &thr_ret) != 0) {
        puts("pthread_join error()");
        return -1;
    }
    printf("end thread,return value is %s", (char *) thr_ret);
    return 0;
}

void *thread_main(void *argc) {
    int cnt = *((int *) argc);
    for (int j = 0; j < cnt; ++j) {
        sleep(1);
        puts("running thread");
    }
    return "233";
}