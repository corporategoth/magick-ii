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
#define RCSID(x,y) const char *rcsid_mstring_cpp_ ## x () { return y; }
RCSID(mstring_cpp, "@(#)$Id$");
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.116  2001/12/12 03:31:15  prez
** Re-wrote the occurances/find/replace functions in mstring to actually work
** with contents that includes a binary 0.  Also fixed PreParse in mconfig.
**
** Revision 1.115  2001/12/07 02:51:39  prez
** Added doxygen comments to mstring, and removed doxygen generated stuff
** from CVS -- you can now just generate it yourself with the config file.
**
** Revision 1.114  2001/11/23 21:02:56  prez
** Fixed problem in FormatV that caused corruptions in strings.
**
** Revision 1.113  2001/11/22 17:54:09  prez
** Fixed potential fatal error in mstring insert
**
** Revision 1.112  2001/11/22 17:32:18  prez
** Some fixes to lockable for mpatrol, and mstring overwriting its own memory.
**
** Revision 1.111  2001/11/12 01:05:03  prez
** Added new warning flags, and changed code to reduce watnings ...
**
** Revision 1.110  2001/11/03 21:02:53  prez
** Mammoth change, including ALL changes for beta12, and all stuff done during
** the time GOTH.NET was down ... approx. 3 months.  Includes EPONA conv utils.
**
** Revision 1.109  2001/07/21 18:09:44  prez
** Fixed IsBool in mstring and made SVINFO actually give a GMT timestamp.
**
** Revision 1.108  2001/07/16 03:36:14  prez
** Got rid of mstring's strcmp, now using memcmp.  Also did a little
** tweaking with the protocol support.
**
** Revision 1.107  2001/06/17 05:22:12  prez
** Resolved compatability issues with ACE 5.1.17
**
** Revision 1.106  2001/06/15 07:20:40  prez
** Fixed windows compiling -- now works with MS Visual Studio 6.0
**
** Revision 1.105  2001/05/14 04:46:32  prez
** Changed to use 3BF (3 * blowfish) encryption.  DES removed totally.
**
** Revision 1.104  2001/05/05 17:33:58  prez
** Changed log outputs from printf-style to tokenized style files.
** Now use LOG/NLOG/SLOG/SNLOG rather than just LOG for output.  All
** formatting must be done BEFORE its sent to the logger (use fmstring).
**
** Revision 1.103  2001/05/04 03:43:33  prez
** Fixed UMODE problems (re-oper) and problems in mstring erase
**
** Revision 1.102  2001/04/09 07:52:22  prez
** Fixed /nickserv.  Fixed cordump in nick expiry.  Fixed slight bugs in mstring.
**
** Revision 1.101  2001/04/08 18:53:10  prez
** It now all compiles and RUNS with -fno-default-inline OFF.
**
** Revision 1.100  2001/04/02 02:11:23  prez
** Fixed up some inlining, and added better excption handling
**
** Revision 1.99  2001/03/27 07:04:32  prez
** All maps have been hidden, and are now only accessable via. access functions.
**
** Revision 1.98  2001/03/20 14:22:14  prez
** Finished phase 1 of efficiancy updates, we now pass mstring/mDateTime's
** by reference all over the place.  Next step is to stop using operator=
** to initialise (ie. use mstring blah(mstring) not mstring blah = mstring).
**
** Revision 1.97  2001/03/08 08:07:41  ungod
** fixes for bcc 5.5
**
** Revision 1.96  2001/03/04 02:04:14  prez
** Made mstring a little more succinct ... and added vector/list operations
**
** Revision 1.95  2001/03/02 05:24:41  prez
** HEAPS of modifications, including synching up my own archive.
**
** Revision 1.94  2001/02/11 07:41:28  prez
** Enhansed support for server numerics, specifically for Unreal.
**
** Revision 1.93  2001/02/03 02:21:34  prez
** Loads of changes, including adding ALLOW to ini file, cleaning up
** the includes, RCSID, and much more.  Also cleaned up most warnings.
**
** Revision 1.92  2001/01/16 02:30:08  prez
** Fixed the IsNumber so an IP address or +/- number matches
**
** Revision 1.91  2001/01/01 05:32:44  prez
** Updated copywrights.  Added 'reversed help' syntax (so ACCESS HELP ==
** HELP ACCESS).
**
** Revision 1.90  2000/12/25 06:36:14  prez
** Added locking around the threadtoself map, and removed a bunch of
** defines from mstring (while keeping it the same!)
**
** Revision 1.89  2000/12/19 07:24:53  prez
** Massive updates.  Linux works again, added akill reject threshold, and
** lots of other stuff -- almost ready for b6 -- first beta after the
** re-written strings class.  Also now using log adapter!
**
** Revision 1.88  2000/12/10 07:34:51  prez
** Added a flush to the no memory error output.
**
** Revision 1.87  2000/12/10 02:30:05  prez
** More misc fixes on mstring ... one of them fixing single char 0 strings.
**
** Revision 1.86  2000/12/09 20:16:41  prez
** Fixed SubString and Left to have correct count/end possitions.  Also
** adjusted rest of source to follow suit.
**
** Revision 1.85  2000/12/09 16:45:37  prez
** Fixed rfind in mstring.
**
** Revision 1.84  2000/12/09 15:40:13  prez
** Fixed some stuff with mstring (ie. Contains called find_first_of
** not find, and the makeupper/makelower calls had != NULL not == NULL).
**
** Revision 1.83  2000/12/09 12:53:56  prez
** Forgot to add the 'return' to all find_first_of, find_last_of,
** find_first_not_of and find_last_not_of calls ...
**
** Revision 1.82  2000/12/09 10:34:14  prez
** Added ACE style alloc as an option -- COPY STILL HAS PRINTF's.
**
** Revision 1.81  2000/12/05 07:56:16  prez
** OOps ... forgot to #if / #endif something ...
**
** Revision 1.80  2000/12/05 07:49:13  prez
** Added the ability (in the .h file) to chose what memory scheme to use.
** This is necessary for testing to try and get mstring working!
**
** Revision 1.79  2000/11/28 10:15:05  prez
** Trying to figure out why the free list doesnt work in mmemory.
** ALOT OF PRINTF'S - YOU'VE BEEN WARNED, OUTPUT STDOUT TO A FILE!
**
** Revision 1.78  2000/11/09 11:03:06  prez
** OOps, forgot some printf's :)
**
** Revision 1.77  2000/11/09 10:58:19  prez
** THINK I have it working again ... with the free list.
** Will check, still thinking of sorting free list by size.
**
** Revision 1.76  2000/10/26 07:59:52  prez
** The goddamn memory system and mstring WORK!  Well, so far ;)
**
** Revision 1.75  2000/10/18 18:46:33  prez
** Well, mstring still coredumps, but it gets past the initial loading of
** all the STATIC (or const) strings, etc -- now its coring on loading a
** file (or possibly language.h or something).  Still investigating.
**
** Revision 1.74  2000/10/15 03:29:27  prez
** Mods to the memory system, LOTS of printf's to try and figure out why
** the damn thing coredumps on init.
**
** Revision 1.73  2000/10/14 04:25:31  prez
** Added mmemory.h -- MemCluster and the MemoryManager are now in it.
** TODO - make mstring use MemoryManager.
**
** Revision 1.72  2000/10/11 08:38:10  prez
** Changes so mstring works better ... kinda
**
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
const mstring IRC_CTCP(static_cast<char>(1));		// ^A
const mstring IRC_Bold(static_cast<char>(2));		// ^B
const mstring IRC_Underline(static_cast<char>(31));	// ^_
const mstring IRC_Reverse(static_cast<char>(22));	// ^V
const mstring IRC_Color(static_cast<char>(3));		// ^C
const mstring IRC_Off(static_cast<char>(15));		// ^O

#ifdef MAGICK_HAS_EXCEPTIONS
char *mstring::alloc(const size_t sz) throw(mstring_noalloc)
#else
char *mstring::alloc(const size_t sz)
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
	out = new char[sz];
    }
    catch (ACE_bad_alloc &e)
    {
	e.what();
	errno = ENOMEM;
    }
# else
    out = new char[sz];
    if (out == NULL)
    {
	errno = ENOMEM;
    }
# endif

#elif ALLOCTYPE == 2

    /* Standard C++ Allocation */
    out = new char[sz];

#else

    /* Standard C Allocation */
    out = (char *) malloc(sz);

#endif

    if (out == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	char reason[1024];
	ACE_OS::sprintf(reason, "Could not allocate mstring %d: %s",
						errno, strerror(errno));
	reason[1023]=0;
	throw mstring_noalloc(reason);
#else
	ACE_OS::fprintf(stderr, "Could not allocate mstring %d: %s",
						errno, strerror(errno));
	ACE_OS::fflush(stderr);
#endif
    }

    return out;
}

#ifdef MAGICK_HAS_EXCEPTIONS
void mstring::dealloc(char * & in) throw(mstring_nodealloc)
#else
void mstring::dealloc(char * & in)
#endif
{
    if (in == NULL)
	return;

#if ALLOCTYPE == 4

    /* Use our own Memory Map for clustered alloc */
    memory_area.dealloc(in);

#elif ALLOCTYPE == 3

    /* Duplicate ACE's new, but with no return's. */
    delete [] in;

#elif ALLOCTYPE == 2

    /* Standard C++ Allocation */
    delete [] in;

#else

    /* Standard C Allocation */
    free(in);

#endif

    in = NULL;
}

void mstring::lock_read() const
{
#ifdef MSTRING_LOCKS_WORK
    if (i_lock != NULL && i_lock->acquire_read() < 0)
    {
	ACE_OS::fprintf(stderr, "WARNING: Failed to acquire read lock on line %d of %s\n", __LINE__, __FILE__);
	ACE_OS::fflush(stderr);
    }
#endif
}

void mstring::lock_write() const
{
#ifdef MSTRING_LOCKS_WORK
    if (i_lock != NULL && i_lock->acquire_write() < 0)
    {
	ACE_OS::fprintf(stderr, "WARNING: Failed to acquire write lock on line %d of %s\n", __LINE__, __FILE__);
	ACE_OS::fflush(stderr);
    }
#endif
}

void mstring::lock_rel() const
{
#ifdef MSTRING_LOCKS_WORK
    if (i_lock != NULL && i_lock->release() < 0)
    {
	ACE_OS::fprintf(stderr, "WARNING: Failed to release lock on line %d of %s\n", __LINE__, __FILE__);
	ACE_OS::fflush(stderr);
    }
#endif
}

void mstring::init()
{
#ifdef MSTRING_LOCKS_WORK
    char lockname[30];
    UNIQ_LOCK_TYPE uniq_lock("mstring_lock_id");
    uniq_lock.acquire();
    next_lock_id++;
    sprintf(lockname, "mstring_%08x%08x", time(NULL), next_lock_id);
    uniq_lock.release();
    i_lock = new LOCK_TYPE(lockname);
#endif

    lock_write();
    i_len = 0;
    i_res = 0;
    i_str = NULL;
    lock_rel();
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

    if (i_res==0)
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

    if (end < 0 || end >= static_cast<int>(i_len))
	if (begin >= static_cast<int>(i_len))
	{
	    lock_rel();
	    return;
	}
	else
	    end = i_len-1;
    if (begin < 0)
	begin = 0;
    if (begin > end)
    {
	i = begin;
	begin = end;
	end = i;
    }
    end++;

    if (begin > 0 || end < static_cast<int>(i_len))
    {
	i=0;
	if (i_res==0)
	    i_res = sizeof(int);
	while (i_res >= sizeof(int) && i_res / 2 > i_len - (end-begin))
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

	if (end < static_cast<int>(i_len))
	    memmove(&tmp[i], &i_str[end], i_len-end);
	i_len -= end-begin;
	memset(&tmp[i_len], 0, i_res-i_len);
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
    if (i_res==0)
	i_res = sizeof(int);
    while (i_res <= i_len + len)
	i_res *= 2;

    if (oldres != i_res)
    {
	tmp = alloc(i_res);
	if(tmp == NULL)
	{
	    lock_rel();
	    return;
	}
	memset(tmp, 0, i_res);
    }
    else
	tmp = i_str;

    i=0;
    if (pos > 0 && tmp != i_str)
	memcpy(tmp, i_str, pos);
    i += pos;
    memmove(&tmp[i+len], &i_str[pos], i_len-pos);
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
int mstring::compare (const char *in, const size_t len) const
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

void mstring::swap(mstring &in)
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

    return static_cast<const char *>(retval);
}

const unsigned char *mstring::uc_str() const
{
    unsigned char *retval = reinterpret_cast<unsigned char *>(const_cast<char *>(""));

    lock_read();
    if (i_str != NULL)
	retval = reinterpret_cast<unsigned char *>(i_str);
    lock_rel();

    return static_cast<const unsigned char *>(retval);
}

const char mstring::first(size_t off) const
{
    char retval = 0;

    lock_read();
    if (i_str != NULL && off > 0 && off <= i_len)
	retval = i_str[off - 1];
    lock_rel();

    return static_cast<const char>(retval);
}

const char mstring::last(size_t off) const
{
    char retval = 0;

    lock_read();
    if (i_str != NULL && off > 0 && off <= i_len)
	retval = i_str[i_len - off];
    lock_rel();

    return static_cast<const char>(retval);
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

    for (i=0; i < len; i++)
    {
	char *ptr = static_cast<char *>(memchr(i_str, str[i], i_len));
	if (ptr != NULL && ptr - i_str < retval)
	    retval = ptr - i_str;
    }
    if (retval > static_cast<int>(i_len))
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

    for (i=0; i<len; i++)
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
    tmp = alloc(len+1);
    if (tmp == NULL)
    {
	lock_rel();
	return -1;
    }
    memcpy(tmp, str, len);
    tmp[len] = 0;

    for (i=0; i<i_len; i++)
    {
	if (memchr(tmp, i_str[i], len)==NULL)
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
    tmp = alloc(len+1);
    if (tmp == NULL)
    {
	lock_rel();
	return -1;
    }
    memcpy(tmp, str, len);
    tmp[len] = 0;

    for (i=i_len-1; i>=0; i--)
    {
	if (memchr(tmp, i_str[i], len)==NULL)
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
    end = strstr(i_str, str);
    while (consumed < i_len)
    {
	// Find each instance of the search pattern, at least, up until
	// the first binary 0 we encounter ...
	while (end != NULL)
	{
	    count++;
	    consumed += (end-start) + len;
	    start = end + len;
	    end = strstr(start, str);
	}
	// Accomodate for any binary 0's, by moving past the next binary
	// zero, checking to see if we've used the entire contents, and
	// if we havn't, resuming the above while.
	consumed += strlen(start) + 1;
	start += strlen(start) + 1;
	if (consumed >= i_len)
	    break;
	end = strstr(start, str);
    }
    return count;
}

// Find occurance of full string
int mstring::find(const mstring &str, int occurance) const
{
    int count = 0, retval = -1;
    char *start, *end;

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
    end = strstr(i_str, str.c_str());
    // While we're not at the end of the WHOLE string ...
    while (consumed < i_len)
    {
	// Find each instance of the search pattern, at least, up until
	// the first binary 0 we encounter ... Break when we got what
	// we want.
	while (end != NULL)
	{
	    count++;
	    if (occurance == count)
		break;
	    consumed += (end-start) + str.length();
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
	if (consumed >= i_len)
	    break;
	end = strstr(start, str.c_str());
    }
    if (end != NULL)
	retval = end - i_str;

    lock_rel();
    return retval;
}


// Reverse Find occurance of full string
int mstring::rfind(const mstring &str, int occurance) const
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
void mstring::replace(const mstring &i_find, const mstring &i_replace, const bool all)
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

    if (i_res==0)
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
    end = strstr(i_str, i_find.c_str());
    size_t i=0, consumed = 0;
    while (consumed < old_len)
    {
	// Find each instance of the search pattern, at least, up until
	// the first binary 0 we encounter ... Break after the first
	// replace if !all ...
	while (end != NULL)
	{
	    if ((end-start) > 0)
	    {
		memcpy(&tmp[i], start, (end-start));
		i += (end-start);
	    }
	    if (i_replace.length())
	    {
		memcpy(&tmp[i], i_replace.c_str(), i_replace.length());
		i += i_replace.length();
	    }
	    consumed += (end-start) + i_find.length();
	    start = end + i_find.length();
	    if (!all)
		break;
	    end = strstr(start, i_find.c_str());
	}
	// Accomodate the situation where !all, simply copy the rest
	// of the REAL string, and be done with it.
	if (end != NULL)
	{
	    if (old_len-(start-i_str) > 0)
		memcpy(&tmp[i], start, old_len-(start-i_str));
	    break;
	}
	// Accomodate for any binary 0's, by moving past the next binary
	// zero, checking to see if we've used the entire contents, and
	// if we havn't, resuming the above while.
	consumed += strlen(start) + 1;
	memcpy(&tmp[i], start, strlen(start)+1);
	i += strlen(start);
	start += strlen(start) + 1;
	if (consumed >= old_len)
	    break;
	end = strstr(start, i_find.c_str());
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
	if ((nCount + nFirst) > static_cast<int>(i_len))
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

    for (size_t i=0; i<i_len; i++)
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
    size_t i=0;
    if (i_str[i] == '-' || i_str[i] == '+')
	i++;

    bool gotdec = false;
    for (; i<i_len; i++)
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

    for (size_t i=0; i<i_len; i++)
	if (!isascii(i_str[i]))
	    retval = false;

    lock_rel();
    return retval;
}

bool mstring::IsBool() const
{
  if (IsSameAs("true", true) || IsSameAs("on", true) ||
      IsSameAs("yes", true) || IsSameAs("y", true) ||
      IsSameAs("t", true) || IsSameAs("1") ||
      IsSameAs("false", true) || IsSameAs("off", true) ||
      IsSameAs("no", true) || IsSameAs("n", true) ||
      IsSameAs("f", true) || IsSameAs("0"))
    return true;
  else
    return false;
}

bool mstring::GetBool() const
{
  if (IsSameAs("true", true) || IsSameAs("on", true) ||
      IsSameAs("yes", true) || IsSameAs("y", true) ||
      IsSameAs("t", true) || IsSameAs("1"))
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

    for (size_t i=0; i<i_len; i++)
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

    for (size_t i=0; i<i_len; i++)
	i_str[i] = tolower(i_str[i]);
    lock_rel();
}


int mstring::Occurances(const mstring &in, const bool NoCase) const
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

int mstring::Find(const mstring &in, const bool NoCase, const int occurance) const
{
    if (NoCase)
    {
	mstring tmp(in);
	return UpperCase().find(tmp.UpperCase().c_str(), occurance);
    }
    else
	return find(in.c_str(), occurance);
}

int mstring::RevFind(const mstring &in, const bool NoCase, const int occurance) const
{
    if (NoCase)
    {
	mstring tmp(in);
	return UpperCase().rfind(tmp.UpperCase().c_str(), occurance);
    }
    else
	return rfind(in.c_str(), occurance);
}

int mstring::Cmp(const mstring &in, const bool NoCase) const
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

void mstring::Trim(const bool right, const mstring &delims)
{
    int pos = 0;
    if (right)
    {
	pos = find_last_not_of(delims.c_str());
	if (pos > 0)
	    erase(pos+1);
    }
    else
    {
	pos = find_first_not_of(delims.c_str());
	if (pos > 0)
	    erase(0, pos-1);
    }
}

mstring mstring::Strip(const bool right, const mstring &deilms) const
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
	len = mstring::vsnprintf(buffer, sz, fmt, argptr);
	if (buffer[sz-1] == 0 && len >= 0 && len < sz)
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
	do {
	    sz *= 2;
	} while (len > sz);
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


mstring mstring::Before(const mstring &in, const int occurance) const
{
    int m_pos = Find(in,false,occurance);
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

mstring mstring::After(const mstring &in, const int occurance) const
{
    int m_pos = Find(in,false,occurance);
    if (m_pos >= 0)
	return Right(m_pos+in.i_len);
    else
    {
	lock_read();
	mstring retval(*this);
	lock_rel();
	return retval;
    }
}

mstring mstring::RevBefore(const mstring &in, const int occurance) const
{
    int m_pos = RevFind(in,false,occurance);
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

mstring mstring::RevAfter(const mstring &in, const int occurance) const
{
    int m_pos = RevFind(in,false,occurance);
    if (m_pos >= 0)
	return Right(m_pos+in.i_len);
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
	if (from < static_cast<int>(i_len))
	    to = i_len-1;
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
    return substr(from, to-from+1);
}


unsigned int mstring::WordCount(const mstring &delim, const bool assemble) const
{
    int Result=0;
    size_t i=0;
    lock_read();
    while(i<i_len)
    {
	while(i<i_len && assemble && delim.Contains(i_str[i]))
	    i++;
	if(i<i_len)
	    Result++;
	while(i<i_len && !delim.Contains(i_str[i]))
	    i++;
    }
    lock_rel();
    return Result;
}

mstring mstring::ExtractWord(const unsigned int count, const mstring &delim,
						const bool assemble) const
{
    int i, begin;
    begin=WordPosition(count, delim, assemble);
    if(begin!=-1)
    {
	i=begin;
	lock_read();
	while(i < static_cast<int>(i_len) &&
		!delim.Contains(i_str[static_cast<unsigned int>(i)]))
	    i++;
	if (i!=begin)
	{
	    lock_rel();
	    return SubString(begin, i-1);
	}
	lock_rel();
    }
    return "";
}

int mstring::WordPosition(const unsigned int count, const mstring &delim,
						const bool assemble) const
{
    unsigned int i=0,cnt=0;
    int Result=-1;
    lock_read();
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
    lock_rel();
    return Result;
}

vector<mstring> mstring::Vector(const mstring &delim, const bool assemble) const
{
    vector<mstring> Result;
    size_t begin = 0, i=0;
    lock_read();
    while(i<i_len)
    {
	while(i<i_len && assemble && delim.Contains(i_str[i]))
	    i++;
	if(i<i_len)
	    begin = i;
	while(i<i_len && !delim.Contains(i_str[i]))
	    i++;
	if (i!=begin)
	    Result.push_back(SubString(begin, i-1));
    }
    lock_rel();
    return Result;
}

list<mstring> mstring::List(const mstring &delim, bool const assemble) const
{
    list<mstring> Result;
    size_t begin = 0, i=0;
    lock_read();
    while(i<i_len)
    {
	while(i<i_len && assemble && delim.Contains(i_str[i]))
	    i++;
	if(i<i_len)
	    begin = i;
	while(i<i_len && !delim.Contains(i_str[i]))
	    i++;
	if (i!=begin)
	    Result.push_back(SubString(begin, i-1));
    }
    lock_rel();
    return Result;
}

void mstring::Assemble(const vector<mstring> &text, const mstring &delim)
{
    lock_write();

    if (i_str != NULL)
	dealloc(i_str);

    if (text.size())
    {
	vector<mstring>::const_iterator iter;
	i_len += text.size()-1 * delim.length();
	for (iter=text.begin(); iter != text.end(); iter++)
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

void mstring::Assemble(const list<mstring> &text, const mstring &delim)
{
    lock_write();

    if (i_str != NULL)
	dealloc(i_str);

    if (text.size())
    {
	list<mstring>::const_iterator iter;
	i_len += text.size()-1 * delim.length();
	for (iter=text.begin(); iter != text.end(); iter++)
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

int mstring::snprintf(char *buf, const size_t sz, const char *fmt, ...)
{
    va_list argptr;
    va_start(argptr, fmt);

    int iLen = mstring::vsnprintf(buf, sz, fmt, argptr);

    va_end(argptr);
    return iLen;
}

int mstring::vsnprintf(char *buf, const size_t sz, const char *fmt, va_list ap)
{
#ifndef HAVE_VSNPRINTF
    int iLen = ACE_OS::vsprintf(buf, fmt, ap);
#else
    int iLen = ::vsnprintf(buf, sz, fmt, ap);
#endif
    return iLen;
}

/*  Direct from Magick I, credit to Andy Church for writing this.
 *
 *  match_wild:  Attempt to match a string to a pattern which might contain
 *              '*' or '?' wildcards.  Return 1 if the string matches the
 *              pattern, 0 if not.
 */

bool match_wild (const char *pattern, const char *str, bool nocase)
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
		if ((nocase ? (tolower (*s) == tolower (*pattern)) : *s == *pattern)
		    && match_wild (pattern, s, nocase))
		    return true;
		++s;
	    }
	    break;
	default:
	    if ((nocase ? (tolower (*str++) != tolower (c)) : (*str++ != c)))
		return false;
	    break;
	}			/* switch */
    }
}

mstring fmstring (const char *fmt, ...)
{
    mstring tmp;
    va_list argptr;
    va_start(argptr, fmt);
    tmp.FormatV(fmt, argptr);
    va_end(argptr);
    return tmp;
}

/********************************************************/
