cmake_minimum_required(VERSION 3.9)
project(CLion_Project)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(CLion_Project main.cpp game_server.cpp game_server.h player.cpp player.h tcp_server.cpp tcp_server.h
        safe_queue.h)