//
// Created by yyancy on 2019/11/1.
//
#include "udp.h"


void error_handling(char *string) {
    fputs(string, stderr);
    fputc('\n', stderr);
    exit(1);
}

int Socket(int __domain, int __type, int __protocol) {
    int sock;
    if ((sock = socket(__domain, __type, __protocol)) == -1) {
        printf("错误信息： %s\n",strerror(errno));
        error_handling("socket() error");
    }
    return sock;
}

int Bind(int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __len) {
    if (bind(__fd, __addr, __len) == -1) {
        error_handling("bind() error");
    }
    return __fd;
}

int Listen(int __fd, int __n) {
    if (listen(__fd, __n) == -1) {
        error_handling("listen() error");
    }
    return __fd;
}

int Accept(int __fd, __SOCKADDR_ARG __addr,
           socklen_t *__restrict __addr_len) {
    int client_fd;
    if ((client_fd = accept(__fd, (SA *) &__addr, __addr_len)) == -1) {
        error_handling("accept() error");
    }
    return client_fd;
}

int Connect(int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __len) {

    if (connect(__fd, __addr, __len) == -1) {
        error_handling("Connect() error");
    }
    return __fd;
}

ssize_t Read(int __fd, void *__buf, size_t __nbytes) {
    int n = read(__fd, __buf, __nbytes);
    if (n < 0) {
        error_handling("found error");
    } else if (n == 0) {
        error_handling("received server FIN.");
    }
    return n;
}


