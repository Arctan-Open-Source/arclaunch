#!/usr/bin/perl

# Handle the handshake in Perl
# as perl can deal with text most easily
# All non-upgrade requests

my $req_line = <>;

($method, $target, $version) = split(' ', $req_line, 3);

my %fields;

# Read the fields
while(<>) {
  chomp;
  if($_)
    last;
  ($key, $value) = split(':', $_, 2);
  # remove leading whitespace from value
  $value =~ s/^\s+//;
  # Force field keys to lowercase since they need to be case insensitive
  $fields{lc($key)} = $value;
}

# Needs to be a GET request
# target doesn't really matter
# don't worry with HTTP version

if($method ne "GET") {
  # Needs to fail for non-get requests
  # Send a 405 "method not allowed" error with GET in the Allow header field
  print("HTTP/1.1 405 method not allowed\r\n");
  print("Allow:GET\r\n\r\n");
  # Exit with 1 to indicate failure
  exit 1;
}
# Check Upgrade tag
if($fields{"upgrade"} ne "") {
  # Send a 426 "Upgrade Required" error
  print("HTTP/1.1 426 upgrade required\r\n");
  print("Upgrade:websocket\r\n\r\n");
  exit 1;
}
  
# Check Connection tag case insensitively
if(lc($fields{"connection"}) ne "upgrade") {
  # Give a more general 400 response
  print("HTTP/1.1 400 bad request\r\n\r\n");
  exit 1;
}

# Host doesn't do anything important in this case
# Origin doesn't do anything important either
# Sec-WebSocket-Protocol doesn't do anything important in this case

# Sec-WebSocket-Version must contain 13
# if it fails, send out the acceptable version
if($fields{"sec-websocket-version"}) {
  printf("HTTP/1.1 400 bad request\r\n");
  printf("Sec-WebSocket-Version:13\r\n\r\n");
  exit 1;
}

# Check Sec-WebSocket-Key for generation of Sec-WebSocket-Accept


print("HTTP/1.1 101 switching protocols\r\n");
print("Upgrade:websocket\r\n");
print("Connection:Upgrade\r\n");
print("Sec-WebSocket-Accept:\r\n\r\n");

# Pass everything onto the next process

# Exit with 0 to indicate success
exit 0;

