//
// Created by yyancy on 2019/11/28.
//

#include "ping.h"

void sig_alrm(int sign) {
    (*pr->fsend)();
    alarm(1);
    return;
}