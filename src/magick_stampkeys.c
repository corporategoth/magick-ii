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
#define RCSID(x,y) const char *rcsid_genrankeys_c_ ## x () { return y; }
RCSID(genrankeys_c, "@(#)$Id$");
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.4  2001/07/12 00:28:42  prez
** Added propper support for Anarchy mode
**
** Revision 1.3  2001/06/15 07:20:40  prez
** Fixed windows compiling -- now works with MS Visual Studio 6.0
**
** Revision 1.2  2001/05/14 04:46:32  prez
** Changed to use 3BF (3 * blowfish) encryption.  DES removed totally.
**
** Revision 1.1  2001/04/15 03:10:51  prez
** Changed stampkeys -> magick_stampkeys and updated the RPM spec
**
** Revision 1.2  2001/04/13 07:42:49  prez
** Fixed the stampkeys program
**
** Revision 1.1  2001/04/13 07:12:49  prez
** Changed genrankeys style random key generation to binary stamping
** (allowing people to stamp the binary AFTER it has been created, and
** thus, allowing pre-compiled binaries to be stamped for security).
**
** Revision 1.10  2001/03/02 05:24:41  prez
** HEAPS of modifications, including synching up my own archive.
**
**
** ========================================================== */

#include <stdio.h>
#include <string.h>
#include "config.h"

#ifdef HASCRYPT
#include <stdlib.h>
#ifndef WIN32
#include <unistd.h>
#else
#include <windows.h>
#endif
#include <time.h>
#include "crypt/blowfish.h"
#define MAX_KEYLEN	((BF_ROUNDS+2)*4)

int mrandom(int upper);

#endif /* HASCRYPT */

int main(int argc, char **argv)
{
#ifndef HASCRYPT
    fprintf(stderr, "You do not have encryption support.\n");
#else
    unsigned char k1[MAX_KEYLEN], k2[MAX_KEYLEN];
    int i;
    FILE *fout;

    if (argc < 2)
    {
	fprintf(stderr, "Usage: %s [files to stamp]\n\n", argv[0]);
	fprintf(stderr, "ALL files you intend to have the same keys stamped into them\n");
	fprintf(stderr, "should be specified on the commandline.  Each time this program\n");
	fprintf(stderr, "is run, it will generate DIFFERENT keys of %d bytes.\n", MAX_KEYLEN);
	return 1;
    }

#ifdef WIN32
    srand(time(NULL) * (long)GetCurrentProcess());
#else
    srand(time(NULL) * getpid());
#endif
    for (i=0; i<MAX_KEYLEN; i++)
    {
	k1[i] = mrandom(256) * 100;
	if (k1[i] == 0)
	    i--;
    }

#ifdef WIN32
    srand(time(NULL) * (long)GetCurrentProcess());
#else
    srand(time(NULL) * getpid());
#endif
    for (i=0; i<MAX_KEYLEN; i++)
    {
	k2[i] = mrandom(256) * 100;
	if (k2[i] == 0)
	    i--;
    }

    for (i=1; i<argc; i++)
    {
	char c;

	if ((fout = fopen(argv[i], "r+")) == NULL)
	{
	    fprintf(stderr, "Could not open file %s\n", argv[i]);
	    continue;
	}

	while (!feof(fout))
	{
	    c = fgetc(fout);
	    if (c == '|')
	    {
		int read;
		char buf[MAX_KEYLEN+1];
		memset(buf, 0, MAX_KEYLEN+1);
		buf[0] = '|';
		read = fread(&buf[1], 1, MAX_KEYLEN-1, fout);
		if (read < MAX_KEYLEN-1)
		    break;

		if (memcmp(buf, CRYPTO_KEY1, MAX_KEYLEN) == 0)
		{
		    // Matched the first 
		    fseek(fout, -1 * MAX_KEYLEN, SEEK_CUR);
		    fwrite(k1, MAX_KEYLEN, 1, fout);
		}
		else if (memcmp(buf, CRYPTO_KEY2, MAX_KEYLEN) == 0)
		{
		    // Matched the second
		    fseek(fout, -1 * MAX_KEYLEN, SEEK_CUR);
		    fwrite(k2, MAX_KEYLEN, 1, fout);
		    fseek(fout, -1, SEEK_CUR);
		}
		else
		{
		    fseek(fout, -1 * (MAX_KEYLEN-1), SEEK_CUR);
		}
	    }
	}

	fclose(fout);
    }
#endif /* !HASCRYPT */
    return 0;
}

#ifdef HASCRYPT

int mrandom(int upper)
{
#ifdef __BORLANDC__
	return random(upper);
#elif _MSC_VER
	return rand() % upper;
#else
	return random() % upper;
#endif
}

#endif /* HASCRYPT */
