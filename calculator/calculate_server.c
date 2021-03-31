//
// Created by yyancy on 2019/10/31.
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "lib/udp.h"


int calculate_result(int oper, int body[], int cnt) {
    int result = body[0];
    switch (oper) {
        case '+':
            for (int j = 1; j < cnt; ++j) {
                result += body[j];
            }
            break;
        case '-':
            for (int j = 1; j < cnt; ++j) {
                result -= body[j];
            }
            break;
        case '*':
            for (int j = 1; j < cnt; ++j) {
                result *= body[j];
            }
            break;
        case '/':
            for (int j = 1; j < cnt; ++j) {
                result /= body[j];
            }
            break;
        default:
            result = -1;
    }
    printf("result=  %d \n", result);
    return result;
}

int server() {
    struct sockaddr_in server_sock, client_sock;
    int server_fd, client_fd;
    socklen_t client_sock_len;
    char buf[100];
    char *mes = "hello,world";

    server_sock.sin_family = PF_INET;
    server_sock.sin_addr.s_addr = htonl(INADDR_ANY);
//    server_sock.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_sock.sin_port = htons(8080);

    server_fd = Socket(AF_INET, SOCK_STREAM, 0);

    Bind(server_fd, (SA *) &server_sock, sizeof(server_sock));

    Listen(server_fd, 4);

    client_sock_len = sizeof(client_sock);
    client_fd = Accept(server_fd, (SA *) &client_sock, &client_sock_len);

    for (int i = 0; i < 5; ++i) {
        int body[100];
        int cnt, oper;
        read(client_fd, &cnt, 4);
        read(client_fd, body, cnt * 4);
        read(client_fd, &oper, 4);
        int result = calculate_result(oper, body, cnt);

        printf("result=  %d \n", result);
        write(client_fd, &result, 4);

    }

    close(client_fd);
    close(server_fd);

    return 0;
}


int main() {

    server();
    return 0;
}