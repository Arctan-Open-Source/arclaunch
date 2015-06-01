file(COPY
    scripts/from.py
    scripts/to.py
  DESTINATION
    ${CMAKE_CURRENT_BINARY_DIR}/scripts)

configure_file(xml/launch.xml.in ${CMAKE_CURRENT_BINARY_DIR}/xml/launch.xml)
ADD_LINKAGE_TEST("python" "${CMAKE_CURRENT_BINARY_DIR}/xml/launch.xml")
