/*  Magick IRC Services
**
** (c) 1997-2000 Preston Elder <prez@magick.tm>
** (c) 1998-2000 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any
** circumstances, however it may be added to if any
** modifications are made to this file.  All modified
** code must be clearly documented and labelled.
**
** ========================================================== */
static const char *ident = "@(#)$Id$";
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.3  2000/02/27 11:06:34  prez
** Added standard header...
**
**
** ========================================================== */
#include <stdio.h>

void main(int argc, char **argv)
{
    unsigned char c;
    int i, sequence;
    FILE *in;

    for (i=1; i<argc; i++)
    {
	if ((in = fopen(argv[i], "r")) == NULL)
	{
	    fprintf(stderr, "Cannot open file: %s\n", argv[i]);
	}
	else
	{
	    printf("%s:\n", argv[i]);
	    sequence = 0;
	    for (c=fgetc(in); !feof(in); c=fgetc(in))
	    {
		if (c >= 32 && c <= 126 && sequence == 4)
		{
		    printf("%c", c);
		}
		else if ((c == '@' && sequence == 0) ||
		    (c == '(' && sequence == 1) ||
		    (c == '#' && sequence == 2))
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
}
