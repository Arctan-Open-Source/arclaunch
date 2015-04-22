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
void FileNode::waitFor() const {
}

// Connect to append to a file
void FileNode::linkStdin(int fd) {
  // TODO implement to connect to a file
}

// 
int FileNode::getStdout() {
  // TODO return an output file descriptor
  return 0;
}

int FileNode::getStderr() {
  // TODO determine a better way to handle this
  return 0;
}

}
