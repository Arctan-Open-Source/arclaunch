#include "http_encode_decode.h"
#include "binary_tree.h"
#include "string.h"

make_traverser(traverse_method_tree, const char*, enum HTTP_METHOD, strcmp);

struct traverse_method_tree_node method_tree[8] = {
  {"CONNECT", CONNECT},
  {"DELETE", DELETE},
  {"GET", GET},
  {"HEAD", HEAD},
  {"OPTIONS", OPTIONS},
  {"POST", POST},
  {"PUT", PUT},
  {"TRACE", TRACE}
};

enum HTTP_METHOD str_to_http_method(const char* str) {
  return traverse_method_tree(str, method_tree, 8);
}

