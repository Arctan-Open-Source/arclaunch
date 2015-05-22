#include <stdio.h>
// something similar to a C++ map template
// Should work for an arbitrary number of nodes
// Access speed grows at log(N)/log(2)

#define make_traverser(NAME, KEY, VALUE, CMP) \
struct NAME##_node { \
  KEY key; \
  VALUE value; \
}; \
\
VALUE NAME(const KEY key, const struct NAME##_node* tree, unsigned int size) { \
  unsigned int n, root, levels; \
  n = size; \
  root = 1; \
  levels = 0; \
  while(n >>= 1) \
    levels += 1; \
  root <<= levels; \
  root -= 1; \
  int result = CMP(key, tree[root].key); \
  if(result == 0) { \
    return tree[root].value; \
  } else if(result > 0) { \
    tree += root + 1; \
    size -= root + 1; \
  } else { \
    size = 0xFFFF >> (16 - levels); \
  } \
  return NAME(key, tree, size); \
}


