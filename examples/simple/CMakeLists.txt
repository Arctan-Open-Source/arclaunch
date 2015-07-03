cmake_minimum_required(VERSION 2.8)
project(simple_example)
find_package(arclaunch 1.0.1 REQUIRED)
configure_file(xml/launch.xml.in xml/launch.xml)
