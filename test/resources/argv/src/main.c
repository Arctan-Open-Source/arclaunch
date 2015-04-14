#include <stdio.h>

int main(int argc, char** argv) {
  int c;
  for(c = 0; c < argc; c++) {
    printf("%s\n", argv[c]);
  }
  return 0;
}
