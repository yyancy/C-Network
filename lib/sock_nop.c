//
// Created by yyancy on 2019/12/3.
//

#include "udp.h"

/**
 * 将网络地址转换为可读的地址
 * @param sa
 * @param len
 * @return
 */
char *sock_ntop(const struct sockaddr *sa, socklen_t len) {
    char portstr[8];
    static char str[128];
    switch (sa->sa_family) {
        case AF_INET: {
            struct sockaddr_in *sin = (struct sockaddr_in *) sa;
            if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL) {
                return NULL;
            }
            if (ntohs(sin->sin_port) != 0) {
                snprintf(portstr, sizeof(portstr), ":%d", ntohs(sin->sin_port));
                strcat(str, portstr);
            }
            return str;
        }
    }
    return NULL;
}