# - Find Julia
# this module looks for Julia
#
#  JULIA_EXECUTABLE     - the full path to the Julia interpreter
#  JULIA_FOUND          - If false, don't attempt to use Julia
#  JULIA_VERSION_STRING - version of Julia found

# Attempt to locate the Julia executable
find_program(JULIA_EXECUTABLE 
  NAMES
    julia
  PATHS 
    /usr/bin
    /usr/local/bin)

find_package(PackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Julia 
  REQUIRED_VARS JULIA_EXECUTABLE
  VERSION_VAR JULIA_VERSION_STRING)
