#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "lib/udp.h"


#define BUFSIZE 100

int main() {

    int sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;
    char buf[BUFSIZE];
    ssize_t str_len;
    sock = Socket(PF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8080);

    Connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    for (;;) {
        fgets(buf, sizeof(buf), stdin); //不处理\n
        printf("str: %s", buf);
        str_len = strlen(buf);
        if (!strcmp(buf, "q\n") || !strcmp(buf, "Q\n"))
            break;

        if (write(sock, buf, strlen(buf)) == EOF) {
            error_handling("write() error");
        }

        // 1234567890
        ssize_t n = 0;
        memset(buf, 0, sizeof(buf));
        while (n < str_len) {
            int t;
            t = Read(sock, &buf[n], str_len - n);
            printf("%d %s\n", t, buf);
            n += t;
        }
        buf[n] = 0;
        printf("received str : %s", buf);
    }


    close(sock);
    return 0;
}

