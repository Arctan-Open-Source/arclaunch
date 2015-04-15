add_subdirectory(gtest)
add_subdirectory(resources)
enable_testing()

include_directories(${CMAKE_CURRENT_BINARY_DIR}/resources)

add_executable(xsd_test 
  xsd.cxx
  )
target_link_libraries(xsd_test 
  gtest_main
  arclaunch
  )

add_test(NAME test_xsd COMMAND xsd_test)
