#include <stdio.h>
#include <stdlib.h>
#ifndef WIN32
#include <unistd.h>
#else
#include <windows.h>
#endif
#include <time.h>
#include "config.h"

int mrandom(int upper)
{
#ifdef __BORLANDC__
	return random(upper);
#else
	return random() % upper;
#endif
};

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

#ifdef WIN32
    srand(time(NULL) * (long)GetCurrentProcess());
#else
    srand(time(NULL) * getpid());
#endif
    for (i=0; i<KEYLEN; i++)
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
    for (i=0; i<KEYLEN; i++)
    {
	k2[i] = mrandom(256) * 100;
	if (k2[i] == 0)
	    i--;
    }

    fprintf(fout, "#ifndef WIN32\n\
#pragma interface\n\
#endif\n\
/*  Magick IRC Services\n\
**\n\
** (c) 1997-2001 Preston Elder <prez@magick.tm>\n\
** (c) 1998-2001 William King <ungod@magick.tm>\n\
**\n\
** The above copywright may not be removed under any\n\
** circumstances, however it may be added to if any\n\
** modifications are made to this file.  All modified\n\
** code must be clearly documented and labelled.\n\
**\n\
** ========================================================== */\n\
#ifndef _CRYPT_H\n\
#define _CRYPT_H\n\
\n\
/* Automatically generated by compile process, and will be\n\
 * automatically removed when the compilation is finished.\n\
 * Created on %s %s\n\
 */\n\
\n\
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
