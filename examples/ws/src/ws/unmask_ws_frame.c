#include "ws_encode_decode.h"


void unmask_ws_frame(struct ws_frame* pack) {
  size_t size, c;
  char* mask;
  size = get_ws_payload_size(pack);
  // Just return early if the frame isn't masked
  if(!ws_frame_is_masked(pack))
    return;
  mask = get_ws_frame_mask(pack);
  // Unset the mask bit
  pack->size &= 0x7F;
  // Unmask the frame
  for(c = 0; c < size; c++) {
    // XOR the payload
    pack->payload[c] ^= mask[c % 4];
  }
}
