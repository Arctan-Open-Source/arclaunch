
set(LAUNCH_XML_SCHEMA ${CMAKE_CURRENT_SOURCE_DIR}/xsd/launch.xsd PARENT_SCOPE)
XSD_SCHEMA( LAUNCH_SRC xsd/launch.xsd --std c++11 --generate-polymorphic --polymorphic-type node_t )

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

set(INSTALLED_LAUNCH_XML_SCHEMA "${CMAKE_INSTALL_PREFIX}/share/arclaunch/launch.xsd" PARENT_SCOPE)
set(INSTALLED_LAUNCH_EXECUTABLE "${CMAKE_INSTALL_PREFIX}/bin/launch" PARENT_SCOPE)
set(INSTALLED_LAUNCH_LIBRARIES "${CMAKE_INSTALL_PREFIX}/lib/libarclaunch.so" PARENT_SCOPE)
# Maybe not the cleanest way to handle this
if(WIN32)
  set(INSTALLED_LAUNCH_EXECUTABLE "${CMAKE_INSTALL_PREFIX}/bin/launch.exe" PARENT_SCOPE)
  set(INSTALLED_LAUNCH_LIBRARIES "${CMAKE_INSTALL_PREFIX}/lib/arclaunch.dll" PARENT_SCOPE)
endif(WIN32)

install(
  TARGETS
    launch
    arclaunch
  LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
  RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
  )

install(
  FILES
    xsd/launch.xsd
  DESTINATION
    ${CMAKE_INSTALL_PREFIX}/share/arclaunch
    )
