




add_executable(echo_server echo_server.c)
target_link_libraries(echo_server udp)

add_executable(echo_selectserv echo_selectserv.c)
target_link_libraries(echo_selectserv udp)

add_executable(echo_epollserv echo_epollserv.c)
target_link_libraries(echo_epollserv udp)

add_executable(echo_epollserv2 echo_epollserv2.c)
target_link_libraries(echo_epollserv2 udp)
add_executable(echo_pollserv echo_pollserv.c)
target_link_libraries(echo_pollserv udp)

add_executable(echo_client echo_client.c)
target_link_libraries(echo_client udp)

add_executable(pingpong_client2 pingpong_client2.c)
target_link_libraries(pingpong_client2 udp)