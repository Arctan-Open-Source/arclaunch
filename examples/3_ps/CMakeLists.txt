cmake_minimum_required(VERSION 2.8)
project(ps_example)

find_package(arclaunch 1.0.1 REQUIRED)
configure_file(xml/launch.xml.in ${CMAKE_CURRENT_BINARY_DIR}/xml/launch.xml)
