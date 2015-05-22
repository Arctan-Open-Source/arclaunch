#include "ws_encode_decode.h"

int packet_is_final(const struct ws_packet *pack) {
  return pack->opcode & 0x80;
}
