#include "ws_encode_decode.h"
#include <stdlib.h>

void free_ws_payload(struct ws_packet* pack) {
  free(pack->payload);
  pack->payload = NULL;
}

