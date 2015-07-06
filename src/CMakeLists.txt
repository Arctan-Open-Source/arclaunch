
configure_file(xsd/launch.xsd.in ${CMAKE_CURRENT_BINARY_DIR}/xsd/launch.xsd)

set(LAUNCH_XML_SCHEMA ${CMAKE_CURRENT_BINARY_DIR}/xsd/launch.xsd PARENT_SCOPE)
XSD_SCHEMA( LAUNCH_SRC ${CMAKE_CURRENT_BINARY_DIR}/xsd/launch.xsd --std c++11 --generate-polymorphic --polymorphic-type node_t )

set(LAUNCH_SRC "${LAUNCH_SRC}" PARENT_SCOPE)
set(LAUNCH_HEADER "${LAUNCH_SRC_HEADER}" PARENT_SCOPE)

add_library(arclaunch SHARED
  ${LAUNCH_SRC}
  Node.cxx
  NodeStorage.cxx
  NodeContext.cxx
  # FileNode.cxx
  ExecutableNode.cxx
  ScriptNode.cxx
  GroupNode.cxx
  LaunchNode.cxx
  SequenceNode.cxx
  SocketNode.cxx
  )

target_link_libraries(arclaunch 
  ${XSD_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
  )

add_executable(launch
  main.cxx
  )

target_link_libraries(launch arclaunch)
set_target_properties(launch PROPERTIES OUTPUT_NAME arclaunch)

# CPack introduces some complexity here
set(LAUNCH_XML_SCHEMA_RELATIVE_PATH "share/arclaunch/launch.xsd" PARENT_SCOPE)
set(LAUNCH_EXECUTABLE_RELATIVE_PATH "bin/arclaunch" PARENT_SCOPE)
set(LAUNCH_LIBRARIES_RELATIVE_PATH "lib/libarclaunch.so" PARENT_SCOPE)

install(
  TARGETS
    launch
    arclaunch
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  )

install(
  FILES
    xsd/launch.xsd
  DESTINATION
    share/arclaunch
    )
