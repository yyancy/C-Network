
#add_compile_options(-lpthread)
find_package(Threads)

add_executable(ping main.c
        readloop.c
        sig_alrm.c
        send_v4.c
        in_cksum.c
        proc_v4.c
        host_serv.c)
#add_executable(chat_clnt chat_clnt.c)
#
target_link_libraries(ping ${CMAKE_THREAD_LIBS_INIT} udp)
#target_link_libraries(chat_clnt ${CMAKE_THREAD_LIBS_INIT} udp)
