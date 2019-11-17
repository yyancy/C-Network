#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "lib/udp.h"

#define BUF_SIZE 1024


/**
 * 发送数字,计算结果 calculate
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[]) {
    int sock;
    char message[BUF_SIZE];
    int body[BUF_SIZE];
    int str_len;
    struct sockaddr_in serv_adr;

    sock = Socket(PF_INET, SOCK_STREAM, 0);

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_adr.sin_port = htons(8080);

    Connect(sock, (struct sockaddr *) &serv_adr, sizeof(serv_adr));
    puts("Connected...........");

    for (int m = 0; m < 5; m++) {
        fputs("input count of number : ", stdout);
        int cnt;
        scanf("%d", &cnt);
        body[0] = cnt;
        for (int i = 0; i < cnt; ++i) {
            printf("请输入第%d个数字:", i + 1);
            scanf("%d", &body[i + 1]);
        }
        printf("请输入第操作符(+,-,*,/):");
        while (getchar() != '\n');
        char c;
        scanf("%c", &c);
        body[cnt + 1] = c;

        write(sock, body, (cnt + 2) * 4);
        int result;
        if ((str_len = read(sock, &result, 4)) < 0) {
            error_handling("read() error");
        }

        printf("Message from server: %d\n", result);
    }

    close(sock);
    return 0;
}
