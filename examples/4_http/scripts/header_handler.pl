#!/usr/bin/perl

# Read the request line
my $req_line = <>;
(my $method, my $target, my $version) = split(' ', $req_line, 3);

# Ensure that this is a 

my %fields;

# Read the subsequent header lines
while(<>) {
  # Remove trailing newline characters
  chomp;
  # HTTP requests terminate with an empty line
  if($_ eq "") {
    last;
  }
  # Could do something with header lines, but don't in this case
  (my $key, my $value) = split(':', $_, 2);
  # Remove optional leading whitespace from value
  $value =~ s/^\s+//;
  # Force keys to lower case as they must be treated case-insensitively
  $fields{lc($key)} = $value;
}

if($method == "GET") {
  # Print out the upload page
  print "HTTP/1.1 200 OK\r\n";
  print "\r\n";
  print "<!DOCTYPE html>";
  print "<html><head>";
  print "<title>Gunzip Page</title>";
  print "</head><body>";
  print "<h1>Gunzip over HTTP</h1>";
  print "<form ";
  print "action=\"gunzip\" ";
  print "method=\"POST\" ";
  print "enctype=\"multipart/form-data\">";
  print "Upload: <input name=\"upload\" type=\"file\">";
  print "<input type=\"submit\" value=\"send\">";
  print "</form>";
  print "</body></html>";
  # Quit with code 1 to prevent continuing
  exit(1);
} elsif($method == "POST") {
  # 200 status
  print "HTTP/1.1 200 OK\r\n\r\n";
  # Read the body until the form entry
  exit(0);
} else {
  # METHOD NOT ALLOWED ERROR
  print "HTTP/1.1 405 METHOD NOT ALLOWED\r\n";
  print "Allow: GET, POST\r\n"; # Allow header
  print "\r\n";
  print "<!DOCTYPE html>";
  print "<html><head>";
  print "<title>METHOD NOT ALLOWED</title>";
  print "</head><body>";
  print "Only GET and POST requests allowed on this server.";
  print "</body></html>";
  # Quit with code 1 to prevent continuing
  exit(1);
}

