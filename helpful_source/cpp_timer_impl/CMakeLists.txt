cmake_minimum_required(VERSION 3.9)
project(Boost_test)
set(CMAKE_EXE_LINKER_FLAGS "-lboost_system -lpthread")
set(CMAKE_CXX_STANDARD 11)

add_executable(Boost_test main.cpp Board.cpp Board.h)