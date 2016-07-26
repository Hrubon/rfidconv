# GNU licence - copyleft	  #
# author:		David Èepelík #

#!/usr/bin/perl

use strict;
use warnings;
use v5.20;

use FindBin qw($Bin);
use Data::Dumper;

my @formats = qw(alpus safeq provis);
my @data = <>;

my $header = shift @data;
chomp $header;
my @cols = split /,/, $header;

my $num_ok = 0;
my $num_err = 0;
my $total = 0;

for (@data) {
	chomp;

	my %test_data = ();
	@test_data{@cols} = split ',', $_;

	for my $from (@formats) {
		for my $to (@formats) {
			my $output = qx( $Bin/../src/rfidconv $from $to $test_data{$from} );
			chomp $output;

			if ($output ne $test_data{$to}) {
				say "Invalid output for $from -> $to:";
				say "\tExpected: $test_data{$to}";
				say "\tActual: $output";

				$num_err++;
			}
			else {
				$num_ok++;
			}

			$total++;
		}
	}
}

say "$total tests, $num_ok OK, $num_err errors."
