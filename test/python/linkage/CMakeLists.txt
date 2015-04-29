file(COPY
    scripts/from.py
    scripts/to.py
  DESTINATION
    ${CMAKE_CURRENT_BINARY_DIR}/scripts)

ADD_LINKAGE_TEST("python" "${CMAKE_CURRENT_SOURCE_DIR}/xml/launch.xml")
