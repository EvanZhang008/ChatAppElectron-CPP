cmake_minimum_required(VERSION 3.13)
project(core)

set(CMAKE_CXX_STANDARD 14)

add_executable(core server.cpp client.cpp main.cpp server.h client.h)