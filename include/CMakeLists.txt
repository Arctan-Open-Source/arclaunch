# Install details
message(STATUS "Generated header: ${LAUNCH_HEADER}")

# If cpack is used this is wrong
if(GENERATE_PACKAGE)
  set(INSTALLED_LAUNCH_INCLUDE_DIR ${CPACK_PACKAGING_INSTALL_PREFIX}/include/arclaunch PARENT_SCOPE)
else(GENERATE_PACKAGE)
  set(INSTALLED_LAUNCH_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include/arclaunch PARENT_SCOPE)
endif(GENERATE_PACKAGE)

configure_file(arclaunch.hxx.in ${CMAKE_CURRENT_BINARY_DIR}/arclaunch.hxx)

install(
  FILES
    ${LAUNCH_HEADER}
    ${CMAKE_CURRENT_BINARY_DIR}/arclaunch.hxx
    ExecutableNode.hxx
    # FileNode.hxx
    GroupNode.hxx
    LaunchNode.hxx
    NodeContext.hxx
    Node.hxx
    NodeStorage.hxx
    ScriptNode.hxx
    SequenceNode.hxx
    SocketNode.hxx
  DESTINATION
    include/arclaunch
    )
