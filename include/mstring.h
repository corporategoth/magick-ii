#ifndef WIN32
#pragma interface
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
#ifndef _MSTRING_H
#define _MSTRING_H
#include "pch.h"
static const char *ident_mstring_h = "@(#) $Id$";
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.60  2000/12/12 02:52:14  prez
** Moved the do_nothing function
**
** Revision 1.59  2000/12/09 20:16:41  prez
** Fixed SubString and Left to have correct count/end possitions.  Also
** adjusted rest of source to follow suit.
**
** Revision 1.58  2000/12/09 16:45:37  prez
** Fixed rfind in mstring.
**
** Revision 1.57  2000/12/09 15:40:13  prez
** Fixed some stuff with mstring (ie. Contains called find_first_of
** not find, and the makeupper/makelower calls had != NULL not == NULL).
**
** Revision 1.56  2000/12/09 12:53:56  prez
** Forgot to add the 'return' to all find_first_of, find_last_of,
** find_first_not_of and find_last_not_of calls ...
**
** Revision 1.55  2000/12/09 10:33:47  prez
** Added ACE style alloc as an option.
**
** Revision 1.54  2000/12/05 07:48:26  prez
** *** empty log message ***
**
** Revision 1.53  2000/10/15 03:29:27  prez
** Mods to the memory system, LOTS of printf's to try and figure out why
** the damn thing coredumps on init.
**
** Revision 1.52  2000/10/14 04:25:31  prez
** Added mmemory.h -- MemCluster and the MemoryManager are now in it.
** TODO - make mstring use MemoryManager.
**
** Revision 1.51  2000/10/11 08:38:10  prez
** Changes so mstring works better ... kinda
**
** Revision 1.50  2000/10/10 11:47:50  prez
** mstring is re-written totally ... find or occurances
** or something has a problem, but we can debug that :)
**
** Revision 1.49  2000/10/07 11:00:11  ungod
** no message
**
** Revision 1.48  2000/09/30 10:48:06  prez
** Some general code cleanups ... got rid of warnings, etc.
**
** Revision 1.47  2000/08/03 13:06:29  prez
** Fixed a bunch of stuff in mstring (caused exceptions on FreeBSD machines).
**
** Revision 1.46  2000/07/29 21:58:52  prez
** Fixed XML loading of weird characters ...
** 2 known bugs now, 1) last_seen dates are loaded incorrectly on alot
** of nicknames, which means we expire lots of nicknames.  2) services
** wont rejoin a +i/+k channel when last user exits.
**
** Revision 1.45  2000/07/28 14:49:34  prez
** Ditched the old wx stuff, mconfig now in use, we're now ready to
** release (only got some conversion tests to do).
**
** Revision 1.44  2000/05/28 05:05:13  prez
** More makefile stuff ... Now we should work on all platforms.
** Added alot of checking for different .h files, functions, etc.
** So now all #define's are config.h based (also added a default
** windows config.h, which will need to be copied on these systems).
**
** Revision 1.43  2000/05/17 14:08:11  prez
** More tweaking with DCC, and getting iostream mods working ...
**
** Revision 1.42  2000/02/27 03:58:39  prez
** Fixed the WHAT program, also removed RegEx from Magick.
**
** Revision 1.41  2000/02/23 12:21:02  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.40  2000/02/17 12:55:03  ungod
** still working on borlandization
**
** Revision 1.39  2000/02/15 10:37:47  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

/* Memory Allocation Scheme
 *
 * 1. C: malloc/free
 * 2. C++: new/delete
 * 3. ACE: new/delete with try/catch
 * 4. CLUSTER: memory_area.alloc/memory_area.dealloc
 *
 */
#define ALLOCTYPE	3

#if ALLOCTYPE == 4

/* Use our own Memory Map for clustered alloc */
#include "mmemory.h"
#define ALLOC(X, Y)	X = (char *) memory_area.alloc(Y)
#define DEALLOC(X)	memory_area.dealloc(X)

#elif ALLOCTYPE == 3

/* Duplicate ACE's new, but with no return's.
#ifdef MAGICK_HAS_EXCEPTIONS
#define ALLOC(X, Y)	try { X = new char[Y]; } \
			catch (ACE_bad_alloc) { errno = ENOMEM; }
#else
#define ALLOC(X, Y)	X = new char[Y]; \
			if (X == NULL) { errno = ENOMEM; }
#endif
#define DEALLOC(X)	delete [] X

#elif ALLOCTYPE == 2

/* Standard C++ Allocation */
#define ALLOC(X, Y)	X = new char[Y]
#define DEALLOC(X)	delete [] X

#else

/* Standard C Allocation */
#define ALLOC(X, Y)	X = (char *) malloc(Y)
#define DEALLOC(X)	free(X)

#endif


/* This would have to be the most interoperable class
 * in the existance of C++ ;P */

class mstring;

inline int do_nothing() { return 1; }

#ifndef HAVE_ITOA
const char *itoa(int i);
#endif
#ifndef HAVE_LTOA
const char *ltoa(long l);
#endif
#ifndef HAVE_FTOA
const char *ftoa(float f);
#endif
#ifndef HAVE_DTOA
const char *dtoa(double d);
#endif
#ifndef HAVE_ULTOA
const char *ultoa(unsigned long ul);
#endif
#ifndef HAVE_UITOA
const char *uitoa(unsigned int ui);
#endif
#ifndef HAVE_SNPRINTF
int snprintf(char *buf, size_t size, const char *fmt, ...);
#endif
#ifndef HAVE_VSNPRINTF
int vsnprintf(char *buf, size_t size, const char *fmt, va_list ap);
#endif

extern const mstring DirSlash;
extern const mstring Blank;
extern const mstring IRC_Bold;
extern const mstring IRC_Underline;
extern const mstring IRC_Reverse;
extern const mstring IRC_Colour;
extern const mstring IRC_Off;

bool match_wild (const char *pattern, const char *str, bool docase);
bool operator== (const mstring &lhs, const mstring &rhs);
bool operator== (const mstring &lhs, const string &rhs);
bool operator== (const mstring &lhs, const char *rhs);
bool operator== (const mstring &lhs, const char rhs);
bool operator== (const mstring &lhs, const unsigned char rhs);
bool operator== (const string &lhs, const mstring &rhs);
bool operator== (const char *lhs, const mstring &rhs);
bool operator== (const char lhs, const mstring &rhs);
bool operator== (const unsigned char lhs, const mstring &rhs);

bool operator!= (const mstring &lhs, const mstring &rhs);
bool operator!= (const mstring &lhs, const string &rhs);
bool operator!= (const mstring &lhs, const char *rhs);
bool operator!= (const mstring &lhs, const char rhs);
bool operator!= (const mstring &lhs, const unsigned char rhs);
bool operator!= (const string &lhs, const mstring &rhs);
bool operator!= (const char *lhs, const mstring &rhs);
bool operator!= (const char lhs, const mstring &rhs);
bool operator!= (const unsigned char lhs, const mstring &rhs);

bool operator< (const mstring &lhs, const mstring &rhs);
bool operator< (const mstring &lhs, const string &rhs);
bool operator< (const mstring &lhs, const char *rhs);
bool operator< (const mstring &lhs, const char rhs);
bool operator< (const mstring &lhs, const unsigned char rhs);
bool operator< (const string &lhs, const mstring &rhs);
bool operator< (const char *lhs, const mstring &rhs);
bool operator< (const char lhs, const mstring &rhs);
bool operator< (const unsigned char lhs, const mstring &rhs);

bool operator> (const mstring &lhs, const mstring &rhs);
bool operator> (const mstring &lhs, const string &rhs);
bool operator> (const mstring &lhs, const char *rhs);
bool operator> (const mstring &lhs, const char rhs);
bool operator> (const mstring &lhs, const unsigned char rhs);
bool operator> (const string &lhs, const mstring &rhs);
bool operator> (const char *lhs, const mstring &rhs);
bool operator> (const char lhs, const mstring &rhs);
bool operator> (const unsigned char lhs, const mstring &rhs);

bool operator<= (const mstring &lhs, const mstring &rhs);
bool operator<= (const mstring &lhs, const string &rhs);
bool operator<= (const mstring &lhs, const char *rhs);
bool operator<= (const mstring &lhs, const char rhs);
bool operator<= (const mstring &lhs, const unsigned char rhs);
bool operator<= (const string &lhs, const mstring &rhs);
bool operator<= (const char *lhs, const mstring &rhs);
bool operator<= (const char lhs, const mstring &rhs);
bool operator<= (const unsigned char lhs, const mstring &rhs);

bool operator>= (const mstring &lhs, const mstring &rhs);
bool operator>= (const mstring &lhs, const string &rhs);
bool operator>= (const mstring &lhs, const char *rhs);
bool operator>= (const mstring &lhs, const char rhs);
bool operator>= (const mstring &lhs, const unsigned char rhs);
bool operator>= (const string &lhs, const mstring &rhs);
bool operator>= (const char *lhs, const mstring &rhs);
bool operator>= (const char lhs, const mstring &rhs);
bool operator>= (const unsigned char lhs, const mstring &rhs);

mstring operator+ (const mstring &lhs, const mstring &rhs);
mstring operator+ (const mstring &lhs, const string &rhs);
mstring operator+ (const mstring &lhs, const char *rhs);
mstring operator+ (const mstring &lhs, const char rhs);
mstring operator+ (const mstring &lhs, const unsigned char rhs);
mstring operator+ (const mstring &lhs, const int rhs);
mstring operator+ (const mstring &lhs, const unsigned int rhs);
mstring operator+ (const mstring &lhs, const long rhs);
mstring operator+ (const mstring &lhs, const unsigned long rhs);
mstring operator+ (const mstring &lhs, const float rhs);
mstring operator+ (const mstring &lhs, const double rhs);
mstring operator+ (const string &lhs, const mstring &rhs);
mstring operator+ (const char *lhs, const mstring &rhs);
mstring operator+ (const char lhs, const mstring &rhs);
mstring operator+ (const unsigned char lhs, const mstring &rhs);
mstring operator+ (const int lhs, const mstring &rhs);
mstring operator+ (const unsigned int lhs, const mstring &rhs);
mstring operator+ (const long lhs, const mstring &rhs);
mstring operator+ (const unsigned long lhs, const mstring &rhs);
mstring operator+ (const float lhs, const mstring &rhs);
mstring operator+ (const double lhs, const mstring &rhs);

class mstring
{
    char *i_str;
    size_t i_len, i_res;
#if ALLOCTYPE == 4
    static MemoryManager<ACE_Thread_Mutex> memory_area;
#endif

public:
    mstring()
	{ i_str = NULL; i_len = 0, i_res = 0; }
    mstring(const mstring &in)
	{ i_str = NULL; copy(in); }
    mstring(const string &in)
	{ i_str = NULL; copy(in); }
    mstring(const char *in, size_t length)
	{ i_str = NULL; copy(in, length); }
    mstring(const char *in)
	{ i_str = NULL; copy(in); }
    mstring(const char in)
	{ i_str = NULL; copy(in); }
    mstring(const unsigned char in)
	{ i_str = NULL; copy(in); }
    mstring(const int in)
	{ i_str = NULL; copy(in); }
    mstring(const unsigned int in)
	{ i_str = NULL; copy(in); }
    mstring(const long in)
	{ i_str = NULL; copy(in); }
    mstring(const unsigned long in)
	{ i_str = NULL; copy(in); }
    mstring(const float in)
	{ i_str = NULL; copy(in); }
    mstring(const double in)
	{ i_str = NULL; copy(in); }
    ~mstring()
	{ if (i_str != NULL) DEALLOC(i_str); }

    void copy(const char *in, size_t length);
    void append(const char *in, size_t length);
    void erase(int begin = 0, int end = -1);
    void insert(size_t pos, const char *in, size_t length);
    int compare(const char *in, size_t length) const;
    void swap(mstring &in);
    const char *c_str() const;
    char first() const;
    char last() const;

    size_t length() const
	{ return i_len; }
    size_t size() const
	{ return i_len; }
    size_t capacity() const
	{ return i_res; }
    bool empty() const
	{ return (i_str == NULL); }

    // Aliases for the above ...
    void copy(const mstring &in)
	{ copy(in.i_str, in.i_len); }
    void copy(const string &in)
	{ copy(in.c_str(), in.length()); }
    void copy(const char *in)
	{ copy(in, strlen(in)); }
    void copy(const char in)
	{ copy(&in, 1); }
    void copy(const unsigned char in)
	{ copy((const char *) &in, 1); }
    void copy(const int in)
	{ mstring out; out.Format("%d", in); copy(out); }
    void copy(const unsigned int in)
	{ mstring out; out.Format("%u", in); copy(out); }
    void copy(const long in)
	{ mstring out; out.Format("%d", in); copy(out); }
    void copy(const unsigned long in)
	{ mstring out; out.Format("%u", in); copy(out); }
    void copy(const float in)
	{ mstring out; out.Format("%f", in); copy(out); }
    void copy(const double in)
	{ mstring out; out.Format("%f", in); copy(out); }

    void append(const mstring &in)
	{ append(in.i_str, in.i_len); }
    void append(const string &in)
	{ append(in.c_str(), in.length()); }
    void append(const char *in)
	{ append(in, strlen(in)); }
    void append(const char in)
	{ append(&in, 1); }
    void append(const unsigned char in)
	{ append((const char *) &in, 1); }
    void append(const int in)
	{ mstring out(in); append(out); }
    void append(const unsigned int in)
	{ mstring out(in); append(out); }
    void append(const long in)
	{ mstring out(in); append(out); }
    void append(const unsigned long in)
	{ mstring out(in); append(out); }
    void append(const float in)
	{ mstring out(in); append(out); }
    void append(const double in)
	{ mstring out(in); append(out); }

    void insert(size_t pos, const mstring &in)
	{ insert(pos, in.i_str, in.i_len); }
    void insert(size_t pos, const string &in)
	{ insert(pos, in.c_str(), in.length()); }
    void insert(size_t pos, const char *in)
	{ insert(pos, in, strlen(in)); }
    void insert(size_t pos, const char in)
	{ insert(pos, &in, 1); }
    void insert(size_t pos, const unsigned char in)
	{ insert(pos, (const char *) &in, 1); }
    void insert(size_t pos, const int in)
	{ mstring out(in); insert(pos, out); }
    void insert(size_t pos, const unsigned int in)
	{ mstring out(in); insert(pos, out); }
    void insert(size_t pos, const long in)
	{ mstring out(in); insert(pos, out); }
    void insert(size_t pos, const unsigned long in)
	{ mstring out(in); insert(pos, out); }
    void insert(size_t pos, const float in)
	{ mstring out(in); insert(pos, out); }
    void insert(size_t pos, const double in)
	{ mstring out(in); insert(pos, out); }

    int compare(const mstring &in) const
	{ return compare(in.i_str, in.i_len); }
    int compare(const string &in) const
	{ return compare(in.c_str(), in.length()); }
    int compare(const char *in) const
	{ return compare(in, strlen(in)); }
    int compare(const char in) const
	{ return compare(&in, 1); }
    int compare(const unsigned char in) const
	{ return compare((const char *) &in, 1); }
    int compare(const int in) const
	{ mstring out(in); return compare(in); }
    int compare(const unsigned int in) const
	{ mstring out(in); return compare(in); }
    int compare(const long in) const
	{ mstring out(in); return compare(in); }
    int compare(const unsigned long in) const
	{ mstring out(in); return compare(in); }
    int compare(const float in) const
	{ mstring out(in); return compare(in); }
    int compare(const double in) const
	{ mstring out(in); return compare(in); }

    char operator[] (size_t off) const;
    operator const char *() const
	{ return c_str(); }
    operator const string () const
	{ return string(c_str()); }

    mstring &operator= (const mstring &rhs)
	{ copy(rhs); return *this; }
    mstring &operator= (const string &rhs)
	{ copy(rhs); return *this; }
    mstring &operator= (const char *rhs)
	{ copy(rhs); return *this; }
    mstring &operator= (const char rhs)
	{ copy(rhs); return *this; }
    mstring &operator= (const unsigned char rhs)
	{ copy(rhs); return *this; }
    mstring &operator= (const int rhs)
	{ copy(rhs); return *this; }
    mstring &operator= (const unsigned int rhs)
	{ copy(rhs); return *this; }
    mstring &operator= (const long rhs)
	{ copy(rhs); return *this; }
    mstring &operator= (const unsigned long rhs)
	{ copy(rhs); return *this; }
    mstring &operator= (const float rhs)
	{ copy(rhs); return *this; }
    mstring &operator= (const double rhs)
	{ copy(rhs); return *this; }

    mstring &operator+= (const mstring &rhs)
	{ append(rhs); return *this; }
    mstring &operator+= (const string &rhs)
	{ append(rhs); return *this; }
    mstring &operator+= (const char *rhs)
	{ append(rhs); return *this; }
    mstring &operator+= (const char rhs)
	{ append(rhs); return *this; }
    mstring &operator+= (const unsigned char rhs)
	{ append(rhs); return *this; }
    mstring &operator+= (const int rhs)
	{ append(rhs); return *this; }
    mstring &operator+= (const unsigned int rhs)
	{ append(rhs); return *this; }
    mstring &operator+= (const long rhs)
	{ append(rhs); return *this; }
    mstring &operator+= (const unsigned long rhs)
	{ append(rhs); return *this; }
    mstring &operator+= (const float rhs)
	{ append(rhs); return *this; }
    mstring &operator+= (const double rhs)
	{ append(rhs); return *this; }

    mstring &operator<< (const mstring &rhs)
	{ append(rhs); return *this; }
    mstring &operator<< (const string &rhs)
	{ append(rhs); return *this; }
    mstring &operator<< (const char *rhs)
	{ append(rhs); return *this; }
    mstring &operator<< (const char rhs)
	{ append(rhs); return *this; }
    mstring &operator<< (const unsigned char rhs)
	{ append(rhs); return *this; }
    mstring &operator<< (const int rhs)
	{ append(rhs); return *this; }
    mstring &operator<< (const unsigned int rhs)
	{ append(rhs); return *this; }
    mstring &operator<< (const long rhs)
	{ append(rhs); return *this; }
    mstring &operator<< (const unsigned long rhs)
	{ append(rhs); return *this; }
    mstring &operator<< (const float rhs)
	{ append(rhs); return *this; }
    mstring &operator<< (const double rhs)
	{ append(rhs); return *this; }

    // ALL return -1 if not found, or the offset
    // of the requested char/string.

    // str here is a list of delimiters
    int find_first_of(const char *str, size_t length) const;
    int find_last_of(const char *str, size_t length) const;
    int find_first_not_of(const char *str, size_t length) const;
    int find_last_not_of(const char *str, size_t length) const;

    int find_first_of(const mstring &in) const
	{ return find_first_of(in.i_str, in.i_len); }
    int find_first_of(const string &in) const
	{ return find_first_of(in.c_str(), in.length()); }
    int find_first_of(const char *in) const
	{ return find_first_of(in, strlen(in)); }
    int find_first_of(const char in) const
	{ return find_first_of(&in, 1); }
    int find_first_of(const unsigned char in) const
	{ return find_first_of((const char *) &in, 1); }

    int find_last_of(const mstring &in) const
	{ return find_last_of(in.i_str, in.i_len); }
    int find_last_of(const string &in) const
	{ return find_last_of(in.c_str(), in.length()); }
    int find_last_of(const char *in) const
	{ return find_last_of(in, strlen(in)); }
    int find_last_of(const char in) const
	{ return find_last_of(&in, 1); }
    int find_last_of(const unsigned char in) const
	{ return find_last_of((const char *) &in, 1); }

    int find_first_not_of(const mstring &in) const
	{ return find_first_not_of(in.i_str, in.i_len); }
    int find_first_not_of(const string &in) const
	{ return find_first_not_of(in.c_str(), in.length()); }
    int find_first_not_of(const char *in) const
	{ return find_first_not_of(in, strlen(in)); }
    int find_first_not_of(const char in) const
	{ return find_first_not_of(&in, 1); }
    int find_first_not_of(const unsigned char in) const
	{ return find_first_not_of((const char *) &in, 1); }

    int find_last_not_of(const mstring &in) const
	{ return find_last_not_of(in.i_str, in.i_len); }
    int find_last_not_of(const string &in) const
	{ return find_last_not_of(in.c_str(), in.length()); }
    int find_last_not_of(const char *in) const
	{ return find_last_not_of(in, strlen(in)); }
    int find_last_not_of(const char in) const
	{ return find_last_not_of(&in, 1); }
    int find_last_not_of(const unsigned char in) const
	{ return find_last_not_of((const char *) &in, 1); }


    // str here is used completely
    int occurances(const char *str) const;
    int find(const char *str, int occurance = 1) const;
    int rfind(const char *str, int occurance = 1) const;
    void replace(const char *find, const char *replace, bool all = true);
    void replace(int begin, int end, char *replace, size_t length);

    mstring substr(int nFirst, int nCount) const;

    /* From here is our own additions ... */
    bool Contains(const char *in) const
	{ return (find(in) >= 0); }
    bool Contains(const char in) const
	{ return (find(mstring(in)) >= 0); }
    bool Contains(const unsigned char in) const
	{ return (find(mstring(in)) >= 0); }
    bool IsWord() const;
    bool IsNumber() const;
    bool IsAscii() const;
    bool IsBool() const;
    bool GetBool() const;

    mstring UpperCase() const;
    mstring LowerCase() const;
    void MakeUpper();
    void MakeLower();

    int Occurances(const mstring &in, bool NoCase = false) const;
    int Find(const mstring &in, bool NoCase = false, int occurance = 1) const;
    int RevFind(const mstring &in, bool NoCase = false, int occurance = 1) const;
    int Cmp(const mstring &in, bool NoCase = false) const;
    bool IsSameAs(const mstring &in, bool NoCase = false) const
	{ return (Cmp(in, NoCase)==0); }
    bool Matches(const mstring &in, bool NoCase = false) const;

    void Prepend(const mstring &in)
	{ insert(0, in); }
    void Append(const mstring &in)
	{ append(in); }
    void Replace(const mstring &f, const mstring &r, bool All = true)
	{ replace(f.i_str, r.i_str, All); }
    void Remove(const mstring &in, bool All = true)
	{ replace(in.i_str, "", All); }
    void Truncate(size_t pos)
	{ erase(pos); }
    void Trim(bool right=true, const mstring &delims = " \n\r\t");
    mstring Strip(bool right=true, const mstring &delims = " \n\r\t") const;

    int Format(const char *fmt, ...);
    int FormatV(const char *fmt, va_list argptr);

    mstring Before(const mstring &in, int occurance = 1) const;
    mstring After(const mstring &in, int occurance = 1) const;
    mstring RevBefore(const mstring &in, int occurance = 1) const;
    mstring RevAfter(const mstring &in, int occurance = 1) const;
    mstring SubString(int from = 0, int to = -1) const;
    mstring Left(int pos) const
	{ return SubString(0, pos-1); }
    mstring Right(int pos) const
	{ return SubString(pos); }
    unsigned int WordCount(const mstring &delim, bool assemble = true) const;
    mstring ExtractWord(unsigned int count, const mstring &delim,
						bool assemble = true) const;
    int WordPosition(unsigned int count, const mstring &delim,
						bool assemble = true) const;
};

#endif
