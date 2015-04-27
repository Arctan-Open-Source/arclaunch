file(COPY
    scripts/main.py
  DESTINATION
    ${CMAKE_CURRENT_BINARY_DIR}/scripts)

set(ENV "${CMAKE_CURRENT_BINARY_DIR}/scripts/main.py" PARENT_SCOPE)
set(ENV_LAUNCH "${CMAKE_CURRENT_SOURCE_DIR}/xml/launch.xml" PARENT_SCOPE)
