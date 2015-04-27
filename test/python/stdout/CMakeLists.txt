
file(COPY
    scripts/main.py
  DESTINATION
    ${CMAKE_CURRENT_BINARY_DIR}/scripts)

ADD_STDOUT_TEST("python" "${CMAKE_CURRENT_SOURCE_DIR}/xml/launch.xml")
