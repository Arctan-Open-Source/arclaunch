
#include "launch.hxx"

// Linux/Unix libraries
// TODO: adjust based on operating system constants
#include "os/linux.h"

#include <unistd.h>
#include <sys/types.h>

#ifndef _NODE_HXX_
#define _NODE_HXX_
namespace arclaunch {

// Needs the reference to NodeContext
// in order to compile correctly
class NodeContext;

// An interface class for executing node elements
class Node {
public:
  virtual ~Node();
  virtual bool isRunning() const = 0;
  virtual pid_t getPid() const = 0;
  virtual void waitFor() const = 0;
  // The other end of the various file descriptors
  // Very much low-level action
  virtual int stdinFd() const = 0;
  virtual int stdoutFd() const = 0;
  virtual int stderrFd() const = 0;
  static std::vector<std::vector<char> > argSequenceToArgData(executable_t::arg_sequence& args);
  static std::vector<std::vector<char> > envSequenceToEnvData(executable_t::env_sequence& envs);
  static executable_t::arg_sequence fuseArgSequence(executable_t::arg_sequence& args1, executable_t::arg_sequence& args2);
  static executable_t::env_sequence fuseEnvSequence(executable_t::env_sequence& envs1, executable_t::env_sequence& envs2);
  static std::vector<char> pathElemToPathData(const path_t& pathElem);
};

}
#endif
