#include "FileNode.hxx"

namespace arclaunch {

FileNode::FileNode(NodeContext& ctx, const file_t& elem) {
}

FileNode::~FileNode() {
}

// Startup doesn't make as much sense here
void FileNode::startup() {
}

// Can't really run a file
bool FileNode::isRunning() const {
  return false;
}

// No pid for just a file
int FileNode::getPid() const {
  return 0;
}

// Nothing to wait for
void FileNode::waitFor() {
}

// Can only connect STDIN and STDOUT meaningfully
void FileNode::linkFd(int fd, int extFd) {
  // TODO implement meaningfully
  return;
}

}
