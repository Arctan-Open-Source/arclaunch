add_subdirectory(src)
set(ENV "${ENV_BINARY}" PARENT_SCOPE)
set(ENV_LAUNCH "${CMAKE_CURRENT_SOURCE_DIR}/xml/launch.xml" PARENT_SCOPE)
