#!/usr/bin/perl

use strict;

local $/ = "\r\n";

sub forward_file_handler
{
  my ($source, $destination) = @_;
  while(<$source>) {
    print {$destination} $_;
  }
}

sub write_html_code_response
{
  my ($fh, $code, $message, $target) = @_;
  print {$fh} "HTTP/1.1 $code $message";
  print {$fh}  "Content-type: text/html\r\n";
  print {$fh}  "\r\n";
  open(my $file, "<", $target);
  forward_file_handler($file, $fh);
  close($file);
}

# Attempts to write a file response to a file handle
sub write_file_response
{
  my ($fh, $target, $fallback) = @_;
  # Attempt to open a file
  if (-r $target)
  {
    write_html_code_response($fh, 202, "OKAY", $target);
  } else {
    write_html_code_response($fh, 404, "NOT FOUND", $fallback);
  }
}

sub read_header
{
  local $/ = "\r\n";
  my ($fh) = @_;
  my %fields;
  # Should return a map
  while(<$fh>) {
    # Remove trailing newline characters
    chomp;
    # HTTP requests terminate with an empty line
    if($_ eq "") {
      last;
    }
    (my $key, my $value) = split(':', $_, 2);
    # Remove optional leading whitespace from value
    $value =~ s/^\s+//;
    # Force keys to lower case as they must be treated case-insensitively
    $fields{lc($key)} = $value;
  }
  return %fields;
}


sub forward_multipart_data
{ 
  local $/ = "\r\n";
  my ($source, $destination, $part_name, $boundary) = @_;
  my $inEncapsulation = 0;
  my $encapsulationBoundary = "";
  my $inContent = 0;
  my $inCorrectPart = 0;
  my %part_fields;
  # First read the multipart/form-data header
  while(<$source>)
  {
    my $line = $_;
    chomp $line;
    if($inEncapsulation) {
      if($inContent) {
        # Check for the matching encapsulation boundary
        my $lined = $_;
        chomp $lined;
        if($lined eq $encapsulationBoundary) {
          $inContent = 0;
          $inEncapsulation = 0;
          $inCorrectPart = 0;
        } else {
          print {$destination} $_;
        }
      } else {
        # Read the part's header
        if($_ == "") {
          $inContent = 1;
        }
      }
    } else {
      # Look for encapsulation boundaries
      chomp;
      if($_ eq "--$boundary") {
        $encapsulationBoundary = $_;
        $inEncapsulation = 1;
        # Read the header
        %part_fields = read_header($source);
        # Check the content-disposition
        split();
      }
    }
  }
}

# ---- START MAIN PROGRAM ----
# Open STDIN, STDOUT, and STDERR explicitly using file handle numbers
open(my $stdin, "<&=", 0);
open(my $stdout, ">>&=", 1);
open(my $stderr, ">>&=", 2);
# Read the request line
my $req_line = <$stdin>;

(my $method, my $target, my $version) = split(' ', $req_line, 3);
chomp $req_line;

print {$stderr} "$req_line\n";

# Ensure that this is a 

my %fields;
# Set the newline sequence to HTTP style

my %fields = read_header($stdin);

if($method eq "GET") {
  # Do a very simple redirect
  if($target =~ /\/$/) {
    $target .= "index.html";
  }
  # Handle '..' to reform to '_'
  # More safe than practical
  my @dirs = split('/', $target);
  foreach my $dir (@dirs) {
    if($dir eq "..") {
      $dir = '_';
    }
  }
  # Write the requested file
  write_file_response($stdout, "res" . join("\/", @dirs), "res/404.html");
  # Quit with code 1 to prevent continuing
  exit(1);
} elsif($method eq "POST") {
  # Check content-type as multipart/form-data, boundary=XXXX
  my $content_type = $fields{'content-type'};
  my ($type, $boundary_text) = split($content_type, ';');
  # Remove possible whitespace between parts of the field
  $boundary_text =~ s/^\s+//;
  my ($name_text, $boundary) = split($boundary_text, '=');
  if($type ne "multipart/form-data" or $name_text ne "boundary") {
    write_code_html_response($stdout, 415, "UNSUPPORTED MEDIA TYPE", "res/415.html");
  }
  # Post requests do interesting things
  # 200 status
  if($target eq "/gunzip")
  {
    print {$stderr} "gunzip\n";
    print "HTTP/1.1 200 OK\r\n";
    print "Content-type: application/octet-stream\r\n";
    print "\r\n";
    # Write to file descriptor 3
    open(my $fto, ">>&=", 3);
    forward_multipart_data($stdin, $fto, "upload", $boundary);
    print STDERR "WROTE TO GUNZIP\n";
    close($fto);
    # Read from file descriptor 5
    open(my $ffrom, "<&=", 5);
    forward_file_handler($ffrom, $stdout);
    close($ffrom);
    # Write to and read from gunzip
    exit(0);
  }
  elsif($target eq "/gzip.gz")
  {
    print STDERR "gzip.gz\n";
    print "HTTP/1.1 200 OK\r\n";
    print "Content-type: application/x-gzip\r\n";
    print "\r\n";
    # Write to file descriptor 4
    open(my $fto, ">>&=", 4);
    forward_multipart_data($stdin, $fto, "upload", $boundary);
    close($fto);
    # Read from file descriptor 6
    open(my $ffrom, "<&=", 6);
    forward_file_handler($ffrom, $stdout);
    close($ffrom);
    exit(0);
  }
  else
  {
    # Force a fallback
    write_code_html_response($stdout, "404", "NOT FOUND", "res/404.html");
    exit(0);
  }
} else {
  print STDERR "OTHER request\n";
  # METHOD NOT ALLOWED ERROR
  print "HTTP/1.1 405 METHOD NOT ALLOWED\r\n";
  print "Allow: GET, POST\r\n"; # Allow header
  print "Content-type: text/html\r\n";
  print "\r\n";
  print "<!DOCTYPE html>";
  print "<html><head>";
  print "<title>METHOD NOT ALLOWED</title>";
  print "</head><body>";
  print "Only GET and POST requests allowed on this server.";
  print "</body></html>";
  exit(0);
}

