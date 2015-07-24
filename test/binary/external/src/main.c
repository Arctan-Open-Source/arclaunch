#include <unistd.h>

#define BUFF_SIZE 128

int main(int argc, char** argv) {
  ssize_t nread;
  char buff[BUFF_SIZE];
  nread = read(STDIN_FILENO, buff, BUFF_SIZE);
  write(STDOUT_FILENO, buff, nread);
  return 0;
}
