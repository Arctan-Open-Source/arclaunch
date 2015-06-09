#include "ws_simple_frames.h"

void ws_simple_ping_handler(struct ws_connection *conn, const struct ws_frame* frame, void** buffer) {
  // Respond with a pong packet with the same payload
  ws_tx_frame(conn, WS_PONG, frame->payload, get_ws_payload_size(frame));
}

