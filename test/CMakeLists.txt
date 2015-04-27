add_subdirectory(gtest)
enable_testing()

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/resources ${CMAKE_CURRENT_SOURCE_DIR}/gtest/include)
set(LOCATION_IN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/locations.hxx.in)
set(ARGV_CXX_FILE ${CMAKE_CURRENT_SOURCE_DIR}/argv.cxx)
set(ENV_CXX_FILE ${CMAKE_CURRENT_SOURCE_DIR}/env.cxx)

# A matching test is built to run for each language
function(ADD_ARGV_TEST TEST_NAME)
  configure_file(${LOCATION_IN_FILE} ${CMAKE_CURRENT_BINARY_DIR}/locations.hxx)
  include_directories(${CMAKE_CURRENT_BINARY_DIR})
  add_executable(argv_${TEST_NAME}_test
    ${ARGV_CXX_FILE}
    )
  target_link_libraries(argv_${TEST_NAME}_test
    gtest_main
    arclaunch
    )
  add_test(NAME test_${TEST_NAME}_argv COMMAND argv_${TEST_NAME}_test)
endfunction(ADD_ARGV_TEST)

function(ADD_ENV_TEST TEST_NAME)
  configure_file(${LOCATION_IN_FILE} ${CMAKE_CURRENT_BINARY_DIR}/locations.hxx)
  include_directories(${CMAKE_CURRENT_BINARY_DIR})
  add_executable(env_${TEST_NAME}_test
    ${ENV_CXX_FILE}
    )
  target_link_libraries(env_${TEST_NAME}_test
    gtest_main
    arclaunch
    )
  add_test(NAME test_${TEST_NAME}_env COMMAND env_${TEST_NAME}_test)
endfunction(ADD_ENV_TEST)

# Test for runnning binaries
add_subdirectory(binary)

# Test for running python scripts
add_subdirectory(python)

# Test for running perl scripts
add_subdirectory(perl)

# Test for running R scripts
add_subdirectory(R)
