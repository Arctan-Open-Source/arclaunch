
# I need another executable to wrap the TLS encryption
# Alternately, a secure socket group type would also be acceptable
# In the future a WebSocket group type may also be desirable
# extension group types could also work via a plugin system which the current design could probably handle easily

add_library(ws_utility
  # UTILITY functions
  utility/reversell.c # done
  )


add_library(ws_encode_decode
  # WS functions
  ws/get_ws_opcode.c # done
  ws/set_ws_opcode.c # done
  ws/ws_frame_is_final.c # done
  ws/make_ws_frame_final.c # done
  ws/make_ws_frame_non_final.c # done

  ws/ws_frame_is_masked.c # done
  ws/mask_ws_frame.c # done
  ws/unmask_ws_frame.c # done

  ws/allocate_ws_payload.c # done
  ws/free_ws_payload.c # done
  
  ws/get_ws_header_size.c # done
  ws/get_ws_payload_size.c # done
  ws/set_ws_payload_size.c # done
  
  ws/get_ws_frame_mask.c # done
  
  # User facing functions
  ws/ws_rx_frame.c
  ws/ws_tx_frame.c
  )

target_link_libraries(ws_encode_decode
  ws_utility
  )


add_library(ws_simple_frame_handler
  ws_simple/ws_simple_continuation_handler.c
  ws_simple/ws_simple_text_handler.c
  ws_simple/ws_simple_binary_handler.c
  ws_simple/ws_simple_close_handler.c
  ws_simple/ws_simple_ping_handler.c
  ws_simple/ws_simple_pong_handler.c
  ws_simple/ws_simple_clear_buffer.c
  )

target_link_libraries(ws_simple_frame_handler
  ws_encode_decode
  )


add_library(ws_fd_encode_decode
  ws_fd/ws_fd_client_connection.c
  ws_fd/ws_fd_server_connection.c
  ws_fd/ws_fd_rx.c
  ws_fd/ws_fd_tx.c
  )

target_link_libraries(ws_fd_encode_decode
  ws_simple_frame_handler
  ws_encode_decode
  )

# A set of short functions for managing shared memory
add_library(shared_mem 
  shared_mem.c
  )


# Needs threading abilities to control semaphores
target_link_libraries(shared_mem
  ${CMAKE_THREAD_LIBS_INIT}
  ${SHM_LIB}
  )

# The encode_decode executable handles websockets
add_executable(encode_decode
  ws_trans.c
  )

add_executable(mem_manage
  shared_mem_manage.c
  )

target_link_libraries(mem_manage
  shared_mem
  )

target_link_libraries(encode_decode
  ws_fd_encode_decode
  shared_mem
  )

