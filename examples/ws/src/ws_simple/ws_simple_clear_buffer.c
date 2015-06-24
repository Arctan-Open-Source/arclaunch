#include "ws_simple_frames.h"

void ws_simple_clear_buffer(struct ws_connection *conn, int finished) {
  if(conn->buffer == NULL)
    return;
  struct ws_simple_buffer* first_buff = conn->buffer;
  struct ws_simple_buffer* buff = first_buff;
  // Don't destroy an incomplete buffer
  if(!buff->is_complete && !finished)
  {
    buff = buff->prev;
    first_buff->prev = NULL;
  } else
    conn->buffer = NULL;
  while(buff != NULL) {
    // deallocate the data
    free(buff->data.binary);
    // deallocate the buffer
    struct ws_simple_buffer* last_buff = buff;
    buff = buff->prev;
    free(last_buff);
  }
}

