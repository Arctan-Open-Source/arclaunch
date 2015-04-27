file(COPY
    scripts/main.pl
  DESTINATION
    ${CMAKE_CURRENT_BINARY_DIR}/scripts)

set(ENV "${CMAKE_CURRENT_BINARY_DIR}/scripts/main.pl" PARENT_SCOPE)
set(ENV_LAUNCH "${CMAKE_CURRENT_SOURCE_DIR}/xml/launch.xml" PARENT_SCOPE)
