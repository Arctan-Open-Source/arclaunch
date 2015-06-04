file(COPY
    scripts/main.pl
  DESTINATION
    ${CMAKE_CURRENT_BINARY_DIR}/scripts)
configure_file(xml/launch.xml.in ${CMAKE_CURRENT_BINARY_DIR}/xml/launch.xml)

ADD_STDIN_TEST("perl" "${CMAKE_CURRENT_BINARY_DIR}/xml/launch.xml")
