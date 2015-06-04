#include "encode_decode.h"

union parts64 {
  uint64_t whole;
  char byte[8];
};

uint64_t reversell(uint64_t bytes) {
  union parts64 parts;
  char tmp;
  parts.whole = bytes;
  // swap bytes
  tmp = parts.byte[0];
  parts.byte[0] = parts.byte[7];
  parts.byte[7] = tmp;
  tmp = parts.byte[1];
  parts.byte[1] = parts.byte[6];
  parts.byte[6] = tmp;
  tmp = parts.byte[2];
  parts.byte[2] = parts.byte[5];
  parts.byte[5] = tmp;
  tmp = parts.byte[3];
  parts.byte[3] = parts.byte[4];
  parts.byte[4] = tmp;
  return parts.whole;
}

