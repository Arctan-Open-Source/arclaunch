#include "ScriptNode.hxx"
#include "NodeContext.hxx"

namespace arclaunch {
// ScriptNode
ScriptNode::ScriptNode(NodeContext& ctx, script_t* scriptElem) :
  ExecutableNode(ctx, &scriptElem->interpreter()) {
  // Combine the passed in argument list with the contained argument list
  // Combine the passed in environment list with the contained argument list
  // append the arguments
  executable_t::arg_sequence path;
  for(executable_t::path_sequence::iterator it = scriptElem->path().begin(); 
    it != scriptElem->path().end(); it++) {
    if(*(it->os()) == OS_STRING) {
      std::vector<char> aPath = pathElemToPathData(*it);
      path.push_back(aPath.data());
      break;
    }
  }
  appendArguments(path);
  appendArguments(scriptElem->arg());
  appendEnvironment(scriptElem->env());
  // append the environment variables
}


ScriptNode::~ScriptNode() {}

}
