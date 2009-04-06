#!/usr/bin/perl -w

use strict;
my $log = "$ARGV[0]/Log";
open LOG, "<$log" or die "$log: open $!\n";
my @F;
while (<LOG>) {
    @F = split;
    print scalar(localtime $F[0]), " $F[1] $F[2]\n";
}
