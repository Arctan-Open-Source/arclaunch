#include "arclaunch.hxx"
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcgiapp.h>

void version_act() {
  std::cout << "Arclaunch version " 
    << ARCLAUNCH_VERSION_MAJOR << "." 
    << ARCLAUNCH_VERSION_MINOR << "."
    << ARCLAUNCH_VERSION_PATCH << std::endl;
}

bool persist(false);

void persist_act() {
  persist = true;
}

bool fcgi(false);

void fcgi_act() {
  fcgi = true;
}


int main(int argc, char** argv) {
  // Remove initial options
  // --version
  // Prints out the version string and exits
  
  // --persist
  // Keeps going until SIGINT is received
  // also opens a named pipe that can be used to send
  // send commands

  // --cgi
  // CGI mode is fairly simple and forwards environment variables to the base 
  // node's children, programmaticly much simpler on the backend.

  // --fcgi
  // similar to cgi but very different
  
  const char* options[] = {
    "--version",
    "--persist",
    "--fcgi",
    NULL
  };
  
  typedef void (*option_actions)();

  option_actions opt_acts[] = {
    &version_act,
    &persist_act,
    &fcgi_act
  };
  
  int argn = 1;
  while(argn < argc && argv[argn][0] == '-' && argv[argn][1] == '-') {
    int c;
    std::string arg(argv[argn]);
    for(c = 0; options[c] != NULL; c++)
      if(arg == options[c]) {
        opt_acts[c]();
      }
    argn++;
  }
  
  std::string path;
  if(argc <= argn) {
    // Attempt to find launch.xml as a standard
    std::cerr << "Could not find launch.xml" << std::endl;
    return 1;
  }
  const char* fifo_name = "/tmp/launch_fifo";
  bool made_fifo = false;
  try {
    // Create the context and register the necessary node types
    std::unique_ptr<launch_t> file(launch(argv[argn]));
    arclaunch::Node* elem = &arclaunch::context().execute(*file);
    // Always link the external stdin, stdout, and stderr to the base node
    // TODO: they'll probably become unlinked in nodes like socket nodes that 
    // have multiple startup
    elem->linkStdin(STDIN_FILENO);
    elem->linkStdout(STDOUT_FILENO);
    elem->linkStderr(STDERR_FILENO);
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
