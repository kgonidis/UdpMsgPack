cmake_minimum_required(VERSION 3.14)
project(UdpMsgPack)

set(CMAKE_CXX_STANDARD 14)

include_directories("/home/kiron/CLionProjects/msgpack-c/include/")

add_executable(UdpMsgPack main.cpp)