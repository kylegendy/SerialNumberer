cmake_minimum_required(VERSION 3.19)
project(SerialNumberer)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(SerialNumberer
        include/SerialNumberer.h
        src/SerialNumberer.cpp)
