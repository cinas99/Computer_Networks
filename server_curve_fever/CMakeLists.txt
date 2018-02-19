cmake_minimum_required(VERSION 3.9)
project(server_curve_fever)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(server_curve_fever main.cpp game_server.cpp game_server.h player.cpp player.h tcp_server.cpp tcp_server.h
        safe_queue.h)