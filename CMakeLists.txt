cmake_minimum_required(VERSION 3.10)

# Define the project name
project(UniversalDBProject)

# Set the C++ standard (C++11, C++14, C++17, etc.)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Create the executable
# Syntax: add_executable(TargetName SourceFiles...)
add_executable(UniversalDB
    main.cpp
    UniversalDB.cpp
    UniversalDB.h
)

# Optional: Add include directories if you eventually have a specific include folder
# target_include_directories(UniversalDB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})