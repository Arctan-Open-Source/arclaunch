#include "ws_encode_decode.h"
#include <stdlib.h>

void allocate_ws_payload(struct ws_frame* pack) {
  // uses the header to determine the amount of memory to allocate
  pack->payload = malloc(get_ws_payload_size(pack));
}

