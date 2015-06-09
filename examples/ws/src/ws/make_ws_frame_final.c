#include "ws_encode_decode.h"

void make_ws_frame_final(struct ws_frame *pack) {
  pack->opcode |= 0x80;
}
