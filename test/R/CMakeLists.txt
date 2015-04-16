
if(R_INTERPRETER)
  message(STATUS "creating R tests")
  add_subdirectory(argv)
  configure_file(locations.hxx.in ${CMAKE_CURRENT_BINARY_DIR}/locations.hxx)
  include_directories(${CMAKE_CURRENT_BINARY_DIR})
  add_executable(r_test
    R.cxx
    )
  target_link_libraries(r_test
    gtest_main
    arclaunch
    )
  add_test(NAME test_r COMMAND r_test)
  message(STATUS "argv R script:           ${ARGV_R}")
  message(STATUS "argv R launch file:      ${ARGV_R_LAUNCH}")
else(R_INTERPRETER)
  message(STATUS "Could not locate R executable")
endif(R_INTERPRETER)
