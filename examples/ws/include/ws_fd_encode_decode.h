// ws_fd_connection.h header
#ifndef WS_FD_CONNECTION_H_
#define WS_FD_CONNECTION_H_

#include "ws_encode_decode.h"

size_t ws_fd_rx(size_t nbytes, void* buffer, const void* rx_data);
size_t ws_fd_tx(size_t nbytes, const void* buffer, const void* tx_data);

// create connections from file descriptors
struct ws_connection make_ws_fd_server_connection(int *in_fd, int *out_fd);
struct ws_connection make_ws_fd_client_connection(int *in_fd, int *out_fd);

#endif
