// An include file for easy inclusion of arclaunch into another project
// Note that using this include file WILL require that the final project be released under a GPLv2 compatible license
#include "Node.hxx"
#include "NodeStorage.hxx"
#include "NodeContext.hxx"
// #include "FileNode.hxx"
#include "ExecutableNode.hxx"
#include "ScriptNode.hxx"
#include "LaunchNode.hxx"
#include "SequenceNode.hxx"
#include "SocketNode.hxx"


#include <iostream>
namespace arclaunch {

// Create a basic, already populated context
NodeContext& context() {
  static bool first(true);
  static NodeContext ctx;
  if(first) {
    // notify<FileNode, file_t>(ctx);
    notify<ExecutableNode, executable_t>(ctx);
    notify<ScriptNode, script_t>(ctx);
    notify<LaunchNode, launch_t>(ctx);
    notify<SequenceNode, sequence_node_t>(ctx);
    notify<SocketNode, socket_node_t>(ctx);
    first = false;
  }
  return ctx;
}

}
