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
** Revision 1.1  2000/06/27 18:56:59  prez
** Added choosing of keys to configure, also created the keygen,
** and scrambler (so keys are not stored in clear text, even in
** the .h and binary files).  We should be set to do the decryption
** process now, as encryption (except for encryption of db's) is
** all done :)
**
**
** ========================================================== */
#include <stdio.h>

#define LOWER_CHAR 35
#define UPPER_CHAR 126

int main(int argc, char **argv)
{
    int i, j, rotation;
    char s1[1024], s2[1024];
    memset(s1, 0, 1024);
    memset(s2, 0, 1024);
    fgets(s1, 1024, stdin);
    s1[1023] = 0;

    if (argc > 1)
	rotation = atoi(argv[1]);
    else
	rotation = 13;

    for (i=0, j=0; i<strlen(s1); i++)
    {
	if (s1[i] < LOWER_CHAR || s1[i] > UPPER_CHAR)
	{
	    s2[j++] = s1[i];
	}
	else if (s1[i] <= LOWER_CHAR + rotation)
	{
	    s2[j++] = s1[i] - rotation - LOWER_CHAR + UPPER_CHAR;
	}
	else
	{
	    s2[j++] = s1[i] - rotation;
	}
    }
    printf("%s\n", s2);
}
