#include "arclaunch.hxx"
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

void version_act() {
  std::cout << "Arclaunch version 0.1.0" << std::endl;
}

bool persist(false);

void persist_act() {
  persist = true;
}


int main(int argc, char** argv) {
  // Remove initial options
  // --version
  // Prints out the version string and exits
  
  // --persist
  // Keeps going until SIGINT is received
  // also opens a named pipe that can be used to send
  // send commands
  
  const char* options[] = {
    "--version",
    "--persist"
  };
  
  typedef void (*option_actions)();

  option_actions opt_acts[] = {
    &version_act,
    &persist_act
  };
  
  int argn = 1;
  while(argn < argc && argv[argn][0] == '-' && argv[argn][1] == '-') {
    int c;
    std::string arg(argv[argn]);
    for(c = 0; c < 2; c++)
      if(arg == options[c]) {
        opt_acts[c]();
        break;
      }
    if(c == 2)
      break;
    argn++;
  }
  
  if(argc <= argn) {
    std::cerr << "No valid filename provided" << std::endl;
    return 1;
  }
  const char* fifo_name = "/tmp/launch_fifo";
  bool made_fifo = false;
  try {
    // Create the context and register the necessary node types
    std::unique_ptr<launch_t> file(launch(argv[argn]));
    arclaunch::Node* elem = &arclaunch::context().execute(*file);
    elem->startup();
    if(persist && !mkfifo(fifo_name, S_IRUSR | S_IWUSR)) {
      made_fifo = true;
      int fd = open(fifo_name, O_CLOEXEC, O_RDONLY);
      char buff;
      // should block until something is written to the fifo
      read(fd, &buff, 1);
      unlink(fifo_name);
      made_fifo = false;
    }
    elem->waitFor();
  }
  catch(const xml_schema::exception& e) {
    unlink(fifo_name);
    std::cerr << e << std::endl;
    return 1;
  }
  catch(...) {
    if(made_fifo)
      unlink(fifo_name);
  }
  return 0;
}
