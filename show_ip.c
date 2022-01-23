#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

int inet_pton_loose(int family, const char *strptr, void *addrptr) {
  int status;
  if ((status = inet_pton(family, strptr, addrptr)) < 0) return status;
  if (status == 0 && (status = inet_aton(strptr, addrptr)) <= 0) return status;
  return 1;
}
int main(int argc, char *args[]) {
  struct sockaddr_in sci;
  int ret = inet_pton_loose(AF_INET, "abc", &sci.sin_addr);

  uint8_t *buf = (uint8_t *)&sci.sin_addr;
  printf("ret = %d, ip = %d, %d, %d, %d\n", ret, buf[0], buf[1], buf[2],
         buf[3]);
  INADDR_ANY;
  // ---
  int status;
  struct addrinfo hints;
  struct addrinfo *servinfo;  // will point to the results

  memset(&hints, 0, sizeof hints);  // make sure the struct is empty
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;  // TCP stream sockets
  hints.ai_flags = AI_PASSIVE;
  // get ready to connect
  // status = getaddrinfo("www.baidu.com", "80", &hints, &servinfo);
  status = getaddrinfo(args[1], "80", &hints, &servinfo);
  //
  if (status != 0) {
    fprintf(stderr, "getaddrinfo error %s\n", gai_strerror(status));
    exit(1);
  }
  struct addrinfo *ap = servinfo;
  for (; ap != NULL; ap = ap->ai_next) {
    if (ap->ai_family == AF_INET) {
      char ip4[INET_ADDRSTRLEN];
      struct sockaddr_in *sa = ((struct sockaddr_in *)ap->ai_addr);
      inet_ntop(AF_INET, &(sa->sin_addr), ip4, INET_ADDRSTRLEN);
      printf("name %s ip4 = %s\n", ap->ai_canonname, ip4);
    } else if (ap->ai_family == AF_INET6) {
      char ip6[INET6_ADDRSTRLEN];
      struct sockaddr_in6 *sa = ((struct sockaddr_in6 *)ap->ai_addr);
      inet_ntop(AF_INET6, &(sa->sin6_addr), ip6, INET6_ADDRSTRLEN);
      printf("name %s ip6 = %s\n", ap->ai_canonname, ip6);
    }
  }

  freeaddrinfo(servinfo);
}
