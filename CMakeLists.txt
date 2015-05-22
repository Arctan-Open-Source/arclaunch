cmake_minimum_required(VERSION 2.8)
project(tryxsd)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Later versions of cmake support this
set(CMAKE_CXX_STANDARD 11)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
  message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

find_package(XSD REQUIRED)
find_package(Threads REQUIRED)

include_directories(include ${XSD_INCLUDE_DIR} ${CMAKE_BINARY_DIR}/src)

add_subdirectory(src)
add_subdirectory(include)

add_subdirectory(test)
