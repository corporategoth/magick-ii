#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
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

#include "utils.h"
#include "trace.h"
#include "magick.h"
#ifdef HASCRYPT
#include "des/spr.h"
#endif
#include "des/md5_locl.h"

const unsigned long TxnIds::min = 1000000000;
const unsigned long TxnIds::keeptime = 60 * 60 * 24;
map<unsigned long, mDateTime> TxnIds::i_Ids;
unsigned long TxnIds::i_Current = TxnIds::min;

unsigned long TxnIds::Create()
{
    NFT("TxnIds::Create");

    WLOCK(("TxnIds"));
    i_Current++;
    while (i_Current >= min && i_Ids.find(i_Current) != i_Ids.end())
	i_Current++;

    if (i_Current >= min)
    {
	i_Ids[i_Current] = Now();
	RET(i_Current);
    }
    else
    {
	i_Current = min;
	while (i_Current >= min && i_Ids.find(i_Current) != i_Ids.end())
	    i_Current++;
    }

    if (i_Current >= min)
    {
	i_Ids[i_Current] = Now();
	RET(i_Current);
    }
    else
    {
	Log(LM_ERROR, Parent->getLogMessage("SYS_ERRORS/OUTOFTXNIDS"));
	RET(0);
    }
}

unsigned long TxnIds::Current()
{
    NFT("TxnIds::Current");
    RET(i_Current);
}

bool TxnIds::Register(unsigned long id)
{
    FT("TxnIds::Register", (id));

    WLOCK(("TxnIds"));
    if (i_Ids.find(id) != i_Ids.end())
	RET(false);

    i_Ids[id] = Now();
    if (i_Current < id)
	i_Current = id;

    RET(true);
}

void TxnIds::Expire()
{
    NFT("TxnIds::Expire");

    WLOCK(("TxnIds"));
    map<unsigned long, mDateTime>::iterator iter;
    vector<unsigned long> kill;
    for (iter=i_Ids.begin(); iter != i_Ids.end(); iter++)
    {
	if (iter->second.SecondsSince() > keeptime)
	    kill.push_back(iter->first);
    }
    for (unsigned int i=0; i<kill.size(); i++)
	i_Ids.erase(kill[i]);
}

vector<int> ParseNumbers(mstring what)
{
    FT("ParseNumbers", (what));
    vector<int> numbers;
    unsigned int i;
    mstring tmp;

    what.Replace(",", " ");
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
	    j = ACE_OS::atoi(tmp.Before("-").c_str());
	    limit = ACE_OS::atoi(tmp.After("-").c_str());
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
	    numbers.push_back(ACE_OS::atoi(tmp.c_str()));
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

    mstring retval = ltoa(in);

    if (in >= 11 && in <= 13)
    {
	retval += "th";
	RET(retval);
    }

    switch (retval[retval.Len()-1])
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
    int i, j;

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
