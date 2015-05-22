
# I need another executable to wrap the TLS encryption
# Alternately, a secure socket group type would also be acceptable
# In the future a WebSocket group type may also be desirable
# extension group types could also work via a plugin system which the current design could probably handle easily

add_library(ws_utility
  # UTILITY functions
  utility/reversell.c # done
  )

add_library(http_encode_decode
  # HTTP functions
  http/decode_http_request_line.c
  http/encode_http_request_line.c

  http/decode_http_header_line.c
  http/encode_http_header_line.c

  http/decode_http_response_line.c
  http/encode_http_response_line.c

  http/str_to_http_method.c # done
  http/http_method_to_str.c # done
  http/str_to_http_version.c # done
  )

add_library(ws_encode_decode
  # WS functions
  ws/get_ws_opcode.c # done
  ws/set_ws_opcode.c # done
  ws/ws_packet_is_final.c # done
  ws/make_ws_packet_final.c # done
  ws/make_ws_packet_non_final.c # done

  ws/ws_packet_is_masked.c # done
  ws/mask_ws_packet.c # done
  ws/unmask_ws_packet.c # done

  ws/allocate_ws_payload.c # done
  ws/free_ws_payload.c # done
  
  ws/get_ws_header_size.c # done
  ws/get_ws_payload_size.c # done
  ws/set_ws_payload_size.c # done
  
  ws/get_ws_packet_mask.c # done
  
  # User facing functions
  ws/ws_rx_packet.c
  ws/ws_tx_packet.c
  )

target_link_libraries(ws_encode_decode
  ws_utility
  )

# The encode_decode executable handles websockets
add_executable(encode_decode
  encode_decode.c
  )

target_link_libraries(encode_decode
  http_encode_decode
  ws_encode_decode
  )

