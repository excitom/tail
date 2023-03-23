#!/usr/bin/perl
#
# Create a file with a number of random length lines,
# for testing the tail program.
#
my $lines = shift or die "No lines specified on the command line.\n"; 
$min = 10;
$max = 1000;
while ($lines-- > 0) {
	$len = $min + int(rand($max-$min));
	print $len . " ";
	print "x" x $len;
	print "\n";
}
