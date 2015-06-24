#!/usr/bin/perl

use Digest::SHA qw(sha1_base64);

# Check a comma seperated list for a string
sub found_in_cs_list {
  foreach $token (split(',', $_[1])) {
    # remove leading whitespace from value
    $token =~ s/^\s+//;
    # remove trailing whitespace from value
    $token =~ s/\s+$//;
    if($token eq $_[0]) {
      return 1;
    }
  }
  return 0;
}

# Handle the handshake in Perl
# as perl can deal with text most easily
# All non-upgrade requests

# Set \r\n as the EOL
$/ = "\r\n";

open(my $sock_in, "<&=", 3);
open(my $sock_out, ">&=", 4);

my $req_line = <$sock_in>;

($method, $target, $version) = split(' ', $req_line, 3);

my %fields;

# Read the fields
while(<$sock_in>) {
  chomp;
  if($_ eq "") {
    last;
  }
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
  print {$sock_out} "HTTP/1.1 405 method not allowed\r\n";
  print {$sock_out} "Allow:GET\r\n\r\n";
  # Exit with 1 to indicate failure
  exit 1;
}

# Check Upgrade tag
if(lc($fields{"upgrade"}) ne "websocket") {
  # Send a 426 "Upgrade Required" error
  print {$sock_out} "HTTP/1.1 426 upgrade required\r\n";
  print {$sock_out} "Upgrade:websocket\r\n\r\n";
  exit 1;
}
  
if(!found_in_cs_list("upgrade", lc($fields{"connection"}))) {
  print {$sock_out} "HTTP/1.1 400 bad request\r\n\r\n";
  exit 1;
}

# Host doesn't do anything important in this case
# Origin doesn't do anything important either
# Sec-WebSocket-Protocol doesn't do anything important in this case

# Sec-WebSocket-Version must contain 13
# if it fails, send out the acceptable version
if(!found_in_cs_list("13", $fields{"sec-websocket-version"})) {
  print {$sock_out} "HTTP/1.1 400 bad request\r\n";
  print {$sock_out} "Sec-WebSocket-Version:13\r\n\r\n";
  exit 1;
}

# Check Sec-WebSocket-Key for generation of Sec-WebSocket-Accept
if(!$fields{"sec-websocket-key"}) {
  print {$sock_out} "HTTP/1.1 400 missing key\r\n";
  exit 1;
}

# Calculate Sec-WebSocket-Accept from Sec-Websocket-Key
$key_accept = sha1_base64($fields{'sec-websocket-key'}, "258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
while(length($key_accept) % 4) {
  $key_accept .= '=';
}

print {$sock_out} "HTTP/1.1 101 switching protocols\r\n";
print {$sock_out} "Upgrade: websocket\r\n";
print {$sock_out} "Connection: Upgrade\r\n";
print {$sock_out} "Sec-WebSocket-Accept: $key_accept\r\n\r\n";

# Pass everything onto the next process

# Exit with 0 to indicate success
exit 0;

