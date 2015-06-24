#include <unistd.h>

int main(int argc, char** argv) {
  ssize_t nread;
  char buffer[50];
  while(nread = read(STDIN_FILENO, buffer, 50)) {
    // Read from STDIN
    // Write to STDOUT
    write(STDOUT_FILENO, buffer, nread);
  }
  return 0;
}

