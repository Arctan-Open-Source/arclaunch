
// ------------------------------------------------------------------------------
// Seperate into http_encode_decode.h

enum HTTP_METHOD {
  GET = 0, // transfer a current representation of the target resource
  HEAD = 1, // same as GET, but only transfers the status line and header section
  POST = 2, // Perform resource-specific processing on the request payload
  PUT = 3, // Replace all current representations of the target resource
  DELETE = 4, // Remove all current representations of the target resource
  CONNECT = 5, // Establish a tunnel to the server identified by the target resource
  OPTIONS = 6, // Describe the communications options for the target resource
  TRACE = 7, // Perform a message loop-back test along the path to the target resource
  INVALID = 8 // Invalid if you receive none of these
};


struct header_pair {
  char* key;
  char* value;
};

struct request_group {
  enum HTTP_METHOD method;
  char* uri;
  float version;
};

struct response_group {
  float version;
  unsigned char code;
  char* description;
};

struct header_pair_list {
  struct header_pair data;
  struct header_pair_list* next;
};

// The buffer can be mutilated at will in all of these functions
void decode_http_request_line(int fd, struct request_group* req);
void encode_http_request_line(int fd, struct request_group req);

void decode_http_header_line(int fd, struct header_pair* pair);
void encode_http_header_line(int fd, struct header_pair pair);

void decode_http_response_line(int fd, struct response_group* res);
void encode_http_response_line(int fd, struct response_group res);

enum HTTP_METHOD str_to_http_method(const char* str);
const char* http_method_to_str(enum HTTP_METHOD method);

float str_to_http_version(const char* str);
