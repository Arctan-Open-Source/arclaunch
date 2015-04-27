# Copy the script to the binary directory
file(COPY
    scripts/main.pl
  DESTINATION
    ${CMAKE_CURRENT_BINARY_DIR}/scripts)

# The perl argument echo-back test
ADD_ARGV_TEST("perl" "${CMAKE_CURRENT_SOURCE_DIR}/xml/launch.xml")
