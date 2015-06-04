# Copy the script to the binary directory
file(COPY
    scripts/main.pl
  DESTINATION
    ${CMAKE_CURRENT_BINARY_DIR}/scripts)
configure_file(xml/launch.xml.in ${CMAKE_CURRENT_BINARY_DIR}/xml/launch.xml)

# The perl argument echo-back test
ADD_ARGV_TEST("perl" "${CMAKE_CURRENT_BINARY_DIR}/xml/launch.xml")
