// RCS-ID:      $Id$
#include "magick.h"

Magick *MagickObject;
void main(int argc, const char **argv)
{
	Magick internalobject(argc, argv);
	MagickObject=&internalobject;
	internalobject.Start();
}