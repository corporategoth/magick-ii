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
#define RCSID(x,y) const char *rcsid_magick_keygen_c_ ## x () { return y; }
RCSID(magick_keygen_c, "@(#)$Id$");
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.12  2001/05/13 00:55:18  prez
** More patches to try and fix deadlocking ...
**
** Revision 1.11  2001/04/13 07:12:48  prez
** Changed genrankeys style random key generation to binary stamping
** (allowing people to stamp the binary AFTER it has been created, and
** thus, allowing pre-compiled binaries to be stamped for security).
**
** Revision 1.10  2001/04/02 02:11:23  prez
** Fixed up some inlining, and added better excption handling
**
** Revision 1.9  2001/02/03 02:21:34  prez
** Loads of changes, including adding ALLOW to ini file, cleaning up
** the includes, RCSID, and much more.  Also cleaned up most warnings.
**
** Revision 1.8  2001/01/01 05:32:44  prez
** Updated copywrights.  Added 'reversed help' syntax (so ACCESS HELP ==
** HELP ACCESS).
**
** Revision 1.7  2000/09/05 10:53:07  prez
** Only have operserv.cpp and server.cpp to go with T_Changing / T_Modify
** tracing -- also modified keygen to allow for cmdline generation (ie.
** specify 1 option and enter keys, or 2 options and the key is read from
** a file).  This allows for paragraphs with \n's in them, and helps so you
** do not have to type out 1024 bytes :)
**
** Revision 1.6  2000/08/28 10:51:38  prez
** Changes: Locking mechanism only allows one lock to be set at a time.
** Activation_Queue removed, and use pure message queue now, mBase::init()
** now resets us back to the stage where we havnt started threads, and is
** called each time we re-connect.  handle_close added to ircsvchandler.
** Also added in locking for all accesses of ircsvchandler, and checking
** to ensure it is not null.
**
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
#include <stdarg.h>
#include "config.h"
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
int mstring_snprintf(char *buf, const size_t size, const char *fmt, ...);
int mstring_vsnprintf(char *buf, const size_t size, const char *fmt, va_list ap);

int main(int argc, char **argv)
{
#ifndef HASCRYPT
    printf("Magick IRC Services - http://www.magick.tm\n");
    printf("    (c) 1997-2001 Preston A. Elder <prez@magick.tm>\n");
    printf("    (c) 1998-2001 William King <ungod@magick.tm>\n\n");

    fprintf(stderr, "You do not have encryption support.\n");

#else
    size_t i, key_size;
    unsigned char inkey[KEYLEN], outkey[KEYLEN], filename[512];
    unsigned char instr[128], outstr[128];
    FILE *infile, *outfile, *tty;
    des_key_schedule key1, key2;
    des_cblock ckey1, ckey2;
#ifdef HAVE_TERMIO_H
    struct termio tty_new, tty_orig;
#endif

    printf("Magick IRC Services - http://www.magick.tm\n");
    printf("    (c) 1997-2001 Preston A. Elder <prez@magick.tm>\n");
    printf("    (c) 1998-2001 William King <ungod@magick.tm>\n\n");

    memset(filename, 0, 512);
    memset(inkey, 0, KEYLEN);
    memset(outkey, 0, KEYLEN);

    if (argc>1)
    {
	strcpy(filename, argv[1]);
	filename[511]=0;
    }
    else
    {
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
    }
    if ((outfile = fopen(filename, "w")) == NULL)
    {
	fprintf(stderr, "Could not open output file.\n");
	return 1;
    }

    memset(filename, 0, 512);
    if (argc>2)
    {
	strcpy(filename, argv[2]);
	filename[511]=0;
	if ((infile = fopen(filename, "r")) == NULL)
	{
	    fprintf(stderr, "Could not open plain-text keyfile.\n");
	    return 2;
	}
	key_size = fread(inkey, sizeof(char), KEYLEN, infile);
    }
    else
    {

	/* This crap is needed to turn of echoing password */
#ifdef HAVE_TERMIO_H
	if ((tty = fopen("/dev/tty", "r")) == NULL)
	    tty = stdin;
	ioctl(fileno(tty), TCGETA,&tty_orig);
	memcpy(&tty_new, &tty_orig, sizeof(tty_orig));
	tty_new.c_lflag &= ~ECHO;
	ioctl(fileno(tty), TCSETA, &tty_new);
#else
	tty = stdin;
#endif

	printf("Enter database key: ");
	fgets(inkey, KEYLEN, tty);
	inkey[KEYLEN-1]=0;
	key_size = strlen(inkey);
	if (key_size != KEYLEN-1)
	    inkey[--key_size]=0;
	printf("\n");
	if (key_size < MIN_KEYLEN)
	{
	    fprintf(stderr, "Key must be at least %d characters.\n", MIN_KEYLEN);
#ifdef HAVE_TERMIO_H
	    ioctl(fileno(tty), TCSETA, &tty_orig);
#endif
	    return 3;
	}

	printf("Re-Enter database key: ");
	fgets(outkey, KEYLEN, tty);
	outkey[KEYLEN-1]=0;
	key_size = strlen(outkey);
	if (key_size != KEYLEN-1)
	    outkey[--key_size]=0;
	printf("\n");
	if (memcmp(inkey, outkey, KEYLEN)!=0)
	{
	    fprintf(stderr, "Key mismatch, aborting key generation.\n");
#ifdef HAVE_TERMIO_H
	    ioctl(fileno(tty), TCSETA, &tty_orig);
#endif
	    return 4;
	}

	memset(outkey, 0, KEYLEN);
#ifdef HAVE_TERMIO_H
	ioctl(fileno(tty), TCSETA, &tty_orig);
#endif
    }

    des_string_to_key(CRYPTO_KEY1,&ckey1);
    des_set_key(&ckey1,key1);
    des_string_to_key(CRYPTO_KEY2,&ckey2);
    des_set_key(&ckey2,key2);

    memset(instr, 0, 128);
#if defined(BUILD_NODE) && defined(BUILD_TYPE) && defined(BUILD_REL)
    mstring_snprintf(instr, 128, "%s %s Keyfile: %s %s %s", PACKAGE, VERSION, BUILD_NODE, BUILD_TYPE, BUILD_REL);
#else
    mstring_snprintf(instr, 128, "%s %s Keyfile: No host information available", PACKAGE, VERSION);
#endif
    mDES(instr, outstr, 128, key1, key2, 1);
    fwrite(outstr, sizeof(unsigned char), 128, outfile);

    // normalize to a derivitive of sizeof(unsigned long) * 2
    key_size += (key_size % (sizeof(unsigned long) * 2));
    mDES(inkey, outkey, key_size, key1, key2, 1);
    fwrite(outkey, sizeof(unsigned char), key_size, outfile);
    fclose(outfile);
    printf("Created %d byte keyfile.\n", key_size);

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

int mstring_snprintf(char *buf, const size_t size, const char *fmt, ...)
{
    int iLen;
    va_list argptr;
    va_start(argptr, fmt);

    iLen = mstring_vsnprintf(buf, size, fmt, argptr);

    va_end(argptr);
    return iLen;
}

int mstring_vsnprintf(char *buf, const size_t size, const char *fmt, va_list ap)
{
    int iLen;
#ifndef HAVE_VSNPRINTF
    iLen = vsprintf(buf, fmt, ap);
#else
    iLen = vsnprintf(buf, size, fmt, ap);
#endif
    return iLen;
}
