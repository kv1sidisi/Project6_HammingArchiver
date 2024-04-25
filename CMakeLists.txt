cmake_minimum_required(VERSION 3.26)
project(Archivator)

set(CMAKE_CXX_STANDARD 14)

add_executable(Archivator
        main.cpp
        Archive.cpp
        Hamming.cpp
        Parcer.cpp)
