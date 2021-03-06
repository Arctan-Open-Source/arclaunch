#include "ws_simple_frames.h"
#include <string.h>

void ws_simple_binary_handler(struct ws_connection *conn, const struct ws_frame* frame, void** buffer) {
  struct {
    uint16_t code;
    char message[59];
  } close_frame;
  close_frame.code = htons(1002);
  strncpy(close_frame.message, "received binary frame while waiting for continuation frames", 59);
  struct ws_simple_buffer *prev = *buffer, *buff;
  size_t frame_size;
  if(!prev->is_complete) {


    ws_tx_frame(conn, WS_CLOSE, &close_frame, sizeof(close_frame));
    return;
  }
  frame_size = get_ws_payload_size(frame);
  buff = malloc(sizeof(struct ws_simple_buffer));
  buff->data.binary = malloc(frame_size);
  buff->nalloc = frame_size;
  buff->nbytes = frame_size;
  buff->is_complete = ws_frame_is_final(frame);
  buff->prev = prev;
  *buffer = buff;
}

