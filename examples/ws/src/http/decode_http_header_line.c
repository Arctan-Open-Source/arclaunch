#include "http_encode_decode.h"
#include <stdlib.h>
#include <string.h>

char* http_header_line(char* line, ssize_t bound, struct header_pair* pair) {
  char *token_save_point, *key, *value;
  key = strtok_r(line, ":", &token_save_point);
  value = strtok_r(NULL, "\r\n", &token_save_point);
  pair->key = key;
  pair->value = value;
  return token_save_point;
}

