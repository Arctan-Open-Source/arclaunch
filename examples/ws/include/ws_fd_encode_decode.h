// ws_fd_connection.h header
#include "ws_encode_decode.h"

size_t ws_fd_rx(size_t nbytes, void* buffer, void* rx_data);
size_t ws_fd_tx(size_t nbytes, void* buffer, void* tx_data);

void ws_fd_continuation_handler(const char* payload, size_t payload_size, void* buffer);
void ws_fd_text_handler(const char* payload, size_t payload_size, void* buffer);
void ws_fd_binary_handler(const char* payload, size_t payload_size, void* buffer);
void ws_fd_close_handler(const char* payload, size_t payload_size, void* buffer);
void ws_fd_ping_handler(const char* payload, size_t payload_size, void* buffer);
void ws_fd_pong_handler(const char* payload, size_t payload_size, void* buffer);


// create connections from file descriptors
struct ws_connection make_ws_fd_server_connection(int *fd);
struct ws_connection make_ws_fd_client_connection(int *fd);

