#include <string>
#include "interpret_error.h"
#include <unistd.h>

#ifndef DRAIN_FD_H_
#define DRAIN_FD_H_
// Used to read everything available from a file descriptor
std::string drainFd(int fd, int limit = 5) {
  std::string str_data;
  char data[50];
  ssize_t numread;
  do {
    numread = read(fd, data, 49);
    if(numread == -1) {
      interpretError();
      break;
    }
    data[numread] = '\0';
    str_data += data;
    limit--;
  } while(numread > 0 && limit > 0);
  return str_data;
}
#endif
