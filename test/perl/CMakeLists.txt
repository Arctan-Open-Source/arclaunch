
find_package(Perl)
if(PERL_FOUND)
  message(STATUS "Creating Perl tests")
  add_subdirectory(argv)
  ADD_ARGV_TEST("perl")
else(PERL_FOUND)
  message(STATUS "Could not locate Perl executable")
endif(PERL_FOUND)
