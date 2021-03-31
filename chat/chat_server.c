#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "lib/udp.h"

#define BUF_SIZE 100
#define MAX_CLNT 256

void *handle_clnt(void *arg);

void send_msg(char *msg, int len);

void *handle_server(void *arg);

int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutx;

int main(int argc, char *argv[]) {
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    int clnt_adr_sz;
    pthread_t t_id;
//    if(argc!=2) {
//        printf("Usage : %s <port>\n", argv[0]);
//        exit(1);
//    }

    pthread_mutex_init(&mutx, NULL);
    serv_sock = Socket(PF_INET, SOCK_STREAM, 0);

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi("8080"));

    Bind(serv_sock, (struct sockaddr *) &serv_adr, sizeof(serv_adr));
    Listen(serv_sock, 5);

    pthread_create(&t_id, NULL, handle_server, (void *) &serv_sock);
    pthread_detach(t_id);

    while (1) {
        char ch = getchar();
        if (ch == 'q') {
            printf("退出服务器\n");
            break;
        }
//        clnt_adr_sz = sizeof(clnt_adr);
//        clnt_sock = Accept(serv_sock, (SA) &clnt_adr, &clnt_adr_sz);
//
//        pthread_mutex_lock(&mutx);
//        clnt_socks[clnt_cnt++] = clnt_sock;
//        pthread_mutex_unlock(&mutx);
//
//        pthread_create(&t_id, NULL, handle_clnt, (void *) &clnt_sock);
//        pthread_detach(t_id);
//        printf("Connected client IP: %s \n", inet_ntoa(clnt_adr.sin_addr));


    }
    close(serv_sock);
    return 0;
}

void *handle_clnt(void *arg) {
    int clnt_sock = *((int *) arg);
    int str_len = 0, i;
    char msg[BUF_SIZE];

    while ((str_len = read(clnt_sock, msg, sizeof(msg))) != 0)
        send_msg(msg, str_len);

    pthread_mutex_lock(&mutx);
    for (i = 0; i < clnt_cnt; i++)   // remove disconnected client
    {
        if (clnt_sock == clnt_socks[i]) {
            while (i++ < clnt_cnt - 1)
                clnt_socks[i] = clnt_socks[i + 1];
            break;
        }
    }
    clnt_cnt--;
    pthread_mutex_unlock(&mutx);
    close(clnt_sock);
    return NULL;
}


void *handle_server(void *arg) {
    while (1) {
        int server_sock = *((int *) arg);

        int clnt_sock;
        struct sockaddr_in serv_adr, clnt_adr;
        int clnt_adr_sz;
        pthread_t t_id;
        clnt_adr_sz = sizeof(clnt_adr);
        clnt_sock = Accept(server_sock, (SA *) &clnt_adr, &clnt_adr_sz);

        pthread_mutex_lock(&mutx);
        clnt_socks[clnt_cnt++] = clnt_sock;
        pthread_mutex_unlock(&mutx);

        pthread_create(&t_id, NULL, handle_clnt, (void *) &clnt_sock);
        pthread_detach(t_id);
        printf("Connected client IP: %s \n", inet_ntoa(clnt_adr.sin_addr));
    }
}

void send_msg(char *msg, int len)   // send to all
{
    int i;
    pthread_mutex_lock(&mutx);
    for (i = 0; i < clnt_cnt; i++)
        write(clnt_socks[i], msg, len);
    pthread_mutex_unlock(&mutx);
}
