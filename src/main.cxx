#include "arclaunch.hxx"
#include <iostream>


int main(int argc, char** argv) {
  if(argc < 2) {
    std::cerr << "No valid filename provided" << std::endl;
    return 1;
  }
  std::cout << "File: " << argv[1] << std::endl;
  try {
    // Create the context and register the necessary node types
    arclaunch::NodeContext ctx;
    arclaunch::notify<arclaunch::ExecutableNode, executable_t>(ctx);
    arclaunch::notify<arclaunch::ScriptNode, script_t>(ctx);
    arclaunch::notify<arclaunch::LaunchNode, launch_t>(ctx);
    
    std::unique_ptr<launch_t> file(launch(argv[1]));
    arclaunch::Node* elem = ctx.execute(file.get());
    elem->waitFor();
  }
  catch(const xml_schema::exception& e) {
    std::cerr << e << std::endl;
    return 1;
  }
  return 0;
}
