#include "ws_encode_decode.h"


void ws_unmask_packet(struct ws_packet* pack) {
  size_t size, c;
  char* mask;
  size = get_ws_payload_size(pack);
  // Just return early if the packet isn't masked
  if(!packet_is_masked(pack))
    return;
  mask = get_ws_packet_mask(pack);
  // Unset the mask bit
  pack->size &= 0x7F;
  // Unmask the packet
  for(c = 0; c < size; c++) {
    // XOR the payload
    pack->payload[c] ^= mask[c % 4];
  }
}
