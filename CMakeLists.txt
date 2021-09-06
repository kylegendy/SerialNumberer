cmake_minimum_required(VERSION 3.18)
project(SerialNumberer)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(SerialNumberer
        test/test.cpp
        include/SerialNumberer.h
        src/SerialNumberer.cpp)
