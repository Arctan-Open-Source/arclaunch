#include "ws_simple_frames.h"
#include <string.h>

void ws_simple_continuation_handler(struct ws_connection *conn, const struct ws_frame* frame, void** buffer) {
  struct {
    uint16_t code;
    char message[65];
  } close_frame;
  close_frame.code = htons(1002);
  strncpy(close_frame.message, "received continuation frame while waiting for initial data frame", 65);
  
  struct ws_simple_buffer *buff;
  buff = *buffer;
  if(buff->is_complete) {
    // send a 1002 error
    ws_tx_frame(conn, WS_CLOSE, &close_frame, sizeof(close_frame));
    return;
  }
  size_t frame_size = get_ws_payload_size(frame);
  if(buff->nalloc < buff->nbytes + frame_size) {
    buff->nalloc = buff->nbytes + frame_size;
    buff->data.text = realloc(buff->data.binary, buff->nalloc);
  }
  switch(buff->type) {
  case WS_TEXT:
    strncpy(buff->data.text + buff->nbytes, frame->payload, frame_size);
    buff->nbytes += frame_size;
    break;
  case WS_BINARY:
    memcpy(buff->data.binary + buff->nbytes, frame->payload, frame_size);
    buff->nbytes += frame_size;
    break;
  }
  buff->is_complete = ws_frame_is_final(frame);
}

