
find_package(Julia)
if(JULIA_FOUND)
  message(STATUS "creating Julia tests")
else(JULIA_FOUND)
  message(STATUS "Could not locate Julia executable")
endif(JULIA_FOUND)
