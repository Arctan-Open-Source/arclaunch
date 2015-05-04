
XSD_SCHEMA( LAUNCH_SRC xsd/launch.xsd --std c++11 --generate-polymorphic --polymorphic-type node_t )

set(LAUNCH_SRC "${LAUNCH_SRC}" PARENT_SCOPE)
set(LAUNCH_HEADER "${LAUNCH_SRC_HEADER}" PARENT_SCOPE)

add_library(arclaunch
  ${LAUNCH_SRC}
  Node.cxx
  NodeStorage.cxx
  NodeContext.cxx
  FileNode.cxx
  ExecutableNode.cxx
  ScriptNode.cxx
  LaunchNode.cxx
  )

target_link_libraries(arclaunch 
  ${XSD_LIBRARIES}
  )

add_executable(launch
  main.cxx
  )

target_link_libraries(launch arclaunch)
