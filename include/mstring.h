#ifndef WIN32
  #pragma interface
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
#ifndef _MSTRING_H
#define _MSTRING_H
#include "pch.h"
RCSID(mstring_h, "@(#) $Id$");
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.79  2001/11/12 01:05:01  prez
** Added new warning flags, and changed code to reduce watnings ...
**
** Revision 1.78  2001/11/03 21:02:50  prez
** Mammoth change, including ALL changes for beta12, and all stuff done during
** the time GOTH.NET was down ... approx. 3 months.  Includes EPONA conv utils.
**
** Revision 1.77  2001/06/20 06:07:01  prez
** ome GCC 3.0 and solaris fixes
**
** Revision 1.76  2001/06/15 07:20:39  prez
** Fixed windows compiling -- now works with MS Visual Studio 6.0
**
** Revision 1.75  2001/05/13 00:55:17  prez
** More patches to try and fix deadlocking ...
**
** Revision 1.74  2001/05/05 17:33:58  prez
** Changed log outputs from printf-style to tokenized style files.
** Now use LOG/NLOG/SLOG/SNLOG rather than just LOG for output.  All
** formatting must be done BEFORE its sent to the logger (use fmstring).
**
** Revision 1.73  2001/04/09 07:52:22  prez
** Fixed /nickserv.  Fixed cordump in nick expiry.  Fixed slight bugs in mstring.
**
** Revision 1.72  2001/04/05 05:59:50  prez
** Turned off -fno-default-inline, and split up server.cpp, it should
** compile again with no special options, and have default inlines :)
**
** Revision 1.71  2001/04/02 02:13:27  prez
** Added inlines, fixed more of the exception code.
**
** Revision 1.70  2001/03/27 07:04:30  prez
** All maps have been hidden, and are now only accessable via. access functions.
**
** Revision 1.69  2001/03/20 14:22:14  prez
** Finished phase 1 of efficiancy updates, we now pass mstring/mDateTime's
** by reference all over the place.  Next step is to stop using operator=
** to initialise (ie. use mstring blah(mstring) not mstring blah = mstring).
**
** Revision 1.68  2001/03/04 02:08:27  prez
** Enhansed mstring
**
** Revision 1.67  2001/03/02 05:24:41  prez
** HEAPS of modifications, including synching up my own archive.
**
** Revision 1.66  2001/02/11 07:41:27  prez
** Enhansed support for server numerics, specifically for Unreal.
**
** Revision 1.65  2001/02/03 03:20:33  prez
** Fixed up some differences in previous committed versions ...
**
** Revision 1.62  2000/12/25 06:36:14  prez
** Added locking around the threadtoself map, and removed a bunch of
** defines from mstring (while keeping it the same!)
**
** Revision 1.61  2000/12/19 07:24:53  prez
** Massive updates.  Linux works again, added akill reject threshold, and
** lots of other stuff -- almost ready for b6 -- first beta after the
** re-written strings class.  Also now using log adapter!
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

/* This class is totally written by Preston A. Elder <prez@magick.tm>.
 * Primerily this was written for Magick IRC Services (and by default,
 * the ACE library).  It is totally portable with 2 or 3 steps:
 *   1. Either choose ALLOCTYPE 1, 2 or 4 or modify the mstring::malloc
 *      function to not use ACE_bad_alloc in ALLOCTYPE 3.  If you use
 *      ALLOCTYPE 4, you must write your own memory allocation scheme,
 *      change the MEMORY_AREA #define, and include your own header.
 *   2. Replace all instances of 'ACE_OS::' with nothing throughout
 *      the mstring.cpp file -- to force the use of the OS calls,
 *      rather than ACE's version of the OS calls.
 *   3. (optional) Change the contents of the mstring::lock_*() and
 *      init() funcitons so they use some other locking mechanism
 *      rather than ACE's (eg. pthreads).  You must also change the
 *      LOCK_TYPE and UNIQ_LOCK_TYPE #define's to reflect this. This
 *      only needs to be done if you activate per-mstring locking, by
 *      #defining MSTRING_LOCKS_WORK.
 *
 * If you use this library in your program, please let me know!
 *
 */

/* Memory Allocation Scheme
 *
 * 1. C: malloc/free
 * 2. C++: new/delete
 * 3. ACE: new/delete with try/catch
 * 4. CLUSTER: memory_area.alloc/memory_area.dealloc
 *
 */
#define ALLOCTYPE	3


#ifdef MAGICK_LOCKS_WORK

/* Commented out to stop mstring locks for now */
/* #define MSTRING_LOCKS_WORK */

/* MUST have an acquire_read and acquire_write! */
#define LOCK_TYPE	ACE_RW_Thread_Mutex

/* MUST have an acquire that defaults to WRITE */
#define UNIQ_LOCK_TYPE	ACE_Thread_Mutex

#endif

/* --------------- End of hard-config section --------------- */


#if ALLOCTYPE == 4

/* Use our own Memory Map for clustered alloc */
#include "mmemory.h"

/* MUST have void *alloc(size_t) and dealloc(void *) functions */
#define MEMORY_AREA	MemoryManager<ACE_Thread_Mutex>

#endif

#ifdef MAGICK_HAS_EXCEPTIONS
class mstring_noalloc : public exception
{
    char i_reason[1024];
public:
    mstring_noalloc(const char *reason = "") throw()
	{ ACE_OS::strncpy(i_reason, reason, 1024); }
    ~mstring_noalloc() throw() {}
    const char *what() const throw()
	{ return i_reason; }
};

class mstring_nodealloc : public exception
{
    char i_reason[1024];
public:
    mstring_nodealloc(const char *reason = "") throw() 
	{ ACE_OS::strncpy(i_reason, reason, 1024); }
    ~mstring_nodealloc() throw() {}
    const char *what() const throw()
	{ return i_reason; }
};
#endif

/* This would have to be the most interoperable class
 * in the existance of C++ ;P */

class mstring;

extern const mstring DirSlash;
extern const mstring Blank;
extern const mstring IRC_CTCP;
extern const mstring IRC_Bold;
extern const mstring IRC_Underline;
extern const mstring IRC_Reverse;
extern const mstring IRC_Color;
extern const mstring IRC_Off;

bool match_wild (const char *pattern, const char *str, bool nocase);
mstring fmstring (const char *fmt, ...);

class mstring
{
    char *i_str;
    size_t i_len, i_res;

#if ALLOCTYPE == 4
    static MEMORY_AREA memory_area;
#endif
#ifdef MSTRING_LOCKS_WORK
    LOCK_TYPE *i_lock;
#endif

#ifdef MAGICK_HAS_EXCEPTIONS
    static char *alloc(const size_t sz) throw(mstring_noalloc);
    static void dealloc(char * & in) throw(mstring_nodealloc);
#else
    static char *alloc(const size_t sz);
    static void dealloc(char * & in);
#endif
    void lock_read() const;
    void lock_write() const;
    void lock_rel() const;
    void init();
    int occurances(const char *str, const size_t len) const;

public:
    static int snprintf(char *buf, const size_t sz, const char *fmt, ...);
    static int vsnprintf(char *buf, const size_t sz, const char *fmt, va_list ap);

    mstring()
	{ init(); }
    mstring(const mstring &in)
	{ init(); copy(in); }
    mstring(const string &in)
	{ init(); copy(in); }
    mstring(const char *in, const size_t len)
	{ init(); copy(in, len); }
    mstring(const char *in)
	{ init(); copy(in); }
    mstring(const char in)
	{ init(); copy(in); }
    mstring(const unsigned char in)
	{ init(); copy(in); }
    mstring(const int in)
	{ init(); copy(in); }
    mstring(const unsigned int in)
	{ init(); copy(in); }
    mstring(const long in)
	{ init(); copy(in); }
    mstring(const unsigned long in)
	{ init(); copy(in); }
    mstring(const float in)
	{ init(); copy(in); }
    mstring(const double in)
	{ init(); copy(in); }
    mstring(const vector<mstring> &in)
	{ init(); Assemble(in); }
    mstring(const list<mstring> &in)
	{ init(); Assemble(in); }
    ~mstring();

    void copy(const char *in, const size_t len);
    void append(const char *in, const size_t len);
    void erase(int begin = 0, int end = -1);
    void insert(const size_t pos, const char *in, const size_t len);
    int compare(const char *in, const size_t len) const;
    void swap(mstring &in);
    const char *c_str() const;
    const unsigned char *uc_str() const;
    const char first(size_t off = 1) const;
    const char last(size_t off = 1) const;
    size_t length() const;
    size_t size() const;
    size_t capacity() const;
    bool empty() const;

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
	{ copy(reinterpret_cast<const char *>(&in), 1); }
    void copy(const int in)
	{ mstring out; out.Format("%d", in); copy(out); }
    void copy(const unsigned int in)
	{ mstring out; out.Format("%u", in); copy(out); }
    void copy(const long in)
	{ mstring out; out.Format("%ld", in); copy(out); }
    void copy(const unsigned long in)
	{ mstring out; out.Format("%lu", in); copy(out); }
    void copy(const float in)
	{ mstring out; out.Format("%f", in); copy(out); }
    void copy(const double in)
	{ mstring out; out.Format("%f", in); copy(out); }

    void prepend(const mstring &in)
	{ insert(0, in.i_str, in.i_len); }
    void append(const mstring &in)
	{ append(in.i_str, in.i_len); }
    void insert(const size_t pos, const mstring &in)
	{ insert(pos, in.i_str, in.i_len); }
    int compare(const mstring &in) const
	{ return compare(in.i_str, in.i_len); }

    const char operator[] (const size_t off) const;
    operator const char *() const
	{ return c_str(); }
    operator const string () const
	{ return string(c_str()); }

    mstring &operator= (const mstring &rhs)
	{ copy(rhs); return *this; }
    mstring &operator+= (const mstring &rhs)
	{ append(rhs); return *this; }
    mstring &operator<< (const mstring &rhs)
	{ append(rhs); return *this; }

    // ALL return -1 if not found, or the offset
    // of the requested char/string.

    // str here is a list of delimiters
    int find_first_of(const char *str, const size_t len) const;
    int find_last_of(const char *str, const size_t len) const;
    int find_first_not_of(const char *str, const size_t len) const;
    int find_last_not_of(const char *str, const size_t len) const;

    int find_first_of(const mstring &in) const
	{ return find_first_of(in.i_str, in.i_len); }
    int find_last_of(const mstring &in) const
	{ return find_last_of(in.i_str, in.i_len); }
    int find_first_not_of(const mstring &in) const
	{ return find_first_not_of(in.i_str, in.i_len); }
    int find_last_not_of(const mstring &in) const
	{ return find_last_not_of(in.i_str, in.i_len); }

    // str here is used completely
    int find(const mstring &str, int occurance = 1) const;
    int rfind(const mstring &str, int occurance = 1) const;
    void replace(const mstring &i_find, const mstring &i_replace, const bool all = true);
    void replace(const int begin, const int end, const char *i_replace, const size_t len)
	{
	    erase(begin, end);
	    insert(begin, i_replace, len);
	}
    void replace(const int begin, const int end, const mstring &i_replace)
	{ replace(begin, end, i_replace.i_str, i_replace.i_len); }
    bool replace(const size_t offs, const char c);

    mstring substr(int nFirst, int nCount) const;

    /* From here is our own additions ... */
    bool Contains(const mstring &in) const
	{ return (find(in) >= 0); }

    int Format(const char *fmt, ...);
    int FormatV(const char *fmt, va_list argptr);

    bool IsWord() const;
    bool IsNumber() const;
    bool IsAscii() const;
    bool IsBool() const;
    bool GetBool() const;

    mstring UpperCase() const;
    mstring LowerCase() const;
    void MakeUpper();
    void MakeLower();

    int Occurances(const mstring &in, const bool NoCase = false) const;
    int Find(const mstring &in, const bool NoCase = false, const int occurance = 1) const;
    int RevFind(const mstring &in, const bool NoCase = false, const int occurance = 1) const;
    int Cmp(const mstring &in, const bool NoCase = false) const;
    bool IsSameAs(const mstring &in, const bool NoCase = false) const
	{ return (Cmp(in, NoCase)==0); }
    bool Matches(const mstring &in, const bool NoCase = false) const
	{ return match_wild(in.c_str(), c_str(), NoCase); }

    void Remove(const mstring &in, const bool All = true)
	{ replace(in, "", All); }
    void Truncate(const size_t pos, const bool right = true)
	{
	    if (right)
		erase(pos);
	    else
		erase(0, pos);
	}
    void Trim(const bool right=true, const mstring &delims = " \n\r\t");
    mstring Strip(const bool right=true, const mstring &delims = " \n\r\t") const;

    mstring Before(const mstring &in, const int occurance = 1) const;
    mstring After(const mstring &in, const int occurance = 1) const;
    mstring RevBefore(const mstring &in, const int occurance = 1) const;
    mstring RevAfter(const mstring &in, const int occurance = 1) const;
    mstring SubString(int from = 0, int to = -1) const;
    mstring Left(const int pos) const
	{ return SubString(0, pos-1); }
    mstring Right(const int pos) const
	{ return SubString(pos); }
    unsigned int WordCount(const mstring &delim, const bool assemble = true) const;
    mstring ExtractWord(const unsigned int count, const mstring &delim,
					const bool assemble = true) const;
    int WordPosition(const unsigned int count, const mstring &delim,
					const bool assemble = true) const;

    vector<mstring> Vector(const mstring &delim, const bool assemble = true) const;
    list<mstring> List(const mstring &delim, const bool assemble = true) const;
    void Assemble(const vector<mstring> &text, const mstring &delim = " ");
    void Assemble(const list<mstring> &text, const mstring &delim = " ");
};

inline bool operator== (const mstring &lhs, const mstring &rhs)
	{ return (lhs.compare(rhs) == 0); }
inline bool operator== (const mstring &lhs, const string &rhs)
	{ return (lhs.compare(rhs) == 0); }
inline bool operator== (const mstring &lhs, const char *rhs)
	{ return (lhs.compare(rhs) == 0); }
inline bool operator== (const mstring &lhs, const char rhs)
	{ return (lhs.compare(rhs) == 0); }
inline bool operator== (const mstring &lhs, const unsigned char rhs)
	{ return (lhs.compare(rhs) == 0); }
inline bool operator== (const string &lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) == 0); }
inline bool operator== (const char *lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) == 0); }
inline bool operator== (const char lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) == 0); }
inline bool operator== (const unsigned char lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) == 0); }

inline bool operator!= (const mstring &lhs, const mstring &rhs)
	{ return (lhs.compare(rhs) != 0); }
inline bool operator!= (const mstring &lhs, const string &rhs)
	{ return (lhs.compare(rhs) != 0); }
inline bool operator!= (const mstring &lhs, const char *rhs)
	{ return (lhs.compare(rhs) != 0); }
inline bool operator!= (const mstring &lhs, const char rhs)
	{ return (lhs.compare(rhs) != 0); }
inline bool operator!= (const mstring &lhs, const unsigned char rhs)
	{ return (lhs.compare(rhs) != 0); }
inline bool operator!= (const string &lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) != 0); }
inline bool operator!= (const char *lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) != 0); }
inline bool operator!= (const char lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) != 0); }
inline bool operator!= (const unsigned char lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) != 0); }

inline bool operator< (const mstring &lhs, const mstring &rhs)
	{ return (lhs.compare(rhs) < 0); }
inline bool operator< (const mstring &lhs, const string &rhs)
	{ return (lhs.compare(rhs) < 0); }
inline bool operator< (const mstring &lhs, const char *rhs)
	{ return (lhs.compare(rhs) < 0); }
inline bool operator< (const mstring &lhs, const char rhs)
	{ return (lhs.compare(rhs) < 0); }
inline bool operator< (const mstring &lhs, const unsigned char rhs)
	{ return (lhs.compare(rhs) < 0); }
inline bool operator< (const string &lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) > 0); }
inline bool operator< (const char *lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) > 0); }
inline bool operator< (const char lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) > 0); }
inline bool operator< (const unsigned char lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) > 0); }

inline bool operator> (const mstring &lhs, const mstring &rhs)
	{ return (lhs.compare(rhs) > 0); }
inline bool operator> (const mstring &lhs, const string &rhs)
	{ return (lhs.compare(rhs) > 0); }
inline bool operator> (const mstring &lhs, const char *rhs)
	{ return (lhs.compare(rhs) > 0); }
inline bool operator> (const mstring &lhs, const char rhs)
	{ return (lhs.compare(rhs) > 0); }
inline bool operator> (const mstring &lhs, const unsigned char rhs)
	{ return (lhs.compare(rhs) > 0); }
inline bool operator> (const string &lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) < 0); }
inline bool operator> (const char *lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) < 0); }
inline bool operator> (const char lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) < 0); }
inline bool operator> (const unsigned char lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) < 0); }

inline bool operator<= (const mstring &lhs, const mstring &rhs)
	{ return (lhs.compare(rhs) <= 0); }
inline bool operator<= (const mstring &lhs, const string &rhs)
	{ return (lhs.compare(rhs) <= 0); }
inline bool operator<= (const mstring &lhs, const char *rhs)
	{ return (lhs.compare(rhs) <= 0); }
inline bool operator<= (const mstring &lhs, const char rhs)
	{ return (lhs.compare(rhs) <= 0); }
inline bool operator<= (const mstring &lhs, const unsigned char rhs)
	{ return (lhs.compare(rhs) <= 0); }
inline bool operator<= (const string &lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) >= 0); }
inline bool operator<= (const char *lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) >= 0); }
inline bool operator<= (const char lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) >= 0); }
inline bool operator<= (const unsigned char lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) >= 0); }

inline bool operator>= (const mstring &lhs, const mstring &rhs)
	{ return (lhs.compare(rhs) >= 0); }
inline bool operator>= (const mstring &lhs, const string &rhs)
	{ return (lhs.compare(rhs) >= 0); }
inline bool operator>= (const mstring &lhs, const char *rhs)
	{ return (lhs.compare(rhs) >= 0); }
inline bool operator>= (const mstring &lhs, const char rhs)
	{ return (lhs.compare(rhs) >= 0); }
inline bool operator>= (const mstring &lhs, const unsigned char rhs)
	{ return (lhs.compare(rhs) >= 0); }
inline bool operator>= (const string &lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) <= 0); }
inline bool operator>= (const char *lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) <= 0); }
inline bool operator>= (const char lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) <= 0); }
inline bool operator>= (const unsigned char lhs, const mstring &rhs)
	{ return (rhs.compare(lhs) <= 0); }

inline mstring operator+ (const mstring &lhs, const mstring &rhs)
{
    mstring str(lhs);
    str.append(rhs.c_str(), rhs.length());
    return str;
}

inline mstring operator+ (const mstring &lhs, const string &rhs)
{
    mstring str(lhs);
    str.append(rhs.c_str(), rhs.length());
    return str;
}

inline mstring operator+ (const mstring &lhs, const char *rhs)
{
    mstring str(lhs);
    str.append(rhs, strlen(rhs));
    return str;
}

inline mstring operator+ (const mstring &lhs, const char rhs)
{
    mstring str(lhs);
    str.append(&rhs, 1);
    return str;
}

inline mstring operator+ (const mstring &lhs, const unsigned char rhs)
{
    mstring str(lhs);
    str.append(reinterpret_cast<const char *>(&rhs), 1);
    return str;
}

inline mstring operator+ (const mstring &lhs, const int rhs)
{
    mstring str(lhs);
    str.append(rhs);
    return str;
}

inline mstring operator+ (const mstring &lhs, const unsigned int rhs)
{
    mstring str(lhs);
    str.append(rhs);
    return str;
}

inline mstring operator+ (const mstring &lhs, const long rhs)
{
    mstring str(lhs);
    str.append(rhs);
    return str;
}

inline mstring operator+ (const mstring &lhs, const unsigned long rhs)
{
    mstring str(lhs);
    str.append(rhs);
    return str;
}

inline mstring operator+ (const mstring &lhs, const float rhs)
{
    mstring str(lhs);
    str.append(rhs);
    return str;
}

inline mstring operator+ (const mstring &lhs, const double rhs)
{
    mstring str(lhs);
    str.append(rhs);
    return str;
}

inline mstring operator+ (const string &lhs, const mstring &rhs)
{
    mstring str(lhs);
    str.append(rhs.c_str(), rhs.length());
    return str;
}

inline mstring operator+ (const char *lhs, const mstring &rhs)
{
    mstring str(lhs);
    str.append(rhs.c_str(), rhs.length());
    return str;
}

inline mstring operator+ (const char lhs, const mstring &rhs)
{
    mstring str(lhs);
    str.append(rhs.c_str(), rhs.length());
    return str;
}

inline mstring operator+ (const unsigned char lhs, const mstring &rhs)
{
    mstring str(lhs);
    str.append(rhs.c_str(), rhs.length());
    return str;
}

inline mstring operator+ (const int lhs, const mstring &rhs)
{
    mstring str(lhs);
    str.append(rhs.c_str(), rhs.length());
    return str;
}

inline mstring operator+ (const unsigned int lhs, const mstring &rhs)
{
    mstring str(lhs);
    str.append(rhs.c_str(), rhs.length());
    return str;
}

inline mstring operator+ (const long lhs, const mstring &rhs)
{
    mstring str(lhs);
    str.append(rhs.c_str(), rhs.length());
    return str;
}

inline mstring operator+ (const unsigned long lhs, const mstring &rhs)
{
    mstring str(lhs);
    str.append(rhs.c_str(), rhs.length());
    return str;
}

inline mstring operator+ (const float lhs, const mstring &rhs)
{
    mstring str(lhs);
    str.append(rhs.c_str(), rhs.length());
    return str;
}

inline mstring operator+ (const double lhs, const mstring &rhs)
{
    mstring str(lhs);
    str.append(rhs.c_str(), rhs.length());
    return str;
}

#ifndef HAVE_ITOA
inline const char *itoa(int i)
{ return mstring(i).c_str(); }
#endif

#ifndef HAVE_LTOA
inline const char *ltoa(long l)
{ return mstring(l).c_str(); }
#endif

#ifndef HAVE_FTOA
inline const char *ftoa(float f)
{ return mstring(f).c_str(); }
#endif

#ifndef HAVE_DTOA
inline const char *dtoa(double d)
{ return mstring(d).c_str(); }
#endif

#ifndef HAVE_ULTOA
inline const char *ultoa(unsigned long ul)
{ return mstring(ul).c_str(); }
#endif

#ifndef HAVE_UITOA
inline const char *uitoa(unsigned int ui)
{ return mstring(ui).c_str(); }
#endif


#endif
