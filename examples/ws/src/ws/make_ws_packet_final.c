#include "ws_encode_decode.h"

void make_ws_packet_final(struct ws_packet *pack) {
  pack->opcode |= 0x80;
}
