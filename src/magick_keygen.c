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
** Revision 1.2  2000/06/28 12:20:48  prez
** Lots of encryption stuff, but essentially, we now have random
** key generation for the keyfile keys, and we can actually encrypt
** something, and get it back as we sent it in (specifically, the
** keyfile itself).
**
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
#ifndef WIN32
#include <termio.h>
#endif

#include "config.h"
#include "crypt.h"
#ifdef HASCRYPT
#include "des/des_locl.h"
#include "des/spr.h"
#endif

void mDES(unsigned char *in, unsigned char *out, size_t size,
	des_key_schedule key1, des_key_schedule key2, int enc);

int main(int argc, char **argv)
{
#ifndef HASCRYPT
    printf("Magick IRC Services - http://www.magick.tm\n");
    printf("    (c) 1997-2000 Preston A. Elder <prez@magick.tm>\n");
    printf("    (c) 1998-2000 William King <ungod@magick.tm>\n\n");

    fprintf(stderr, "You do not have encryption support.\n");

#else
    int i, data_sz, data_cnt;
    unsigned char inkey[KEYLEN], outkey[KEYLEN], filename[512];
    FILE *outfile, *tty;
    des_key_schedule key1, key2;
    des_cblock ckey1, ckey2;
#ifndef WIN32
    struct termio tty_new, tty_orig;
#endif

    printf("Magick IRC Services - http://www.magick.tm\n");
    printf("    (c) 1997-2000 Preston A. Elder <prez@magick.tm>\n");
    printf("    (c) 1998-2000 William King <ungod@magick.tm>\n\n");

    memset(filename, 0, 512);
    memset(inkey, 0, KEYLEN);
    memset(outkey, 0, KEYLEN);

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

#ifndef WIN32
    if ((tty = fopen("/dev/tty", "r")) == NULL)
	tty = stdin;
    ioctl(fileno(tty), TCGETA,&tty_orig);
    memcpy(&tty_new, &tty_orig, sizeof(tty_orig));
    tty_new.c_lflag &= ~ECHO;
    ioctl(fileno(tty), TCSETA, &tty_new);
#endif

    printf("Enter database key: ");
    fgets(inkey, KEYLEN, tty);
    inkey[KEYLEN-1]=0;
    printf("\n");
    if (strlen(inkey) < 16)
    {
	fprintf(stderr, "Key must be at least 16 characters.\n");
#ifndef WIN32
	ioctl(fileno(tty), TCSETA, &tty_orig);
#endif
	return 2;
    }

    printf("Re-Enter database key: ");
    fgets(outkey, KEYLEN, tty);
    outkey[KEYLEN-1]=0;
    printf("\n");
    if (strcmp(inkey, outkey)!=0)
    {
	fprintf(stderr, "Key mismatch, aborting key generation.\n");
#ifndef WIN32
	ioctl(fileno(tty), TCSETA, &tty_orig);
#endif
	return 3;
    }

    memset(outkey, 0, KEYLEN);
#ifndef WIN32
    ioctl(fileno(tty), TCSETA, &tty_orig);
#endif

    des_string_to_key(crypto_key1,&ckey1);
    des_set_key(&ckey1,key1);
    des_string_to_key(crypto_key2,&ckey2);
    des_set_key(&ckey2,key2);

    mDES(inkey, outkey, KEYLEN, key1, key2, 1);

    fwrite(outkey, sizeof(unsigned char), KEYLEN, outfile);
    fclose(outfile);

#endif
    return 0;
}

void mDES(unsigned char *in, unsigned char *out, size_t size,
	des_key_schedule key1, des_key_schedule key2, int enc)
{
#ifdef HASCRYPT
    DES_LONG tuple[2], t0, t1;
    unsigned char *iptr, *optr, tmp[8];
    int i;

    memset(out, 0, size);
    for (iptr=in, optr=out, i=0; i<size; i+=8)
    {
	c2l(iptr, t0); tuple[0] = t0;
	c2l(iptr, t1); tuple[1] = t1;
	des_encrypt(tuple, key1, enc ? DES_ENCRYPT : DES_DECRYPT);
	des_encrypt(tuple, key2, enc ? DES_DECRYPT : DES_ENCRYPT);
	des_encrypt(tuple, key1, enc ? DES_ENCRYPT : DES_DECRYPT);
	t0 = tuple[0]; l2c(t0, out);
	t1 = tuple[1]; l2c(t1, out);
    }
#endif
}

