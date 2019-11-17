#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <fcntl.h>
#include <errno.h>
#include "lib/udp.h"

#include <sys/epoll.h>

#define BUF_SIZE 4
#define EPOLL_SIZE 10

/**
 * EPOLLET 边界触发,数据来时只注册一次事件
 */

/**
 * 设置目标描述符为非阻塞模式
 * @param fd
 */
void setnonblockingmode(int fd) {
    int flag = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}

/**
 * 测试epoll边界事件条件
 * @param argc
 * @param argv
 * @return
 */

int main(int argc, char *argv[]) {
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    struct timeval timeout;

    socklen_t adr_sz;
    int fd_max, str_len, fd_num, i;
    char buf[BUF_SIZE];

    serv_sock = Socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi("8080"));

    Bind(serv_sock, (struct sockaddr *) &serv_adr, sizeof(serv_adr));
    Listen(serv_sock, 5);

    struct epoll_event *ep_events;
    struct epoll_event event;
    int epfd, event_cnt;

    epfd = epoll_create(EPOLL_SIZE);
    ep_events = malloc(sizeof(struct epoll_event) * EPOLL_SIZE);

    setnonblockingmode(serv_sock);
    event.events = EPOLLIN;
    event.data.fd = serv_sock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);

    while (1) {
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;
        event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
        if (event_cnt == -1) {
            error_handling("epoll() error()");
            break;
        }
        puts("return epoll_wait");
        for (i = 0; i < event_cnt; i++) {
            int fd = ep_events[i].data.fd;
            if (fd == serv_sock) {
                // connection request!
                adr_sz = sizeof(clnt_adr);
                clnt_sock = Accept(serv_sock, (struct sockaddr *) &clnt_adr, &adr_sz);

                setnonblockingmode(clnt_sock);
                event.events = EPOLLIN | EPOLLET; // 设置为边界触发
                event.data.fd = clnt_sock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
                printf("connected client: %d \n", clnt_sock);
            } else    // read message!
            {
                for (;;) {
                    str_len = read(fd, buf, BUF_SIZE);
                    if (str_len == 0) {   // close request!
                        epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
                        close(fd);
                        printf("closed client: %d \n", i);
                        break;
                    } else if (str_len < 0) {
                        if (errno == EAGAIN)
                            break;
                    } else {
                        write(fd, buf, str_len);    // echo!

                    }
                }

            }
        }
    }


    close(serv_sock);
    return 0;
}
