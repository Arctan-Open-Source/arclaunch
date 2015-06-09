#include "ws_encode_decode.h"

void ws_rx_frame(struct ws_connection* conn) {
  struct ws_frame pack;
  // receive sufficient information for the size of the header
  conn->rx(2, &pack, conn->rx_data);
  // receive the remainder of the header
  conn->rx(get_ws_header_size(&pack) - 2, &pack.v, conn->rx_data);
  // allocate space for the payload
  allocate_ws_payload(&pack);
  size_t payload_size = get_ws_payload_size(&pack);
  // Receive the payload
  conn->rx(payload_size, pack.payload, conn->rx_data);
  // Handle the frame
  conn->frame_handlers.array[pack.opcode](conn, &pack, &(conn->buffer));
  // Deallocate the payload
  free_ws_payload(&pack);
}

