


add_library(udp udp.c getaddrinfo.c gai_strerror.c sock_ntop_host.c error.c udp.h sock_nop.c signal.c sigchldwait.c)


add_executable(sign_test sign_test.c)
target_link_libraries(sign_test udp)