
#include "launch.hxx"

// Linux/Unix libraries
// TODO: adjust based on operating system constants
#include "os/linux.h"

#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

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
  // Used to set the constructed node running
  virtual void startup() = 0;
  virtual bool isRunning() const = 0;
  virtual void waitFor() const = 0;
  // Used to link file descriptors to the process that will be forked
  // Very much low-level action
  virtual void linkStdin(int fd) = 0;
  // Retrieves the read fd of the stdout pipe
  virtual int getStdout() = 0;
  virtual int getStderr() = 0;
  static std::vector<std::vector<char> > argSequenceToArgData(executable_t::arg_sequence& args);
  static std::vector<std::vector<char> > envSequenceToEnvData(executable_t::env_sequence& envs);
  static executable_t::arg_sequence fuseArgSequence(executable_t::arg_sequence& args1, executable_t::arg_sequence& args2);
  static executable_t::env_sequence fuseEnvSequence(executable_t::env_sequence& envs1, executable_t::env_sequence& envs2);
  static std::vector<char> pathElemToPathData(const path_t& pathElem);
};

}
#endif
