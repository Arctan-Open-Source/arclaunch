cmake_minimum_required(VERSION 2.8)
project(ws_example)
find_package(arclaunch)
find_package(Threads)

configure_file(xml/launch.xml.in ${CMAKE_CURRENT_BINARY_DIR}/xml/launch.xml)

include_directories(include)

add_subdirectory(src)
add_subdirectory(include)

