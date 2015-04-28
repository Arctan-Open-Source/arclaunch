add_subdirectory(gtest)
enable_testing()

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/resources ${CMAKE_CURRENT_SOURCE_DIR}/gtest/include)
set(ARGV_CXX_FILE ${CMAKE_CURRENT_SOURCE_DIR}/argv.cxx)
set(ENV_CXX_FILE ${CMAKE_CURRENT_SOURCE_DIR}/env.cxx)
set(STDOUT_CXX_FILE ${CMAKE_CURRENT_SOURCE_DIR}/stdout.cxx)
set(STDIN_CXX_FILE ${CMAKE_CURRENT_SOURCE_DIR}/stdin.cxx)

# 
function(TEST_TEMPLATE TEMPLATE_NAME TEST_NAME LAUNCH_FILE)
  # Should generate the configured file
  # Capitalize the TEMPLATE_NAME
  string(TOUPPER ${TEMPLATE_NAME} CAP_TEMPLATE_NAME)
  set(${CAP_TEMPLATE_NAME}_LAUNCH ${LAUNCH_FILE})
  set(LOCATION_CONTENT_LINE1
    "#cmakedefine ${CAP_TEMPLATE_NAME} \"@${CAP_TEMPLATE_NAME}@\"\n")
  set(LOCATION_CONTENT_LINE2
    "#cmakedefine ${CAP_TEMPLATE_NAME}_LAUNCH \"@${CAP_TEMPLATE_NAME}_LAUNCH@\"\n")
  set(LOCATION_IN_FILE "${CMAKE_CURRENT_BINARY_DIR}/locations.hxx.in")
  file(WRITE ${LOCATION_IN_FILE} "${LOCATION_CONTENT_LINE1}")
  file(APPEND ${LOCATION_IN_FILE} "${LOCATION_CONTENT_LINE2}")
  configure_file(${LOCATION_IN_FILE} "${CMAKE_CURRENT_BINARY_DIR}/locations.hxx")
  include_directories("${CMAKE_CURRENT_BINARY_DIR}")
  add_executable(${TEMPLATE_NAME}_${TEST_NAME}_test
    ${${CAP_TEMPLATE_NAME}_CXX_FILE}
    )
  target_link_libraries(${TEMPLATE_NAME}_${TEST_NAME}_test
    gtest_main
    arclaunch
    )
  add_test(NAME test_${TEST_NAME}_${TEMPLATE_NAME} COMMAND ${TEMPLATE_NAME}_${TEST_NAME}_test)
endfunction(TEST_TEMPLATE)

# Matching tests are built to run for each language
function(ADD_STDOUT_TEST TEST_NAME LAUNCH_FILE)
  TEST_TEMPLATE(stdout ${TEST_NAME} ${LAUNCH_FILE})
endfunction(ADD_STDOUT_TEST)

function(ADD_ARGV_TEST TEST_NAME LAUNCH_FILE)
  TEST_TEMPLATE(argv ${TEST_NAME} ${LAUNCH_FILE})
endfunction(ADD_ARGV_TEST)

function(ADD_ENV_TEST TEST_NAME LAUNCH_FILE)
  TEST_TEMPLATE(env ${TEST_NAME} ${LAUNCH_FILE})
endfunction(ADD_ENV_TEST)

function(ADD_STDIN_TEST TEST_NAME LAUNCH_FILE)
  TEST_TEMPLATE(stdin ${TEST_NAME} ${LAUNCH_FILE})
endfunction(ADD_STDIN_TEST)

# Test for runnning binaries
add_subdirectory(binary)

# Test for running python scripts
add_subdirectory(python)

# Test for running perl scripts
add_subdirectory(perl)

# Test for running R scripts
add_subdirectory(R)
