#include "Node.hxx"
#ifndef _SCRIPT_NODE_HXX
#define _SCRIPT_NODE_HXX
namespace arclaunch {

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
  virtual int stdin_fd() const;
  virtual int stdout_fd() const;
  virtual int stderr_fd() const;
};

}
#endif
