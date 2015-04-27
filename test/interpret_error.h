
#include <errno.h>
#include <iostream>

#ifndef INTERPRET_ERROR_H_
#define INTERPRET_ERROR_H_
void interpretError() {
  switch(errno) {
  case 0:
    std::cerr << "No error set" << std::endl;
    break;
#if EWOULDBLOCK!=EAGAIN
  case EWOULDBLOCK:
    std::cerr << "Would block" << std::endl;
    break;
#endif
  case EAGAIN:
    std::cerr << "Again error or Would block" << std::endl;
    break;
  case EBADF:
    std::cerr << "Bad File Descriptor" << std::endl;
    break;
  case EFAULT:
    std::cerr << "Buffer is outside of memory space" << std::endl;
    break;
  case EINTR:
    std::cerr << "Read call was interrupted" << std::endl;
    break;
  case EINVAL:
    std::cerr << "File descriptor invalid for reading" << std::endl;
    break;
  case EIO:
    std::cerr << "I/O Error" << std::endl;
    break;
  case EISDIR:
    std::cerr << "File descriptor is a directory" << std::endl;
    break;
  default:
    std::cerr << "Indefinite Error" << std::endl;
    break;
  }
}

#endif
