#include "Node.hxx"
#ifndef _FILE_NODE_H_
#define _FILE_NODE_H_

namespace arclaunch {

class FileNode : Node {
public:
  int fd;
private:
  FileNode(NodeContext& ctx, file_t* elem);
  virtual ~FileNode();
  virtual void startup();
  virtual bool isRunning() const;
  virtual pid_t getPid() const;
  virtual void waitFor() const;
  virtual void linkStdin(int fd);
  virtual int getStdout();
  virtual int getStderr();
};

}
#endif
