cmake_minimum_required(VERSION 2.8)
project(arclaunch)

set(ARCLAUNCH_VERSION_MAJOR 1)
set(ARCLAUNCH_VERSION_MINOR 1)
set(ARCLAUNCH_VERSION_PATCH 0)

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

include_directories(include ${XSD_INCLUDE_DIR} ${CMAKE_BINARY_DIR}/src ${CMAKE_BINARY_DIR}/include)

add_subdirectory(src)
add_subdirectory(include)

set(LAUNCH_CONFIG_RELATIVE_PATH share/arclaunch)
configure_file(cmake/arclaunch-config.cmake.in 
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/arclaunch-config.cmake @ONLY)
configure_file(cmake/arclaunch-config-version.cmake.in 
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/arclaunch-config-version.cmake @ONLY)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/arclaunch-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/arclaunch-config-version.cmake
  DESTINATION
    share/arclaunch
    )

if(GENERATE_PACKAGE)
  set(CPACK_PACKAGE_VENDOR "Arctan, Inc.")
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A utility for starting up and managing a set of programs in parallel.")
  set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.md")
  set(CPACK_PACKAGE_CONTACT "matthew.balch@arctan-group.com")
  set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
  set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/txt_readme.txt")
  set(CPACK_PACKAGE_VERSION_MAJOR "${ARCLAUNCH_VERSION_MAJOR}")
  set(CPACK_PACKAGE_VERSION_MINOR "${ARCLAUNCH_VERSION_MINOR}")
  set(CPACK_PACKAGE_VERSION_PATCH "${ARCLAUNCH_VERSION_PATCH}")
  set(CPACK_PACKAGE_EXECUTABLES "arclaunch")
  
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "xsdcxx (>= 4.0.0) | xsd (>= 4.0.0)")
  set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
  set(CPACK_DEBIAN_PACKAGE_PRIORITY "extra")
  
  set(CPACK_RPM_PACKAGE_LICENSE "${CPACK_RESOURCE_FILE_LICENSE}")
  set(CPACK_RPM_PACKAGE_GROUP "Development/Tools")
  set(CPACK_RPM_PACKAGE_REQUIRES "xsd >= 4.0.0")
  set(CPACK_RPM_PACKAGE_ARCHITECTURE "x86_64")

  include(CPack)
endif(GENERATE_PACKAGE)

if(ENABLE_TEST)
  add_subdirectory(test)
endif(ENABLE_TEST)

