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
** Revision 1.5  2000/08/03 13:06:31  prez
** Fixed a bunch of stuff in mstring (caused exceptions on FreeBSD machines).
**
** Revision 1.4  2000/07/21 00:18:49  prez
** Fixed database loading, we can now load AND save databases...
**
** Almost ready to release now :)
**
** Revision 1.3  2000/06/29 06:30:57  prez
** Added the support for the 'extra' chars (ie. at the end of a string)
** so we support odd-length strings.  Also updated documentation.
**
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
#include "config.h"
#include "crypt.h"
#ifdef HASCRYPT
#include "des/des_locl.h"
#include "des/spr.h"
#endif
#ifdef HAVE_TERMIO_H
#include <termio.h>
#endif

#define MIN_KEYLEN	16
#define DEF_KEYNAME	"magick.key"

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
#ifdef HAVE_TERMIO_H
    struct termio tty_new, tty_orig;
#endif

    printf("Magick IRC Services - http://www.magick.tm\n");
    printf("    (c) 1997-2000 Preston A. Elder <prez@magick.tm>\n");
    printf("    (c) 1998-2000 William King <ungod@magick.tm>\n\n");

    memset(filename, 0, 512);
    memset(inkey, 0, KEYLEN);
    memset(outkey, 0, KEYLEN);

    printf("Enter filename to output to [%s]: ", DEF_KEYNAME);
    fgets(filename, 512, stdin);
    if (strlen(filename) < 2)
    {
	strcpy(filename, DEF_KEYNAME);
    }
    else
    {
	filename[511]=0;
	for (i=0; i<strlen(filename); i++)
	    if (filename[i]<32)
		filename[i]=0;
    }
    if ((outfile = fopen(filename, "w")) == NULL)
    {
	fprintf(stderr, "Could not open output file.\n");
	return 1;
    }

    /* This crap is needed to turn of echoing password */
#ifdef HAVE_TERMIO_H
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
    if (strlen(inkey) != KEYLEN-1)
	inkey[strlen(inkey)-1]=0;
    printf("\n");
    if (strlen(inkey) < MIN_KEYLEN)
    {
	fprintf(stderr, "Key must be at least %d characters.\n", MIN_KEYLEN);
#ifdef HAVE_TERMIO_H
	ioctl(fileno(tty), TCSETA, &tty_orig);
#endif
	return 2;
    }

    printf("Re-Enter database key: ");
    fgets(outkey, KEYLEN, tty);
    outkey[KEYLEN-1]=0;
    if (strlen(outkey) != KEYLEN-1)
	outkey[strlen(outkey)-1]=0;
    printf("\n");
    if (strcmp(inkey, outkey)!=0)
    {
	fprintf(stderr, "Key mismatch, aborting key generation.\n");
#ifdef HAVE_TERMIO_H
	ioctl(fileno(tty), TCSETA, &tty_orig);
#endif
	return 3;
    }

    memset(outkey, 0, KEYLEN);
#ifdef HAVE_TERMIO_H
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
    if (i<size)
    {
	memset(tmp, 0, 8);
	size -= i;
	for (i=0; i<size; i++)
	    tmp[i] = iptr[i];
	iptr = tmp;
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

