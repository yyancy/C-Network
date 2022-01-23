#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <poll.h>
#include <errno.h>
#include "lib/udp.h"


#define BUF_SIZE 4
#define EPOLL_SIZE 10

/**
 * EPOLLET 边界触发,数据来时只注册一次事件
 */

/**
 *
 * @param argc
 * @param argv
 * @return
 */

int main(int argc, char *argv[]) {
    int listenfd, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    struct timeval timeout;

    socklen_t adr_sz;
    struct pollfd client[FOPEN_MAX];
    int fd_max, str_len, fd_num, i;
    char buf[BUF_SIZE];

    listenfd = Socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi("8080"));

    Bind(listenfd, (struct sockaddr *) &serv_adr, sizeof(serv_adr));
    Listen(listenfd, 5);

    client[0].fd = listenfd;
    client[0].events = POLLRDNORM;
    for (int i = 1; i < FOPEN_MAX; ++i) {
        client[i].fd = -1;
    }
    int maxi = 0, clilen;
    int epfd, event_cnt;


    for (;;) {
        int nready = poll(client, maxi + 1, -1);
        if (client[0].revents & POLLRDNORM) { // new client connection
            clilen = sizeof(clnt_adr);
            clnt_sock = Accept(listenfd, (SA*) &clnt_adr, &clilen);
            int i;
            for (i = 1; i < FOPEN_MAX; ++i) {
                if (client[i].fd < 0) {
                    client[i].fd = clnt_sock;
                    break;
                }
            }
            if (i == FOPEN_MAX) {
                error_handling("too many clients");
            }
            client[i].events = POLLRDNORM;
            if (i > maxi)
                maxi = i;
            if (--nready <= 0)
                continue;
        }
        // for client
        int sockfd;
        for (int i = 1; i <= maxi; ++i) {
            if ((sockfd = client[i].fd) < 0)
                continue;
            if (client[i].revents & (POLLRDNORM | POLLERR)) {
                int n;
                if ((n = read(sockfd, buf, BUF_SIZE)) < 0) {
                    if (errno == ECONNRESET) {
                        // connection reset by client
                        close(sockfd);
                        client[i].fd = -1;
                    } else {
                        error_handling("read error");
                    }
                } else if (n == 0) {
                    // connection closed by client
                    close(sockfd);
                    client[i].fd = -1;
                } else {
                    write(sockfd, buf, n);
                }

                if (--nready <= 0)
                    break;
            }
        }

    }


    close(listenfd);
    return 0;
}
