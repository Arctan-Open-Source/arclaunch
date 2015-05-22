#include "ws_encode_decode.h"
#include <stdlib.h>

void mask_ws_packet(struct ws_packet* pack) {
  size_t size, c;
  char* mask;
  size = get_ws_payload_size(pack);
  // Just return early if the packet is already masked
  if(packet_is_masked(pack))
    return;
  // set the mask bit
  pack->size |= 0x80;
  mask = get_ws_packet_mask(pack);
  // generate a mask
  mask[0] = (char) random();
  mask[1] = (char) random();
  mask[2] = (char) random();
  mask[3] = (char) random();
  // Mask the packet
  for(c = 0; c < size; c++) {
    // XOR the payload
    pack->payload[c] ^= mask[c % 4];
  }
}

