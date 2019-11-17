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
#include <string.h>

#define SA struct sockaddr *


void error_handling(char *string);

int Socket(int __domain, int __type, int __protocol);

int Bind(int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __len);

int Listen(int __fd, int __n);

int Connect(int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __len);

int Accept(int __fd, __SOCKADDR_ARG __addr,
           socklen_t *__restrict __addr_len);

ssize_t Read (int __fd, void *__buf, size_t __nbytes);


#endif //LEARNING_NETWORK_UDP_H
