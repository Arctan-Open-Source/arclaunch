#include "ws_simple_frames.h"

void ws_simple_close_handler(struct ws_connection *conn, const struct ws_frame* frame, void** buffer) {
  conn->received_close = 1;
  if(conn->transmitted_close)
    return;
  // sends back a close frame with the same payload
  ws_tx_frame(conn, WS_CLOSE, frame->payload, get_ws_payload_size(frame));
  conn->transmitted_close = 1;
}
