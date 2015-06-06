#include <unistd.h>
#include <string.h>

int main(int argc, char** argv) {
  char data[50];
  ssize_t numread;
  // simple echo-back program
  do {
    numread = read(STDIN_FILENO, data, 50);
    write(STDOUT_FILENO, data, numread);
  } while(numread > 0);
  return 0;
}
