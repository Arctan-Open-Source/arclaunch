#include "ws_encode_decode.h"

int ws_frame_is_final(const struct ws_frame *pack) {
  return pack->opcode & 0x80;
}
