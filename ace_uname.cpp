
/* Magick IRC Services
**
** (c) 1997-2003 Preston Elder <prez@magick.tm>
** (c) 1998-2003 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any circumstances,
** however it may be added to if any modifications are made to this
** file.  All modified code must be clearly documented and labelled.
**
** This code is released under the Artistic License v2.0 or better.
** The full text of this license should be contained in a file called
** COPYING distributed with this code.  If this file does not exist,
** it may be viewed here: http://www.magick.tm/m2/license.html
**
** ======================================================================= */
#define RCSID(x,y) const char *rcsid_what_c_ ## x () { return y; }
RCSID(what_c, "@(#)$Id$");

/* ======================================================================= **
**
** For official changes (by the Magick Development Team),please
** check the ChangeLog* files that come with this distribution.
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** ======================================================================= */

#include <ace/OS.h>

int main(int argc, char *argv[])
{
    struct utsname *info = new struct utsname;

    ACE_OS::uname(info);

    if (argc < 2 || ACE_OS::strcmp(argv[1], "sysname") == 0)
	printf("%s\n", info->sysname);
    if (argc < 2 || ACE_OS::strcmp(argv[1], "nodename") == 0)
	printf("%s\n", info->nodename);
    if (argc < 2 || ACE_OS::strcmp(argv[1], "release") == 0)
	printf("%s\n", info->release);
    if (argc < 2 || ACE_OS::strcmp(argv[1], "version") == 0)
	printf("%s\n", info->version);
    if (argc < 2 || ACE_OS::strcmp(argv[1], "machine") == 0)
	printf("%s\n", info->machine);

    delete info;

    return 0;
}
