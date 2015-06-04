#include "ws_encode_decode.h"

void set_ws_opcode(struct ws_packet *pack, enum WS_OPCODE op) {
  pack->opcode &= 0xF0 | op;
  // All control codes are final
  if(op & 0x08)
    make_ws_packet_final(pack);
}
