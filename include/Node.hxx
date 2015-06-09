
#include "launch.hxx"
#include <vector>
#include <map>

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
  typedef void (*CompletionCallback)(int, Node*, void*);
  struct CompletionHandler {
  public:
    CompletionHandler(CompletionCallback call, void* deathData);
    CompletionCallback callback;
    void* data;
  };
  std::vector<CompletionHandler> onDeath;
protected:
  std::map<int, int> fdMap;
  void closeFds();
public:
  Node();
  virtual ~Node();
  // Used to set the constructed node running
  virtual void startup() = 0;
  virtual bool isRunning() const = 0;
  virtual void waitFor() = 0;
  // A function that gets called when the node completes
  virtual void addCompletionHandler(CompletionCallback handle, void* data);
  // Used to pass file descriptors to the process that will be forked
  // Very much low-level action
  virtual void linkFd(int fd, int extFd);
  virtual void linkStdin(int fd);
  virtual void linkStdout(int fd);
  virtual void linkStderr(int fd);
  // Useful static methods
  static bool isReadable(int fd);
  static bool isWritable(int fd);
  static std::vector<std::vector<char> > argSequenceToArgData(executable_t::arg_sequence& args);
  static std::vector<std::vector<char> > envSequenceToEnvData(executable_t::env_sequence& envs);
  static executable_t::arg_sequence fuseArgSequence(executable_t::arg_sequence& args1, executable_t::arg_sequence& args2);
  static executable_t::env_sequence fuseEnvSequence(executable_t::env_sequence& envs1, executable_t::env_sequence& envs2);
  static std::vector<char> pathElemToPathData(const path_t& pathElem);
};

}
#endif
