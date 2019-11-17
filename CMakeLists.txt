cmake_minimum_required(VERSION 3.10)
project(learning_network C)

set(CMAKE_C_STANDARD 99)

# The version number.
set(Tutorial_VERSION_MAJOR 1)
#add_compile_options(-lpthread)
#set (Tutorial_VERSION_MINOR 0)

#configure_file (
#        "${PROJECT_SOURCE_DIR}/lib/udp.c"
#)
# 添加本地目录为搜索路径
include_directories(${PROJECT_SOURCE_DIR})


add_subdirectory(echo)
add_subdirectory(calculator)
add_subdirectory(lib)
add_subdirectory(thread)
add_subdirectory(chat)

add_executable(learning_network main.c)