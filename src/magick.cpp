// RCS-ID:      $Id$
#include "magick.h"
#include "log.h"

Magick::Magick(int inargc, char **inargv)
{
	for(int i=0;i<inargc;i++)
	{
		argv.push_back(inargv[i]);
	}
}

int Magick::Start()
{
	// this is our main routine, when it leaves here, this sucker's done.
	if(bob.StartBob("")==false)
	{
		/*log that bob couldn't start*/
		bob.bobavail=false;
	}
	FILE *logfile=fopen("magick.log","w+");
	// the below defaults to stderr if logfile cannot be opened
	wxLogStderr logger(logfile);
	// more stuff to do

	
	
	if(logfile!=NULL)
		fclose(logfile);
	return MAGICK_TERMINATE;
}