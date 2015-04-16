
add_subdirectory(argv)
configure_file(locations.hxx.in ${CMAKE_CURRENT_BINARY_DIR}/locations.hxx)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(binary_test
  binary.cxx
  )
target_link_libraries(binary_test
  gtest_main
  arclaunch
  )

add_test(NAME test_binary COMMAND binary_test)
message(STATUS "argv binary executable:  ${ARGV_BINARY}")
message(STATUS "argv binary launch file: ${ARGV_BINARY_LAUNCH}")
