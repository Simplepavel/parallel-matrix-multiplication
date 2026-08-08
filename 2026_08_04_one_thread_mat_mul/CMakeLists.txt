cmake_minimum_required(VERSION 3.10)

project(one_thread_matmul)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

add_executable(main main.cpp)