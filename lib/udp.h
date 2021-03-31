//
// Created by yyancy on 2019/11/9.
//

#ifndef LEARNING_NETWORK_UDP_H
#define LEARNING_NETWORK_UDP_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <signal.h>

//#include "addrinfo.h"

#define SA struct sockaddr

typedef void Sigfunc(int);


Sigfunc *Signal(int signo, Sigfunc *func);
void error_handling(char *string);

int Socket(int __domain, int __type, int __protocol);

int Bind(int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __len);

int Listen(int __fd, int __n);

int Connect(int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __len);

int Accept(int __fd, __SOCKADDR_ARG __addr,
           socklen_t *__restrict __addr_len);

ssize_t Read (int __fd, void *__buf, size_t __nbytes);

struct addrinfo *host_serv(const char *, const char *, int, int);
char	*sock_ntop_host(const SA *, socklen_t);

char	*Sock_ntop_host(const SA *, socklen_t);

struct addrinfo *Host_serv(const char *, const char *, int, int);
#endif //LEARNING_NETWORK_UDP_H

int
getaddrinfo(const char *hostname, const char *servname,
            const struct addrinfo *hintsp, struct addrinfo **result);


char *sock_ntop(const struct sockaddr *sa,socklen_t len);

//char *
//gai_strerror(int err);

void
err_quit(const char *fmt, ...);

void sig_chld(int signo);