#include <unistd.h>
#include <string.h>

int main(int argc, char** argv) {
  char* msg;
  msg = "STDOUT TEST";
  write(STDOUT_FILENO, msg, strlen(msg));
  return 0;
}
