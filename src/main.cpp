// RCS-ID:      $Id$
#include "magick.h"

Magick *MagickObject;

int start_server(int argc, char **argv)
{
	Magick internalobject(argc, argv);
	MagickObject=&internalobject;
	return internalobject.Start();
}
int main(int argc, char **argv)
{
	try
	{
		int Result;
restart:
		Result=start_server(argc,argv);
		if(Result==MAGICK_RET_RESTART)
			goto restart;
		return Result;
	}
	catch(...)
	{
		printf("Unhandled exception\n");
	}
}