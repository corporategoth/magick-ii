
/* Magick IRC Services
**
** (c) 1997-2002 Preston Elder <prez@magick.tm>
** (c) 1998-2002 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any circumstances,
** however it may be added to if any modifications are made to this
** file.  All modified code must be clearly documented and labelled.
**
** This code is released under the GNU General Public License, which
** means (in short), it may be distributed freely, and may not be sold
** or used as part of any closed-source product.  Please check the
** COPYING file for full rights and restrictions of this software.
**
** ======================================================================= */
#define RCSID(x,y) const char *rcsid_magick_stampkeys_c_ ## x () { return y; }
RCSID(magick_stampkeys_c, "@(#)$Id$");

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
    srand(time(NULL) * (long) GetCurrentProcess());
#else
    srand(time(NULL) * getpid());
#endif
    for (i = 0; i < MAX_KEYLEN; i++)
    {
	k1[i] = mrandom(256) * 100;
	if (k1[i] == 0)
	    i--;
    }

#ifdef WIN32
    srand(time(NULL) * (long) GetCurrentProcess());
#else
    srand(time(NULL) * getpid());
#endif
    for (i = 0; i < MAX_KEYLEN; i++)
    {
	k2[i] = mrandom(256) * 100;
	if (k2[i] == 0)
	    i--;
    }

    for (i = 1; i < argc; i++)
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
	    if (feof(fout))
		break;
	    if (c == '|')
	    {
		int rc;
		char buf[MAX_KEYLEN + 1];

		memset(buf, 0, MAX_KEYLEN + 1);
		buf[0] = '|';
		rc = fread(&buf[1], 1, MAX_KEYLEN - 1, fout);
		if (rc < MAX_KEYLEN - 1)
		{
		    fprintf(stderr, "%s: fread failed trying to get buffer ...\n", argv[i]);
		    break;
		}

		if (memcmp(buf, CRYPTO_KEY1, MAX_KEYLEN) == 0)
		{
		    // Matched the first 
		    rc = fseek(fout, -1 * MAX_KEYLEN, SEEK_CUR);
		    if (rc < 0)
		    {
			fprintf(stderr, "%s: fseek (rewind) failed trying to substitute key 1 ...\n", argv[i]);
			break;
		    }
		    rc = fwrite(k1, MAX_KEYLEN, 1, fout);
		    if (rc < 0)
		    {
			fprintf(stderr, "%s: fwrite failed trying to substitute key 1 ...\n", argv[i]);
			break;
		    }
		    rc = fseek(fout, 0, SEEK_CUR);
		    if (rc < 0)
		    {
			fprintf(stderr, "%s: fseek (tell) failed trying to substitute key 1 ...\n", argv[i]);
			break;
		    }
		}
		else if (memcmp(buf, CRYPTO_KEY2, MAX_KEYLEN) == 0)
		{
		    // Matched the second
		    rc = fseek(fout, -1 * MAX_KEYLEN, SEEK_CUR);
		    if (rc < 0)
		    {
			fprintf(stderr, "%s: fseek failed trying to substitute key 2 ...\n", argv[i]);
			break;
		    }
		    rc = fwrite(k2, MAX_KEYLEN, 1, fout);
		    if (rc < 0)
		    {
			fprintf(stderr, "%s: fwrite failed trying to substitute key 2 ...\n", argv[i]);
			break;
		    }
		    rc = fseek(fout, 0, SEEK_CUR);
		    if (rc < 0)
		    {
			fprintf(stderr, "%s: fseek (tell) failed trying to substitute key 2 ...\n", argv[i]);
			break;
		    }
		}
		else
		{
		    rc = fseek(fout, -1 * (MAX_KEYLEN - 1), SEEK_CUR);
		    if (rc < 0)
		    {
			fprintf(stderr, "%s: fseek failed trying to skip non-substitution ...\n", argv[i]);
			break;
		    }
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
#else
#  ifdef _MSC_VER
    return rand() % upper;
#  else
    return random() % upper;
#  endif
#endif
}

#endif /* HASCRYPT */
