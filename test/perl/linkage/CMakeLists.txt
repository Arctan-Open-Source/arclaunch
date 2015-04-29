file(COPY
    scripts/from.pl
    scripts/to.pl
  DESTINATION
    ${CMAKE_CURRENT_BINARY_DIR}/scripts)

ADD_LINKAGE_TEST("perl" "${CMAKE_CURRENT_SOURCE_DIR}/xml/launch.xml")
