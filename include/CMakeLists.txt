# Install details
message(STATUS "Generated header: ${LAUNCH_HEADER}")

set(LAUNCH_INCLUDE_DIR_RELATIVE_PATH include/arclaunch PARENT_SCOPE)

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
