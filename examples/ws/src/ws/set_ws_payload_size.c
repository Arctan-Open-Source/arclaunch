#include "ws_encode_decode.h"

void set_ws_payload_size(struct ws_packet* pack, size_t size) {
  // Note that this does not allocate space correctly
  // space will need to be reallocated
  size_t old_size = get_ws_payload_size(pack);
  // store the mask if needed
  char *mask_loc;
  char mask[4];
  int is_masked;
  is_masked = packet_is_masked(pack);
  if(is_masked) {
    // record the mask
    mask_loc = get_ws_packet_mask(pack);
    mask[0] = mask_loc[0];
    mask[1] = mask_loc[1];
    mask[2] = mask_loc[2];
    mask[3] = mask_loc[3];
  }
  
  if(size > 0xFFFF) {
    // 64 bit size
    pack->size = (pack->size & 0x80) | 0x7F;
    // host to network
    pack->v.l.size = reversell((uint64_t) size);
  } else if(size > 0x7D) {
    // 16 bit size
    pack->size = (pack->size & 0x80) | 0x7E;
    pack->v.m.size = htons((uint16_t) size);
  } else {
    // 7 bit size
    pack->size = (pack->size & 0x80) | size;
  }
  
  if(is_masked) {
    // rewrite the mask
    mask_loc = get_ws_packet_mask(pack);
    mask_loc[0] = mask[0];
    mask_loc[1] = mask[1];
    mask_loc[2] = mask[2];
    mask_loc[3] = mask[3];
  }
}

