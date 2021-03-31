//
// Created by yyancy on 2019/12/4.
//

#include "hostent.h"
#include "lib/udp.h"


void lookup_service() {
    struct servent *sptr;
    sptr = getservbyname("domain", "udp");
    printf("hostname: %s\n", sptr->s_name);
}

int main(int argc, char **argv) {
    char *ptr, **pptr;
    char str[INET_ADDRSTRLEN];
    struct hostent *hptr;

    lookup_service();

    while (--argc > 0) {
        ptr = *++argv;
        if ((hptr = gethostbyname(ptr)) == NULL) {
            err_quit("gethostbyname error for host: %s %s", ptr, hstrerror(h_errno));
            continue;
        }
        printf("official hostname: %s\n", hptr->h_name);

        for (pptr = hptr->h_aliases; *pptr != NULL; pptr++) {
            printf("\talias: %s\n", *pptr);
        }

        switch (hptr->h_addrtype) {
            case AF_INET:
                pptr = hptr->h_addr_list;
                printf("\taddress: %s\n",
                       inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
                break;
            default:
                error_handling("unknown address type");
                break;
        }
    }


    return 0;
}