#include "ws_encode_decode.h"
#include <string.h>

void ws_tx_frame(struct ws_connection* conn, enum WS_OPCODE opcode, const void* payload, size_t numbytes) {
  struct ws_frame pack = {
    0x80 | opcode, // opcode byte
    0x00, //size or mask byte
    {
      .m_size = 0x0000
    },
    NULL
  };
  set_ws_opcode(&pack, opcode);
  set_ws_payload_size(&pack, numbytes);
  allocate_ws_payload(&pack);
  memcpy(pack.payload, payload, numbytes);
  if(conn->endpoint == WS_CLIENT)
    mask_ws_frame(&pack);
  // TODO add a means to use non-final frames
  make_ws_frame_final(&pack);
  conn->tx(get_ws_header_size(&pack), &pack, conn->tx_data);
  conn->tx(get_ws_payload_size(&pack), pack.payload, conn->tx_data);
  free_ws_payload(&pack);
}

