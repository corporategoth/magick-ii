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
** This code is released under the GNU General Public License, which
** means (in short), it may be distributed freely, and may not be sold
** or used as part of any closed-source product.  Please check the
** COPYING file for full rights and restrictions of this software.
**
** ======================================================================= */
#define RCSID(x,y) const char *rcsid_mstring_cpp_ ## x () { return y; }
RCSID(mstring_cpp, "@(#)$Id$");

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
#include "mstring.h"

/* This class is totally written by Preston A. Elder <prez@magick.tm>.
 * Primerily this was written for Magick IRC Services (and by default,
 * the ACE library).  Read mstring.h for portability notes.
 */

#if ALLOCTYPE == 4
// This defaults to 8192 chunks.  Use init(size) before
// any use of it, or pass size as a 2nd param to the first
// use of it, or use blocksize(size) at any time to change.
MEMORY_AREA mstring::memory_area;
#endif

#ifdef MSTRING_LOCKS_WORK
static unsigned long next_lock_id = 0;

/* static set<unsigned long> lock_id_array; */
#endif

#ifdef WIN32
const mstring DirSlash("\\");
#else
const mstring DirSlash("/");
#endif
const mstring Blank;
const mstring IRC_CTCP(static_cast < char > (1));	// ^A
const mstring IRC_Bold(static_cast < char > (2));	// ^B
const mstring IRC_Underline(static_cast < char > (31));	// ^_
const mstring IRC_Reverse(static_cast < char > (22));	// ^V
const mstring IRC_Color(static_cast < char > (3));	// ^C
const mstring IRC_Off(static_cast < char > (15));	// ^O

#ifdef MAGICK_HAS_EXCEPTIONS
inline char *mstring::alloc(const size_t sz) throw(mstring_noalloc)
#else
inline char *mstring::alloc(const size_t sz)
#endif
{
    char *out = NULL;

#if ALLOCTYPE == 4

    /* Use our own Memory Map for clustered alloc */
    out = (char *) memory_area.alloc(sz);

#elif ALLOCTYPE == 3

    /* Duplicate ACE's new, but with no return's. */
# if defined(MAGICK_HAS_EXCEPTIONS) && \
	defined(ACE_NEW_THROWS_EXCEPTIONS)
    try
    {
	out = new char [sz];
    }
    catch(ACE_bad_alloc & e)
    {
	e.what();
	errno = ENOMEM;
    }
# else
    out = new char [sz];

    if (out == NULL)
    {
	errno = ENOMEM;
    }
# endif

#elif ALLOCTYPE == 2

    /* Standard C++ Allocation */
    out = new char [sz];

#else

    /* Standard C Allocation */
    out = (char *) malloc(sz);

#endif

    if (out == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	char reason[1024];

	ACE_OS::sprintf(reason, "Could not allocate mstring %d: %s", errno, strerror(errno));
	reason[1023] = 0;
	throw mstring_noalloc(reason);
#else
	ACE_OS::fprintf(stderr, "Could not allocate mstring %d: %s", errno, strerror(errno));
	ACE_OS::fflush(stderr);
#endif
    }

    return out;
}

#ifdef MAGICK_HAS_EXCEPTIONS
inline void mstring::dealloc(char *&in) throw(mstring_nodealloc)
#else
inline void mstring::dealloc(char *&in)
#endif
{
    if (in == NULL)
	return;

#if ALLOCTYPE == 4

    /* Use our own Memory Map for clustered alloc */
    memory_area.dealloc(in);

#elif ALLOCTYPE == 3

    /* Duplicate ACE's new, but with no return's. */
    delete[] in;

#elif ALLOCTYPE == 2

    /* Standard C++ Allocation */
    delete[] in;

#else

    /* Standard C Allocation */
    free(in);

#endif

    in = NULL;
}

inline void mstring::lock_read() const
{
#ifdef MSTRING_LOCKS_WORK
    if (i_lock != NULL && i_lock->acquire_read() < 0)
    {
	ACE_OS::fprintf(stderr, "WARNING: Failed to acquire read lock on line %d of %s\n", __LINE__, __FILE__);
	ACE_OS::fflush(stderr);
    }
#endif
}

inline void mstring::lock_write() const
{
#ifdef MSTRING_LOCKS_WORK
    if (i_lock != NULL && i_lock->acquire_write() < 0)
    {
	ACE_OS::fprintf(stderr, "WARNING: Failed to acquire write lock on line %d of %s\n", __LINE__, __FILE__);
	ACE_OS::fflush(stderr);
    }
#endif
}

inline void mstring::lock_rel() const
{
#ifdef MSTRING_LOCKS_WORK
    if (i_lock != NULL && i_lock->release() < 0)
    {
	ACE_OS::fprintf(stderr, "WARNING: Failed to release lock on line %d of %s\n", __LINE__, __FILE__);
	ACE_OS::fflush(stderr);
    }
#endif
}

mstring::~mstring()
{
    lock_write();

    if (i_str != NULL)
	dealloc(i_str);
    i_len = 0;
    i_res = 0;

    lock_rel();

#ifdef MSTRING_LOCKS_WORK
    if (i_lock != NULL)
	delete i_lock;
#endif
}

void mstring::copy(const char *in, const size_t len)
{
    lock_write();

    if (i_str != NULL)
	dealloc(i_str);
    if (len && in)
    {
	i_len = len;
	i_res = sizeof(int);
	while (i_res <= i_len)
	    i_res *= 2;
	i_str = alloc(i_res);
	if (i_str == NULL)
	{
	    lock_rel();
	    return;
	}
	memset(i_str, 0, i_res);
	memcpy(i_str, in, i_len);
    }
    else
    {
	i_str = NULL;
	i_len = 0;
	i_res = 0;
    }

    lock_rel();
}

void mstring::append(const char *in, const size_t len)
{
    if (len == 0 || in == NULL)
	return;

    lock_write();

    if (i_str == NULL)
    {
	lock_rel();
	copy(in, len);
	return;
    }

    size_t oldres = i_res;
    char *tmp = NULL;

    if (i_res == 0)
	i_res = sizeof(int);
    while (i_res <= i_len + len)
	i_res *= 2;
    if (oldres != i_res)
    {
	tmp = alloc(i_res);
	if (tmp == NULL)
	{
	    lock_rel();
	    return;
	}
	memset(tmp, 0, i_res);
	if (i_str != NULL)
	    memcpy(tmp, i_str, i_len);
    }
    else
	tmp = i_str;
    memcpy(&tmp[i_len], in, len);

    if (tmp != i_str)
    {
	if (i_str != NULL)
	    dealloc(i_str);
	i_str = tmp;
    }

    i_len += len;

    lock_rel();
}

void mstring::erase(int begin, int end)
{
    lock_write();

    size_t i, oldres = i_res;
    char *tmp = NULL;

    if (i_str == NULL)
    {
	lock_rel();
	return;
    }

    if (end < 0 || end >= static_cast < int > (i_len))
	if (begin >= static_cast < int > (i_len))
	{
	    lock_rel();
	    return;
	}
	else
	    end = i_len - 1;
    if (begin < 0)
	begin = 0;
    if (begin > end)
    {
	i = begin;
	begin = end;
	end = i;
    }
    end++;

    if (begin > 0 || end < static_cast < int > (i_len))
    {
	i = 0;
	if (i_res == 0)
	    i_res = sizeof(int);
	while (i_res >= sizeof(int) && i_res / 2 > i_len - (end - begin))
	    i_res /= 2;
	if (i_res != oldres)
	{
	    tmp = alloc(i_res);
	    if (tmp == NULL)
	    {
		lock_rel();
		return;
	    }
	    memset(tmp, 0, i_res);
	    if (begin > 0)
	    {
		memcpy(tmp, i_str, begin);
		i = begin;
	    }
	}
	else
	{
	    i = begin;
	    tmp = i_str;
	}

	if (end < static_cast < int > (i_len))
	    memmove(&tmp[i], &i_str[end], i_len - end);
	i_len -= end - begin;
	memset(&tmp[i_len], 0, i_res - i_len);
    }
    if (tmp != i_str)
    {
	if (i_str != NULL)
	    dealloc(i_str);
	i_str = tmp;
	if (i_str == NULL)
	{
	    i_len = 0;
	    i_res = 0;
	}
    }

    lock_rel();
}

void mstring::insert(const size_t pos, const char *in, const size_t len)
{
    size_t i;
    char *tmp = NULL;

    if (len == 0)
	return;

    lock_write();

    if (i_str == NULL || pos >= i_len)
    {
	lock_rel();
	append(in, len);
	return;
    }

    size_t oldres = i_res;

    if (i_res == 0)
	i_res = sizeof(int);
    while (i_res <= i_len + len)
	i_res *= 2;

    if (oldres != i_res)
    {
	tmp = alloc(i_res);
	if (tmp == NULL)
	{
	    lock_rel();
	    return;
	}
	memset(tmp, 0, i_res);
    }
    else
	tmp = i_str;

    i = 0;
    if (pos > 0 && tmp != i_str)
	memcpy(tmp, i_str, pos);
    i += pos;
    memmove(&tmp[i + len], &i_str[pos], i_len - pos);
    memcpy(&tmp[i], in, len);

    if (i_str != tmp)
    {
	dealloc(i_str);
	i_str = tmp;
    }
    i_len += len;

    lock_rel();
}

// We compare up to a length they both have.  If its the
// the same, same or one is zero length, we return -1 if
// we are smaller, and 1 if they are smaller.  Otherwise
// we will return 0 (meaning exact match).
int mstring::compare(const char *in, const size_t len) const
{
    lock_read();

    int retval = 0;
    size_t shortest = (i_len < len ? i_len : len);

    if (shortest != 0)
	retval = memcmp(i_str, in, shortest);

    if (retval == 0)
    {
	if (i_len < len)
	    retval = -1;
	else if (len < i_len)
	    retval = 1;
    }

//    retval = strcmp((i_len ? i_str : ""), (len ? in : ""));

    lock_rel();

    return retval;
}

void mstring::swap(mstring & in)
{
    lock_write();
    in.lock_write();

    char *str = i_str;
    size_t len = i_len;
    size_t res = i_res;

    i_str = in.i_str;
    i_len = in.i_len;
    i_res = in.i_res;

    in.i_str = str;
    in.i_len = len;
    in.i_res = res;

    in.lock_rel();
    lock_rel();
}

const char *mstring::c_str() const
{
    const char *retval = "";

    lock_read();
    if (i_str != NULL)
	retval = i_str;
    lock_rel();

    return static_cast < const char * > (retval);
}

const unsigned char *mstring::uc_str() const
{
    unsigned char *retval = reinterpret_cast < unsigned char * > (const_cast < char * > (""));

    lock_read();
    if (i_str != NULL)
	retval = reinterpret_cast < unsigned char * > (i_str);

    lock_rel();

    return static_cast < const unsigned char * > (retval);
}

const char mstring::first(size_t off) const
{
    char retval = 0;

    lock_read();
    if (i_str != NULL && off > 0 && off <= i_len)
	retval = i_str[off - 1];
    lock_rel();

    return static_cast < const char > (retval);
}

const char mstring::last(size_t off) const
{
    char retval = 0;

    lock_read();
    if (i_str != NULL && off > 0 && off <= i_len)
	retval = i_str[i_len - off];
    lock_rel();

    return static_cast < const char > (retval);
}

size_t mstring::length() const
{
    lock_read();
    size_t retval = i_len;

    lock_rel();
    return retval;
}

size_t mstring::size() const
{
    lock_read();
    size_t retval = i_len;

    lock_rel();
    return retval;
}

size_t mstring::capacity() const
{
    lock_read();
    size_t retval = i_res;

    lock_rel();
    return retval;
}

bool mstring::empty() const
{
    lock_read();
    bool retval = (i_str == NULL);

    lock_rel();
    return retval;
}

// Index value of any of these chars
int mstring::find_first_of(const char *str, const size_t len) const
{
    lock_read();

    unsigned int i;
    int retval = i_len + 1;

    if (i_str == NULL || str == NULL || len == 0)
    {
	lock_rel();
	return -1;
    }

    for (i = 0; i < len; i++)
    {
	char *ptr = static_cast < char * > (memchr(i_str, str[i], i_len));

	if (ptr != NULL && ptr - i_str < retval)
	    retval = ptr - i_str;
    }
    if (retval > static_cast < int > (i_len))
	retval = -1;

    lock_rel();
    return retval;
}

// Reverse Index value of any of these chars
int mstring::find_last_of(const char *str, const size_t len) const
{
    unsigned int i;
    int retval = -1;

    lock_read();
    if (i_str == NULL || str == NULL || len == 0)
    {
	lock_rel();
	return -1;
    }

    for (i = 0; i < len; i++)
    {
	char *ptr = strrchr(i_str, str[i]);

	if (ptr != NULL && ptr - i_str > retval)
	    retval = ptr - i_str;
    }

    lock_rel();
    return retval;
}

// Opposite to index for any of these chars
int mstring::find_first_not_of(const char *str, const size_t len) const
{
    unsigned int i;
    int retval = -1;

    lock_read();
    if (i_str == NULL || str == NULL || len == 0)
    {
	lock_rel();
	return -1;
    }

    char *tmp;

    tmp = alloc(len + 1);
    if (tmp == NULL)
    {
	lock_rel();
	return -1;
    }
    memcpy(tmp, str, len);
    tmp[len] = 0;

    for (i = 0; i < i_len; i++)
    {
	if (memchr(tmp, i_str[i], len) == NULL)
	{
	    retval = i;
	    break;
	}
    }
    dealloc(tmp);

    lock_rel();
    return retval;
}

// Opposite to rindex for any of these chars
int mstring::find_last_not_of(const char *str, const size_t len) const
{
    int i, retval = -1;

    lock_read();
    if (i_str == NULL || str == NULL || len == 0)
    {
	lock_rel();
	return -1;
    }

    char *tmp;

    tmp = alloc(len + 1);
    if (tmp == NULL)
    {
	lock_rel();
	return -1;
    }
    memcpy(tmp, str, len);
    tmp[len] = 0;

    for (i = i_len - 1; i >= 0; i--)
    {
	if (memchr(tmp, i_str[i], len) == NULL)
	{
	    retval = i;
	    break;
	}
    }
    dealloc(tmp);

    lock_rel();
    return retval;
}

/* PRIVATE METHOD - NO LOCKING! */
int mstring::occurances(const char *str, const size_t len) const
{
    size_t consumed = 0, count = 0;
    char *start, *end;

    if (i_str == NULL || str == NULL || len < 1)
	return 0;

    start = i_str;
    while (consumed < i_len)
    {
	// Find each instance of the search pattern, at least, up until
	// the first binary 0 we encounter ...
	end = strstr(start, str);
	while (end != NULL)
	{
	    count++;
	    consumed += (end - start) + len;
	    start = end + len;
	    end = strstr(start, str);
	}
	// Accomodate for any binary 0's, by moving past the next binary
	// zero, checking to see if we've used the entire contents, and
	// if we havn't, resuming the above while.
	consumed += strlen(start) + 1;
	start += strlen(start) + 1;
    }
    return count;
}

// Find occurance of full string
int mstring::find(const mstring & str, int occurance) const
{
    int count = 0, retval = -1;
    char *start, *end = NULL;

    if (str.empty())
	return -1;

    lock_read();
    if (i_str == NULL)
    {
	lock_rel();
	return -1;
    }

    if (occurance < 1)
	occurance = 1;

    size_t consumed = 0;

    start = i_str;
    // While we're not at the end of the WHOLE string ...
    while (consumed < i_len)
    {
	// Find each instance of the search pattern, at least, up until
	// the first binary 0 we encounter ... Break when we got what
	// we want.
	end = strstr(start, str.c_str());
	while (end != NULL)
	{
	    count++;
	    if (occurance == count)
		break;
	    consumed += (end - start) + str.length();
	    start = end + str.length();
	    end = strstr(start, str.c_str());
	}
	// if we're done, break ...
	if (end != NULL)
	    break;
	// Accomodate for any binary 0's, by moving past the next binary
	// zero, checking to see if we've used the entire contents, and
	// if we havn't, resuming the above while.
	consumed += strlen(start) + 1;
	start += strlen(start) + 1;
    }
    if (end != NULL)
	retval = end - i_str;

    lock_rel();
    return retval;
}

// Reverse Find occurance of full string
int mstring::rfind(const mstring & str, int occurance) const
{
    int occ, retval = -1;

    if (str.empty())
	return -1;

    if (occurance < 1)
	occurance = 1;

    lock_read();
    occ = occurances(str.c_str(), str.length());
    lock_rel();
    if (occurance <= occ)
	retval = find(str.c_str(), occ - occurance + 1);

    return retval;
}

// Replace find string with replace string (optionally for all)
void mstring::replace(const mstring & i_find, const mstring & i_replace, const bool all)
{
    size_t old_len, amt_replace = 0;
    char *tmp, *start, *end;

    if (i_find.empty())
	return;

    lock_write();
    if (i_str == NULL)
    {
	lock_rel();
	return;
    }
    old_len = i_len;

    amt_replace = occurances(i_find.c_str(), i_find.length());
    if (amt_replace < 1)
	return;

    i_len += (amt_replace * (i_replace.length() - i_find.length()));

    if (i_len <= 0)
    {
	i_res = 0;
	dealloc(i_str);
	lock_rel();
	return;
    }

    if (i_res == 0)
	i_res = sizeof(int);
    while (i_res <= i_len)
	i_res *= 2;
    while (i_res >= sizeof(int) && i_res / 2 > i_len)
	i_res /= 2;
    tmp = alloc(i_res);
    if (tmp == NULL)
    {
	lock_rel();
	return;
    }
    memset(tmp, 0, i_res);

    start = i_str;
    size_t i = 0, consumed = 0;

    while (consumed < old_len)
    {
	// Find each instance of the search pattern, at least, up until
	// the first binary 0 we encounter ... Break after the first
	// replace if !all ...
	end = strstr(start, i_find.c_str());
	while (end != NULL)
	{
	    if ((end - start) > 0)
	    {
		memcpy(&tmp[i], start, (end - start));
		i += (end - start);
	    }
	    if (i_replace.length())
	    {
		memcpy(&tmp[i], i_replace.c_str(), i_replace.length());
		i += i_replace.length();
	    }
	    consumed += (end - start) + i_find.length();
	    start = end + i_find.length();
	    if (!all)
		break;
	    end = strstr(start, i_find.c_str());
	}
	// Accomodate the situation where !all, simply copy the rest
	// of the REAL string, and be done with it.
	if (end != NULL)
	{
	    if (old_len - (start - i_str) > 0)
		memcpy(&tmp[i], start, old_len - (start - i_str));
	    break;
	}
	// Accomodate for any binary 0's, by moving past the next binary
	// zero, checking to see if we've used the entire contents, and
	// if we havn't, resuming the above while.
	consumed += strlen(start) + 1;
	memcpy(&tmp[i], start, strlen(start) + 1);
	i += strlen(start);
	start += strlen(start) + 1;
    }
    dealloc(i_str);
    i_str = tmp;
    lock_rel();
}

bool mstring::replace(const size_t offs, const char c)
{
    bool retval = false;

    lock_write();
    if (i_str != NULL && offs < i_len)
    {
	i_str[offs] = c;
	retval = true;
    }
    lock_rel();
    return retval;
}

mstring mstring::substr(int nFirst, int nCount) const
{
    mstring retval;

    lock_read();
    if (i_str != NULL)
    {
	if (nFirst < 0)
	    nFirst = 0;
	if (nCount < 0)
	    nCount = i_len - nFirst;
	if ((nCount + nFirst) > static_cast < int > (i_len))
	    nCount = i_len - nFirst;
	lock_rel();
	if (nCount > 0)
	    retval.copy(&i_str[nFirst], nCount);
    }
    else
    {
	lock_rel();
    }

    return retval;
}

/********************************************************/

bool mstring::IsWord() const
{
    bool retval = true;

    lock_read();

    if (i_str == NULL)
    {
	lock_rel();
	return false;
    }

    for (size_t i = 0; i < i_len; i++)
	if (!isalpha(i_str[i]))
	    retval = false;

    lock_rel();
    return retval;
}

bool mstring::IsNumber() const
{
    bool retval = false;

    lock_read();

    if (i_str == NULL)
    {
	lock_rel();
	return false;
    }

    // Accepts: [-|+]###.###  Must have at least one #
    // Accepts multipal decimal places, so an IP address
    // shows up as a number (easy way to say 'IsIp').

    /* Required this way else will coredump on a blank
     * string (ie. accessing i_str[i] of NULL) */
    size_t i = 0;

    if (i_str[i] == '-' || i_str[i] == '+')
	i++;

    bool gotdec = false;

    for (; i < i_len; i++)
    {
	if (i_str[i] == '.' && !gotdec)
	{
	    gotdec = true;
	}
	else if (isdigit(i_str[i]))
	{
	    retval = true;
	}
	else
	{
	    retval = false;
	    break;
	}
    }

    lock_rel();
    return retval;
}

bool mstring::IsAscii() const
{
    bool retval = true;

    lock_read();

    if (i_str == NULL)
	retval = false;

    for (size_t i = 0; i < i_len; i++)
	if (!isascii(i_str[i]))
	    retval = false;

    lock_rel();
    return retval;
}

bool mstring::IsBool() const
{
    if (IsSameAs("true", true) || IsSameAs("on", true) || IsSameAs("yes", true) || IsSameAs("y", true) || IsSameAs("t", true)
	|| IsSameAs("1") || IsSameAs("false", true) || IsSameAs("off", true) || IsSameAs("no", true) || IsSameAs("n", true) ||
	IsSameAs("f", true) || IsSameAs("0"))
	return true;
    else
	return false;
}

bool mstring::GetBool() const
{
    if (IsSameAs("true", true) || IsSameAs("on", true) || IsSameAs("yes", true) || IsSameAs("y", true) || IsSameAs("t", true)
	|| IsSameAs("1"))
	return true;
    else
	return false;
}

mstring mstring::UpperCase() const
{
    lock_read();
    mstring tmp(*this);

    lock_rel();
    tmp.MakeUpper();
    return tmp;
}

mstring mstring::LowerCase() const
{
    lock_read();
    mstring tmp(*this);

    lock_rel();
    tmp.MakeLower();
    return tmp;
}

void mstring::MakeUpper()
{
    lock_write();
    if (i_str == NULL)
    {
	lock_rel();
	return;
    }

    for (size_t i = 0; i < i_len; i++)
	i_str[i] = toupper(i_str[i]);
    lock_rel();
}

void mstring::MakeLower()
{
    lock_write();
    if (i_str == NULL)
    {
	lock_rel();
	return;
    }

    for (size_t i = 0; i < i_len; i++)
	i_str[i] = tolower(i_str[i]);
    lock_rel();
}

int mstring::Occurances(const mstring & in, const bool NoCase) const
{
    int retval = 0;

    if (NoCase)
    {
	/* Not worried here, even though we call occurances,
	 * its through an UpperCase, so using a temp string
	 * created by the system (and out of our control).
	 */
	mstring tmp(in);

	retval = UpperCase().occurances(tmp.UpperCase().c_str(), tmp.length());
    }
    else
    {
	lock_read();
	retval = occurances(in.c_str(), in.length());
	lock_rel();
    }
    return retval;
}

int mstring::Find(const mstring & in, const bool NoCase, const int occurance) const
{
    if (NoCase)
    {
	mstring tmp(in);

	return UpperCase().find(tmp.UpperCase().c_str(), occurance);
    }
    else
	return find(in.c_str(), occurance);
}

int mstring::RevFind(const mstring & in, const bool NoCase, const int occurance) const
{
    if (NoCase)
    {
	mstring tmp(in);

	return UpperCase().rfind(tmp.UpperCase().c_str(), occurance);
    }
    else
	return rfind(in.c_str(), occurance);
}

int mstring::Cmp(const mstring & in, const bool NoCase) const
{
    int retval = 0;

    if (NoCase)
    {
	mstring tmp(in);

	retval = UpperCase().compare(tmp.UpperCase());
    }
    else
	retval = compare(in);
    return retval;
}

void mstring::Trim(const bool right, const mstring & delims)
{
    int pos = 0;

    if (right)
    {
	pos = find_last_not_of(delims.c_str());
	if (pos > 0)
	    erase(pos + 1);
    }
    else
    {
	pos = find_first_not_of(delims.c_str());
	if (pos > 0)
	    erase(0, pos - 1);
    }
}

mstring mstring::Strip(const bool right, const mstring & deilms) const
{
    lock_read();
    mstring tmp(*this);

    lock_rel();
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
    int len = -1, sz = sizeof(int);
    char *buffer;

    while (1)
    {
	buffer = alloc(sz);
	if (buffer == NULL)
	    return -1;

	memset(buffer, 0, sz);
	len = vsnprintf(buffer, sz, fmt, argptr);
	if (buffer[sz - 1] == 0 && len >= 0 && len < sz)
	{
	    if (len == 0)
	    {
		dealloc(buffer);
		buffer = NULL;
	    }
	    break;
	}

	dealloc(buffer);
	buffer = NULL;
	// Must happen at least once ...
	do
	{
	    sz *= 2;
	} while (len >= sz);
    }
    if (buffer)
    {
	copy(buffer, len);
	dealloc(buffer);
	buffer = NULL;
    }
    else
	copy(NULL, 0);
    return len;
}

mstring mstring::Before(const mstring & in, const int occurance) const
{
    int m_pos = Find(in, false, occurance);

    if (m_pos >= 0)
	return Left(m_pos);
    else
    {
	lock_read();
	mstring retval(*this);

	lock_rel();
	return retval;
    }
}

mstring mstring::After(const mstring & in, const int occurance) const
{
    int m_pos = Find(in, false, occurance);

    if (m_pos >= 0)
	return Right(m_pos + in.i_len);
    else
    {
	lock_read();
	mstring retval(*this);

	lock_rel();
	return retval;
    }
}

mstring mstring::RevBefore(const mstring & in, const int occurance) const
{
    int m_pos = RevFind(in, false, occurance);

    if (m_pos >= 0)
	return Left(m_pos);
    else
    {
	lock_read();
	mstring retval(*this);

	lock_rel();
	return retval;
    }
}

mstring mstring::RevAfter(const mstring & in, const int occurance) const
{
    int m_pos = RevFind(in, false, occurance);

    if (m_pos >= 0)
	return Right(m_pos + in.i_len);
    else
    {
	lock_read();
	mstring retval(*this);

	lock_rel();
	return retval;
    }
}

mstring mstring::SubString(int from, int to) const
{
    if (to < 0)
    {
	lock_read();
	if (from < static_cast < int > (i_len))
	    to = i_len - 1;
	else
	{
	    lock_rel();
	    return "";
	}
	lock_rel();
    }
    if (to < from)
    {
	int i = from;

	from = to;
	to = i;
    }
    return substr(from, to - from + 1);
}

unsigned int mstring::WordCount(const mstring & delim, const bool assemble) const
{
    int Result = 0;
    size_t i = 0;

    lock_read();
    while (i < i_len)
    {
	while (i < i_len && assemble && delim.Contains(i_str[i]))
	    i++;
	if (i < i_len)
	    Result++;
	while (i < i_len && !delim.Contains(i_str[i]))
	    i++;
    }
    lock_rel();
    return Result;
}

mstring mstring::ExtractWord(const unsigned int count, const mstring & delim, const bool assemble) const
{
    int i, begin;

    begin = WordPosition(count, delim, assemble);
    if (begin != -1)
    {
	i = begin;
	lock_read();
	while (i < static_cast < int > (i_len) && !delim.Contains(i_str[static_cast < unsigned int > (i)]))
	    i++;

	if (i != begin)
	{
	    lock_rel();
	    return SubString(begin, i - 1);
	}
	lock_rel();
    }
    return "";
}

int mstring::WordPosition(const unsigned int count, const mstring & delim, const bool assemble) const
{
    unsigned int i = 0, cnt = 0;
    int Result = -1;

    lock_read();
    while (i < i_len && cnt != count)
    {
	// Skip past multi-seperators IF we assemble them.
	while (i < i_len && assemble && delim.Contains(i_str[i]))
	    i++;
	if (i < i_len)
	    cnt++;
	if (cnt != count)
	{
	    while (i < i_len && !delim.Contains(i_str[i]))
		i++;
	    i++;
	}
	else
	    Result = i;
    }
    lock_rel();
    return Result;
}

vector < mstring > mstring::Vector(const mstring & delim, const bool assemble) const
{
    vector < mstring > Result;
    size_t begin = 0, i = 0;

    lock_read();
    while (i < i_len)
    {
	while (i < i_len && assemble && delim.Contains(i_str[i]))
	    i++;
	if (i < i_len)
	    begin = i;
	while (i < i_len && !delim.Contains(i_str[i]))
	    i++;
	if (i != begin)
	    Result.push_back(SubString(begin, i - 1));
    }
    lock_rel();
    return Result;
}

list < mstring > mstring::List(const mstring & delim, bool const assemble) const
{
    list < mstring > Result;
    size_t begin = 0, i = 0;

    lock_read();
    while (i < i_len)
    {
	while (i < i_len && assemble && delim.Contains(i_str[i]))
	    i++;
	if (i < i_len)
	    begin = i;
	while (i < i_len && !delim.Contains(i_str[i]))
	    i++;
	if (i != begin)
	    Result.push_back(SubString(begin, i - 1));
    }
    lock_rel();
    return Result;
}

void mstring::Assemble(const vector < mstring > & text, const mstring & delim)
{
    lock_write();

    if (i_str != NULL)
	dealloc(i_str);

    if (text.size())
    {
	vector < mstring >::const_iterator iter;
	i_len += text.size() - 1 * delim.length();
	for (iter = text.begin(); iter != text.end(); iter++)
	    i_len += iter->length();

	i_res = sizeof(int);
	while (i_res <= i_len)
	    i_res *= 2;
	i_str = alloc(i_res);
	if (i_str == NULL)
	{
	    lock_rel();
	    return;
	}

	memset(i_str, 0, i_res);

	size_t offs = 0;

	iter = text.begin();
	memcpy(i_str, iter->c_str(), iter->length());
	offs += iter->length();
	iter++;
	while (iter != text.end())
	{
	    memcpy(&i_str[offs], delim.c_str(), delim.length());
	    offs += delim.length();
	    memcpy(&i_str[offs], iter->c_str(), iter->length());
	    offs += iter->length();
	    iter++;
	}
    }
    else
    {
	i_str = NULL;
	i_len = 0;
	i_res = 0;
    }

    lock_rel();
}

void mstring::Assemble(const list < mstring > & text, const mstring & delim)
{
    lock_write();

    if (i_str != NULL)
	dealloc(i_str);

    if (text.size())
    {
	list < mstring >::const_iterator iter;
	i_len += text.size() - 1 * delim.length();
	for (iter = text.begin(); iter != text.end(); iter++)
	    i_len += iter->length();

	i_res = sizeof(int);
	while (i_res <= i_len)
	    i_res *= 2;
	i_str = alloc(i_res);
	if (i_str == NULL)
	{
	    lock_rel();
	    return;
	}

	memset(i_str, 0, i_res);

	size_t offs = 0;

	iter = text.begin();
	memcpy(i_str, iter->c_str(), iter->length());
	offs += iter->length();
	iter++;
	while (iter != text.end())
	{
	    memcpy(&i_str[offs], delim.c_str(), delim.length());
	    offs += delim.length();
	    memcpy(&i_str[offs], iter->c_str(), iter->length());
	    offs += iter->length();
	    iter++;
	}
    }
    else
    {
	i_str = NULL;
	i_len = 0;
	i_res = 0;
    }

    lock_rel();
}

/********************************************************/

/*  Direct from Magick I, credit to Andy Church for writing this.
 *
 *  match_wild:  Attempt to match a string to a pattern which might contain
 *              '*' or '?' wildcards.  Return 1 if the string matches the
 *              pattern, 0 if not.
 */

bool match_wild(const char *pattern, const char *str, bool nocase)
{
    char c;
    const char *s;

    /* This WILL eventually terminate: either by *pattern == 0, or by a
     * trailing '*'. */

    for (;;)
    {
	switch (c = * pattern++)
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
		if ((nocase ? (tolower(*s) == tolower(*pattern)) : *s == * pattern) && match_wild(pattern, s, nocase))
		    return true;
		++s;
	    }
	    break;
	default:
	    if ((nocase ? (tolower(*str++) != tolower(c)) : (*str++ != c)))
		return false;
	    break;
	}			/* switch */
    }
}

mstring fmstring(const char *fmt, ...)
{
    mstring tmp;
    va_list argptr;

    va_start(argptr, fmt);
    tmp.FormatV(fmt, argptr);
    va_end(argptr);
    return tmp;
}

/********************************************************/
