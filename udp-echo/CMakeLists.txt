
#add_compile_options(-lpthread)
#find_package(Threads)
#
add_executable(uecho_server uecho_server.c)
add_executable(uecho_client uecho_client.c)
add_executable(file-example file.c)
add_executable(myls myls.c)
#add_executable(tcprelay tcprelay.c)
#
target_link_libraries(uecho_server ${CMAKE_THREAD_LIBS_INIT} udp)


target_link_libraries(uecho_client ${CMAKE_THREAD_LIBS_INIT} udp)
#target_link_libraries(chat_clnt ${CMAKE_THREAD_LIBS_INIT} udp)
