#include "ws_encode_decode.h"

size_t get_ws_header_size(const struct ws_packet* pack) {
  switch(pack->size) {
  case 0xFF:
    // masked with 64 bit payload size
    return 14;
  case 0xFE:
    // masked
    return 8;
  case 0x7F:
    // unmasked with 64 bit payload size
    return 10;
  case 0x7E:
    // unmasked with 16 bit payload size
    return 4;
  default:
    // less than 126 byte payload
    if(packet_is_masked(pack))
      return 6;
    else
      return 2;
  }
}

