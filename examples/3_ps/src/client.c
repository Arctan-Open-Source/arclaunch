#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <string.h>
#include <sys/select.h>

#define BUFFER_SIZE 512

void write_err_msg(const char* msg) {
  write(STDERR_FILENO, msg, strlen(msg));
}

void checkpoint() {
  write(STDERR_FILENO, "CHECK\n", 6);
}

int main(int argc, char** argv) {
  int fd, nfds, gai_result, con_result, hasData;
  ssize_t nread, nwrite;
  const char *input_err_msg = "node and service must be provided\n",
    *read_err_msg = "An error occurred while reading\n",
    *select_err_msg = "An error occurred while waiting\n",
    *write_error_msg = "An error occurred while writing\n",
    *gai_err_msg = "An error occurred while interpretting the service\n",
    *con_err_msg = "An error occurred while connecting to the service\n";
  struct addrinfo *info, hints = {
    0, // AI_FLAGS
    AF_UNSPEC, // AI_FAMILY, IPv4 or IPv6
    SOCK_STREAM, // AI_SOCKTYPE
    0,    // AI_PROTOCOL
    0,    // AI_ADDRLEN, must be 0
    NULL, // AI_ADDR, must be null
    NULL, // AI_CANONNAME, must be null
    NULL  // AI_NEXT, must be null
  };
  struct timeval t, r = {
    0,  // tv_sec
    100000 // tv_usec, 100ms
  };
  char buffer[BUFFER_SIZE];
  fd_set fds;
  if(argc < 3) {
    write_err_msg(input_err_msg);
    return 1;
  }
  // getaddrinfo
  gai_result = getaddrinfo(argv[1], argv[2], &hints, &info);
  if(gai_result) {
    write_err_msg(gai_err_msg);
    write_err_msg(gai_strerror(gai_result));
    write_err_msg("\n");
    return 1;
  }
  // try connecting to the lowest returned service
  fd = socket(AF_INET, SOCK_STREAM, 0);
  con_result = connect(fd, info->ai_addr, info->ai_addrlen);
  if(con_result < 0) {
    // An error occurred
    write_err_msg(con_err_msg);
    return 1;
  }
  freeaddrinfo(info);
  // Listen for everything
  // Give 500ms to time out
  t = r;
  FD_ZERO(&fds);
  FD_SET(fd, &fds);
  while(nfds = select(fd + 1, &fds, NULL, NULL, &t)) {
    if(nfds < 0) {
      // A select error occurred
      write_err_msg(select_err_msg);
      return 1;
    }
    hasData = FD_ISSET(fd, &fds);
    FD_ZERO(&fds);
    if(hasData) {
      nread = read(fd, buffer, BUFFER_SIZE);
      if(nread < 0) {
        write_err_msg(read_err_msg);
        return 1;
      }
      nwrite = write(STDOUT_FILENO, buffer, nread);
      if(nwrite < 0) {
        write_err_msg(write_error_msg);
        return 1;
      }
      if(nread == 0) {
        close(fd);
      } else {
        FD_SET(fd, &fds);
      }
    }
    t = r;
  }
  close(fd);
  return 0;
}

