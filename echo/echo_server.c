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

int server() {
    struct sockaddr_in server_sock, client_sock;
    int server_fd, client_fd;
    socklen_t clien_sock_len;
    char buf[100];
    char *mes = "hello,world";

    server_sock.sin_family = PF_INET;
    server_sock.sin_addr.s_addr = htonl(INADDR_ANY);
//    server_sock.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_sock.sin_port = htons(8080);

    server_fd = Socket(AF_INET, SOCK_STREAM, 0);
    Bind(server_fd, (SA) &server_sock, sizeof(server_sock));
    Listen(server_fd, 4);

    for (int i = 0; i < 5; ++i) {
        clien_sock_len = sizeof(client_sock);
        client_fd = Accept(server_fd, (SA) &client_sock, &clien_sock_len);
        ssize_t n;
        while ((n = read(client_fd, buf, sizeof(buf))) > 0) {
            printf("%ld %s \n", n, buf);
            write(client_fd, buf, n);
        }
    }

    close(client_fd);
    close(server_fd);

    return 0;
}

int main() {
    server();
    return 0;
}