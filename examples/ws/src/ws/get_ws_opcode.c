#include "ws_encode_decode.h"

enum WS_OPCODE get_ws_opcode(const struct ws_packet *pack) {
  return pack->opcode & 0x0F;
}
