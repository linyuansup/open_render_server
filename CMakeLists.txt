cmake_minimum_required(VERSION 3.26)
project(open_server_engine)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

set(src
    open_render_server.cpp)

add_subdirectory(open_render_engine)

add_executable(open_render_server ${src})

target_link_libraries(open_render_server open_render_engine)
