cmake_minimum_required(VERSION 2.8)
project(http_gunzip_example)

find_package(ARCLAUNCH 1.0.1 REQUIRED)

configure_file(xml/launch.xml.in ${CMAKE_CURRENT_BINARY_DIR}/xml/launch.xml)
