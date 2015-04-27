# Copy the script to the binary directory
file(COPY
    scripts/main.pl
  DESTINATION
    ${CMAKE_CURRENT_BINARY_DIR}/scripts)

# The perl argument echo-back test
set(ARGV "${CMAKE_CURRENT_SOURCE_DIR}/scripts/main.pl" PARENT_SCOPE)
set(ARGV_LAUNCH "${CMAKE_CURRENT_SOURCE_DIR}/xml/launch.xml" PARENT_SCOPE)
