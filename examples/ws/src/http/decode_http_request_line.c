#include "http_encode_decode.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void http_request_line(int fd, struct request_group* req) {
  // read a line in
  char *token_save_point, *line, *method_str, *uri_str, *version_str;
  FILE* p;
  size_t len, n;
  p = fdopen(dup(fd), "r");
  line = NULL;
  getline(&line, &n, p);
  method_str = strtok_r(line, " ", &token_save_point);
  uri_str = strtok_r(NULL, " ", &token_save_point);
  version_str = strtok_r(NULL, "\r\n", &token_save_point);
  // parse the method
  req->method = str_to_http_method(method_str);
  len = strlen(uri_str);
  req->uri = malloc(len + 1);
  req->uri[len] = '\0';
  memcpy(req->uri, uri_str, strlen(uri_str));
  req->version = str_to_http_version(version_str);
  
  free(line);
  fclose(p);
}

