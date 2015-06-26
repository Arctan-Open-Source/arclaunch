#include "ws_encode_decode.h"
#include "ws_fd_encode_decode.h"
#include "ws_simple_frames.h"
#include "shared_mem.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>


int main(int argc, char** argv) {
  // Start by creating a connection from the known file desciptor
  struct ws_connection conn;
  int in_fd = 3, out_fd = 4, mem_fd;
  long PAGE_SIZE;
  PAGE_SIZE = sysconf(_SC_PAGE_SIZE);
  conn = make_ws_fd_server_connection(&in_fd, &out_fd);
  // Run the connection until a close message is received
  // Open the shared memory
  mem_fd = open_shared_memory(argv[1]);
  // TODO: map the necessary shared memory
  
  fd_set fds;
  struct timeval tout;
  while(!conn.received_close) {
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    FD_SET(in_fd, &fds);
    tout.tv_sec = 0;
    tout.tv_usec = 500000;
    // Wait for one of the read descriptors using select, 0 and 3
    select(in_fd + 1, &fds, NULL, NULL, &tout);
    if(FD_ISSET(STDIN_FILENO, &fds)) {
      // Read from stdin
      size_t nbytes;
      read(STDIN_FILENO, &nbytes, sizeof(size_t));
      char *data = malloc(nbytes);
      read(STDIN_FILENO, data, nbytes);
      // Transmit packed in a text packet
      ws_tx_frame(&conn, WS_TEXT, data, nbytes);
      free(data);
    }
    if(FD_ISSET(in_fd, &fds)) {
      ws_rx_frame(&conn);
      if(conn.buffer == NULL)
        continue;
      struct ws_simple_buffer* data = conn.buffer;
      while(data != NULL) {
        // Write the contained data to shared memory
        write(STDOUT_FILENO, &data->nbytes, sizeof(data->nbytes));
        write(STDOUT_FILENO, data->data.binary, data->nbytes);
        data = data->prev;
      }
      // Deallocate the received data
      ws_simple_clear_buffer(&conn, 0);
    }
  }
  ws_simple_clear_buffer(&conn, 1);
  // write(STDOUT_FILENO, "finish rx\n", 10);
  return 0;
}
