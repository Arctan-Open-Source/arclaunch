#include "http_encode_decode.h"
#include <stdlib.h>

float str_to_http_version(const char* str) {
  // Use http version
  if(strncmp(str, "HTTP/", 5) != 0)
    return 0.0;
  str += 5;
  return atof(str);
}

