#include <signal.h>
#include "ping.h"

struct proto proto_v4 = {proc_v4,
                         send_v4, NULL,
                         NULL,
                         NULL,
                         0,
                         IPPROTO_ICMP};

#ifdef    IPV6
struct proto	proto_v6 = { proc_v6, send_v6, init_v6, NULL, NULL, 0, IPPROTO_ICMPV6 };
#endif

int datalen = 56;        /* data that goes with ICMP echo request */



void tv_sub(struct timeval *out, struct timeval *in) {
    if ((out->tv_usec -= in->tv_usec) < 0) {
        --out->tv_sec;
        out->tv_usec += 1000000;
    }
    out->tv_sec -= in->tv_sec;
}

int
main(int argc, char **argv) {
    int c;
    struct addrinfo *ai;
    char *h;

    opterr = 0;        /* don't want getopt() writing to stderr */
    while ((c = getopt(argc, argv, "vI:")) != -1) {
        switch (c) {
            case 'v':
                verbose++;
                break;
          case 'I':
            bzero(&interface , sizeof(interface));
            int len;
            if ((len = strlen(optarg))> INTERFACE_SIZE -1 || len<=0){
              fprintf(stderr, "The interface too long to fill the buffer: %s\n", optarg);
              exit(1);
            }

            strncpy((char *)interface ,optarg , strlen(optarg));
            break;
            case '?':
                fprintf(stderr, "unrecognized option: %s", opterr);
                exit(1);
        }
    }

    if (optind != argc - 1)
        fprintf(stderr, "usage: ping [ -v ] <hostname>");
    host = argv[optind];

    pid = getpid() & 0xffff;    /* ICMP ID field is 16 bits */
    signal(SIGALRM, sig_alrm);

    ai = Host_serv(host, NULL, 0, 0);

    h = Sock_ntop_host(ai->ai_addr, ai->ai_addrlen);
    printf("PING %s (%s): %d data bytes\n",
           ai->ai_canonname ? ai->ai_canonname : h,
           h, datalen);

    /* 4initialize according to protocol */
    if (ai->ai_family == AF_INET) {
        pr = &proto_v4;
#ifdef    IPV6
        } else if (ai->ai_family == AF_INET6) {
        pr = &proto_v6;
        if (IN6_IS_ADDR_V4MAPPED(&(((struct sockaddr_in6 *)
                                 ai->ai_addr)->sin6_addr)))
            err_quit("cannot ping IPv4-mapped IPv6 address");
#endif
    } else
        fprintf(stderr, "unknown address family %d", ai->ai_family);

    pr->sasend = ai->ai_addr;
    pr->sarecv = calloc(1, ai->ai_addrlen);
    pr->salen = ai->ai_addrlen;

    readloop();

    exit(0);
}
