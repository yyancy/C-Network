//
// Created by yyancy on 2019/12/3.
//

#include "udp.h"

Sigfunc *Signal(int signo, Sigfunc *func) {
    struct sigaction act, oact;
    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if (signo != SIGALRM) {
        act.sa_flags |= SA_RESTART;
    }

    if (sigaction(signo, &act, &oact) < 0) {
        return SIG_IGN;
    }
    return oact.sa_handler;
}