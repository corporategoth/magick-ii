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
** Revision 1.90  2001/12/27 04:54:46  prez
** Converted SXP to not use STL strings, use mstring instead.
**
** Revision 1.89  2001/12/27 01:02:15  prez
** Fixed up init, moved to header (for efficiancy)
**
** Revision 1.88  2001/12/27 00:40:44  prez
** Some efficiancy changes to mstring
**
** Revision 1.87  2001/12/16 00:12:44  prez
** Some code in mstring to make its vsnprintf more safe, if possible.
**
** Revision 1.86  2001/12/12 15:40:21  prez
** Made *toa functions guarentee the result is terminated.
**
** Revision 1.85  2001/12/12 07:43:52  prez
** Some more platform changes.  Made it look for _snprintf and _vsnprintf
** aswell (is the case on windows).  Also updated windows config.h.win.
**
** Revision 1.84  2001/12/12 07:19:20  prez
** Added check for snprintf, and changed *toa functions to use snprintf.  Also
** moved magick::snprintf and magick::vsnprintf to just snprintf and vsnprintf
** for systems without the system calls.  Made them inline.  Finally, made
** mstring's copy() commands for non-string types use *toa functions.
**
** Revision 1.83  2001/12/12 03:51:00  prez
** Forgot to return SV in all the *toa functions
**
** Revision 1.82  2001/12/12 03:48:47  prez
** Fixed all the *toa functions, to NOT return c_str() of a temporary veriable.
**
** Revision 1.81  2001/12/07 02:51:39  prez
** Added doxygen comments to mstring, and removed doxygen generated stuff
** from CVS -- you can now just generate it yourself with the config file.
**
** Revision 1.80  2001/12/04 22:45:04  prez
** Re-vamped the operator overloads, making it even MORE portable, and using
** up alot less room in the process.
**
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

/** Memory Allocation Scheme
 * Valid values are:
 * 1. C: malloc/free
 * 2. C++: new/delete
 * 3. ACE: new/delete with try/catch
 * 4. CLUSTER: memory_area.alloc/memory_area.dealloc
 */
#define ALLOCTYPE	3

#if ALLOCTYPE == 4

/* Use our own Memory Map for clustered alloc */
#include "mmemory.h"

/** Custom memory allocation scheme for when ALLOCTYPE == 4
 *  Must have a void *alloc(size_t) and void dealloc(void *) function.
 *  This must be a type, as it will be instanced statically.
 *  @see ALLOCTYPE
 */
#define MEMORY_AREA	MemoryManager<ACE_Thread_Mutex>

#endif /* ALLOCTYPE == 4 */

#ifdef MAGICK_LOCKS_WORK

/** Enables or disables the internal locking mechanisms in mstring. */
#undef MSTRING_LOCKS_WORK

/** The locking mechanism used for all accesses of an mstring
 *  Must have acquire_read(), acquire_write() and release() member functions.
 *  This must be a type, as it will be instanced.
 *  @see MSTRING_LOCKS_WORK
 */
#define LOCK_TYPE	ACE_RW_Thread_Mutex

/** The locking mechanism used for the unique lock identifier
 *  Must have acquire() and release() member functions.
 *  This must be a type, as it will be instanced.
 *  @see MSTRING_LOCKS_WORK
 */
#define UNIQ_LOCK_TYPE	ACE_Thread_Mutex

#endif /* MAGICK_LOCKS_WORK */

/* --------------- End of hard-config section --------------- */


#ifdef MAGICK_HAS_EXCEPTIONS
/** Exception thrown when allocation of memory fails inside mstring */
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

/** Exception thrown when deallocation of memory fails inside mstring */
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

extern const mstring DirSlash;		/**< The correct directory separator for
					     whatever platform you are on */
extern const mstring Blank;		/**< Equivalent to mstring("") */
extern const mstring IRC_CTCP;		/**< The IRC CTCP code, usually ^A */
extern const mstring IRC_Bold;		/**< The IRC bold code, usually ^B */
extern const mstring IRC_Underline;	/**< The IRC underline code, usually ^_ */
extern const mstring IRC_Reverse;	/**< The IRC reverse code, usually ^V */
extern const mstring IRC_Color;		/**< The IRC color code, usually ^C */
extern const mstring IRC_Off;		/**< The IRC off code, usually ^O */

/** Wildcard match of a pattern to a string
 *  The pattern and string may not be NULL's.
 *  @param pattern The wildcard pattern to match.  A '*' represents
	0 or more of any character.  A '?' represents exactly 1
	character.
 *  @param str The string to match against (no wildcards).
 *  @param nocase Setting this to true makes the search case insensative.
 *  @return true if the pattern matched, otherwise false.
 */
bool match_wild (const char *pattern, const char *str, bool nocase);

/** Formats a string using printf formatting codes
 *  The formatting string may not be NULL, and you must supply as many
 *  parameters as '%' symbols in the formatting string.  See the format
 *  man page for more information on printf-style formatting.
 *  @param fmt The formatting string to apply using all other parameters.
 *  @return The fmt string with all formatting applied, as an mstring. */
mstring fmstring (const char *fmt, ...);

#ifndef HAVE_VSNPRINTF
#ifdef HAVE__VSNPRINTF
#define vsnprintf _vsnprintf
#else
/** For systems that dont have vsnprintf
 *  A simple wrapper to ACE_OS::vsprintf
 */
inline int vsnprintf(char *buf, const size_t sz, const char *fmt, va_list ap)
{
    if (buf == NULL)
	return -1;

    int iLen = 0;
    char *nbuf = new char[sz];
    if (nbuf != NULL)
    {
        iLen = ACE_OS::vsprintf(nbuf, fmt, ap);
        strncpy(buf, nbuf, sz);
        delete [] nbuf;
    }
    else
    {
        iLen = ACE_OS::vsprintf(buf, fmt, ap);
    }
    return iLen;
}
#endif /* _vsnprintf */
#endif /* vsnprintf */

#ifndef HAVE_SNPRINTF
#ifdef HAVE__SNPRINTF
#define snprintf _snprintf
#else
/** For systems that dont have snprintf
 *  A simple wrapper to vsnprintf.
 *  @see vsnprintf
 */
inline int snprintf(char *buf, const size_t sz, const char *fmt, ...)
{
    va_list argptr;
    va_start(argptr, fmt);
    int iLen = vsnprintf(buf, sz, fmt, argptr);
    va_end(argptr);
    return iLen;
}
#endif /* _snprintf */
#endif /* snprintf */

#ifndef HAVE_ITOA
/** Reverse of atoi */
inline const char *itoa(int i)
{
    static char sv[16];
    memset(sv, 0, sizeof(sv));
    snprintf(sv, sizeof(sv)-1, "%d", i);
    return sv;
}
#endif

#ifndef HAVE_LTOA
/** Reverse of strtol */
inline const char *ltoa(long l)
{
    static char sv[16];
    memset(sv, 0, sizeof(sv));
    snprintf(sv, sizeof(sv)-1, "%ld", l);
    return sv;
}
#endif

#ifndef HAVE_FTOA
/** Reverse of atof */
inline const char *ftoa(float f)
{
    static char sv[64];
    memset(sv, 0, sizeof(sv));
    snprintf(sv, sizeof(sv)-1, "%f", f);
    return sv;
}
#endif

#ifndef HAVE_DTOA
/** Reverse of strtod */
inline const char *dtoa(double d)
{
    static char sv[512];
    memset(sv, 0, sizeof(sv));
    snprintf(sv, sizeof(sv)-1, "%g", d);
    return sv;
}
#endif

#ifndef HAVE_ULTOA
/** Reverse of strtoul */
inline const char *ultoa(unsigned long ul)
{
    static char sv[16];
    memset(sv, 0, sizeof(sv));
    snprintf(sv, sizeof(sv)-1, "%lu", ul);
    return sv;
}
#endif

#ifndef HAVE_UITOA
/** Reverse of strtoul, but for int */
inline const char *uitoa(unsigned int ui)
{
    static char sv[16];
    memset(sv, 0, sizeof(sv));
    snprintf(sv, sizeof(sv)-1, "%u", ui);
    return sv;
}
#endif

/** Magick's string class
 *  This class is designed to emulate the standard STL string in many
 *  ways, however there are some notable differences in syntax.  It also
 *  adds many of the more contemporary string needs.  This class is
 *  designed to work with 3 standard memory allocation schemes, plus
 *  give you the option of specifying your own.  It is also designed to
 *  be fully thread safe.
 *  @see MSTRING_LOCKS_WORK
 *  @see LOCK_TYPE
 *  @see UNIQ_LOCK_TYPE
 *  @see ALLOCTYPE
 *  @see MEMORY_AREA
 */
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
    static inline char *alloc(const size_t sz) throw(mstring_noalloc);
    static inline void dealloc(char * & in) throw(mstring_nodealloc);
#else
    static inline char *alloc(const size_t sz);
    static inline void dealloc(char * & in);
#endif
    inline void lock_read() const;
    inline void lock_write() const;
    inline void lock_rel() const;
    inline void init()
    {
#ifdef MSTRING_LOCKS_WORK
	char lockname[32];
	UNIQ_LOCK_TYPE uniq_lock("mstring_lock_id");
	uniq_lock.acquire();
	next_lock_id++;
	snprintf(lockname, 32, "mstring_%08x%08x", time(NULL), next_lock_id);
	uniq_lock.release();
	i_lock = new LOCK_TYPE(lockname);
#endif

	i_len = 0;
	i_res = 0;
	i_str = NULL;
    }

    int occurances(const char *str, const size_t len) const;

public:
    //@{
    /** 
     *  Constructors used to create a new mstring.  Everything will be
     *  converted to a string one way or another.
     */
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
    //@}

    /** Destructor */
    ~mstring();

    /** Overwrite the current contents with a string
     *  This will deallocate/allocate memory as needed.
     *  @param in The string to set.
     *  @param len The length of the string
     */
    void copy(const char *in, const size_t len);

    /** Append a string to the end of current contents
     *  This will automatically expand the internal buffer as needed.
     *  @param in The string to append.
     *  @param len The length of the string.
     */
    void append(const char *in, const size_t len);

    /** Erase part of, or all of the current contents
     *  @param begin The location of the first character to erase,
		starting from, and defaulting to 0.
     *  @param end The location of the last character to erase,
		defaulting to the last character.
     */
    void erase(int begin = 0, int end = -1);

    /** Insert a string into an arbitary possition of the current contents
     *  @param pos The possition at which we will put the new string.
     *  @param in The string to insert.
     *  @param len The length of the string.
     */
    void insert(const size_t pos, const char *in, const size_t len);

    /** Compare the current contents to a string
     *  This function mimics the behavior of strcmp/memcmp.
     *  @param in The string to compare to.
     *  @param len The length of the string.
     *  @return 0 for an exact match, below 0 when the string is
		alphabetically 'higher' than the current contents, and
		above 0, when it is alphabetically 'lower'.
     */
    int compare(const char *in, const size_t len) const;

    /** Swap the contents of this mstring with another
     *  @param in The mstring to swap contents with.
     */
    void swap(mstring &in);

    /** Represent the current contents as a C-style string
     *  @return A C-style string (char *) representation of current
		contents.  This will be "" if there is none.
     */
    const char *c_str() const;

    /** Represent the current contents as a C-style string
     *  @return A C-style string (unsigned char *) representation of
		current contents.  This will be "" if there is none.
     */
    const unsigned char *uc_str() const;

    /** Grab the Xth character from the beginning of the current contents
     *  @param off Offset from the begnning of the current contents,
		starting with and defaulting to 1.
     *  @return The Xth character from the beginning of current contents,
		or 0 if the specified offset is past either end.
     */
    const char first(size_t off = 1) const;

    /** Grab the Xth character from the end of the current contents
     *  @param off Offset from the end of the current contents,
		starting with and defaulting to 1.
     *  @return The Xth character from the end of current contents,
		or 0 if the specified offset is past either end.
     */
    const char last(size_t off = 1) const;

    /** Length of the current contents */
    size_t length() const;

    /** Length of the current contents */
    size_t size() const;

    /** Amount of reserved bytes we currently have for current contents */
    size_t capacity() const;

    /** Check to see if the current contents are empty
     *  @return true if the current contents are empty
     */
    bool empty() const;

    //@{
    /** Alias for the copy() function.
     *  This is required to satisfy the constructors.
     *  @see copy()
     */
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
	{ copy(itoa(in)); }
    void copy(const unsigned int in)
	{ copy(uitoa(in)); }
    void copy(const long in)
	{ copy(ltoa(in)); }
    void copy(const unsigned long in)
	{ copy(ultoa(in)); }
    void copy(const float in)
	{ copy(ftoa(in)); }
    void copy(const double in)
	{ copy(dtoa(in)); }
    //@}

    /** Insert a string at the beginning of the current contents
     *  This function handles not only mstring, but all data types
     *  that we have valid constructors for.
     *  @param in The string to insert
     *  @see insert()
     */
    void prepend(const mstring &in)
	{ insert(0, in.i_str, in.i_len); }

    /** Alias for the append function
     *  This function handles not only mstring, but all data types
     *  that we have valid constructors for.
     *  @param in The string to append.
     *  @see append()
     */
    void append(const mstring &in)
	{ append(in.i_str, in.i_len); }

    /** Alias for the insert function
     *  This function handles not only mstring, but all data types
     *  that we have valid constructors for.
     *  @param in The string to inserted.
     *  @see insert()
     */
    void insert(const size_t pos, const mstring &in)
	{ insert(pos, in.i_str, in.i_len); }

    /** Alias for the compare function
     *  This function handles not only mstring, but all data types
     *  that we have valid constructors for.
     *  @param in The string to compared.
     *  @see compare()
     */
    int compare(const mstring &in) const
	{ return compare(in.i_str, in.i_len); }

    /** Operator alias for the first() function
     *  This operator allows you to use syntax like:
     *    char c = mystring[3];
     *  @param off Offset from the begnning of the current contents,
		starting with 0.
     *  @see first();
     */
    const char operator[] (const size_t off) const
	{ return first(off+1); }

    /** Operator alias for the c_str() function
     *  This operator allows you to use syntax like:
     *    char *s = mystring;
     *  @see c_str()
     */
    operator const char *() const
	{ return c_str(); }

    /** Represent the current contents as a STL string
     *  This operator allows you to use syntax like:
     *    string s = mystring;
     *  @return An STL string representation of current contents.
		This will be "" if there is none.
     */
    operator const string () const
	{ return string(c_str()); }

    /** Operator alias for the copy() function
     *  This operator allows you to use syntax like:
     *    mstring s = mystring;
     *  @see copy()
     */
    mstring &operator= (const mstring &rhs)
	{ copy(rhs); return *this; }

    /** Operator alias for the append() function
     *  This operator allows you to use syntax like:
     *    mystring += "hello world";
     *  @see append()
     */
    mstring &operator+= (const mstring &rhs)
	{ append(rhs); return *this; }

    /** Operator alias for the append() function
     *  This operator allows you to use syntax like:
     *    mystring << "hello world";
     *  @see append()
     */
    mstring &operator<< (const mstring &rhs)
	{ append(rhs); return *this; }

    // ALL return -1 if not found, or the offset
    // of the requested char/string.

    /** Find first instance of any of the given characters
     *  @param str Sequence of characters to search for.
     *  @param len Length of the sequence of characters.
     *  @return The offset (from 0) where the any of the specified
		characters were found, or -1 none were found.
     */
    int find_first_of(const char *str, const size_t len) const;

    /** Find last instance of any of the given characters
     *  @param str Sequence of characters to search for.
     *  @param len Length of the sequence of characters.
     *  @return The offset (from 0) where the any of the specified
		characters were found, or -1 none were found.
     */
    int find_last_of(const char *str, const size_t len) const;

    /** Find first instance of any character NOT given
     *  @param str Sequence of characters to ignore.
     *  @param len Length of the sequence of characters.
     *  @return The offset (from 0) where any character but those
		specified were found, or -1 none were found.
     */
    int find_first_not_of(const char *str, const size_t len) const;

    /** Find last instance of any character NOT given
     *  @param str Sequence of characters to ignore.
     *  @param len Length of the sequence of characters.
     *  @return The offset (from 0) where any character but those
		specified were found, or -1 none were found.
     */
    int find_last_not_of(const char *str, const size_t len) const;

    /** Alias for the find_first_of function
     *  This function handles not only mstring, but all data types
     *  that we have valid constructors for.
     *  @param in The sequence of characters to search for.
     *  @see find_first_of()
     */
    int find_first_of(const mstring &in) const
	{ return find_first_of(in.i_str, in.i_len); }

    /** Alias for the find_last_of function
     *  This function handles not only mstring, but all data types
     *  that we have valid constructors for.
     *  @param in The sequence of characters to search for.
     *  @see find_last_of()
     */
    int find_last_of(const mstring &in) const
	{ return find_last_of(in.i_str, in.i_len); }

    /** Alias for the find_first_not_of function
     *  This function handles not only mstring, but all data types
     *  that we have valid constructors for.
     *  @param in The sequence of characters to ignore.
     *  @see find_first_not_of()
     */
    int find_first_not_of(const mstring &in) const
	{ return find_first_not_of(in.i_str, in.i_len); }

    /** Alias for the find_last_not_of function
     *  This function handles not only mstring, but all data types
     *  that we have valid constructors for.
     *  @param in The sequence of characters to ignore.
     *  @see find_last_not_of()
     */
    int find_last_not_of(const mstring &in) const
	{ return find_last_not_of(in.i_str, in.i_len); }

    /** Find an instance of the specified string from the beginning
     *  This does not quite work the same as STL string's find,
     *  as the optional second parameter is the occurance number
     *  to find, not the starting offset to search from.
     *  @param str String to search for.
     *  @param occurance Instance of string to search for.
     *  @return Offset (from 0) of the starting location of the
		found string, or -1 if not found.
     */
    int find(const mstring &str, int occurance = 1) const;

    /** Find an instance of the specified string from the end
     *  This does not quite work the same as STL string's find,
     *  as the optional second parameter is the occurance number
     *  to find, not the starting offset to search from.
     *  @param str String to search for.
     *  @param occurance Instance of string to search for.
     *  @return Offset (from 0) of the starting location of the
		found string, or -1 if not found.
     */
    int rfind(const mstring &str, int occurance = 1) const;

    /** Replace instances of the specified string with another string
     *  @param i_find String to search for.
     *  @param i_replace String to replace search string with.
     *  @param all If true, replace ALL instances, otherwise only
		replace the first instance, default is true.
     */
    void replace(const mstring &i_find, const mstring &i_replace, const bool all = true);

    /** Replace a specific segment of current contents with a string
     *  @param begin Offset (from 0) of the first character to replace.
     *  @param end Offset (from 0) of the last character to replace.
     *  @param i_replace String to put in place of the specified characters.
     *  @param len Length of replacement string.
     */
    void replace(const int begin, const int end, const char *i_replace, const size_t len)
	{
	    erase(begin, end);
	    insert(begin, i_replace, len);
	}

    /** Alias for the replace function
     *  This function handles not only mstring, but all data types
     *  that we have valid constructors for.
     *  @param begin Offset (from 0) of the first character to replace.
     *  @param end Offset (from 0) of the last character to replace.
     *  @param i_replace String to put in place of the specified characters.
     *  @see replace()
     */
    void replace(const int begin, const int end, const mstring &i_replace)
	{ replace(begin, end, i_replace.i_str, i_replace.i_len); }

    /** Replace a single character in the current contents
     *  @param offset Offset (from 0) of the character to replace.
     *  @param c Character to put in place of the existing one.
     *  @return true if the replace took place successfully.
     */
    bool replace(const size_t offs, const char c);

    /** Obtain a substring from within the current contents
     *  @param nFirst Offset (from 0) of the first character in the substring.
     *  @param nCount Length of the desired substring.
     *  @return A new mstring, which contains the specified substring.
     */
    mstring substr(int nFirst, int nCount) const;

    /** An alias to the find() command.
     *  @param in String to search for.
     *  @return true if the string was found, false otherwise.
     *  @see find()
     */
    bool Contains(const mstring &in) const
	{ return (find(in) >= 0); }

    /** Use printf style format codes to build the current contents
     *  The formatting string may not be NULL, and you must supply as many
     *  parameters as '%' symbols in the formatting string.  See the format
     *  man page for more information on printf-style formatting.  The result
     *  replaces the the current contents.
     *  @param fmt The formatting string to apply using all other parameters.
     *  @return The length of the fully formatted string.
     */
    int Format(const char *fmt, ...);

    /** An alias for the Format() command.
     *  @param fmt The formatting string to apply using all other parameters.
     *  @param argptr The varargs array generated from a '...' parameter list.
     *  @return The length of the fully formatted string.
     *  @see Format()
     */
    int FormatV(const char *fmt, va_list argptr);

    /** @return true if there is no white space in the current contents */
    bool IsWord() const;

    /** @return true if the current contents only contains numbers, with an
	optional leading '+' or '-', and an optionaldecimal point. */
    bool IsNumber() const;

    /** @return true if all characters are standard ASCII characters */
    bool IsAscii() const;

    /** @return true if the current contents can be recognized as a boolean
	value (true, false, on, off, yes, no, etc). */
    bool IsBool() const;

    /** @return The boolean value contained inside if the value inside is
	indeed a boolean value, false is returned if it is not a boolean. */
    bool GetBool() const;

    /** @return An upper case version of the current contents */
    mstring UpperCase() const;

    /** @return A lower case version of the current contents */
    mstring LowerCase() const;

    /** Convert the current contents to all upper case */
    void MakeUpper();

    /** Convert the current contents to all lower case */
    void MakeLower();

    /** Cound how many times a string appears in the current contents
     *  @param in The string to search for.
     *  @param NoCase If true, the search will be case insensative.
     *  @return The amount of times the specified string appears
		in the current contents.
     */
    int Occurances(const mstring &in, const bool NoCase = false) const;

    /** An alias to the find() command.
     *  This function handles not only mstring, but all data types
     *  that we have valid constructors for.
     *  @param in The string to search for.
     *  @param NoCase If true, the search will be case insensative.
     *  @param occurance The instance to look for.
     *  @return Offset (from 0) of the starting location of the
		found string, or -1 if not found.
     *  @see find()
     */
    int Find(const mstring &in, const bool NoCase = false, const int occurance = 1) const;

    /** An alias to the find() command, reversed
     *  This function handles not only mstring, but all data types
     *  that we have valid constructors for.  This function will
     *  do its search from the end of the string, so occurance 1
     *  is the last occurance of the search string, etc.
     *  @param in The string to search for.
     *  @param NoCase If true, the search will be case insensative.
     *  @param occurance The instance to look for.
     *  @return Offset (from 0) of the starting location of the
		found string, or -1 if not found.
     *  @see find()
     */
    int RevFind(const mstring &in, const bool NoCase = false, const int occurance = 1) const;

    /** An alias of the compare() function
     *  This function handles not only mstring, but all data types
     *  that we have valid constructors for.
     *  @param in The string to compare against
     *  @param NoCase If true, the comparison will be case insensative.
     *  @return 0 for an exact match, below 0 when the string is
		alphabetically 'higher' than the current contents, and
		above 0, when it is alphabetically 'lower'.
     *  @see compare()
     */
    int Cmp(const mstring &in, const bool NoCase = false) const;

    /** An alias of the compare() function
     *  This function handles not only mstring, but all data types
     *  that we have valid constructors for.
     *  @param in The string to compare against
     *  @param NoCase If true, the comparison will be case insensative.
     *  @return true if the current contents and string are the same.
     *  @see compare()
     */
    bool IsSameAs(const mstring &in, const bool NoCase = false) const
	{ return (Cmp(in, NoCase)==0); }

    /** An alias of the match_wild() function
     *  This function handles not only mstring, but all data types
     *  that we have valid constructors for.
     *  @param in The wildcard pattern to match.  A '*' represents
	0 or more of any character.  A '?' represents exactly 1
	character.
     *  @param NoCase If true, the comparison will be case insensative.
     *  @return true if the pattern matched, otherwise false.
     *  @see match_wild()
     */
    bool Matches(const mstring &in, const bool NoCase = false) const
	{ return match_wild(in.c_str(), c_str(), NoCase); }

    /** An alias of the replace() function
     *  This function handles not only mstring, but all data types
     *  that we have valid constructors for.
     *  @param in String to remove from the current contents.
     *  @param All true if ALL instances should be removed, false if
		only the first instance should be, defaults to true.
     *  @see replace()
     */
    void Remove(const mstring &in, const bool All = true)
	{ replace(in, "", All); }

    /** An alias of the erase() function
     *  This function handles not only mstring, but all data types
     *  that we have valid constructors for.
     *  @param pos Offset (from 0) at which we will truncate.
     *  @param right true if we should erase everything from
		the offset to the end of the current contents,
		false if we should erase everything from the
		beginning of the current contents to the offset,
		defaults to true.
     *  @see erase()
     */
    void Truncate(const size_t pos, const bool right = true)
	{
	    if (right)
		erase(pos);
	    else
		erase(0, pos);
	}

    /** Remove all excess delimiters from the current contents
     *  @param right true if we should remove all excess characters
		from the end of the current contents, false if from
		the beginning.
     *  @param delims Characters we consider delimiters, and therefore
		trimworthy.
     */
    void Trim(const bool right=true, const mstring &delims = " \n\r\t");

    /** Get a trimmed copy of the current contents, without modifying them.
     *  @param right true if we should remove all excess characters
		from the end of the current contents, false if from
		the beginning.
     *  @param delims Characters we consider delimiters, and therefore
		trimworthy.
     *  @return an mstring of the current contents, after Trim().
     *  @see Trim()
     */
    mstring Strip(const bool right=true, const mstring &delims = " \n\r\t") const;

    /** Get all contents before an instance of a string
     *  @param in String to search for
     *  @param occurance Instance of string we are looking for
     *  @return All contents before the specified instance of the string
     */
    mstring Before(const mstring &in, const int occurance = 1) const;

    /** Get all contents after an instance of a string
     *  @param in String to search for
     *  @param occurance Instance of string we are looking for
     *  @return All contents after the specified instance of the string
     */
    mstring After(const mstring &in, const int occurance = 1) const;

    /** Get all contents before an instance of a string from the end
     *  @param in String to search for
     *  @param occurance Instance (from the end) of string we are looking for
     *  @return All contents before the specified instance of the string
     */
    mstring RevBefore(const mstring &in, const int occurance = 1) const;

    /** Get all contents after an instance of a string from the end
     *  @param in String to search for
     *  @param occurance Instance (from the end) of string we are looking for
     *  @return All contents after the specified instance of the string
     */
    mstring RevAfter(const mstring &in, const int occurance = 1) const;

    /** An alias for the substr() command
     *  If from or to are beyond the limits of the current contents, the
     *  limits are assumed.
     *  @param from Offset (from 0) of the first character in the substring.
		Defaults to 0.
     *  @param to Offset (from 0) of the last character in the substring.
		Defaults to the end of the current contents.
     *  @return A new mstring, which contains the specified substring.
     */
    mstring SubString(int from = 0, int to = -1) const;

    /** An alias for the SubString() command
     *  @param pos How many characters to obtain.
     *  @return A new mstring with the first X characters of the current
		contents.
     */
    mstring Left(const int pos) const
	{ return SubString(0, pos-1); }

    /** An alias for the SubString() command
     *  @param pos How many characters to obtain.
     *  @return A new mstring with the last X characters of the current
		contents.
     */
    mstring Right(const int pos) const
	{ return SubString(pos); }

    /** Count how many 'words' given a delimiter
     *  @param delim The delimiters to use (may have multipal delimiters)
     *  @param assemble If true, delimiters next to each other will be
		counted as one delimiter, otherwise each delimiter
		counts.  Defaults to true.
     *  @return Amount of 'words' given the specified delimiter.
     */
    unsigned int WordCount(const mstring &delim, const bool assemble = true) const;

    /** Get a specific 'word' given a delimiter
     *  @param count The word number you want (starting at 1).
     *  @param delim The delimiters to use (may have multipal delimiters)
     *  @param assemble If true, delimiters next to each other will be
		counted as one delimiter, otherwise each delimiter
		counts.  Defaults to true.
     *  @return A new mstring with the 'word' specified, or "" if that
		word number does not exist.
     */
    mstring ExtractWord(const unsigned int count, const mstring &delim,
					const bool assemble = true) const;

    /** Get the possition of a specific 'word' given a delimiter
     *  @param count The word number you want (starting at 1).
     *  @param delim The delimiters to use (may have multipal delimiters)
     *  @param assemble If true, delimiters next to each other will be
		counted as one delimiter, otherwise each delimiter
		counts.  Defaults to true.
     *  @return An offset (from 0) of the start possition of the specified
		'word', or -1 if that word number does not exist.
     */
    int WordPosition(const unsigned int count, const mstring &delim,
					const bool assemble = true) const;

    /** Create a vector from the current contents given a delimiter
     *  @param delim The delimiters to use (may have multipal delimiters)
     *  @param assemble If true, delimiters next to each other will be
		counted as one delimiter, otherwise each delimiter
		counts.  Defaults to true.
     *  @return A vector of mstrings, which is the current contents broken
		up by delimiter.
     */
    vector<mstring> Vector(const mstring &delim, const bool assemble = true) const;

    /** Create a list from the current contents given a delimiter
     *  @param delim The delimiters to use (may have multipal delimiters)
     *  @param assemble If true, delimiters next to each other will be
		counted as one delimiter, otherwise each delimiter
		counts.  Defaults to true.
     *  @return A list of mstrings, which is the current contents broken
		up by delimiter.
     */
    list<mstring> List(const mstring &delim, const bool assemble = true) const;

    /** Replace current contents with the contents of a vector
     *  @param text The vector you wish to replace the current contets with.
     *  @param deilm The delimiter to put in between each entry of the vector.
     */
    void Assemble(const vector<mstring> &text, const mstring &delim = " ");

    /** Replace current contents with the contents of a list
     *  @param text The list you wish to replace the current contets with.
     *  @param deilm The delimiter to put in between each entry of the list.
     */
    void Assemble(const list<mstring> &text, const mstring &delim = " ");
};

/** @defgroup overloads Operator Overloads
 * @{
 */
inline bool operator== (const mstring &lhs, const mstring &rhs)
	{ return (lhs.compare(rhs) == 0); }
inline bool operator!= (const mstring &lhs, const mstring &rhs)
	{ return (lhs.compare(rhs) != 0); }
inline bool operator< (const mstring &lhs, const mstring &rhs)
	{ return (lhs.compare(rhs) < 0); }
inline bool operator> (const mstring &lhs, const mstring &rhs)
	{ return (lhs.compare(rhs) > 0); }
inline bool operator<= (const mstring &lhs, const mstring &rhs)
	{ return (lhs.compare(rhs) <= 0); }
inline bool operator>= (const mstring &lhs, const mstring &rhs)
	{ return (lhs.compare(rhs) >= 0); }
inline mstring operator+ (const mstring &lhs, const mstring &rhs)
{
    mstring str(lhs);
    str.append(rhs.c_str(), rhs.length());
    return str;
}
/** @} */

#define OPERATOR_SET(TYPE)						\
	inline bool operator== (const TYPE lhs, const mstring &rhs)	\
		{ return (mstring(lhs) == rhs); }			\
	inline bool operator!= (const TYPE lhs, const mstring &rhs)	\
		{ return (mstring(lhs) != rhs); }			\
	inline bool operator< (const TYPE lhs, const mstring &rhs)	\
		{ return (mstring(lhs) < rhs); }			\
	inline bool operator> (const TYPE lhs, const mstring &rhs)	\
		{ return (mstring(lhs) > rhs); }			\
	inline bool operator<= (const TYPE lhs, const mstring &rhs)	\
		{ return (mstring(lhs) <= rhs); }			\
	inline bool operator>= (const TYPE lhs, const mstring &rhs)	\
		{ return (mstring(lhs) >= rhs); }			\
	inline mstring operator+ (const TYPE lhs, const mstring &rhs)	\
		{ return (mstring(lhs) + rhs); }			\
	inline bool operator== (const mstring &lhs, const TYPE rhs)	\
		{ return (lhs == mstring(rhs)); }			\
	inline bool operator!= (const mstring &lhs, const TYPE rhs)	\
		{ return (lhs != mstring(rhs)); }			\
	inline bool operator< (const mstring &lhs, const TYPE rhs)	\
		{ return (lhs < mstring(rhs)); }			\
	inline bool operator> (const mstring &lhs, const TYPE rhs)	\
		{ return (lhs > mstring(rhs)); }			\
	inline bool operator<= (const mstring &lhs, const TYPE rhs)	\
		{ return (lhs <= mstring(rhs)); }			\
	inline bool operator>= (const mstring &lhs, const TYPE rhs)	\
		{ return (lhs >= mstring(rhs)); }			\
	inline mstring operator+ (const mstring &lhs, const TYPE rhs)	\
		{ return (lhs + mstring(rhs)); }			\

OPERATOR_SET(string &)
OPERATOR_SET(char *)
OPERATOR_SET(char)
OPERATOR_SET(unsigned char)
OPERATOR_SET(int)
OPERATOR_SET(unsigned int)
OPERATOR_SET(long)
OPERATOR_SET(unsigned long)
OPERATOR_SET(float)
OPERATOR_SET(double)
OPERATOR_SET(vector<mstring> &)
OPERATOR_SET(list<mstring> &)

#endif
