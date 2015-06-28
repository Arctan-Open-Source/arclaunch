#include "shared_mem.h"
#include <string.h>
#include <fcntl.h>

void replace_char(char* str, char from, char to) {
  while(*str != 0) {
    if(*str == from)
      *str = to;
    ++str;
  }
}

int create_shared_memory(const char* name) {
  char mem_name[255];
  mem_name[0] = '/';
  strcpy(mem_name + 1, name);
  replace_char(mem_name, '/', '_');
  return shm_open(mem_name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
}

int open_shared_memory(const char* name) {
  char mem_name[255];
  mem_name[0] = '/';
  strcpy(mem_name + 1, name);
  replace_char(mem_name, '/', '_');
  return shm_open(mem_name, O_RDWR, S_IRUSR | S_IWUSR);
}

void destroy_shared_memory(const char* name) {
  char mem_name[255];
  mem_name[0] = '/';
  strcpy(mem_name + 1, name);
  replace_char(mem_name, '/', '_');
  shm_unlink(name);
}

void replace_char(char* str, char from, char to);
int create_shared_memory(const char* name);
int open_shared_memory(const char* name);
void destroy_shared_memory(const char* name);
