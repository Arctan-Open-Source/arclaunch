# arclaunch
A utility for starting up and managing a set of programs in parallel.

## Compilation
Compilation depends upon [cmake 2.8 or later](http://www.cmake.org/), a C++11 supporting compiler, and [Code Synthesis XSD 4.0.0 or later](http://www.codesynthesis.com/projects/xsd/).

On Linux, after all dependencies are installed the following commands can be used to compile arclaunch starting from the repository directory.

> mkdir build  
> cd build  
> cmake ..  
> make  

### Troubleshooting
Installing Code Synthesis XSD may be complicated by name reuse between the Ubuntu package for Mono XSD and Code Synthesis's XSD package. The most clear cut way to resolve this problem is to uninstall Mono XSD.

## Install
After compilation has been completed installation on Ubuntu Linux can be performed by entering the build directory and running the install build target.

> make install
> sudo ldconfig

Depending on the configured install location, root permissions may be necessary.

> sudo make install
> sudo ldconfig

## Remove

In order to remove this program after installation on Linux the install manifest (which is generated at install time by cmake) can be used to remove all installed components. Enter the build directory and run the following command:

> rm $(cat install\_manifest.txt)

Or if arclaunch was installed with root permissions:

> sudo rm $(cat install\_manifest.txt)

## Concept
The basic idea behind arclaunch is to allow programming different pieces of an application in the most convenient language. The easiest way to accomplish this in a Unix-like environment is with the use of file descriptors. Using file descriptors instead of an API saves the work required to learn those APIs. In some cases, interaction between programs can occur entirely using only using STDIN or STDOUT as is currently often done from the command line. In other cases a file descriptor can originate in one program for manipulations then pass onto another program.


