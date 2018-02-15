cmake_minimum_required(VERSION 3.9)
project(Testowy3)

set(CMAKE_CXX_STANDARD 11)

add_executable(Testowy3 main.cpp PrepareGame.cpp PrepareGame.h Player.cpp Player.h UdpServer.cpp UdpServer.h)