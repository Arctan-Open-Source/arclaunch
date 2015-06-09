#include "ws_fd_encode_decode.h"
#include <unistd.h>

size_t ws_fd_tx(size_t nbytes, const void* buffer, const void* tx_data) {
  const int* fd = tx_data;
  return write(*fd, buffer, nbytes);
}
