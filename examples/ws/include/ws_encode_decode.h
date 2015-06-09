
#ifndef WS_ENCODE_DECODE_H_
#define WS_ENCODE_DECODE_H_
#include <stdint.h>
#include <stdlib.h>
// -------------------------------------------------------------------
// ws_encode_decode.h
// mask, no extended payload

#define SOCKETIN_FILENO 3
#define SOCKETOUT_FILENO 4
#define SOCKET_BUFFER_SIZE 8192

// TODO: Need to reterm packets to frames, respecting the nomenclature of the specification

enum WS_OPCODE {
  WS_CONTINUATION = 0x0,
  WS_TEXT = 0x1,
  WS_BINARY = 0x2,
  WS_DATA3 = 0x3,
  WS_DATA4 = 0x4,
  WS_DATA5 = 0x5,
  WS_DATA6 = 0x6,
  WS_DATA7 = 0x7,
  WS_CLOSE = 0x8,
  WS_PING = 0x9,
  WS_PONG = 0xA,
  WS_CONTROLB = 0xB,
  WS_CONTROLC = 0xC,
  WS_CONTROLD = 0xD,
  WS_CONTROLE = 0xE,
  WS_CONTROLF = 0xF
};

enum WS_CLOSE_CODE {
  NORMAL = 1000,
  GOING_AWAY = 1001,
  PROTOCOL_ERROR = 1002,
  UNSUPPORTED_DATA = 1003,
  RESERVED4 = 1004,
  NO_STATUS_RCVD = 1005,
  ABNORMAL_CLOSURE = 1006,
  INVALID_FRAME_PAYLOAD_DATA = 1007,
  POLICY_VIOLATION = 1008,
  MESSAGE_TOO_BIG = 1009,
  MANDATORY_EXT = 1010,
  INTERNAL_SERVER_ERROR = 1011,
  TLS_HANDSHAKE = 1015
};

struct ws_frame {
  // opcode and size are always the same
  unsigned char opcode; // opcode and final bit
  unsigned char size; // mask bit and payload size
  union {
    uint16_t m_size;
    uint64_t l_size;
    char s_mask[4]; // small masked frame
    struct {
      uint16_t size;
      char mask[4];
    } m; // midsize masked frame
    struct {
      uint64_t size;
      char mask[4];
    } l; // large masked frame
  } v; // the variable section of the header
  char* payload; // data
};

// Stores a pointer to the decoded contained data in the provided pointer

enum WS_OPCODE get_ws_opcode(const struct ws_frame *pack);
void set_ws_opcode(struct ws_frame *pack, enum WS_OPCODE op);

int ws_frame_is_final(const struct ws_frame *pack);

void make_ws_frame_final(struct ws_frame* pack);
void make_ws_frame_non_final(struct ws_frame* pack);

// either masks or unmasks the frame
void mask_ws_frame(struct ws_frame* pack);
void unmask_ws_frame(struct ws_frame* pack);
int ws_frame_is_masked(const struct ws_frame* pack);

// Allocates the payload space based on information from the header
void allocate_ws_payload(struct ws_frame* pack);
// Deallocates the payload space
void free_ws_payload(struct ws_frame* pack);

// gets the size of the payload based on the header
size_t get_ws_payload_size(const struct ws_frame* pack);
// sets the size of the payload based on the header
void set_ws_payload_size(struct ws_frame* pack, size_t size);

// gets the size in bytes of just the header
size_t get_ws_header_size(const struct ws_frame* pack);
char* get_ws_frame_mask(struct ws_frame* pack);

enum WS_ENDPOINT_TYPE {
  WS_SERVER,
  WS_CLIENT
};

struct ws_connection;
// Uses a pointer to a pointer to allow reallocation
typedef void (*ws_frame_handler)(struct ws_connection* conn, const struct ws_frame*, void**);

struct ws_connection {
  enum WS_ENDPOINT_TYPE endpoint; // whether server or client
  int received_close; // whether the connection has received a close packet
  int transmitted_close; // whether the connection has transmitted a close packet
  void *tx_data;
  void *rx_data;
  void *buffer;
  // Functions to abstract receiving and transmitting data
  // size_t rx(size_t n, void* buffer, const void* rx_data)
  size_t (*rx)(size_t, void*, const void*);
  // size_t tx(size_t n, const void* buffer, const void* tx_data)
  size_t (*tx)(size_t, const void*, const void*);
  // handlers for received frames based on their opcode
  // void handler(const char* payload, size_t payload_size, void* buffer);
  union {
    struct {
      ws_frame_handler continuation_handler; // a handler of continuation frames
      ws_frame_handler text_handler;// a handler of text frames
      ws_frame_handler binary_handler;// a handler of binary frames
      ws_frame_handler rsv_data[5]; // handlers for reserved data opcodes
      ws_frame_handler close_handler;// a handler of close frames
      ws_frame_handler ping_handler; // a handler of ping frames
      ws_frame_handler pong_handler;// a handler of pong frames
      ws_frame_handler rsv_control[5]; // handlers for reserved control opcodes
    } named;
    ws_frame_handler array[16];
  } frame_handlers;
};

// Receive a frame and handle it
void ws_rx_frame(struct ws_connection *conn);

// Format and send a data frame
void ws_tx_frame(struct ws_connection *conn, enum WS_OPCODE opcode, const void* payload, size_t numbytes);

#endif
