/*  Magick IRC Services
**
** (c) 1997-2001 Preston Elder <prez@magick.tm>
** (c) 1998-2001 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any
** circumstances, however it may be added to if any
** modifications are made to this file.  All modified
** code must be clearly documented and labelled.
**
** ========================================================== */
#define RCSID(x,y) const char *rcsid_what_c_ ## x () { return y; }
RCSID(what_c, "@(#)$Id$");
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.7  2001/11/03 21:02:21  prez
** Mammoth change, including ALL changes for beta12, and all stuff done during
** the time GOTH.NET was down ... approx. 3 months.  Includes EPONA conv utils.
**
** Revision 1.6  2001/02/03 02:21:30  prez
** Loads of changes, including adding ALLOW to ini file, cleaning up
** the includes, RCSID, and much more.  Also cleaned up most warnings.
**
** Revision 1.5  2000/09/30 10:48:06  prez
** Some general code cleanups ... got rid of warnings, etc.
**
** Revision 1.4  2000/04/06 15:09:11  prez
** More makefile changes...
**
** Revision 1.3  2000/02/27 11:06:34  prez
** Added standard header...
**
**
** ========================================================== */
#include <stdio.h>

int main(int argc, char **argv)
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
    return 0;
}
