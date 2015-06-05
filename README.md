# arclaunch
A utility for starting up and managing a set of programs in parallel.

## Compilation
Compilation depends upon cmake 2.8 or later, a C++11 supporting compiler, and Code Synthesis XSD 4.0.0 or later.

On linux, after all dependencies are installed the following commands can be used to compile arclaunch starting from the repository directory.

> mkdir build  
> cd build  
> cmake ..  
> make  

### Troubleshooting
Installing Code Synthesis XSD may be complicated by name reuse between the Ubuntu package for Mono XSD and Code Synthesis's XSD package. The most clear cut way to resolve this problem is to uninstall Mono XSD.

## Install
After compilation has been completed installation on Linux can be performed by entering the build directory and running the install build target.

> make install

## Concept
The basic idea behind arclaunch is to allow programming different pieces of an application in the most convenient language. The easiest way to accomplish this in a Unix-like environment is with the use of file descriptors. Using file descriptors instead of an API saves the work required to learn those APIs. In some cases, interaction between programs can occur entirely using only using STDIN or STDOUT as is currently often done from the command line. In other cases a file descriptor can originate in one program for manipulations then pass onto another program.


