cmake_minimum_required(VERSION 3.0)
project(RayTracing LANGUAGES CXX )


set(CMAKE_CXX_STANDARD 17 )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra -pedantic -Wconversion")

add_subdirectory(external)
include_directories(.)

add_subdirectory(raytracing)


add_executable(example example.cpp)
target_include_directories(example PRIVATE raytracing/include)
target_link_libraries(example raytracing external png)
