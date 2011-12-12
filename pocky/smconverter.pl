#!/usr/bin/perl

# converts .sm files to .sim files for our game
my $path = $ARGV[0];

# variables for things we need to save
my $title = "";
my $bpm = 0;
my $secsperbeat = 0;
my $length = 0;
my $author = "";
my $music = "";

my $offset = 0;

my @times = ();
my @bpms = ();
my $pointer = 0;

open FILE, $path or die "invalid file name";

my $notes = 0;
my $currenttime = 0;

my @notebuffer = ();

while(my $line = <FILE>){
  chomp($line);
  if($line =~ m/^#(\S*):(\S*);/ && !$notes){
    # it's key value pair, so yeah
    if($1 eq "MUSIC"){
      $music = $2;
    }elsif($1 eq "BPMS"){
      my @bpairs = split(/,/, $2);
      foreach(@bpairs){
      $_ =~ m/(\S*)=(\S*)/;
      #print("adding bpm pair $1, $2\n");
      push(@times, $1);
      push(@bpms, $2);
      }
    }elsif($1 eq "OFFSET"){
      #print("offset is $2\n");
      $offset = $2 * -1;
      $currenttime = $offset;
    }
  }elsif($line =~ m/NOTES/ && !$notes){
      # we're done, so write the header
      print("[HEADER]\nTITLE \nBPM \nLENGTH \nOFFSET $offset\nAUTHOR \nMUSIC $music\n\n[DATA]\n");
      # start note parsing
      $notes = 1;
  }else{
    # we are reading notes
    #print("reading line $line\n");
    if($line =~ m/^(\d)(\d)(\d)(\d)/){
      push(@notebuffer, $1);
      push(@notebuffer, $2);
      push(@notebuffer, $3);
      push(@notebuffer, $4);
    }elsif($line =~ m/^[,;]/){
      #print("hit a period, we have $#notebuffer + 1 notes\n");
      # we have reached the end of the measure
      # number of beats is length of buffer divided by 4 columns
      my $numbeats = ($#notebuffer + 1)/4;
      # this number of beats is split across 4 normal bpm beats (quarter notes)
      for(my $i = 0; $i <= $#notebuffer; $i+=4){
	#print("looking at i = $i\n");
	# find the bpm at this spot, using the offset
	my $bpmfound = 0;
	my $curbpm = $bpms[0];
	while(!bpmfound){
	  if(offset >= $times[$pointer]){
	    $curbpm = $bpms[$pointer];
	  }else{
	    $bpmfound = 1;
	  }
	}
	#print("using bpm $curbpm\n");
	# now we have the correct bpm
	my $secsperbeat = 60.0 / $curbpm;
	my $secpernote = (4/$numbeats) * $secsperbeat;
	# the beat time is just the current offset
	# write the time to the file depending on the number of nonzero things
	for(my $j = 0; $j < 4; $j++){
	  my $val = $notebuffer[$i+$j];
	  #print("value at notebuffer is $val\n");
	  if($val == 1 || $val == 2){
	    print("$currenttime\n");
	  }
	}
	# add the secs per note
	$currenttime += $secpernote;
      }
      @notebuffer = ();
      if($line =~ m/;/){
	exit;
      }
    }
  }
}
