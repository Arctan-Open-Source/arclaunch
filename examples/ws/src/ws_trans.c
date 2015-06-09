#include "ws_encode_decode.h"
#include "ws_fd_encode_decode.h"

int main() {
  // Start by creating a connection from the known file desciptor
  struct ws_connection conn;
  int in_fd = 3, out_fd = 4, closing = 0;
  conn = make_ws_fd_server_connection(&in_fd, &out_fd);
  // Run the connection until a close message is received
  while(!conn.received_close) {
    // Wait for one of the read descriptors using select, 0 and 3
    ws_rx_frame(&conn);
  }
  return 0;
}
