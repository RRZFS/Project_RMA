cmake_minimum_required(VERSION 3.10)

project(UniversalDBProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(UniversalDB
    main.cpp
    UniversalDB.cpp
    UniversalDB.h
)