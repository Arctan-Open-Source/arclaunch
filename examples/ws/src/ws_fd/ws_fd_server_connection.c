#include "ws_encode_decode.h"

struct ws_connection make_ws_fd_server_connection(int *fd) {
  struct ws_connection conn = {
    SERVER,
    NULL,
    fd,
    fd,
    &ws_fd_rx,
    &ws_fd_tx,
    {
      .array = {
        &ws_fd_continuation_handler,
        &ws_fd_text_handler,
        &ws_fd_binary_handler,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        &ws_fd_close_handler,
        &ws_fd_ping_handler,
        &ws_fd_pong_handler,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
      }
    }
  };
}

