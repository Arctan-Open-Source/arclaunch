#include "ws_encode_decode.h"

int ws_frame_is_masked(const struct ws_frame* pack) {
  return pack->size & 0x80;
}

