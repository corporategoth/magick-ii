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

/* TODO: Find out how to turn off keyboard display, and
 * re-compile with the verification code uncommented.
 */

#include <stdio.h>
#include "config.h"
#ifdef HASCRYPT
#include "des/des_locl.h"
#include "des/spr.h"
#endif

#define KEYLEN 1024
#define LOWER_CHAR 35
#define UPPER_CHAR 126

void GetRealKey(char *out, char *key)
{
    int i, j;
    memset(out, 0, 1024);
    for (i=0, j=0; i<strlen(key); i++)
    {
	if (key[i] < LOWER_CHAR || key[i] > UPPER_CHAR)
	{
	    out[j++] = key[i];
	}
	else if (key[i] > UPPER_CHAR - CRYPTO_SCRAMBLE)
	{
	    out[j++] = key[i] + CRYPTO_SCRAMBLE + LOWER_CHAR - UPPER_CHAR;
	}
	else
	{
	    out[j++] = key[i] + CRYPTO_SCRAMBLE;
	}
    }
}

int main(int argc, char **argv)
{
#ifndef HASCRYPT
    fprintf(stderr, "You do not have encryption support.\n");
#else
    int i;
    unsigned char inkey[KEYLEN], outkey[KEYLEN];
    char keykey[1024], filename[512], c;
    FILE *outfile;
    des_key_schedule key1;
    des_key_schedule key2;
    des_cblock ivec;
    des_cblock ckey1, ckey2;

    memset(inkey, 0, KEYLEN);
    memset(outkey, 0, KEYLEN);
    memset(filename, 0, KEYLEN);

    printf("Enter filename to output to: ");
    fgets(filename, 512, stdin);
    filename[511]=0;
    for (i=0; i<strlen(filename); i++)
	if (filename[i]<32)
	    filename[i]=0;
    if ((outfile = fopen(filename, "w")) == NULL)
    {
	fprintf(stderr, "Could not open output file.\n");
	return 1;
    }

    printf("Enter database key: ");
    fgets(inkey, KEYLEN, stdin);
    inkey[KEYLEN-1]=0;
    if (strlen(inkey) < 5)
    {
	fprintf(stderr, "Key must be at least 5 characters.\n");
	return 2;
    }
/*    printf("Re-Enter database key: ");
    fgets(outkey, KEYLEN, stdin);
    outkey[KEYLEN-1]=0;
    if (strcmp(inkey, outkey)!=0)
    {
	fprintf(stderr, "Key mismatch, aborting key generation.\n");
	return 3;
    }
    memset(outkey, 0, KEYLEN); */

    GetRealKey(keykey, CRYPTO_KEY1);
    des_string_to_key(keykey,&ckey1);
    des_set_key(&ckey1,key1);
    GetRealKey(keykey, CRYPTO_KEY2);
    des_string_to_key(keykey,&ckey2);
    des_set_key(&ckey2,key2);

    des_cfb_encrypt(inkey,outkey,8,KEYLEN,key1,&ivec,1);
    des_cfb_encrypt(outkey,inkey,8,KEYLEN,key2,&ivec,0);
    des_cfb_encrypt(inkey,outkey,8,KEYLEN,key1,&ivec,1);

    fprintf(outfile, "%s", outkey);
    fclose(outfile);
#endif
    return 0;
}
