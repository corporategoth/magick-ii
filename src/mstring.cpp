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
** Revision 1.71  2000/10/10 11:47:52  prez
** mstring is re-written totally ... find or occurances
** or something has a problem, but we can debug that :)
**
** Revision 1.70  2000/10/07 11:00:12  ungod
** no message
**
** Revision 1.69  2000/09/30 10:48:08  prez
** Some general code cleanups ... got rid of warnings, etc.
**
** Revision 1.68  2000/09/05 10:53:07  prez
** Only have operserv.cpp and server.cpp to go with T_Changing / T_Modify
** tracing -- also modified keygen to allow for cmdline generation (ie.
** specify 1 option and enter keys, or 2 options and the key is read from
** a file).  This allows for paragraphs with \n's in them, and helps so you
** do not have to type out 1024 bytes :)
**
** Revision 1.67  2000/08/03 13:06:31  prez
** Fixed a bunch of stuff in mstring (caused exceptions on FreeBSD machines).
**
** Revision 1.66  2000/07/28 14:49:35  prez
** Ditched the old wx stuff, mconfig now in use, we're now ready to
** release (only got some conversion tests to do).
**
** Revision 1.65  2000/06/10 07:01:03  prez
** Fixed a bunch of little bugs ...
**
** Revision 1.64  2000/05/28 05:05:14  prez
** More makefile stuff ... Now we should work on all platforms.
** Added alot of checking for different .h files, functions, etc.
** So now all #define's are config.h based (also added a default
** windows config.h, which will need to be copied on these systems).
**
** Revision 1.63  2000/05/19 10:48:14  prez
** Finalized the DCC Sending (now uses the Action map properly)
**
** Revision 1.62  2000/05/17 14:08:12  prez
** More tweaking with DCC, and getting iostream mods working ...
**
** Revision 1.61  2000/05/15 08:43:28  ungod
** fix for recursion issue upon adding of two mstrings on certain platforms.
**
** Revision 1.60  2000/04/30 05:19:39  ungod
** ACE_OS::AC_OS:: hey prez? *smirk*
**
** Revision 1.59  2000/04/30 03:48:29  prez
** Replaced all system calls with ACE_OS equivilants,
** also removed any dependancy on ACE from sxp (xml)
**
** Revision 1.58  2000/02/27 03:58:40  prez
** Fixed the WHAT program, also removed RegEx from Magick.
**
** Revision 1.57  2000/02/23 12:21:03  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.56  2000/02/17 12:55:06  ungod
** still working on borlandization
**
** Revision 1.55  2000/02/16 12:59:40  ungod
** fixing for borland compilability
**
** Revision 1.54  2000/02/15 13:27:04  prez
** *** empty log message ***
**
** Revision 1.53  2000/02/15 10:37:50  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */
#include "mstring.h"

#ifdef WIN32
mstring const DirSlash="\\";
#else
mstring const DirSlash="/";
#endif
mstring const Blank("");
mstring const IRC_Bold((char) 2);	// ^B
mstring const IRC_Underline((char) 31);	// ^_
mstring const IRC_Reverse((char) 21);	// ^V
mstring const IRC_Color((char) 3);	// ^C
mstring const IRC_Off((char) 15);	// ^O


void mstring::copy(const char *in, size_t length)
{
    if (i_str != NULL)
	delete [] i_str;
    i_len = length;
    i_str = new char[i_len+1];
    memset(i_str, 0, i_len+1);
    memcpy(i_str, in, i_len);
}

void mstring::append(const char *in, size_t length)
{
    if (length == 0)
	return;

    char *tmp = new char[i_len + length + 1];
    memset(tmp, 0, i_len + length + 1);
    if (i_str != NULL)
	memcpy(tmp, i_str, i_len);
    memcpy(&tmp[i_len], in, length);

    if (i_str != NULL)
	delete [] i_str;

    i_len += length;
    i_str = tmp;
}

void mstring::erase(int begin, int end)
{
    int i;
    char *tmp = NULL;
    if (i_str == NULL)
	return;

    if (begin < 0)
	begin = 0;
    if (end < 0 || end > i_len)
	end = i_len;
    if (begin > end)
    {
	i = begin;
	begin = end;
	end = i;
    }
    if (begin > i_len)
	return;

    if (begin > 0 || end+1 < i_len)
    {
	i=0;
	tmp = new char[i_len - (end-begin) + 1];
	memset(tmp, 0, i_len - (end-begin) + 1);
	if (begin > 0)
	{
	    memcpy(tmp, i_str, begin);
	    i += begin;
	}
	if (end+1 < i_len)
	{
	    memcpy(&tmp[i], &i_str[end+1], i_len-end+1);
	}
    }
    delete [] i_str;
    if (tmp == NULL)
    {
	i_str = NULL;
	i_len = 0;
    }
    else
    {
	i_str = tmp;
	i_len -= (end-begin);
    }
}

void mstring::insert(size_t pos, const char *in, size_t length)
{
    int i;
    char *tmp = NULL;

    if (pos >= i_len)
    {
	append(in, length);
	return;
    }

    tmp = new char[i_len + length + 1];
    memset(tmp, 0, i_len + length + 1);

    i=0;
    if (pos > 0)
    {
	memcpy(tmp, i_str, pos);
	i += pos;
    }
    memcpy(&tmp[i], in, length);
    i += length;
    memcpy(&tmp[i], &i_str[pos], i_len-pos);

    if (i_str != NULL)
	delete [] i_str;
    i_str = tmp;
    i_len += length;
}

// We compare up to a length they both have.  If its
// the same, then return (-1 * i_len) if i_str is bigger,
// or length if in is bigger.  0 means strings are the
// same length and have the same text.
int mstring::compare (const char *in, size_t length) const
{
    int retval = 0;
    if (length && i_len)
    {
	retval = memcmp(i_str, in, ((length < i_len) ? length : i_len));
	if (retval == 0)
	{
	    if (length < i_len)
		retval = i_len * -1;
	    else if (i_len < length)
		retval = length;
	}
    }
    else if (i_len)
	retval = i_len * -1;
    else if (length)
	retval = length;

    return retval;
}

void mstring::swap(mstring &in)
{
    char *str = i_str;
    size_t len = i_len;

    i_str = in.i_str;
    i_len = in.i_len;

    in.i_str = str;
    in.i_len = len;
}

const char *mstring::c_str() const
{
    if (i_str == NULL)
	return "";
    else
	return (const char *) i_str;
}

char mstring::operator[] (size_t offs) const
{
    if (i_str == NULL || offs > i_len)
	return 0;
    else
	return i_str[offs];
}

char mstring::first() const
{
    if (i_str == NULL)
	return 0;
    else
	return i_str[0];
}

char mstring::last() const
{
    if (i_str == NULL)
	return 0;
    else
	return i_str[i_len-1];
}


// Index value of any of these chars
int mstring::find_first_of(const char *str, size_t length) const
{
    int i, retval = i_len + 1;
    char *ptr;

    if (i_str == NULL)
	return -1;

    for (i=0; i<length; i++)
    {
	char *ptr = index(i_str, str[i]);
	if (ptr != NULL && ptr - i_str < retval)
	    retval = ptr - i_str;
    }
    if (retval > i_len)
	retval = -1;
    return retval;
}

// Reverse Index value of any of these chars
int mstring::find_last_of(const char *str, size_t length) const
{
    int i, retval = -1;
    char *ptr;

    if (i_str == NULL)
	return -1;

    for (i=0; i<length; i++)
    {
	char *ptr = rindex(i_str, str[i]);
	if (ptr != NULL && ptr - i_str > retval)
	    retval = ptr - i_str;
    }
    return retval;
}

// Opposite to index for any of these chars
int mstring::find_first_not_of(const char *str, size_t length) const
{
    int i;

    if (i_str == NULL)
	return -1;

    char *tmp = new char[length+1];
    memset(tmp, 0, length+1);
    memcpy(tmp, str, length);

    for (i=0; i<i_len; i++)
    {
	if (index(tmp, i_str[i])==NULL)
	    return i;
    }
    delete [] tmp;
    return -1;
}

// Opposite to rindex for any of these chars
int mstring::find_last_not_of(const char *str, size_t length) const
{
    int i;

    if (i_str == NULL)
	return -1;

    char *tmp = new char[length+1];
    memset(tmp, 0, length+1);
    memcpy(tmp, str, length);

    for (i=i_len; i>0; i--)
    {
	if (index(tmp, i_str[i-1])==NULL)
	    return i-1;
    }
    delete [] tmp;
    return -1;
}


int mstring::occurances(const char *str) const
{
    int count = 0;
    char *ptr, *ptr2;

    if (i_str == NULL)
	return 0;

    ptr = strstr(i_str, str);
    while (ptr != NULL)
    {
	count++;
	ptr2 = ptr;
	ptr = strstr(ptr2, str);
    }
    return count;
}

// Find occurance of full string
int mstring::find(const char *str, int occurance) const
{
    int i, retval = -1;
    char *ptr, *ptr2;

    if (i_str == NULL)
	return -1;

    ptr = i_str;
    for (i=0; i < occurance; i++)
    {
	ptr2 = ptr;
	ptr = strstr(ptr2, str);
	if (ptr == NULL)
	    break;
    }
    if (ptr != NULL)
	retval = ptr - i_str;
	
    return retval;
}


// Reverse Find occurance of full string
int mstring::rfind(const char *str, int occurance) const
{
    int occ, retval = -1;

    if (occurance < 1)
	occurance = 1;

    occ = occurances(str);
    if (occurance < occ)
	retval = find(str, occ - occurance);

    return retval;
}


// Replace find string with replace string (optionally for all)
void mstring::replace(const char *find, const char *replace, bool all)
{
    int i, j, old_len, find_len, amt_replace = 0, replace_len = 0;
    char *tmp, *start, *end;
    vector<pair<char *, int> > ptrs;
    vector<pair<char *, int> >::iterator iter;

    if (i_str == NULL)
	return;

    old_len = i_len;
    find_len = strlen(find);
    replace_len = strlen(replace);

    start=i_str;
    end=strstr(i_str, find);
    while (end != NULL)
    {
	ptrs.push_back(pair<char *, int>(start, end-start));
	end += find_len;
	start = end;
	amt_replace++;
	if (!all)
	    break;
	end = strstr(start, find);
    }
    ptrs.push_back(pair<char *, int>(start,-1));

    i_len += (amt_replace * (replace_len - find_len));
    tmp = new char[i_len];
    memset(tmp, 0, i_len);

    i = j = 0;
    for (iter=ptrs.begin(); iter!=ptrs.end(); iter++)
    {
	if (iter->second >= 0)
	{
	    memcpy(&tmp[j], iter->first, iter->second);
	    j += iter->second;
	    memcpy(&tmp[j], replace, replace_len);
	    j += replace_len;
	    i += iter->second + find_len;
	}
	else
	{
	    memcpy(&tmp[j], iter->first, old_len - i);
	}
    }
    delete [] i_str;
    i_str = tmp;
}

void mstring::replace(int begin, int end, char *replace, size_t length)
{
    erase(begin, end);
    insert(begin, replace, length);
}

mstring mstring::substr(int nFirst, int nCount) const
{
    mstring retval;
    if (i_str != NULL)
    {
	if (nFirst < 0)
	    nFirst = 0;
	if (nCount < 0)
	    nCount = i_len - nFirst;
	if ((nCount + nFirst) > i_len)
	    nCount = i_len - nFirst;

	retval.copy(&i_str[nFirst], nCount);
    }
    return retval;
}


/********************************************************/


bool mstring::IsWord() const
{
    if (i_str == NULL)
	return false;

    for (size_t i=0; i<i_len; i++)
	if (!isalpha(i_str[i]))
	    return false;
    return true;
}

bool mstring::IsNumber() const
{
    if (i_str == NULL)
	return false;

    for (size_t i=0; i<i_len; i++)
	if (!isdigit(i_str[i]))
	    return false;
    return true;
}

bool mstring::IsAscii() const
{
    if (i_str == NULL)
	return false;

    for (size_t i=0; i<i_len; i++)
	if (!isascii(i_str[i]))
	    return false;
    return true;
}

bool mstring::IsBool() const
{
  if (IsSameAs("true", true) || IsSameAs("on", true) ||
      IsSameAs("yes", true) || IsSameAs("y", true) ||
      IsSameAs("t", true) || compare("1") ||
      IsSameAs("false", true) || IsSameAs("off", true) ||
      IsSameAs("no", true) || IsSameAs("n", true) ||
      IsSameAs("f", true) || compare("0"))
    return true;
  else
    return false;
}

bool mstring::GetBool() const
{
  mstring tmp(*this);
  if (IsSameAs("true", true) || IsSameAs("on", true) ||
      IsSameAs("yes", true) || IsSameAs("y", true) ||
      IsSameAs("t", true) || compare("1"))
    return true;
  else
    return false;
}


mstring mstring::UpperCase() const
{
    mstring tmp(*this);
    tmp.MakeUpper();
    return tmp;
}

mstring mstring::LowerCase() const
{
    mstring tmp(*this);
    tmp.MakeLower();
    return tmp;
}

void mstring::MakeUpper()
{
    if (i_str != NULL)
	return;

    for (size_t i=0; i<i_len; i++)
	i_str[i] = toupper(i_str[i]);
}

void mstring::MakeLower()
{
    if (i_str != NULL)
	return;

    for (size_t i=0; i<i_len; i++)
	i_str[i] = tolower(i_str[i]);
}


int mstring::Occurances(const mstring &in, bool NoCase) const
{
    if (NoCase)
    {
	mstring tmp(in);
	return UpperCase().occurances(tmp.UpperCase().i_str);
    }
    else
	return occurances(in.i_str);
}

int mstring::Find(const mstring &in, bool NoCase, int occurance) const
{
    if (NoCase)
    {
	mstring tmp(in);
	return UpperCase().find(tmp.UpperCase().i_str, occurance);
    }
    else
	return find(in.i_str, occurance);
}

int mstring::RevFind(const mstring &in, bool NoCase, int occurance) const
{
    if (NoCase)
    {
	mstring tmp(in);
	return UpperCase().rfind(tmp.UpperCase().i_str, occurance);
    }
    else
	return rfind(in.i_str, occurance);
}

int mstring::Cmp(const mstring &in, bool NoCase) const
{
    if (NoCase)
    {
	mstring tmp(in);
	return UpperCase().compare(tmp.UpperCase());
    }
    else
	return compare(in);
}

bool mstring::Matches(const mstring &in, bool NoCase) const
{
    return match_wild(in.i_str, i_str, NoCase);
}

void mstring::Trim(bool right, const mstring &delims)
{
    if (right)
	erase(find_last_not_of(delims.i_str)+1);
    else
	erase(0, find_first_not_of(delims.i_str)-1);
}

mstring mstring::Strip(bool right, const mstring &deilms) const
{
    mstring tmp(*this);
    tmp.Trim(right, deilms);
    return tmp;
}
    
int mstring::Format(const char *fmt, ...)
{
    va_list argptr;
    va_start(argptr, fmt);

    int iLen = FormatV(fmt, argptr);

    va_end(argptr);
    return iLen;
}

int mstring::FormatV(const char *fmt, va_list argptr)
{
    int length, size = 1024;
    char *buffer = new char[size];
    memset(buffer, 0, size);
    while (buffer != NULL)
    {
	length = vsnprintf(buffer, size-1, fmt, argptr);
	if (length < size)
	    break;
	delete [] buffer;
	size *= 2;
	buffer = new char[size];
	memset(buffer, 0, size);
    }
    if (buffer && length < 1)
	delete [] buffer;
    if (i_str != NULL)
    {
	i_str = NULL;
	i_len = 0;
    }
    if (buffer)
    {
	i_str = buffer;
	i_len = length;
    }
    return length;
}


mstring mstring::Before(const mstring &in, int occurance) const
{
	int m_pos = Find(in,false,occurance);
	if (m_pos >= 0)
		return Left(m_pos);
	else
		return *this;
}

mstring mstring::After(const mstring &in, int occurance) const
{
	int m_pos = Find(in,false,occurance);
	if (m_pos >= 0)
		return Right(m_pos+in.i_len);
	else
		return *this;
}

mstring mstring::RevBefore(const mstring &in, int occurance) const
{
	int m_pos = RevFind(in,false,occurance);
	if (m_pos >= 0)
		return Left(m_pos);
	else
		return *this;
}

mstring mstring::RevAfter(const mstring &in, int occurance) const
{
	int m_pos = RevFind(in,false,occurance);
	if (m_pos >= 0)
		return Right(m_pos+in.i_len);
	else
		return *this;
}

mstring mstring::SubString(int from, int to) const
{
    if (to < from)
    {
	int i = from;
	from = to;
	to = i;
    }
    return substr(from, to-from);
}


unsigned int mstring::WordCount(const mstring &delim, bool assemble) const
{
    int Result=0;
    size_t i=0;
    while(i<i_len)
    {
	while(i<i_len && assemble && delim.Contains(i_str[i]))
	    i++;
	if(i<i_len)
	    Result++;
	while(i<i_len && !delim.Contains(i_str[i]))
	    i++;
    }
    return Result;
}

mstring mstring::ExtractWord(unsigned int count, const mstring &delim,
						bool assemble) const
{
    mstring Result;
    int i, begin;
    begin=WordPosition(count, delim, assemble);
    if(begin!=-1)
    {
	i=begin;
	while(i < (int) i_len && !delim.Contains(i_str[(unsigned int) i]))
	    i++;
	if (i!=begin)
	    return SubString(begin, i);
    }
    return Result;
}

int mstring::WordPosition(unsigned int count, const mstring &delim,
						bool assemble) const
{
    unsigned int i=0,cnt=0;
    int Result=-1;
    while(i<i_len && cnt!=count)
    {
	// Skip past multi-seperators IF we assemble them.
	while (i<i_len && assemble && delim.Contains(i_str[i]))
	    i++;
	if(i<i_len)
	    cnt++;
	if(cnt!=count)
	{
	    while(i<i_len && !delim.Contains(i_str[i]))
		i++;
	    i++;
	}
	else
	    Result=i;
    }
    return Result;
}



/********************************************************/

#ifndef HAVE_ITOA
const char *itoa(int i)
{
    mstring str(i);
    return str.c_str();
}
#endif

#ifndef HAVE_LTOA
const char *ltoa(long l)
{
    mstring str(l);
    return str.c_str();
}
#endif

#ifndef HAVE_FTOA
const char *ftoa(float f)
{
    mstring str(f);
    return str.c_str();
}
#endif

#ifndef HAVE_DTOA
const char *dtoa(double d)
{
    mstring str(d);
    return str.c_str();
}
#endif

#ifndef HAVE_ULTOA
const char *ultoa(unsigned long ul)
{
    mstring str(ul);
    return str.c_str();
}
#endif

#ifndef HAVE_UITOA
const char *uitoa(unsigned int ui)
{
    mstring str(ui);
    return str.c_str();
}
#endif

#ifndef HAVE_SNPRINTF
int snprintf(char *buf, size_t size, const char *fmt, ...)
{
    va_list argptr;
    va_start(argptr, fmt);

    int iLen = ACE_OS::sprintf(buf, fmt, argptr);

    va_end(argptr);
    return iLen;
}
#endif

#ifndef HAVE_VSNPRINTF
int vsnprintf(char *buf, size_t size, const char *fmt, va_list ap)
{
    int iLen = ACE_OS::vsprintf(buf, fmt, ap);
    return iLen;
}
#endif

/*  Direct from Magick I, credit to Andy Church for writing this.
 *
 *  match_wild:  Attempt to match a string to a pattern which might contain
 *              '*' or '?' wildcards.  Return 1 if the string matches the
 *              pattern, 0 if not.
 */

bool match_wild (const char *pattern, const char *str, bool docase)
{
    char c;
    const char *s;

    /* This WILL eventually terminate: either by *pattern == 0, or by a
     * trailing '*'. */

    for (;;)
    {
	switch (c = *pattern++)
	{
	case 0:
	    if (!*str)
		return true;
	    return false;
	case '?':
	    if (!*str)
		return false;
	    ++str;
	    break;
	case '*':
	    if (!*pattern)
		return true;	/* trailing '*' matches everything else */
	    s = str;
	    while (*s)
	    {
		if ((docase ? *s == *pattern : (tolower (*s) == tolower (*pattern)))
		    && match_wild (pattern, s, docase))
		    return true;
		++s;
	    }
	    break;
	default:
	    if ((docase ? (*str++ != c) : (tolower (*str++) != tolower (c))))
		return false;
	    break;
	}			/* switch */
    }
}

bool operator== (const mstring &lhs, const mstring &rhs)
	{ return (lhs.compare(rhs) == 0); }
bool operator== (const mstring &lhs, const string &rhs)
	{ return (lhs.compare(rhs) == 0); }
bool operator== (const mstring &lhs, const char *rhs)
	{ return (lhs.compare(rhs) == 0); }
bool operator== (const mstring &lhs, const char rhs)
	{ return (lhs.compare(rhs) == 0); }
bool operator== (const mstring &lhs, const unsigned char rhs)
	{ return (lhs.compare(rhs) == 0); }
bool operator== (const string &lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) == 0); }
bool operator== (const char *lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) == 0); }
bool operator== (const char lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) == 0); }
bool operator== (const unsigned char lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) == 0); }

bool operator!= (const mstring &lhs, const mstring &rhs)
	{ return (lhs.compare(rhs) != 0); }
bool operator!= (const mstring &lhs, const string &rhs)
	{ return (lhs.compare(rhs) != 0); }
bool operator!= (const mstring &lhs, const char *rhs)
	{ return (lhs.compare(rhs) != 0); }
bool operator!= (const mstring &lhs, const char rhs)
	{ return (lhs.compare(rhs) != 0); }
bool operator!= (const mstring &lhs, const unsigned char rhs)
	{ return (lhs.compare(rhs) != 0); }
bool operator!= (const string &lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) != 0); }
bool operator!= (const char *lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) != 0); }
bool operator!= (const char lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) != 0); }
bool operator!= (const unsigned char lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) != 0); }

bool operator< (const mstring &lhs, const mstring &rhs)
	{ return (lhs.compare(rhs) < 0); }
bool operator< (const mstring &lhs, const string &rhs)
	{ return (lhs.compare(rhs) < 0); }
bool operator< (const mstring &lhs, const char *rhs)
	{ return (lhs.compare(rhs) < 0); }
bool operator< (const mstring &lhs, const char rhs)
	{ return (lhs.compare(rhs) < 0); }
bool operator< (const mstring &lhs, const unsigned char rhs)
	{ return (lhs.compare(rhs) < 0); }
bool operator< (const string &lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) > 0); }
bool operator< (const char *lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) > 0); }
bool operator< (const char lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) > 0); }
bool operator< (const unsigned char lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) > 0); }

bool operator> (const mstring &lhs, const mstring &rhs)
	{ return (lhs.compare(rhs) > 0); }
bool operator> (const mstring &lhs, const string &rhs)
	{ return (lhs.compare(rhs) > 0); }
bool operator> (const mstring &lhs, const char *rhs)
	{ return (lhs.compare(rhs) > 0); }
bool operator> (const mstring &lhs, const char rhs)
	{ return (lhs.compare(rhs) > 0); }
bool operator> (const mstring &lhs, const unsigned char rhs)
	{ return (lhs.compare(rhs) > 0); }
bool operator> (const string &lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) < 0); }
bool operator> (const char *lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) < 0); }
bool operator> (const char lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) < 0); }
bool operator> (const unsigned char lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) < 0); }

bool operator<= (const mstring &lhs, const mstring &rhs)
	{ return (lhs.compare(rhs) <= 0); }
bool operator<= (const mstring &lhs, const string &rhs)
	{ return (lhs.compare(rhs) <= 0); }
bool operator<= (const mstring &lhs, const char *rhs)
	{ return (lhs.compare(rhs) <= 0); }
bool operator<= (const mstring &lhs, const char rhs)
	{ return (lhs.compare(rhs) <= 0); }
bool operator<= (const mstring &lhs, const unsigned char rhs)
	{ return (lhs.compare(rhs) <= 0); }
bool operator<= (const string &lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) >= 0); }
bool operator<= (const char *lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) >= 0); }
bool operator<= (const char lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) >= 0); }
bool operator<= (const unsigned char lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) >= 0); }

bool operator>= (const mstring &lhs, const mstring &rhs)
	{ return (lhs.compare(rhs) >= 0); }
bool operator>= (const mstring &lhs, const string &rhs)
	{ return (lhs.compare(rhs) >= 0); }
bool operator>= (const mstring &lhs, const char *rhs)
	{ return (lhs.compare(rhs) >= 0); }
bool operator>= (const mstring &lhs, const char rhs)
	{ return (lhs.compare(rhs) >= 0); }
bool operator>= (const mstring &lhs, const unsigned char rhs)
	{ return (lhs.compare(rhs) >= 0); }
bool operator>= (const string &lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) <= 0); }
bool operator>= (const char *lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) <= 0); }
bool operator>= (const char lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) <= 0); }
bool operator>= (const unsigned char lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) <= 0); }

mstring operator+ (const mstring &lhs, const mstring &rhs)
{
    mstring str(lhs);
    str.append(rhs.c_str(), rhs.length());
    return str;
}

mstring operator+ (const mstring &lhs, const string &rhs)
{
    mstring str(lhs);
    str.append(rhs.c_str(), rhs.length());
    return str;
}

mstring operator+ (const mstring &lhs, const char *rhs)
{
    mstring str(lhs);
    str.append(rhs, strlen(rhs));
    return str;
}

mstring operator+ (const mstring &lhs, const char rhs)
{
    mstring str(lhs);
    str.append(&rhs, 1);
    return str;
}

mstring operator+ (const mstring &lhs, const unsigned char rhs)
{
    mstring str(lhs);
    str.append((const char *) &rhs, 1);
    return str;
}

mstring operator+ (const mstring &lhs, const int rhs)
{
    mstring str(lhs);
    str.append(rhs);
    return str;
}

mstring operator+ (const mstring &lhs, const unsigned int rhs)
{
    mstring str(lhs);
    str.append(rhs);
    return str;
}

mstring operator+ (const mstring &lhs, const long rhs)
{
    mstring str(lhs);
    str.append(rhs);
    return str;
}

mstring operator+ (const mstring &lhs, const unsigned long rhs)
{
    mstring str(lhs);
    str.append(rhs);
    return str;
}

mstring operator+ (const mstring &lhs, const float rhs)
{
    mstring str(lhs);
    str.append(rhs);
    return str;
}

mstring operator+ (const mstring &lhs, const double rhs)
{
    mstring str(lhs);
    str.append(rhs);
    return str;
}

mstring operator+ (const string &lhs, const mstring &rhs)
{
    mstring str(lhs);
    str.append(rhs.c_str(), rhs.length());
    return str;
}

mstring operator+ (const char *lhs, const mstring &rhs)
{
    mstring str(lhs);
    str.append(rhs.c_str(), rhs.length());
    return str;
}

mstring operator+ (const char lhs, const mstring &rhs)
{
    mstring str(lhs);
    str.append(rhs.c_str(), rhs.length());
    return str;
}

mstring operator+ (const unsigned char lhs, const mstring &rhs)
{
    mstring str(lhs);
    str.append(rhs.c_str(), rhs.length());
    return str;
}

mstring operator+ (const int lhs, const mstring &rhs)
{
    mstring str(lhs);
    str.append(rhs.c_str(), rhs.length());
    return str;
}

mstring operator+ (const unsigned int lhs, const mstring &rhs)
{
    mstring str(lhs);
    str.append(rhs.c_str(), rhs.length());
    return str;
}

mstring operator+ (const long lhs, const mstring &rhs)
{
    mstring str(lhs);
    str.append(rhs.c_str(), rhs.length());
    return str;
}

mstring operator+ (const unsigned long lhs, const mstring &rhs)
{
    mstring str(lhs);
    str.append(rhs.c_str(), rhs.length());
    return str;
}

mstring operator+ (const float lhs, const mstring &rhs)
{
    mstring str(lhs);
    str.append(rhs.c_str(), rhs.length());
    return str;
}

mstring operator+ (const double lhs, const mstring &rhs)
{
    mstring str(lhs);
    str.append(rhs.c_str(), rhs.length());
    return str;
}


/********************************************************/

