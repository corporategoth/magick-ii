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
** Revision 1.17  2001/06/15 07:20:40  prez
** Fixed windows compiling -- now works with MS Visual Studio 6.0
**
** Revision 1.16  2001/05/14 07:17:28  prez
** Fixed encryption :)
**
** Revision 1.15  2001/05/14 04:46:32  prez
** Changed to use 3BF (3 * blowfish) encryption.  DES removed totally.
**
** Revision 1.14  2001/05/13 18:59:17  prez
** Fixed adding of 8 extra bytes when we align to 8 byte boundaries
**
** Revision 1.13  2001/05/13 18:45:15  prez
** Fixed up the keyfile validation bug, and added more error reporting to
** the db load (also made sure it did not hang on certain circumstances).
**
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
#include <string.h>
#include "config.h"
#ifdef HASCRYPT
#include <stdarg.h>
#include "crypt/blowfish.h"
#ifdef HAVE_TERMIO_H
#include <termio.h>
#endif

#define VERIFY_SIZE	128
#define MIN_KEYLEN	16
#define MAX_KEYLEN	((BF_ROUNDS+2)*4)
#define DEF_KEYNAME	"magick.key"

size_t mCRYPT(const char *in, char *out, const size_t size,
	const char *key1, const char *key2, const int enc);
int mstring_snprintf(char *buf, const size_t size, const char *fmt, ...);
int mstring_vsnprintf(char *buf, const size_t size, const char *fmt, va_list ap);

#endif /* HASCRYPT */

int main(int argc, char **argv)
{
#ifndef HASCRYPT
    printf("Magick IRC Services - http://www.magick.tm\n");
    printf("    (c) 1997-2001 Preston A. Elder <prez@magick.tm>\n");
    printf("    (c) 1998-2001 William King <ungod@magick.tm>\n\n");

    fprintf(stderr, "You do not have encryption support.\n");

#else /* !HASCRYPT */
    size_t i, key_size;
    char key1[MAX_KEYLEN+1], key2[MAX_KEYLEN+1], verify[MAX_KEYLEN+1], filename[512];
    char instr[VERIFY_SIZE], outstr[VERIFY_SIZE];
    FILE *outfile, *tty;
#ifdef HAVE_TERMIO_H
    struct termio tty_new, tty_orig;
#endif

    printf("Magick IRC Services - http://www.magick.tm\n");
    printf("    (c) 1997-2001 Preston A. Elder <prez@magick.tm>\n");
    printf("    (c) 1998-2001 William King <ungod@magick.tm>\n\n");

    memset(filename, 0, 512);

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

    memset(key1, 0, MAX_KEYLEN+1);
    printf("NOTE: A key must be at least %d bytes long and may be up to %d bytes long.\n", MIN_KEYLEN, MAX_KEYLEN);
    printf("Enter database key 1: ");
    fgets(key1, MAX_KEYLEN, tty);
    key_size = strlen(key1);
    printf("\n");
    if (key_size < MIN_KEYLEN)
    {
	fprintf(stderr, "Key must be at least %d characters.\n", MIN_KEYLEN);
#ifdef HAVE_TERMIO_H
	ioctl(fileno(tty), TCSETA, &tty_orig);
#endif
	return 3;
    }

    memset(verify, 0, MAX_KEYLEN+1);
    printf("Re-Enter database key 1: ");
    fgets(verify, MAX_KEYLEN, tty);
    printf("\n");
    if (memcmp(key1, verify, MAX_KEYLEN)!=0)
    {
	fprintf(stderr, "Key mismatch, aborting key generation.\n");
#ifdef HAVE_TERMIO_H
	ioctl(fileno(tty), TCSETA, &tty_orig);
#endif
	return 4;
    }

    memset(key2, 0, MAX_KEYLEN+1);
    printf("Enter database key 2: ");
    fgets(key2, MAX_KEYLEN, tty);
    key_size = strlen(key2);
    printf("\n");
    if (key_size < MIN_KEYLEN)
    {
	fprintf(stderr, "Key must be at least %d characters.\n", MIN_KEYLEN);
#ifdef HAVE_TERMIO_H
	ioctl(fileno(tty), TCSETA, &tty_orig);
#endif
	return 3;
    }

    memset(verify, 0, MAX_KEYLEN+1);
    printf("Re-Enter database key 2: ");
    fgets(verify, MAX_KEYLEN, tty);
    printf("\n");
    if (memcmp(key2, verify, MAX_KEYLEN)!=0)
    {
	fprintf(stderr, "Key mismatch, aborting key generation.\n");
#ifdef HAVE_TERMIO_H
	ioctl(fileno(tty), TCSETA, &tty_orig);
#endif
	return 4;
    }

#ifdef HAVE_TERMIO_H
    ioctl(fileno(tty), TCSETA, &tty_orig);
#endif

    if (memcmp(key1, key2, MAX_KEYLEN)==0)
    {
	fprintf(stderr, "Key 1 and key 2 must be different!\n");
	return 5;
    }

    memset(instr, 0, VERIFY_SIZE);
#if defined(BUILD_NODE) && defined(BUILD_TYPE) && defined(BUILD_REL)
    mstring_snprintf(instr, VERIFY_SIZE, "%s %s Keyfile: %s %s %s", PACKAGE, VERSION, BUILD_NODE, BUILD_TYPE, BUILD_REL);
#else
    mstring_snprintf(instr, VERIFY_SIZE, "%s %s Keyfile: No host information available", PACKAGE, VERSION);
#endif
    mCRYPT(instr, outstr, VERIFY_SIZE, CRYPTO_KEY1, CRYPTO_KEY2, 1);
    fwrite(outstr, sizeof(char), VERIFY_SIZE, outfile);

    memset(verify, 0, MAX_KEYLEN+1);
    mCRYPT(key1, verify, MAX_KEYLEN, CRYPTO_KEY1, CRYPTO_KEY2, 1);
    fwrite(verify, sizeof(char), MAX_KEYLEN, outfile);
    memset(verify, 0, MAX_KEYLEN+1);
    mCRYPT(key2, verify, MAX_KEYLEN, CRYPTO_KEY1, CRYPTO_KEY2, 1);
    fwrite(verify, sizeof(char), MAX_KEYLEN, outfile);
    fclose(outfile);
    printf("Created %d byte keyfile.\n", VERIFY_SIZE + (2*MAX_KEYLEN));

#endif /* !HASCRYPT */
    return 0;
}

#ifdef HASCRYPT
size_t mCRYPT(const char *in, char *out, const size_t size,
	const char *key1, const char *key2, const int enc)
{
    BF_KEY bfkey1, bfkey2;
    unsigned char ivec1[8], ivec2[8], ivec3[8], buf1[8], buf2[8];
    unsigned int i, j;

    BF_set_key(&bfkey1, strlen(key1), (const unsigned char *) key1);
    BF_set_key(&bfkey2, strlen(key2), (const unsigned char *) key2);
    memset(ivec1, 0, 8);
    memset(ivec2, 0, 8);
    memset(ivec3, 0, 8);

    for (i=0; i<size; i+=8)
    {
	memset(buf1, 0, 8);
	memset(buf2, 0, 8);

	if (i+8 < size)
	    memcpy(buf1, &in[i], 8);
	else
	    for (j=0; j<8 && i+j < size; j++)
		buf1[j] = in[i+j];

	BF_cbc_encrypt(buf1, buf2, 8, &bfkey1, ivec1, enc ? BF_ENCRYPT : BF_DECRYPT);
	BF_cbc_encrypt(buf2, buf1, 8, &bfkey2, ivec2, enc ? BF_DECRYPT : BF_ENCRYPT);
	BF_cbc_encrypt(buf1, buf2, 8, &bfkey1, ivec3, enc ? BF_ENCRYPT : BF_DECRYPT);

	memcpy(&out[i], buf2, 8);
    }

    return i;
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

#endif /* HASCRYPT */
