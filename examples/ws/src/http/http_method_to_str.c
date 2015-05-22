#include "http_encode_decode.h"

char* method_list[8] = {
  "GET", 
  "HEAD", 
  "POST", 
  "PUT", 
  "DELETE", 
  "CONNECT", 
  "OPTIONS", 
  "TRACE"};

const char* http_method_to_str(enum HTTP_METHOD method) {
  return method_list[method];
}

