#include "ws_fd_encode_decode.h"
#include <unistd.h>

size_t ws_fd_rx(size_t nbytes, void* buffer, const void* rx_data) {
  const int *fd;
  fd = rx_data;
  return read(*fd, buffer, nbytes);
}
