#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "lib/udp.h"

#define BUF_SIZE 30

void error_handling(char *message);

#define PORT "8889"


void dg_cli(FILE *fp, int sock, const SA *pservaddr, socklen_t servlen) {
    char message[BUF_SIZE];
    socklen_t len;
    struct sockaddr *preply_addr;
    preply_addr = malloc(servlen);
    int n;
    while (1) {
        fputs("Insert message(q to quit): ", stdout);
        fgets(message, sizeof(message), stdin);
        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;

        sendto(sock, message, strlen(message), 0,
               pservaddr, servlen);

        len = servlen;

        n = recvfrom(sock, message, 2, 0,
                     preply_addr, &len);
        if (len != servlen || memcmp(pservaddr, preply_addr, servlen) != 0) {
            message[n] = 0;
            printf("reply from %s (ignored) %s\n", sock_ntop(preply_addr, len), message);
            continue;
        }

        message[n] = 0;
        printf("Message from server: %s\n", message);
    }
}

/**
 * 使用连接套接字
 * @param fp
 * @param sock
 * @param pservaddr
 * @param servlen
 */
void dg_cli3(FILE *fp, int sock, const SA *pservaddr, socklen_t servlen) {
    char message[BUF_SIZE];
    socklen_t len;
    struct sockaddr *preply_addr;
    preply_addr = malloc(servlen);
    int n;

    Connect(sock, (SA *) pservaddr, servlen);
    while (1) {
        fputs("Insert message(q to quit): ", stdout);
        fgets(message, sizeof(message), stdin);
        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;

        write(sock, message, strlen(message));

        len = servlen;

        n = read(sock, message, 4);
        message[n] = 0;
        printf("Message from server: %s\n", message);
    }
}

int main(int argc, char *argv[]) {
    int sock;
    char message[BUF_SIZE];
    int str_len;
    socklen_t adr_sz;

    struct sockaddr_in serv_adr, from_adr;
//    if (argc != 3) {
//        printf("Usage : %s <IP> <port>\n", argv[0]);
//        exit(1);
//    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
        error_handling("socket() error");
//    serv_adr = malloc(sizeof(struct sockaddr_in));
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_adr.sin_port = htons(atoi("8888"));
    dg_cli3(stdin, sock, (SA *) &serv_adr, sizeof(serv_adr));
    close(sock);
    return 0;
}
