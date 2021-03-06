#ifndef SHARED_MEM_H_
#define SHARED_MEM_H_
#include <semaphore.h>

struct meta_page {
  // Used to hold this process until ready to close out
  sem_t close_mem;
  // Only one user at a time can allocate space
  sem_t allocate; 
  // Tracks the number of connected users
  unsigned int user_count;
  unsigned int connected_user_count;
  // Used to balance the connections across different users
  unsigned int delegator;
};

struct user_page {
  // Used to block others from requesting from this user
  sem_t requesting;
  // Used to force users to wait for requests
  sem_t requested;
  // Used to contain SDP packets
  char buffer[1024];
};

void replace_char(char* str, char from, char to);

// Safely initializes the shared memory and meta page
int create_shared_memory(const char* name);
// Safely destroys and unlinks the shared memory and meta page
void destroy_shared_memory(const char* name);

// Safely opens the shared memory
int open_shared_memory(const char* name);

// Safely initializes semaphores on the user page
// and marks it as used
void init_user_page(struct user_page*);

// Safely destroys semaphores on the user page
// and marks it as unused
void destroy_user_page(struct user_page*);

#endif
