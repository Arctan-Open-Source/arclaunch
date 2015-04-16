
find_package(Perl)
if(PERL_FOUND)
  message(STATUS "Creating Perl tests")
  add_subdirectory(argv)
  configure_file(locations.hxx.in ${CMAKE_CURRENT_BINARY_DIR}/locations.hxx)
  include_directories(${CMAKE_CURRENT_BINARY_DIR})
  add_executable(perl_test
    perl.cxx
    )
  target_link_libraries(perl_test
    gtest_main
    arclaunch
    )
  add_test(NAME test_perl COMMAND perl_test)
  message(STATUS "argv perl script:        ${ARGV_PERL}")
  message(STATUS "argv perl launch file:   ${ARGV_PERL_LAUNCH}")
else(PERL_FOUND)
  message(STATUS "Could not locate Perl executable")
endif(PERL_FOUND)
