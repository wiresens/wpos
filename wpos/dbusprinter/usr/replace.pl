#!/usr/bin/perl

open (FD, @ARGV[0]);
@a = <FD>;
close (FD);

open (FD, @ARGV[1]);
@b = <FD>;
close (FD);

foreach (@a) {
  chop;

  if (/\<REPLACE\/\>/s) {
    foreach (@b) {
      chop;
      show ($_);
    }
  } else {
    print "$_\n";
  }
}

sub show {
  if (/AUTOMATICALLY_GENERATED_XML/ or /<?xml/) {
    return;
  }

  print "$_\n";
}
