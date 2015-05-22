#include "ws_encode_decode.h"

int packet_is_masked(const struct ws_packet* pack) {
  return pack->size & 0x80;
}

