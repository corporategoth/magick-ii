// RCS-ID:      $Id$
#include "magick.h"

Magick *MagickObject;

int main(int argc, char **argv)
{
	Magick internalobject(argc, argv);
	MagickObject=&internalobject;
	return internalobject.Start();
}