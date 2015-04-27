#include "arclaunch.hxx"
#include <iostream>


int main(int argc, char** argv) {
  if(argc < 2) {
    std::cerr << "No valid filename provided" << std::endl;
    return 1;
  }
  try {
    // Create the context and register the necessary node types
    std::unique_ptr<launch_t> file(launch(argv[1]));
    arclaunch::Node* elem = &arclaunch::context().execute(*file);
    elem->startup();
    elem->waitFor();
  }
  catch(const xml_schema::exception& e) {
    std::cerr << e << std::endl;
    return 1;
  }
  return 0;
}
