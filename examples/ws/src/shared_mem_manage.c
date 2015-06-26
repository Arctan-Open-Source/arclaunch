#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <semaphore.h>
#include "shared_mem.h"

long PAGE_SIZE;

int main(int argc, char** argv) {
  int fd;
  PAGE_SIZE = sysconf(_SC_PAGE_SIZE);
  // Generate the first page of shared memory
  if(argc < 2)
    return 1;
  fd = create_shared_memory(argv[1]);
  size_t meta_size = sizeof(struct meta_page);
  size_t user_size = sizeof(struct user_page);
  // "truncate" to accomodate the required space
  ftruncate(fd, meta_size);
  // map the memory
  struct meta_page* page = mmap(NULL, meta_size, PROT_READ | PROT_WRITE,
    MAP_SHARED, fd, 0);
  // TODO: initialize the semaphores
  // TODO: wait for the primary semaphore
  munmap(page, meta_size);
  close(fd);
  destroy_shared_memory(argv[1]);
  return 0;
}
