#include <stdio.h>
#include "config.h"

int main(int argc, char **argv)
{
    unsigned char k1[KEYLEN], k2[KEYLEN];
    int i;
    FILE *fout;

    if (argc != 2)
    {
	fprintf(stderr, "Incorrect commandline paramaters\n");
	return 1;
    }

    if ((fout = fopen(argv[1], "w")) == NULL)
    {
	fprintf(stderr, "Could not write file\n");
	return 2;
    }

    srand(time(NULL) * getpid());
    for (i=0; i<KEYLEN; i++)
    {
	k1[i] = random() % 256 * 100;
	if (k1[i] == 0)
	    i--;
    }

    srand(time(NULL) / getpid());
    for (i=0; i<KEYLEN; i++)
    {
	k2[i] = random() % 256 * 100;
	if (k2[i] == 0)
	    i--;
    }

    fprintf(fout, "#ifndef WIN32
#pragma interface
#endif
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
#ifndef _CRYPT_H
#define _CRYPT_H

/* Automatically generated by compile process, and will be
 * automatically removed when the compilation is finished.
 * Created on %s %s
 */

char crypto_key1[%d] = {", __DATE__, __TIME__, KEYLEN);

    for (i=0; i<KEYLEN-1; i++)
    {
	if (i%8==0)
	    fprintf(fout, "\n\t");
	fprintf(fout, "%#04x, ", k1[i]);
    }
    fprintf(fout, "0 };\n\nchar crypto_key2[%d] = {", KEYLEN);

    for (i=0; i<KEYLEN-1; i++)
    {
	if (i%8==0)
	    fprintf(fout, "\n\t");
	fprintf(fout, "%#04x, ", k2[i]);
    }
    fprintf(fout, "0 };\n\n#endif\n");
    return 0;
}
