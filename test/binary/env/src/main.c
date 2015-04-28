#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
  // Output the TEST_ENV environment variable
  char* str = getenv("TEST_ENV");
  write(STDOUT_FILENO, str, strlen(str));
  return 0;
}
