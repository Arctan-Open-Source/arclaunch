add_subdirectory(src)
configure_file(xml/launch.xml.in ${CMAKE_CURRENT_BINARY_DIR}/xml/launch.xml)
ADD_ARGV_TEST("binary" "${CMAKE_CURRENT_BINARY_DIR}/xml/launch.xml")
