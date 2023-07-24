cmake_minimum_required(VERSION 3.25)
project(cpp_projects)

set(CMAKE_CXX_STANDARD 17)

add_executable(cpp_projects main.cpp)
target_include_directories(cpp_projects PRIVATE ../../../Downloads)