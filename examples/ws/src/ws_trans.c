#include "ws_encode_decode.h"
#include "ws_fd_encode_decode.h"
#include "ws_simple_frames.h"
#include <unistd.h>

int main() {
  // Start by creating a connection from the known file desciptor
  struct ws_connection conn;
  int in_fd = 3, out_fd = 4;
  conn = make_ws_fd_server_connection(&in_fd, &out_fd);
  // Run the connection until a close message is received
  while(!conn.received_close) {
    // Wait for one of the read descriptors using select, 0 and 3
    ws_rx_frame(&conn);
    if(conn.buffer == NULL)
      continue;
    struct ws_simple_buffer* data = conn.buffer;
    while(data != NULL) {
      // Write the contained data to stdout
      write(STDOUT_FILENO, data->data.binary, data->nbytes);
      data = data->prev;
    }
    // Deallocate the received data
    ws_simple_clear_buffer(&conn, 0);
  }
  ws_simple_clear_buffer(&conn, 1);
  // write(STDOUT_FILENO, "finish rx\n", 10);
  return 0;
}
