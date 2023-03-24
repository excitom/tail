#!/usr/bin/perl
#
# Create a file with a number of random length lines,
# for testing the tail program.
#
# Wait a random amount of time, then spew some more lines. Repeat.
#
$minLines = 5;
$maxLines = 100;
$minLen = 10;
$maxLen = 1000;
$minWait = 1;
$maxWait = 7;
while (1) {
	$lines = $minLines + int(rand($maxLines-$minLines));
	while ($lines-- > 0) {
		$len = $minLen + int(rand($maxLen-$minLen));
		print $len . " ";
		print "x" x $len;
		print "\n";
	}
	sleep($minWait + int(rand($maxWait-$minWait)));
}
