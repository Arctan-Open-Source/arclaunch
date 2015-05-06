#include "Node.hxx"
#ifndef _FILE_NODE_H_
#define _FILE_NODE_H_

namespace arclaunch {

class FileNode : public Node {
private:
  int fd;
public:
  FileNode(NodeContext& ctx, const file_t& elem);
  virtual ~FileNode();
  virtual void startup();
  virtual bool isRunning() const;
  virtual pid_t getPid() const;
  virtual void waitFor() const;
  virtual void linkFd(int fd, int extFd);
};

}
#endif
