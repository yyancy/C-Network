#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include "lib/udp.h"

#define BUF_SIZE 100

const int proxy_port = 9527;

#define INVALID_DEST -1
#define TABLE_LEN 20
int progress_table[TABLE_LEN];

void init_progress_table() {
    for (int i = 0; i < TABLE_LEN; ++i) {
        progress_table[i] = -1;
    }
}

int find_dest(int sock) {

    return progress_table[sock];
}

// 双向注册
void bi_set(int src, int dest) {
    progress_table[src] = dest;
    progress_table[dest] = src;
}

void clear(int i) {
    int s = progress_table[i];
    progress_table[i] = INVALID_DEST;
    progress_table[s] = INVALID_DEST;
}

int connect_server() {
    int sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;
    char buf[BUF_SIZE];
    ssize_t str_len;
    sock = Socket(PF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8081);

    Connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    return sock;
}

int main(int argc, char *argv[]) {
    int serv_sock, clnt_sock, real_server_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    struct timeval timeout;
    fd_set reads, cpy_reads;
    fd_set writes, cpy_writes;

    socklen_t adr_sz;
    int fd_max, str_len, fd_num, i;
    char buf[BUF_SIZE];

    serv_sock = Socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(proxy_port);

    Bind(serv_sock, (SA *) &serv_adr, sizeof(serv_adr));
    Listen(serv_sock, 5);

    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);
    fd_max = serv_sock;

    init_progress_table();


//    real_server_sock = connect_server();
//    FD_ZERO(&writes);
//    FD_SET(real_server_sock, &writes);
//    if (fd_max < real_server_sock)
//        fd_max = real_server_sock;

    while (1) {
        cpy_reads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;

        if ((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1)
            break;

        if (fd_num == 0)
            continue;

        for (i = 0; i < fd_max + 1; i++) {
            if (FD_ISSET(i, &cpy_reads)) {
                if (i == serv_sock)     // connection request!
                {
                    adr_sz = sizeof(clnt_adr);
                    clnt_sock =
                            Accept(serv_sock, (struct sockaddr *) &clnt_adr, &adr_sz);
                    FD_SET(clnt_sock, &reads);
                    if (fd_max < clnt_sock)
                        fd_max = clnt_sock;
                    printf("connected client: %d \n", clnt_sock);
                } else    // read message!
                {
                    int dest_sock;
                    str_len = read(i, buf, BUF_SIZE);
                    printf("received len: %d ", str_len);
                    if (str_len == 0)    // close request!
                    {
                        FD_CLR(i, &reads);
                        close(i);
                        clear(i);
                        printf("closed client: %d \n", i);
                    } else {
                        dest_sock = find_dest(i);
                        if (dest_sock == INVALID_DEST) {
                            printf("connect server!");
                            if ((dest_sock = connect_server()) == -1) {
                                fprintf(stderr, "connect server error!\n");
                                exit(1);
                            }
                            bi_set(i, dest_sock);

                            FD_SET(dest_sock, &reads);
                            if (fd_max < dest_sock)
                                fd_max = dest_sock;
                        }
                        buf[str_len] = '\0';
                        printf("%d send message to %d; message is %s. \n", i, dest_sock, buf);
                        write(dest_sock, buf, str_len);    // echo!

                    }
                }
            }
        }
    }
    close(serv_sock);
    return 0;
}
