#include "ws_encode_decode.h"

void make_packet_non_final(struct ws_packet *pack) {
  pack->opcode &= 0x7F;
}

