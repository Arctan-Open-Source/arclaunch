#!/usr/bin/env perl

# Perl has the special case of ARGV[0] being the first argument
# In most languages ARGV[0] is the path to the executable

foreach $argnum (0 .. $#ARGV) {
  print "$ARGV[$argnum]\n"
}
