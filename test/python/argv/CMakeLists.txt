# Copy the script to the binary directory
file(COPY 
    scripts/main.py 
  DESTINATION 
    ${CMAKE_CURRENT_BINARY_DIR}/scripts)

ADD_ARGV_TEST("python" "${CMAKE_CURRENT_SOURCE_DIR}/xml/launch.xml")
