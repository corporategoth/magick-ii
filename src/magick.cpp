// RCS-ID:      $Id$
#include "magick.h"

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
	return bob.bobavail;
}

