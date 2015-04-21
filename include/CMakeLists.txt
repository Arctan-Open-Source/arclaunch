# Install details
message(STATUS "Generated header: ${LAUNCH_HEADER}")
install(
  FILES
    ${LAUNCH_HEADER}
    Node.hxx
    NodeStorage.hxx
    NodeContext.hxx
    ExecutableNode.hxx
    ScriptNode.hxx
    LaunchNode.hxx
  DESTINATION
    ${CMAKE_INSTALL_PREFIX}/include
    )
