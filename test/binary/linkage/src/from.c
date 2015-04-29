#include <unistd.h>
#include <string.h>

int main(int argc, char** argv) {
  const char* msg;
  msg = "TEST LINKAGE";
  write(STDOUT_FILENO, msg, strlen(msg));
  // Write to stdout
  return 0;
}
