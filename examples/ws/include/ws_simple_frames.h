#include "ws_encode_decode.h"

void ws_simple_continuation_handler(struct ws_connection *conn, const struct ws_frame* payload, void** buffer);
void ws_simple_text_handler(struct ws_connection *conn, const struct ws_frame* payload, void** buffer);
void ws_simple_binary_handler(struct ws_connection *conn, const struct ws_frame* payload, void** buffer);
void ws_simple_close_handler(struct ws_connection *conn, const struct ws_frame* payload, void** buffer);
void ws_simple_ping_handler(struct ws_connection *conn, const struct ws_frame* payload, void** buffer);
void ws_simple_pong_handler(struct ws_connection *conn, const struct ws_frame* payload, void** buffer);

struct ws_simple_buffer {
  enum WS_OPCODE type;
  size_t nbytes;
  size_t nalloc;
  int is_complete;
  struct ws_simple_buffer* prev;
  union {
    char* text;
    void* binary;
  } data;

};

