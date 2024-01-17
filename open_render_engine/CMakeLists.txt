cmake_minimum_required(VERSION 3.26)
project(open_render_engine)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

set(src
    render_engine.cpp)

add_subdirectory(v5sdk)

add_library(open_render_engine SHARED ${src})

target_link_libraries(open_render_engine libV5SDK)
