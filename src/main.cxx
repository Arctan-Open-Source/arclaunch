#include "arclaunch.hxx"
#include <iostream>


void polymorphTest(launch_t& elem) {
  for(launch_t::node_iterator it(elem.node().begin()); it != elem.node().end(); it++) {
    if(executable_t* e_cast = dynamic_cast<executable_t*>(&(*it)))
      std::cout << "casted " << it->name() << " to executable_t" << std::endl;
    else if(launch_t* l_cast = dynamic_cast<launch_t*>(&(*it)))
      std::cout << "casted " << it->name() << " to launch_t" << std::endl;
    else if(script_t* s_cast = dynamic_cast<script_t*>(&(*it)))
      std::cout << "casted " << it->name() << " to script_t" << std::endl;
    else
      std::cout << "couldn't cast " << it->name() << std::endl;
  }
}

int main(int argc, char** argv) {
  if(argc < 2) {
    std::cerr << "No valid filename provided" << std::endl;
    return 1;
  }
  std::cout << "File: " << argv[1] << std::endl;
  try {
    // Create the context and register the necessary node types
    arclaunch::NodeContext ctx;
    arclaunch::notify<arclaunch::FileNode, file_t>(ctx);
    arclaunch::notify<arclaunch::ExecutableNode, executable_t>(ctx);
    arclaunch::notify<arclaunch::ScriptNode, script_t>(ctx);
    arclaunch::notify<arclaunch::LaunchNode, launch_t>(ctx);
    
    std::unique_ptr<launch_t> file(launch(argv[1]));
    polymorphTest(*file);
    arclaunch::Node* elem = &ctx.execute(*file);
    // 
    elem->startup();
    elem->waitFor();
  }
  catch(const xml_schema::exception& e) {
    std::cerr << e << std::endl;
    return 1;
  }
  return 0;
}
