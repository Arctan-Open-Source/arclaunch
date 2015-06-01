#include <unistd.h>
#include <string.h>

void write_stderr(const char* msg) {
  write(STDERR_FILENO, msg, strlen(msg));
}

int main(int argc, char** argv) {
  char data[50];
  ssize_t numread;
  const char *before_read, *after_read, *before_write, *after_write;
  before_read = "Before read\n";
  after_read = "After read\n";
  before_write = "Before write\n";
  after_write = "After write\n";
  // simple echo-back program
  do {
    write_stderr(before_read);
    numread = read(STDIN_FILENO, data, 50);
    write_stderr(after_read);
    write_stderr(before_write);
    write(STDOUT_FILENO, data, numread);
    write_stderr(after_write);
  } while(numread > 0);
  return 0;
}
