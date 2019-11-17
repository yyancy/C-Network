




add_executable(calculate_server calculate_server.c)
target_link_libraries(calculate_server udp)


add_executable(calculate_client calculate_client.c calculate.h)
target_link_libraries(calculate_client udp)
