
#add_compile_options(-lpthread)
#find_package(Threads)
#
add_executable(tcprelay tcprelay.c)
#add_executable(tcprelay tcprelay.c)
#
target_link_libraries(tcprelay ${CMAKE_THREAD_LIBS_INIT} udp)
#target_link_libraries(chat_clnt ${CMAKE_THREAD_LIBS_INIT} udp)
