#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
#endif

/* Magick IRC Services
**
** (c) 1997-2002 Preston Elder <prez@magick.tm>
** (c) 1998-2002 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any circumstances,
** however it may be added to if any modifications are made to this
** file.  All modified code must be clearly documented and labelled.
**
** This code is released under the Artistic License v2.0 or better.
** The full text of this license should be contained in a file called
** COPYING distributed with this code.  If this file does not exist,
** it may be viewed here: http://www.magick.tm/m2/license.html
**
** ======================================================================= */
#define RCSID(x,y) const char *rcsid_utils_cpp_ ## x () { return y; }
RCSID(utils_cpp, "@(#)$Id$");

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

#include "magick.h"

vector < int > ParseNumbers(const mstring & what)
{
    BTCB();
    FT("ParseNumbers", (what));
    vector < int > numbers;
    unsigned int i;
    mstring tmp;

    for (i = 1; i <= what.WordCount(", "); i++)
    {
	tmp = what.ExtractWord(i, ", ");
	if (!tmp.IsNumber() || tmp[0U] == '-' || tmp[tmp.size() - 1] == '-' || tmp.WordCount("-") > 2)
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
		for (; j <= limit; j++)
		    numbers.push_back(j);
	    }
	    else
	    {
		for (; j >= limit; j--)
		    numbers.push_back(j);
	    }
	}
	else
	{
	    numbers.push_back(atoi(tmp.c_str()));
	}
    }
    NRET(vector < int >, numbers);

    ETCB();
}

bool MakeDirectory(const mstring & in)
{
    BTCB();
    FT("MakeDirectory", (in));

    int i, occ = in.Occurances(DirSlash);

    for (i = 0; i <= occ; i++)
    {
	mstring path(in);
	int pos = in.find(DirSlash, i + 1);

	if (pos >= 1)
	    path.Truncate(pos);
	else if (pos == 0)
	    path = DirSlash;
#ifdef WIN32
	else if (pos == 2 && path[1u] == ':')
	{
	    // We got X:\ (for windows), handle it.
	    path.Truncate(3);
	    pos = 0;
	}
#endif

	int j = ACE_OS::access(path.c_str(), X_OK);

	if (j < 0)
	{
	    // Its the root, forget it ... *sigh*
	    if (pos == 0)
		continue;

	    // Doesnt exist, try and create it ...
	    j = ACE_OS::mkdir(in.c_str());
	    if (j < 0 && errno)
	    {
		LOG(LM_ERROR, "SYS_ERRORS/DIROPERROR", ("mkdir", in, errno, strerror(errno)));
		RET(false);
	    }
	}
    }
    // After all that, make sure we can create files in the target dir.
    if (ACE_OS::access(in.c_str(), W_OK) < 0)
    {
	RET(false);
    }
    RET(true);
    ETCB();
}

unsigned long FromHumanTime(const mstring & in)
{
    BTCB();
    FT("FromHumanTime", (in));

    unsigned int i = 0;
    unsigned long number = 0, total = 0;

    if (in[0u] == '+')
	i++;
    for (; i < in.size(); i++)
    {
	switch (in[i])
	{
	case 'Y':
	case 'y':
	    if (number != 0)
	    {
		total += number * static_cast < unsigned long > (60.0 * 60.0 * 24.0 * 365.25);

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
	    number += static_cast < unsigned long > (in[i] - '0');

	    break;
	default:
	    RET(0);
	}
    }
    if (number != 0)
	total += number;

    RET(total);
    ETCB();
}

mstring ToHumanTime(const unsigned long in, const mstring & source)
{
    BTCB();
    FT("ToHumanTime", (in, source));

    mstring retval;

    if (in == 0)
    {
	retval = Magick::instance().getMessage(source, "VALS/TIME_UNLIMITED");
    }
    else
    {
	retval = DisectTime(static_cast < long > (in), source);
    }

    RET(retval);
    ETCB();
}

mstring ToHumanNumber(const unsigned long in)
{
    BTCB();
    FT("ToHumanNumber", (in));

    mstring retval(in);

    if (in >= 11 && in <= 13)
    {
	retval += "th";
	RET(retval);
    }

    switch (retval[retval.length() - 1])
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
    ETCB();
}

mstring ToHumanSpace(const unsigned long in)
{
    BTCB();
    FT("ToHumanSpace", (in));
    mstring retval;

    retval.Format("%ub", in);

    unsigned long value = 1024;
    unsigned int power = 0;

    while (in > value)
    {
	value *= 1024;
	power++;
    }
    value /= 1024;
    unsigned long num = in / value;
    unsigned long rem = in % value;

    while (rem > 10)
	rem /= 10;

    switch (power)
    {
    case 5:
	if (rem)
	    retval.Format("%u.%uPb", num, rem);
	else
	    retval.Format("%uPb", num);
	break;
    case 4:
	if (rem)
	    retval.Format("%u.%uTb", num, rem);
	else
	    retval.Format("%uTb", num);
	break;
    case 3:
	if (rem)
	    retval.Format("%u.%uGb", num, rem);
	else
	    retval.Format("%uGb", num);
	break;
    case 2:
	if (rem)
	    retval.Format("%u.%uMb", num, rem);
	else
	    retval.Format("%uMb", num);
	break;
    case 1:
	if (rem)
	    retval.Format("%u.%uKb", num, rem);
	else
	    retval.Format("%uKb", num);
	break;
    case 0:
	retval.Format("%ub", num);
	break;
    }

    RET(retval);
    ETCB();
}

unsigned long FromHumanSpace(const mstring & in)
{
    BTCB();
    FT("FromHumanTime", (in));

    unsigned int i = 0;
    unsigned long number = 0, total = 0;

    if (in.first() == '+')
	i++;
    for (i = 0; i < in.size(); i++)
    {
	switch (in[i])
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
	    number += static_cast < unsigned long > (in[i] - '0');

	    break;
	default:
	    RET(0);
	}
    }
    if (number != 0)
	total += number;

    RET(total);
    ETCB();
}

mstring parseMessage(const mstring & message, const mVarArray & va)
{
    BTCB();
    FT("parseMessage", (message, "(const mVarArray&) va"));

    mstring data, tok;
    int start, end, toknum, length = static_cast < int > (message.length());

    start = end = 0;
    while (end < length)
    {
	end = message.SubString(start).find("$");
	if (end < 0)
	{
	    data << message.SubString(start);
	    break;
	}
	end += start;
	if (end > start)
	    data << message.SubString(start, end - 1);
	if (message[static_cast < size_t > (end + 1)] == '$')
	{
	    data << '$';
	    start = end + 2;
	    continue;
	}

	tok.erase();
	while (isdigit(message[static_cast < size_t > (++end)]))
	{
	    tok << message[static_cast < size_t > (end)];
	    if (tok == "0")
		break;
	}
	if (tok.length())
	{
	    toknum = atoi(tok.c_str()) - 1;
	    if (toknum >= 0 && toknum < va.count())
	    {
		data << va[toknum].AsString();
	    }
	}
	else
	{
	    data << '$';
	}
	start = end;
    }

    RET(data);
    ETCB();
}

size_t mCRYPT(const char *in, char *out, const size_t size, const char *key1, const char *key2, const bool enc)
{
    BTCB();
    FT("mCRYPT", ("(const char *) in", "(char *) out", size, "(const char *) key1", "(const char *) key2", enc));

#ifndef HASCRYPT
    memset(out, 0, size);
    memcpy(out, in, size);
    return size;
#else
    BF_KEY bfkey1, bfkey2;
    unsigned char ivec1[8], ivec2[8], ivec3[8], buf1[8], buf2[8];
    unsigned int i, j;

    BF_set_key(&bfkey1, strlen(key1), reinterpret_cast < const unsigned char * > (key1));
    BF_set_key(&bfkey2, strlen(key2), reinterpret_cast < const unsigned char * > (key2));

    memset(ivec1, 0, 8);
    memset(ivec2, 0, 8);
    memset(ivec3, 0, 8);

    for (i = 0; i < size; i += 8)
    {
	memset(buf1, 0, 8);
	memset(buf2, 0, 8);

	if (i + 8 < size)
	    memcpy(buf1, &in[i], 8);
	else
	    for (j = 0; j < 8 && i + j < size; j++)
		buf1[j] = in[i + j];

	BF_cbc_encrypt(buf1, buf2, 8, &bfkey1, ivec1, enc ? BF_ENCRYPT : BF_DECRYPT);
	BF_cbc_encrypt(buf2, buf1, 8, &bfkey2, ivec2, enc ? BF_DECRYPT : BF_ENCRYPT);
	BF_cbc_encrypt(buf1, buf2, 8, &bfkey1, ivec3, enc ? BF_ENCRYPT : BF_DECRYPT);

	memcpy(&out[i], buf2, 8);
    }
    out[i] = 0;

    // Security ... dont leave keys around ...
    memset(&bfkey1, 0, sizeof(BF_KEY));
    memset(&bfkey2, 0, sizeof(BF_KEY));
    return i;
#endif
    ETCB();
}

void mHASH16(const char *in, const size_t size, char *out)
{
    BTCB();
    MD5_CTX c;

    memset(out, 0, MD5_DIGEST_LENGTH);
    MD5_Init(&c);
    MD5_Update(&c, const_cast < unsigned char * > (reinterpret_cast < const unsigned char * > (in)), size);
    MD5_Final(const_cast < unsigned char * > (reinterpret_cast < const unsigned char * > (out)), &c);

    memset(&c, 0, sizeof(MD5_CTX));
    ETCB();
}

void mHASH(const char *in, const size_t size, char *out)
{
    BTCB();
    unsigned char md[MD5_DIGEST_LENGTH];
    mHASH16(in, size, reinterpret_cast < char * > (md));

    memset(out, 0, (MD5_DIGEST_LENGTH * 2) + 1);
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
	sprintf(&out[i * 2], "%02x", md[i]);
    memset(md, 0, MD5_DIGEST_LENGTH);
    ETCB();
}

mstring sysinfo_node()
{
    BTCB();
    mstring retval;
    ACE_utsname *type = new ACE_utsname;

    ACE_OS::uname(type);
    retval = type->nodename;
    delete type;

    return retval;
    ETCB();
}

mstring sysinfo_type()
{
    BTCB();
    mstring retval;
    ACE_utsname *type = new ACE_utsname;

    ACE_OS::uname(type);
    retval << type->sysname << "/" << type->machine;
    delete type;

    return retval;
    ETCB();
}

mstring sysinfo_rel()
{
    BTCB();
    mstring retval;
    ACE_utsname *type = new ACE_utsname;

    ACE_OS::uname(type);
    retval = type->release;
    delete type;

    return retval;
    ETCB();
}

mstring version_string(bool extended)
{
    BTCB();
    mstring tmp(VERSION);

    if (!RELEASE.empty())
	tmp += RELEASE;
    if (!PATCH1.empty())
	tmp += "+" + PATCH1;
    if (!PATCH2.empty())
	tmp += "+" + PATCH2;
    if (!PATCH3.empty())
	tmp += "+" + PATCH3;
    if (!PATCH4.empty())
	tmp += "+" + PATCH4;
    if (!PATCH5.empty())
	tmp += "+" + PATCH5;
    if (!PATCH6.empty())
	tmp += "+" + PATCH6;
    if (!PATCH7.empty())
	tmp += "+" + PATCH7;
    if (!PATCH8.empty())
	tmp += "+" + PATCH8;
    if (!PATCH9.empty())
	tmp += "+" + PATCH9;
    if (extended)
    {
	tmp << " [";
#ifdef HASCRYPT
	tmp << "C";
#else
	tmp << "c";
#endif
#ifdef MAGICK_TRACE_WORKS
	tmp << "T";
#else
	tmp << "t";
#endif
#ifdef MAGICK_LOCKS_WORK
	tmp << "L";
#else
	tmp << "l";
#endif
#ifdef MAGICK_HAS_EXCEPTIONS
	tmp << "E";
#else
	tmp << "e";
#endif
#ifdef CONVERT
	tmp << "V";
#else
	tmp << "v";
#endif
#ifdef GETPASS
	tmp << "G";
#else
	tmp << "g";
#endif
#if defined(MAGICK_USE_MPATROL) || defined(MAGICK_USE_EFENCE)
	tmp << "D";
#else
	tmp << "d";
#endif
	tmp << "] Build #" << BUILD_NUMBER << " (" << BUILD_TIME << ")";
    }
    return tmp;
    ETCB();
}
