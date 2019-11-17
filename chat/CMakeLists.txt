
#add_compile_options(-lpthread)
find_package(Threads)

add_executable(chat_server chat_server.c)
add_executable(chat_clnt chat_clnt.c)

target_link_libraries(chat_server ${CMAKE_THREAD_LIBS_INIT} udp)
target_link_libraries(chat_clnt ${CMAKE_THREAD_LIBS_INIT} udp)
