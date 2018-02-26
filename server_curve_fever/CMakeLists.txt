cmake_minimum_required(VERSION 3.9)
project(server_curve_fever)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(server_curve_fever main.cpp GameServer.cpp GameServer.h Player.cpp Player.h TcpServer.cpp TcpServer.h
        SafeQueue.h Board.cpp Board.h Message.h UdpServer.cpp UdpServer.h Point.cpp Point.h)