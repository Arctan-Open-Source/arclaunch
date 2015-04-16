

add_test(NAME test_python COMMAND python_test)
find_package(PythonInterp)
if(PYTHONINTERP_FOUND)
  message(STATUS "Creating Python tests")
  add_subdirectory(argv)
  configure_file(locations.hxx.in ${CMAKE_CURRENT_BINARY_DIR}/locations.hxx)
  include_directories(${CMAKE_CURRENT_BINARY_DIR})
  add_executable(python_test
    python.cxx
    )
  target_link_libraries(python_test
    gtest_main
    arclaunch
    )
  message(STATUS "argv python script:      ${ARGV_PYTHON}")
  message(STATUS "argv python launch file: ${ARGV_PYTHON_LAUNCH}")
else(PYTHONINTERP_FOUND)
  message(STATUS "Could not locate Python executable")
endif(PYTHONINTERP_FOUND)
