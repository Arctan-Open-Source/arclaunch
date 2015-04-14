#include "Node.hxx"

// TODO: vary by operating system
// TODO: catch the terminate signal and pass it on to the children
#include <sys/types.h>
#include <sys/wait.h>
#define OS_STRING "linux"
#define DIR_SEPERATOR '/'

// TODO: remove iostream
#include <iostream>

namespace arclaunch {

// Node
Node::~Node() {}

// Templatize these? Use functional features of C++11?

std::vector<std::vector<char> > Node::argSequenceToArgData(executable_t::arg_sequence& args) {
  std::vector<std::vector<char> > argData;
  for(executable_t::arg_iterator it = args.begin();
    it != args.end(); it++) {
    argData.emplace_back(it->begin(), it->end());
    argData.back().push_back('\0');
  }
  return argData;
}

std::vector<std::vector<char> > Node::envSequenceToEnvData(executable_t::env_sequence& envs) {
  std::vector<std::vector<char> > envData;
  for(executable_t::env_iterator it = envs.begin();
    it != envs.end(); it++) {
    std::string envstr(*(it->key()) + "=" + *(it->value()));
    envData.emplace_back(envstr.begin(), envstr.end());
    envData.back().push_back('\0');
  }
  return envData;
}

executable_t::arg_sequence Node::fuseArgSequence(executable_t::arg_sequence& args1, executable_t::arg_sequence& args2) {
  executable_t::arg_sequence seq(args1);
  seq.insert(seq.end(), args2.begin(), args2.end());
  return seq;
}

executable_t::env_sequence Node::fuseEnvSequence(executable_t::env_sequence& envs1, executable_t::env_sequence& envs2) {
  executable_t::env_sequence seq(envs1);
  seq.insert(seq.end(), envs2.begin(), envs2.end());
  return seq;
}

std::vector<char> Node::pathElemToPathData(const path_t& pathElem) {
  std::vector<char> path;
  bool add_seperator = pathElem.absolute();
  for(path_t::directory_const_iterator it = pathElem.directory().begin(); 
    it != pathElem.directory().end(); it++)
  {
    if(add_seperator)
      path.insert(path.end(), DIR_SEPERATOR);
    add_seperator = true;
    path.insert(path.end(), it->begin(), it->end());
  }
  return path;
}


// NodeStorage
NodeStorage::~NodeStorage() {}

// NodeContext
void NodeContext::recognize(NodeStorage* store) {
  storages.push_back(store);
}

// Assumes that notifications about the least specific elements are submitted first
Node* NodeContext::execute(executable_t* elem, executable_t::arg_sequence args, executable_t::env_sequence envs) {
  Node* node;
  for(std::vector<NodeStorage*>::reverse_iterator it = storages.rbegin();
    it != storages.rend(); it++)
  {
    if((node = (*it)->construct(*this, elem, args, envs)) != NULL)
    {
      // Store a reference to the created node
      nodes.push_back(node);
      break;
    }
  }
  // Return a reference to the created node or null
  return node;
}

NodeContext::~NodeContext() {}

// ExecutableNode
ExecutableNode::ExecutableNode(NodeContext& ctx, executable_t* elem, executable_t::arg_sequence& args, executable_t::env_sequence& envs) {
  // This is where fork-exec happens
  pid = fork();
  if(pid == 0) {
    // forked thread
    // construct the argument list
    std::vector<std::vector<char> > argData(argSequenceToArgData(elem->arg()));
    std::vector<char*> args;
    args.push_back(NULL);
    for(std::vector<std::vector<char> >::iterator it = argData.begin(); 
      it != argData.end(); it++)
      args.emplace_back(it->data());
    args.push_back(NULL);
    // construct the environment list
    std::vector<std::vector<char> > envData(envSequenceToEnvData(elem->env()));
    std::vector<char*> envs;
    for(std::vector<std::vector<char> >::iterator it = envData.begin();
      it != envData.end(); it++)
      envs.emplace_back(it->data());
    envs.push_back(NULL);
    // Deduce the proper path
    for(executable_t::path_iterator it = elem->path().begin(); 
      it != elem->path().end(); it++) {
      // Check if it's the correct OS
      if(*(it->os()) != OS_STRING)
        continue;
      // Construct the path
      std::vector<char> path(pathElemToPathData(*it));
      std::cout << "Executable Path: " << path.data() << std::endl;
      // Place the path as argv[0]
      args[0] = path.data();
      execve(path.data(), args.data(), envs.data());
    }
  }
  // main thread
}

bool ExecutableNode::isRunning() const {
  if(pid == 0)
    return false;
  else
  {
    // TODO: vary by operating system
    int status;
    waitpid(pid, &status, WNOHANG);
  }
}

pid_t ExecutableNode::getPid() const {
  return pid;
}

void ExecutableNode::waitFor() const {
  int status;
  waitpid(pid, &status, 0);
}

ExecutableNode::~ExecutableNode() {}

// ScriptNode
ScriptNode::ScriptNode(NodeContext& ctx, script_t* scriptElem, executable_t::arg_sequence& args, executable_t::env_sequence& envs) :
  appended(scriptElem) {
  // Combine the passed in argument list with the contained argument list
  // Combine the passed in environment list with the contained argument list
  interpreter = ctx.execute(&scriptElem->interpreter(), fuseArgSequence(scriptElem->arg(), args), fuseEnvSequence(scriptElem->env(), envs));
}

ScriptNode::~ScriptNode() {}

bool ScriptNode::isRunning() const {
  return interpreter->isRunning();
}

pid_t ScriptNode::getPid() const {
  return interpreter->getPid();
}

void ScriptNode::waitFor() const {
  return interpreter->waitFor();
}

// LaunchNode
LaunchNode::LaunchNode(NodeContext& ctx, launch_t* launchElem, executable_t::arg_sequence& args, executable_t::env_sequence& envs) {
  // Emplace nodes based on the launch element's children
  for(launch_t::executable_iterator it = launchElem->executable().begin(); 
    it < launchElem->executable().end(); it++) {
    nodes.push_back(ctx.execute(&(*it), args, envs));
  }
}

LaunchNode::~LaunchNode() {}

pid_t LaunchNode::getPid() const {
  return 0;
}

bool LaunchNode::isRunning() const {
  for(const_node_iterator it = nodes.begin(); it != nodes.end(); it++) {
    if((*it)->isRunning()) {
      return true;
    }
  }
  return false;
}

void LaunchNode::waitFor() const {
  for(const_node_iterator it = nodes.begin(); it != nodes.end(); it++) {
    (*it)->waitFor();
  }
}

}
