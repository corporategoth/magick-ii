#include "magick.h"

Magick *MagickObject;
void main(int argc, char **argv)
{
	Magick internalobject(argc, argv);
	MagickObject=&internalobject;
	internalobject.Start();
}