#!/usr/bin/perl
#
# Check for missing tags in language files.
#
# By, Preston Elder

die "Syntax: $0 sourcefile targetfile\n" unless ($ARGV[1]);

open(FH, $ARGV[0]);
while (<FH>)
{
	next if (/^#/);
	chomp;

	if (/^(\S+)\s*=/)
	{
		push(@file1, $section . "/" . $1);
	}
	else
	{
		$section = $1 if (/^\[(.*)\]$/);
	}
}
close(FH);

open(FH, $ARGV[1]);
while (<FH>)
{
	next if (/^#/);
	chomp;

	if (/^(\S+)\s*=/)
	{
		push(@file2, $section . "/" . $1);
	}
	else
	{
		$section = $1 if (/^\[(.*)\]$/);
	}
}
close(FH);

@sorted1 = sort(@file1);
@sorted2 = sort(@file2);

for ($i=0, $j=0; $i < $#sorted1 || $j < $#sorted2; )
{
	if ($j >= $#sorted2 || $sorted1[$i] lt $sorted2[$j])
	{
		push(@missing2, $sorted1[$i]);
		$i++;
	}
	elsif ($i >= $#sorted1 || $sorted1[$i] gt $sorted2[$j])
	{
		push(@missing1, $sorted2[$j]);
		$j++;
	}
	else
	{
		$i++; $j++;
	}
}

if ($#missing1)
{
	print "Missing from SOURCE file ($ARGV[0]):\n";
	for ($i=0; $i<$#missing1; $i++)
	{
		print "$missing1[$i]\n";
	}
}

if ($#missing2)
{
	print "Missing from TARGET file ($ARGV[1]):\n";
	for ($i=0; $i<$#missing2; $i++)
	{
		print "$missing2[$i]\n";
	}
}
