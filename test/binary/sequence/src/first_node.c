#include <unistd.h>
#include <string.h>

int main(int argc, char** argv) {
  const char* text = "first node\n";
  write(STDOUT_FILENO, text, strlen(text));
  return 0;
}

