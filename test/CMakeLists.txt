add_subdirectory(resources)
add_subdirectory(gtest)
enable_testing()

add_executable(xsd_test 
  xsd.cxx
  )
target_link_libraries(xsd_test 
  gtest_main
  arclaunch
  )

add_test(NAME test_xsd COMMAND xsd_test)
