#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
#endif

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
#define RCSID(x,y) const char *rcsid_utils_cpp_ ## x () { return y; }
RCSID(utils_cpp, "@(#)$Id$");
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.58  2001/02/11 07:41:28  prez
** Enhansed support for server numerics, specifically for Unreal.
**
** Revision 1.57  2001/02/03 02:21:35  prez
** Loads of changes, including adding ALLOW to ini file, cleaning up
** the includes, RCSID, and much more.  Also cleaned up most warnings.
**
** Revision 1.56  2001/01/01 05:32:45  prez
** Updated copywrights.  Added 'reversed help' syntax (so ACCESS HELP ==
** HELP ACCESS).
**
** Revision 1.55  2000/12/19 07:24:54  prez
** Massive updates.  Linux works again, added akill reject threshold, and
** lots of other stuff -- almost ready for b6 -- first beta after the
** re-written strings class.  Also now using log adapter!
**
** Revision 1.54  2000/12/10 13:06:13  prez
** Ditched alot of the *toa's since mstring can do it internally now.
**
** Revision 1.53  2000/10/10 11:47:53  prez
** mstring is re-written totally ... find or occurances
** or something has a problem, but we can debug that :)
**
** Revision 1.52  2000/10/04 07:39:46  prez
** Added MemCluster to speed up lockable, but it cores when we start
** getting real messages -- seemingly in an alloc in the events.
** Lots of printf's left in for debugging (so run as ./magick >output)
**
** Revision 1.51  2000/09/30 10:48:09  prez
** Some general code cleanups ... got rid of warnings, etc.
**
** Revision 1.50  2000/09/13 12:45:34  prez
** Added intergration of mpatrol (memory leak finder).  Default is set OFF,
** must enable with --enable-mpatrol in configure (and have mpatrol in system).
**
** Revision 1.49  2000/09/09 02:17:49  prez
** Changed time functions to actuallt accept the source nick as a param
** so that the time values (minutes, etc) can be customized.  Also added
** weeks to the time output.
**
** Revision 1.48  2000/08/22 09:30:14  prez
** Modified md5 hash to always return non-binary characters
**
** Revision 1.47  2000/08/22 08:43:42  prez
** Another re-write of locking stuff -- this time to essentially make all
** locks re-entrant ourselves, without relying on implementations to do it.
** Also stops us setting the same lock twice in the same thread.
**
** Revision 1.46  2000/08/06 05:27:48  prez
** Fixed akill, and a few other minor bugs.  Also made trace TOTALLY optional,
** and infact disabled by default due to it interfering everywhere.
**
** Revision 1.45  2000/07/28 14:49:36  prez
** Ditched the old wx stuff, mconfig now in use, we're now ready to
** release (only got some conversion tests to do).
**
** Revision 1.44  2000/07/21 00:18:50  prez
** Fixed database loading, we can now load AND save databases...
**
** Almost ready to release now :)
**
** Revision 1.43  2000/07/11 13:22:19  prez
** Fixed loading/saving -- they now work with encryption and compression.
** Tested, it works too!  Now all we need to do is fix the loading, and
** we're set ... :))
**
** Revision 1.42  2000/06/29 06:30:57  prez
** Added the support for the 'extra' chars (ie. at the end of a string)
** so we support odd-length strings.  Also updated documentation.
**
** Revision 1.41  2000/06/28 18:52:42  prez
** OOps, forgot to #include des/spr.h
**
** Revision 1.40  2000/06/28 12:20:50  prez
** Lots of encryption stuff, but essentially, we now have random
** key generation for the keyfile keys, and we can actually encrypt
** something, and get it back as we sent it in (specifically, the
** keyfile itself).
**
** Revision 1.39  2000/06/27 18:56:59  prez
** Added choosing of keys to configure, also created the keygen,
** and scrambler (so keys are not stored in clear text, even in
** the .h and binary files).  We should be set to do the decryption
** process now, as encryption (except for encryption of db's) is
** all done :)
**
** Revision 1.38  2000/05/28 05:05:14  prez
** More makefile stuff ... Now we should work on all platforms.
** Added alot of checking for different .h files, functions, etc.
** So now all #define's are config.h based (also added a default
** windows config.h, which will need to be copied on these systems).
**
** Revision 1.37  2000/05/21 04:49:41  prez
** Removed all wxLog tags, now totally using our own logging.
**
** Revision 1.36  2000/05/20 15:17:00  prez
** Changed LOG system to use ACE's log system, removed wxLog, and
** added wrappers into pch.h and magick.cpp.
**
** Revision 1.35  2000/05/13 07:05:47  prez
** Added displaying of sizes to all file fields..
**
** Revision 1.34  2000/04/30 03:48:30  prez
** Replaced all system calls with ACE_OS equivilants,
** also removed any dependancy on ACE from sxp (xml)
**
** Revision 1.33  2000/03/28 16:20:59  prez
** LOTS of RET() fixes, they should now be safe and not do double
** calculations.  Also a few bug fixes from testing.
**
** Revision 1.32  2000/03/19 08:50:57  prez
** More Borlandization -- Added WHAT project, and fixed a bunch
** of minor warnings that appear in borland.
**
** Revision 1.31  2000/02/27 03:58:40  prez
** Fixed the WHAT program, also removed RegEx from Magick.
**
** Revision 1.30  2000/02/23 12:21:04  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.29  2000/02/17 12:55:08  ungod
** still working on borlandization
**
** Revision 1.28  2000/02/16 12:59:41  ungod
** fixing for borland compilability
**
** Revision 1.27  2000/02/15 13:27:04  prez
** *** empty log message ***
**
** Revision 1.26  2000/02/15 10:37:51  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#include "magick.h"
#ifdef HASCRYPT
#include "des/spr.h"
#endif
#include "des/md5_locl.h"

vector<int> ParseNumbers(mstring what)
{
    FT("ParseNumbers", (what));
    vector<int> numbers;
    unsigned int i;
    mstring tmp;

    what.replace(",", " ");
    for (i=1; i<=what.WordCount(" "); i++)
    {
	tmp = what.ExtractWord(i, " ");
	if (!tmp.IsNumber() || tmp[0U] == '-' ||
	    tmp[tmp.size()-1] == '-' || tmp.WordCount("-") > 2)
	{
	    numbers.push_back(-1);
	}
	else if (tmp.Contains("-"))
	{
	    int j, limit;
	    j = atoi(tmp.Before("-").c_str());
	    limit = atoi(tmp.After("-").c_str());
	    if (limit >= j)
	    {
		for (; j<=limit; j++)
		    numbers.push_back(j);
	    }
	    else
	    {
		for (; j>=limit; j--)
		    numbers.push_back(j);
	    }
	}
	else
	{
	    numbers.push_back(atoi(tmp.c_str()));
	}
    }
    NRET(vector<int>, numbers);
}

unsigned long FromHumanTime(mstring in)
{
    FT("FromHumanTime", (in));

    unsigned int i = 0;
    unsigned long number = 0, total = 0;

    if (in[0u] == '+')
	i++;
    for (; i<in.size(); i++)
    {
	switch(in[i])
	{
	case 'Y':
	case 'y':
	    if (number != 0)
	    {
		total += number * (unsigned long)(60.0 * 60.0 * 24.0 * 365.25);
		number = 0;
	    }
	    break;
	case 'W':
	case 'w':
	    if (number != 0)
	    {
		total += number * 60 * 60 * 24 * 7;
		number = 0;
	    }
	    break;
	case 'D':
	case 'd':
	    if (number != 0)
	    {
		total += number * 60 * 60 * 24;
		number = 0;
	    }
	    break;
	case 'H':
	case 'h':
	    if (number != 0)
	    {
		total += number * 60 * 60;
		number = 0;
	    }
	    break;
	case 'M':
	case 'm':
	    if (number != 0)
	    {
		total += number * 60;
		number = 0;
	    }
	    break;
	case 'S':
	case 's':
	    if (number != 0)
	    {
		total += number;
		number = 0;
	    }
	    break;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	    number *= 10;
	    number += (unsigned long) (in[i] - '0');
	    break;
	default:
	    RET(0);
	}
    }
    if (number != 0)
	total += number;

    RET(total);
}

mstring ToHumanTime(unsigned long in, mstring source)
{
    FT("ToHumanTime", (in, source));

    mstring retval;
    if (in==0)
    {
	retval = Parent->getMessage(source, "VALS/TIME_UNLIMITED");
    }
    else
    {
	retval = DisectTime((long) in, source);
    }

    RET(retval);
}

mstring ToHumanNumber(unsigned long in)
{
    FT("ToHumanNumber", (in));

    mstring retval(in);

    if (in >= 11 && in <= 13)
    {
	retval += "th";
	RET(retval);
    }

    switch (retval[retval.length()-1])
    {
    case '3':
	retval += "rd";
	break;
    case '2':
	retval += "nd";
	break;
    case '1':
	retval += "st";
	break;
    default:
	retval += "th";
	break;
    }
    RET(retval);
}


mstring ToHumanSpace(unsigned long in)
{
    FT("ToHumanSpace", (in));
    mstring retval;
    retval.Format("%ub", in);

    unsigned long value = 1024;
    for (int power = 1; power<5; power++)
    {
	CP(("Comparing %d to %d = %d", in, power, value));
	if (in >= value)
	{
	    switch (power) {
	    case 4:
	    	retval.Format("%uTb", in / value);
		break;
	    case 3:
	    	retval.Format("%uGb", in / value);
		break;
	    case 2:
	    	retval.Format("%uMb", in / value);
		break;
	    case 1:
	    	retval.Format("%uKb", in / value);
		break;
	    }
	}
	else
	    break;
	value *= 1024;
    }
    RET(retval);
}

unsigned long FromHumanSpace(mstring in)
{
    FT("FromHumanTime", (in));

    unsigned int i=0;
    unsigned long number = 0, total = 0;

    if (in[0]=='+')
	i++;
    for (i=0; i<in.size(); i++)
    {
	switch(in[i])
	{
	case 'T':
	case 't':
	    if (number != 0)
	    {
		total += number * 1024 * 1024 * 1024 * 1024;
		number = 0;
	    }
	    break;
	case 'G':
	case 'g':
	    if (number != 0)
	    {
		total += number * 1024 * 1024 * 1024;
		number = 0;
	    }
	    break;
	case 'M':
	case 'm':
	    if (number != 0)
	    {
		total += number * 1024 * 1024;
		number = 0;
	    }
	    break;
	case 'K':
	case 'k':
	    if (number != 0)
	    {
		total += number * 1024;
		number = 0;
	    }
	    break;
	case 'B':
	case 'b':
	    if (number != 0)
	    {
		total += number;
		number = 0;
	    }
	    break;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	    number *= 10;
	    number += (unsigned long) (in[i] - '0');
	    break;
	default:
	    RET(0);
	}
    }
    if (number != 0)
	total += number;

    RET(total);
}

void mDES(unsigned char *in, unsigned char *out, size_t size,
	des_key_schedule key1, des_key_schedule key2, int enc)
{
    FT("mDES", ("(unsigned char *) in", "(unsigned char *) out", size,
	"(des_key_schedule) key1", "(des_key_schedule) key2", enc));
#ifndef HASCRYPT
    memset(out, 0, size);
    memcpy(out, in, size);
#else
    DES_LONG tuple[2], t0, t1;
    unsigned char *iptr, *optr, tmp[8];
    unsigned int i;

    memset(out, 0, size);
    for (iptr=in, optr=out, i=0; i+7<size; i+=8)
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

void mHASH(unsigned char *in, size_t size, unsigned char *out)
{
    unsigned char md[MD5_DIGEST_LENGTH];
    MD5_CTX c;
    memset(md, 0, MD5_DIGEST_LENGTH);
    MD5_Init(&c);
    MD5_Update(&c, in, size);
    MD5_Final(md, &c);
    memset(&c, 0, sizeof(MD5_CTX));
    memset(out, 0, (MD5_DIGEST_LENGTH*2)+1);
    for (int i=0; i<MD5_DIGEST_LENGTH; i++)
    {
	sprintf((char *) &out[i*2], "%02x", md[i]);
    }
    memset(md, 0, MD5_DIGEST_LENGTH);
}

/* Copied direct from Unreal code */
static const char char_to_base64[] = {
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,
	-1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
	25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, -1, -1, -1, -1, -1,
	-1, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
	51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, -1, 63, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};


static const char base64_to_char[] = {
	'0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
	'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
	'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
	'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd',
	'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',
	'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
	'u', 'v', 'w', 'x', 'y', 'z', '{', '}' };


unsigned long str_to_base64(mstring in)
{
	if (!in.length())
	    return 0;

	unsigned long i = 0, v = char_to_base64[(unsigned char) in[i++]];

	while (i < in.length())
	{
		v <<= 6;
		v += char_to_base64[(unsigned char) in[i++]];
	}

	return v;
}


mstring base64_to_str(unsigned long in)
{
	/* 32/6 == max 6 bytes for representation, 
	 * +1 for the null, +1 for byte boundaries 
	 */
	char base64buf[8];
	unsigned long i = 7;

	base64buf[i] = '\0';

	do
	{
		base64buf[--i] = base64_to_char[in & 63];
	}
	while (in >>= 6);

	return mstring(base64buf + i);
}
