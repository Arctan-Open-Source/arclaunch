file(COPY
    scripts/main.pl
  DESTINATION
    ${CMAKE_CURRENT_BINARY_DIR}/scripts)
ADD_STDOUT_TEST("perl" "${CMAKE_CURRENT_SOURCE_DIR}/xml/launch.xml")
