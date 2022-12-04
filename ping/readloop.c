#include <signal.h>
#include <errno.h>
#include <net/if.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <sys/ioctl.h>
#include    "ping.h"

void bind_to_interface(int raw , char *device , int protocol) { 
    struct sockaddr_ll sll;
    struct ifreq ifr; bzero(&sll , sizeof(sll));
    bzero(&ifr , sizeof(ifr)); 
    strncpy((char *)ifr.ifr_name ,device , IFNAMSIZ); 
    //copy device name to ifr 
    if((ioctl(raw , SIOCGIFINDEX , &ifr)) == -1)
    { 
        perror("Unable to find interface index");
        exit(-1); 
    }

//  if (setsockopt(raw, SOL_SOCKET, SO_BINDTODEVICE, (void *)&ifr, sizeof(ifr)) < 0)
    if (setsockopt(raw, SOL_SOCKET, SO_BINDTODEVICE, ifr.ifr_name, sizeof(ifr.ifr_name)) < 0)

  {
    perror("Server-setsockopt() error for SO_BINDTODEVICE");
    printf("%s\n", strerror(errno));
    close(raw);
    exit(-1);
  }


    sll.sll_family = pr->sasend->sa_family;
    sll.sll_ifindex = ifr.ifr_ifindex; 
    sll.sll_protocol = htons(protocol); 
    if((bind(raw , ifr.ifr_name , sizeof(ifr.ifr_name))) ==-1)
    {
        perror("bind: ");
        exit(-1);
    }
} 
void
readloop(void) {
    int size;
    char recvbuf[BUFSIZE];
    char controlbuf[BUFSIZE];
    struct msghdr msg;
    struct iovec iov;
    ssize_t n;
    struct timeval tval;

    sockfd = Socket(pr->sasend->sa_family, SOCK_RAW, pr->icmpproto);
    if (strlen(interface) > 0) // set interface
      bind_to_interface(sockfd, interface, pr->icmpproto);
    setuid(getuid()); /* don't need special permissions any more */
    if (pr->finit)
        (*pr->finit)();

    size = 60 * 1024;        /* OK if setsockopt fails */
    setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));

    sig_alrm(SIGALRM);        /* send first packet */

    iov.iov_base = recvbuf;
    iov.iov_len = sizeof(recvbuf);
    msg.msg_name = pr->sarecv;
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = controlbuf;
    for (;;) {
        msg.msg_namelen = pr->salen;
        msg.msg_controllen = sizeof(controlbuf);
        n = recvmsg(sockfd, &msg, 0);
        if (n < 0) {
            if (errno == EINTR)
                continue;
            else
                fprintf(stderr, "recvmsg error");
        }

//        Gettimeofday(&tval, NULL);
        (*pr->fproc)(recvbuf, n, &msg, &tval);
    }
}
