
#include "launch.hxx"

// Linux/Unix libraries
#include <unistd.h>
#include <sys/types.h>

#ifndef NODE_HXX_

namespace arclaunch {

class Node;
class NodeStorage;
class NodeContext;

// An interface class for executing node elements
class Node {
public:
  virtual ~Node();
  virtual bool isRunning() const = 0;
  virtual pid_t getPid() const = 0;
  virtual void waitFor() const = 0;
  static std::vector<std::vector<char> > argSequenceToArgData(executable_t::arg_sequence& args);
  static std::vector<std::vector<char> > envSequenceToEnvData(executable_t::env_sequence& envs);
  static executable_t::arg_sequence fuseArgSequence(executable_t::arg_sequence& args1, executable_t::arg_sequence& args2);
  static executable_t::env_sequence fuseEnvSequence(executable_t::env_sequence& envs1, executable_t::env_sequence& envs2);
  static std::vector<char> pathElemToPathData(const path_t& pathElem);
};

// An abstract class for Node storage
class NodeStorage {
public:
  virtual ~NodeStorage();
  virtual Node* construct(NodeContext& ctx, executable_t* gNode, executable_t::arg_sequence& args, executable_t::env_sequence& envs) = 0;
};

class NodeContext {
private:
  // These are used as memory containers for the different types of nodes
  std::vector<NodeStorage*> storages;
  std::vector<Node*> nodes;
public:
  virtual ~NodeContext();
  // Registers a type of node storage
  void recognize(NodeStorage* store);
  // Executes a node based on its dynamic type
  Node* execute(executable_t* executableElem, executable_t::arg_sequence args = executable_t::arg_sequence(), executable_t::env_sequence envs = executable_t::env_sequence());
};

// Use a templatized notify function
template<class Derived, class Contained>
void notify(NodeContext& ctx) {
  // Create a local class
  class Storage : public NodeStorage {
  private:
    std::vector<Derived> nodes;
  public:
    virtual ~Storage() {};
    Node* construct(NodeContext& ctx, executable_t* gNode, executable_t::arg_sequence& args, executable_t::env_sequence& envs) {
      if(Contained* sNode = dynamic_cast<Contained*>(gNode)) {
        nodes.emplace_back(ctx, sNode, args, envs);
        return &nodes.back();
      }
      return NULL;
    }
  };
  // This is where memory for all the nodes are allocated
  static Storage nodeStorage;
  ctx.recognize(&nodeStorage);
}

// Derive from the mapped types
class ExecutableNode : public Node {
private:
  pid_t pid;
public:
  ExecutableNode(NodeContext& ctx, executable_t* elem, executable_t::arg_sequence& args, executable_t::env_sequence& envs);
  virtual ~ExecutableNode();
  virtual bool isRunning() const;
  virtual pid_t getPid() const;
  virtual void waitFor() const;
};

class ScriptNode : public Node {
private:
  Node* interpreter;
  script_t* appended;
public:
  ScriptNode(NodeContext& ctx, script_t* scriptElem, executable_t::arg_sequence& args, executable_t::env_sequence& envs);
  virtual ~ScriptNode();
  virtual bool isRunning() const;
  virtual pid_t getPid() const;
  virtual void waitFor() const;
};

class LaunchNode : public Node {
  typedef std::vector<Node*>::iterator node_iterator;
  typedef std::vector<Node*>::const_iterator const_node_iterator;
private:
  launch_t* interior;
  std::vector<Node*> nodes;
public:
  LaunchNode(NodeContext& ctx, launch_t* launchElem, executable_t::arg_sequence& args, executable_t::env_sequence& envs);
  virtual ~LaunchNode();
  virtual bool isRunning() const;
  virtual pid_t getPid() const;
  virtual void waitFor() const;
};

}
#endif
