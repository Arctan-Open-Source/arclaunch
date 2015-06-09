#include "ws_fd_encode_decode.h"
#include "ws_simple_frames.h"

struct ws_connection make_ws_fd_client_connecion(int *in_fd, int *out_fd) {
  struct ws_connection conn = {
    WS_CLIENT,
    0,
    0,
    NULL,
    in_fd,
    out_fd,
    &ws_fd_rx,
    &ws_fd_tx,
    {
      .array = {
        &ws_simple_continuation_handler,
        &ws_simple_text_handler,
        &ws_simple_binary_handler,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        &ws_simple_close_handler,
        &ws_simple_ping_handler,
        &ws_simple_pong_handler,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
      }
    }
  };
  return conn;
}

