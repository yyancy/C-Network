
#include "udp.h"
#include <sys/wait.h>

/**
 * 回收子僵死进程
 * @param signo
 */
void sig_chld(int signo) {
    pid_t pid;
    int stat;// 子进程的退出状态

    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
        printf("child %d terminated\n", pid);
    return;
}