
#add_compile_options(-lpthread)
find_package(Threads)

add_executable(thread1 thread1.c)
add_executable(calculate_sum calculate_sum.c)
add_executable(mutex mutex.c)
add_executable(semaphore semaphore.c)

target_link_libraries(thread1 ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(calculate_sum ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(mutex ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(semaphore ${CMAKE_THREAD_LIBS_INIT})
