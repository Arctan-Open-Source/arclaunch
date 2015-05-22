#include <stdint.h>
#include <stdlib.h>
// -------------------------------------------------------------------
// ws_encode_decode.h
// mask, no extended payload

#define SOCKETIN_FILENO 3
#define SOCKETOUT_FILENO 4
#define SOCKET_BUFFER_SIZE 8192

enum WS_OPCODE {
  CONTINUATION = 0x0,
  TEXT = 0x1,
  BINARY = 0x2,
  NONCONTROL3 = 0x3,
  NONCONTROL4 = 0x4,
  NONCONTROL5 = 0x5,
  NONCONTROL6 = 0x6,
  NONCONTROL7 = 0x7,
  CLOSE = 0x8,
  PING = 0x9,
  PONG = 0xA,
  CONTROLB = 0xB,
  CONTROLC = 0xC,
  CONTROLD = 0xD,
  CONTROLE = 0xE,
  CONTROLF = 0xF
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

struct ws_packet {
  // opcode and size are always the same
  unsigned char opcode; // opcode and final bit
  unsigned char size; // mask bit and payload size
  union {
    uint16_t m_size;
    uint64_t l_size;
    char s_mask[4]; // small masked packet
    struct {
      uint16_t size;
      char mask[4];
    } m; // midsize masked packet
    struct {
      uint64_t size;
      char mask[4];
    } l; // large masked packet
  } v; // the variable section of the header
  char* payload; // data
};

// Stores a pointer to the decoded contained data in the provided pointer

enum WS_OPCODE get_ws_opcode(const struct ws_packet *pack);
void set_ws_opcode(struct ws_packet *pack, enum WS_OPCODE op);

int ws_packet_is_final(const struct ws_packet *pack);

void make_ws_packet_final(struct ws_packet* pack);
void make_ws_packet_non_final(struct ws_packet* pack);

// either masks or unmasks the packet
void mask_ws_packet(struct ws_packet* pack);
void unmask_ws_packet(struct ws_packet* pack);
int ws_packet_is_masked(const struct ws_packet* pack);

// Allocates the payload space based on information from the header
void allocate_ws_payload(struct ws_packet* pack);
// Deallocates the payload space
void free_ws_payload(struct ws_packet* pack);

// gets the size of the payload based on the header
size_t get_ws_payload_size(const struct ws_packet* pack);
// sets the size of the payload based on the header
void set_ws_payload_size(struct ws_packet* pack, size_t size);

// gets the size in bytes of just the header
size_t get_ws_header_size(const struct ws_packet* pack);
char* get_ws_packet_mask(struct ws_packet* pack);

enum WS_ENDPOINT_TYPE {
  SERVER,
  CLIENT
};

typedef void (*ws_packet_handler)(const char*, size_t, void**);

struct ws_connection {
  enum WS_ENDPOINT_TYPE endpoint; // whether server or client
  void *buffer;
  void *tx_data;
  void *rx_data;
  // Functions to abstract receiving and transmitting data
  // size_t rx(size_t n, void* buffer, void* rx_data)
  size_t (*rx)(size_t, void*, const void*);
  // size_t tx(size_t n, void* buffer, void* tx_data)
  size_t (*tx)(size_t, const void*, const void*);
  // handlers for received packets based on their opcode
  // void handler(const char* payload, size_t payload_size, void* buffer);
  union {
    struct {
      ws_packet_handler continuation_handler; // a handler of continuation packets
      ws_packet_handler text_handler;// a handler of text packets
      ws_packet_handler binary_handler;// a handler of binary packets
      ws_packet_handler rsv_data[5]; // handlers for reserved data opcodes
      ws_packet_handler close_handler;// a handler of close packets
      ws_packet_handler ping_handler; // a handler of ping packets
      ws_packet_handler pong_handler;// a handler of pong packets
      ws_packet_handler rsv_control[5]; // handlers for reserved control opcodes
    } named;
    ws_packet_handler array[16];
  } packet_handlers;
};

// Receive a packet and handle it
void ws_rx_packet(struct ws_connection *conn);

// Format and send a data packet
void ws_tx_packet(struct ws_connection *conn, enum WS_OPCODE opcode, const void* payload, size_t numbytes);

