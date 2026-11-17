cmake_minimum_required(VERSION 4.2)
project(Capstone)

set(CMAKE_CXX_STANDARD 17)

add_executable(Capstone main.cpp
        inventory.h
        inventory.cpp
        data_structures.h
        data_structures.cpp
        checkout.h
        checkout.cpp)
