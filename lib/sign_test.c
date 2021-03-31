//
// Created by yyancy on 2019/12/4.
//
#include "udp.h"

int main(int argc, char *argv[]) {
    Signal(SIGCHLD, sig_chld);


    pid_t pid = fork();

    if (pid == 0)     // if Child Process
    {
        puts("Hi I'am a child process");
    } else {
        printf("Child Process ID: %d \n", pid);
     sleep(30);     // Sleep 30 sec.
    }

    if (pid == 0)
        puts("End child process");
    else
        puts("End parent process");
    return 0;
}