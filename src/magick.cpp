#include "magick.h"

Magick::Magick(int inargc, char **inargv)
{
	for(int i=0;i<inargc;i++)
	{
		argv.push_back(inargv[i]);
	}
}

void Magick::Start()
{
	// this is our main routine, when it leaves here, this sucker's done.
}