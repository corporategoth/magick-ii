#!/usr/bin/perl
#
# Pass a language file through babelfish (http://babelfish.altavista.com)
#
# By, Preston Elder

if ($#ARGV < 2)
{
    print "SYNTAX: " . $0 . " <language> <infile> <outfile>\n";
    print "Language format:  FROM_TO\n";
    print "  en = English   fr = French   pt = Portuguese\n";
    print "  it = Italian   de = German   es = Spanish\n";
    print "  ru = Russian   eg. en_fr, it_en, etc\n";
    exit;
}

$lang = $ARGV[0];
$infile = $ARGV[1];
$outfile = $ARGV[2];

open(EXP, "which expect|");
while (<EXP>)
{
    chomp;
    $expect = $_;
}
close(EXP);
die "You must have expect\n" if ($expect =~ /\s+/);

open(IN, $infile) || die "Could not open language file.\n";
open(OUT, ">" . $outfile) || die "Could not open output file.\n";
select(OUT); $| = 1;
select(STDOUT);
while (<IN>)
{
print;
    chomp;
    $text = "";
    $line = $_;
    if ($text eq "" && /^(\;\s+)(.*)/)
    {
	$line = $1;
	$text = $2;
    }
    elsif ($text eq "" && /^(\#\s+)(.*)/)
    {
	$line = $1;
	$text = $2;
    }
    elsif ($text eq "" && /^(\S+\s+=.*:.*:)(.*)/)
    {
	$line = $1;
	$text = $2;
    }
    elsif ($text eq "" && /^(\S+\s+=)(.*)/)
    {
	$line = $1;
	$text = $2;
    }

    if ($text ne "")
    {
	$line = $line . &babel($text);
    }

    print OUT "$line\n";
}
close(OUT);
close(IN);

sub babel
{
    my ($origtext, $text, $line, $outtext, $phase);
    $origtext = $_[0];
    $text = $origtext;
    $text =~ s/\+/\(enc_0\)/g;
    $text =~ s/\s/\+/g;
    $text =~ s/\+\+/\+/g;
    $text =~ s/%/\(enc_1\)/g;
    $text =~ s/"/\(enc_2\)/g;
    $text =~ s/'/\(enc_3\)/g;
    $text =~ s/\//\(enc_4\)/g;
    $text =~ s/\\/\(enc_5\)/g;
    $text =~ s/-/\(enc_6\)/g;
    $text =~ s/\*/\(enc_7\)/g;
    $text =~ s/,/\(enc_8\)/g;
    $text =~ s/\#/\(enc_9\)/g;
    $text =~ s/\[/\(enc_A\)/g;
    $text =~ s/\]/\(enc_B\)/g;
    $text =~ s/\|/\(enc_C\)/g;
    $outtext = "";
    $phase = 0;

    open(OUT2, ">" . "/tmp/babel" . $$);
    print OUT2 <<EOF;
#!$expect -f
spawn telnet babelfish.altavista.digital.com 80
expect {
	Connected	{ send "GET /translate.dyn?lp=$lang&urltext=$text\n"; exp_continue; }
}
EOF
    close(OUT2);
    chmod 0755, "/tmp/babel" . $$;

    open(IN2, "/tmp/babel" . $$ . "|");
    while (<IN2>)
    {
	chomp;
	$_ =~ s/
//g;
	if (/http:\/\/altavista.com\/cgi-bin\/query/ || /textarea/)
	{
	    $phase++;
	    next;
	}
	if (/^<td bgcolor=white>/ || ($phase == 2 && /^<\/td>/))
	{
	    $phase += 2;
	    $_ =~ s/^<td bgcolor=white>//;
	}
	if ($phase == 2)
	{
	    if ($outtext ne "")
	    {
		$tmp = $outtext . " ";
		$outtext = $tmp;
	    }
	    $tmp = $outtext . $_;
	    $outtext = $tmp;
	}
	last if ($phase > 2);
    }
    close(IN2);
    unlink("/tmp/babel" . $$);
    $outtext =~ s/\s+$//g;
    $outtext =~ s/\(enc_0\)/\+/g;
    $outtext =~ s/\(enc_1\)/%/g;
    $outtext =~ s/\(enc_2\)/"/g;
    $outtext =~ s/\(enc_3\)/'/g;
    $outtext =~ s/\(enc_4\)/\//g;
    $outtext =~ s/\(enc_5\)/\\/g;
    $outtext =~ s/\(enc_6\)/-/g;
    $outtext =~ s/\(enc_7\)/\*/g;
    $outtext =~ s/\(enc_8\)/,/g;
    $outtext =~ s/\(enc_9\)/#/g;
    $outtext =~ s/\(enc_A\)/\[/g;
    $outtext =~ s/\(enc_B\)/\]/g;
    $outtext =~ s/\(enc_C\)/\|/g;
    $outtext = $origtext if ($outtext eq "");
    return $outtext;
}
