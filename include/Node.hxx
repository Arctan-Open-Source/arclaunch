
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
private:
  /**
   *  CompletionCallback is used for responding to a node's completion.
   *  
   *  void CompletionCallback(int retval, Node* node, void* data, int instNum);
   */
  typedef void (*CompletionCallback)(int, Node*, void*, int);
  class CompletionHandler {
  public:
    CompletionHandler(CompletionCallback call, void* deathData);
    CompletionCallback callback;
    void* data;
  };
  std::map<int, int> fdMap;
  std::vector<CompletionHandler> onDeath;
  std::vector<CompletionHandler> onInstanceDeath;
  std::map<int, std::vector<CompletionHandler> > instances;
  void closeFds();
protected:
  /**
   *  Must be called by derived nodes when their instance completes.
   *
   *  @param instNum The instance number of the completing node.
   *  @param retval The return value from the instance node.
   */
  void finishInstance(int instNum, int retval);

  /**
   *  Must be called by derived nodes to correctly overwrite file descriptors
   *  
   *  Note: this should be called after the process has forked. Group nodes
   *    generally will not directly call this function.
   */
  void overwriteFds();

  /**
   *  Can be called by Group nodes to devolve file descriptor connections.
   * 
   *  @param child The node to which the file descriptors are devolved.
   */
  void passFds(Node& child);
  
  /**
   *  Can be called by Group nodes to devolve a single linked file descriptor.
   *  
   *  @param child The node to which the file descriptor is devolved.
   *  @param fd The file descriptor to devolve.
   *  @param childFd The file descriptor as experienced by the child.
   */
  void passFd(Node& child, int fd, int childFd);
  
  /**
   *  Must be implemented by derived nodes to start an instance of a node.
   * 
   *  @param instNum The instance number for the newly generated instance.
   */
  virtual void startInstance(int instNum) = 0;
public:
  /**
   * Default constructor doesn't do anything right now.
   */
  Node();

  /**
   * Destructor automatically closes all internally managed file descriptors.
   */
  virtual ~Node();

  /**
   *  Starts a new instance of this node and returns the instance of the new instance.
   *  
   *  @return the instance number for the new instance.
   *
   *  Note: an instance number may be re-used one an instance has finished.
   */
  int startup();

  /**
   *  Detects whether the given instance is currently running.
   *  
   *  @param instNum the instance number of the instance to check
   *  @return true if the given instance is running, false otherwise
   */
  bool instanceIsRunning(int instNum) const;

  /**
   *  Detects whether there are any instances currently running.
   * 
   *  @return true if at least one instance is running, false otherwise
   */
  bool isRunning() const;

  /**
   *  Detects the number of running instances.
   *
   *  @return the number of running instances.
   */
  int numInstancesRunning() const;

  /**
   *  Blocks execution until the given instance has completed.
   *  
   *  @param the instance to wait for
   */
  virtual void waitForInstance(int instNum) = 0;

  /**
   * Waits until all instances of the node have completed.
   */
  void waitFor();

  /**
   *  Sets a callback that is called upon completion of the next started instance.
   */
  void addInstanceCompletionHandler(CompletionCallback handle, void* data);

  /**
   *  Sets a callback that is called upon completion of any instance.
   */
  void addCompletionHandler(CompletionCallback handle, void* data);
  

  /**
   *  Connects the passed in file descriptor to the next node instance started.
   *
   *  extFd is duped so extFd can and should be closed unless used elsewhere in
   *  arclaunch.
   *  
   *  @param fd The file descriptor number as seen by the instance.
   *  @param extFd The file descriptor number as currently experienced by 
   *    arclaunch
   */
  void linkFd(int fd, int extFd);

  /**
   *  Connects the file descriptor as STDIN for the next instance started.
   *  
   *  fd is duped and should be closed unless used elsewhere in arclaunch.
   *
   *  @param fd The file descriptor number as it is in arclaunch.
   */
  void linkStdin(int fd);

  /**
   *  Connects the file descriptor as STDOUT for the next instance started.
   *
   *  fd is duped and should be closed unless used elsewhere in arclaunch.
   *
   *  @param fd The file descriptor number as it is in arclaunch
   */
  void linkStdout(int fd);

  /**
   *  Connects the file descriptor as STDERR for the next instance started.
   *
   *  fd is duped and should be closed unless used elsewhere in arclaunch.
   *
   *  @param fd The file descriptor number as it is in arclaunch
   */
  void linkStderr(int fd);
  
  // Useful static methods
  /**
   *  Used to verify that a file descriptor is readable.
   *
   *  @return true if the file descriptor is readable, false otherwise.
   */
  static bool isReadable(int fd);

  /**
   *  Used to verify that a file descriptor is writable.
   *
   *  @return false if the file descriptor is writable, false otherwise.
   */
  static bool isWritable(int fd);

  /**
   *  
   */
  static std::vector<std::vector<char> > argSequenceToArgData(executable_t::arg_sequence& args);
  static std::vector<std::vector<char> > envSequenceToEnvData(executable_t::env_sequence& envs);
  static executable_t::arg_sequence fuseArgSequence(executable_t::arg_sequence& args1, executable_t::arg_sequence& args2);
  static executable_t::env_sequence fuseEnvSequence(executable_t::env_sequence& envs1, executable_t::env_sequence& envs2);
  static std::vector<char> pathElemToPathData(const path_t& pathElem);
};

}
#endif
