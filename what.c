
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

#include <stdio.h>

int main(int argc, char **argv)
{
    unsigned char c;
    int i, sequence;
    FILE *in;

    for (i = 1; i < argc; i++)
    {
	if ((in = fopen(argv[i], "r")) == NULL)
	{
	    fprintf(stderr, "Cannot open file: %s\n", argv[i]);
	}
	else
	{
	    printf("%s:\n", argv[i]);
	    sequence = 0;
	    for (c = fgetc(in); !feof(in); c = fgetc(in))
	    {
		if (c >= 32 && c <= 126 && sequence == 4)
		{
		    printf("%c", c);
		}
		else if ((c == '@' && sequence == 0) || (c == '(' && sequence == 1) || (c == '#' && sequence == 2))
		{
		    sequence++;
		}
		else if (c == ')' && sequence == 3)
		{
		    sequence++;
		    printf("        ");
		}
		else
		{
		    if (sequence == 4)
			printf("\n");
		    sequence = 0;
		}
	    }
	    fclose(in);
	}
    }
    return 0;
}
