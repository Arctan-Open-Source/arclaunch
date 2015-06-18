# Install details
message(STATUS "Generated header: ${LAUNCH_HEADER}")
set(INSTALLED_LAUNCH_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include/arclaunch)
set(INSTALLED_LAUNCH_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include/arclaunch PARENT_SCOPE)

install(
  FILES
    ${LAUNCH_HEADER}
    arclaunch.hxx
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
    ${INSTALLED_LAUNCH_INCLUDE_DIR}
    )
