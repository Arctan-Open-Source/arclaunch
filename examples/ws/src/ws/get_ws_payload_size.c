#include "ws_encode_decode.h"

size_t get_ws_payload_size(const struct ws_packet* pack) {
  char size = pack->size & 0x7F;
  if(size == 0x7F)
    return reversell(pack->v.m_size); // network to host long long
  else if(size == 0x7E)
    return ntohs(pack->v.l_size); // network to host short
  else
    return size;
}

