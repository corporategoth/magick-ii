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
#ifndef _UTILS_H
#define _UTILS_H
#include "pch.h"
RCSID(utils_h, "@(#) $Id$");

/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.49  2002/01/14 07:16:54  prez
** More pretty printing with a newer indent with C++ fixes (not totally done)
**
** Revision 1.48  2002/01/12 14:42:08  prez
** Pretty-printed all code ... looking at implementing an auto-prettyprint.
**
** Revision 1.47  2002/01/10 19:30:37  prez
** FINALLY finished a MAJOR overhaul ... now have a 'safe pointer', that
** ensures that data being used cannot be deleted while still being used.
**
** Revision 1.46  2001/11/17 03:16:10  prez
** Added MakeDirectory
**
** Revision 1.45  2001/11/12 01:05:01  prez
** Added new warning flags, and changed code to reduce watnings ...
**
** Revision 1.44  2001/07/01 05:02:46  prez
** Added changes to dependancy system so it wouldnt just remove a dependancy
** after the first one was satisfied.
**
** Revision 1.43  2001/05/17 19:18:53  prez
** Added ability to chose GETPASS or SETPASS.
**
** Revision 1.42  2001/05/14 04:46:31  prez
** Changed to use 3BF (3 * blowfish) encryption.  DES removed totally.
**
** Revision 1.41  2001/05/05 17:33:58  prez
** Changed log outputs from printf-style to tokenized style files.
** Now use LOG/NLOG/SLOG/SNLOG rather than just LOG for output.  All
** formatting must be done BEFORE its sent to the logger (use fmstring).
**
** Revision 1.40  2001/04/08 18:53:09  prez
** It now all compiles and RUNS with -fno-default-inline OFF.
**
** Revision 1.39  2001/04/02 02:13:27  prez
** Added inlines, fixed more of the exception code.
**
** Revision 1.38  2001/03/20 14:22:14  prez
** Finished phase 1 of efficiancy updates, we now pass mstring/mDateTime's
** by reference all over the place.  Next step is to stop using operator=
** to initialise (ie. use mstring blah(mstring) not mstring blah = mstring).
**
** Revision 1.37  2001/03/02 05:24:41  prez
** HEAPS of modifications, including synching up my own archive.
**
** Revision 1.36  2001/02/11 07:41:27  prez
** Enhansed support for server numerics, specifically for Unreal.
**
** Revision 1.35  2001/02/03 03:20:33  prez
** Fixed up some differences in previous committed versions ...
**
** Revision 1.32  2000/10/04 07:39:45  prez
** Added MemCluster to speed up lockable, but it cores when we start
** getting real messages -- seemingly in an alloc in the events.
** Lots of printf's left in for debugging (so run as ./magick >output)
**
** Revision 1.31  2000/09/09 02:17:47  prez
** Changed time functions to actuallt accept the source nick as a param
** so that the time values (minutes, etc) can be customized.  Also added
** weeks to the time output.
**
** Revision 1.30  2000/08/22 08:43:39  prez
** Another re-write of locking stuff -- this time to essentially make all
** locks re-entrant ourselves, without relying on implementations to do it.
** Also stops us setting the same lock twice in the same thread.
**
** Revision 1.29  2000/07/28 14:49:35  prez
** Ditched the old wx stuff, mconfig now in use, we're now ready to
** release (only got some conversion tests to do).
**
** Revision 1.28  2000/07/24 16:45:37  ungod
** no message
**
** Revision 1.27  2000/07/11 13:22:18  prez
** Fixed loading/saving -- they now work with encryption and compression.
** Tested, it works too!  Now all we need to do is fix the loading, and
** we're set ... :))
**
** Revision 1.26  2000/06/28 12:20:47  prez
** Lots of encryption stuff, but essentially, we now have random
** key generation for the keyfile keys, and we can actually encrypt
** something, and get it back as we sent it in (specifically, the
** keyfile itself).
**
** Revision 1.25  2000/06/27 18:56:59  prez
** Added choosing of keys to configure, also created the keygen,
** and scrambler (so keys are not stored in clear text, even in
** the .h and binary files).  We should be set to do the decryption
** process now, as encryption (except for encryption of db's) is
** all done :)
**
** Revision 1.24  2000/05/28 05:05:13  prez
** More makefile stuff ... Now we should work on all platforms.
** Added alot of checking for different .h files, functions, etc.
** So now all #define's are config.h based (also added a default
** windows config.h, which will need to be copied on these systems).
**
** Revision 1.23  2000/05/13 07:05:46  prez
** Added displaying of sizes to all file fields..
**
** Revision 1.22  2000/04/30 03:48:28  prez
** Replaced all system calls with ACE_OS equivilants,
** also removed any dependancy on ACE from sxp (xml)
**
** Revision 1.21  2000/02/23 12:21:02  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.20  2000/02/15 10:37:47  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#include "variant.h"
#ifdef HASCRYPT
#include "crypt/blowfish.h"
#define VERIFY_SIZE	128
#define MAX_KEYLEN	((BF_ROUNDS+2)*4)
#endif
#include "crypt/md5_locl.h"

vector < int > ParseNumbers(const mstring & what);
bool MakeDirectory(const mstring & in);
unsigned long FromHumanTime(const mstring & in);
mstring ToHumanTime(const unsigned long in, const mstring & source = "");
mstring ToHumanNumber(const unsigned long in);
unsigned long FromHumanSpace(const mstring & in);
mstring ToHumanSpace(const unsigned long in);
mstring parseMessage(const mstring & message, const mVarArray & va = mVarArray::EmptyArray());
size_t mCRYPT(const char *in, char *out, const size_t size, const char *key1, const char *key2, const bool enc);
void mHASH16(const char *in, const size_t size, char *out);
void mHASH(const char *in, const size_t size, char *out);
unsigned long str_to_base64(const mstring & in);
mstring base64_to_str(unsigned long in);
mstring sysinfo_node();
mstring sysinfo_type();
mstring sysinfo_rel();

// Something required to basically facilitate locking on
// a reference to a class extracted from a map.
class ref_class
{
    size_t i_references;
    bool i_doDelete;
    mVarArray i_lockData;

public:
      ref_class() : i_references(0), i_doDelete(false)
    {
    }
    ref_class(const mVarArray & l) : i_references(0), i_doDelete(false), i_lockData(l)
    {
    }
    virtual ~ref_class()
    {
    }

    virtual void addRef()
    {
	i_references++;
    }
    virtual void remRef()
    {
	if (i_references > 0)
	    i_references--;
    }
    virtual size_t references() const
    {
	return i_references;
    }
    virtual void setDelete(bool in = true)
    {
	i_doDelete = in;
    }
    virtual bool doDelete() const
    {
	return i_doDelete;
    }
    virtual void lockData(const mVarArray & in)
    {
	i_lockData = in;
    }
    virtual const mVarArray &lockData() const
    {
	return i_lockData;
    }
};

class mLOCK;

template < class T > class map_entry
{
    T *entry_ptr;
    mLOCK *lock;

    void Start()
    {
	if (entry_ptr != NULL)
	{
	    lock = new mLOCK(L_Read, entry_ptr->lockData());
	    entry_ptr->addRef();
	}
    }

    void End()
    {
	if (entry_ptr != NULL)
	{
	    entry_ptr->remRef();
	    if (entry_ptr->doDelete() && entry_ptr->references() == 0)
		delete entry_ptr;

	    entry_ptr = NULL;
	}
	if (lock != NULL)
	{
	    delete lock;

	    lock = NULL;
	}
    }

  public:
    map_entry() : entry_ptr(NULL), lock(NULL)
    {
    }
    map_entry(const map < mstring, T * > & map_ptr, const mstring & map_key) : entry_ptr(NULL), lock(NULL)
    {
	if (map_key.empty())
	    return;

	map < mstring, T * >::const_iterator iter = map_ptr.find(map_key);
	if (iter == map_ptr.end())
	    return;

	if (iter->second == NULL)
	    return;

	entry_ptr = iter->second;
	Start();
    }

    map_entry(T * e) : entry_ptr(NULL), lock(NULL)
    {
	if (e == NULL)
	    return;

	entry_ptr = e;
	Start();
    }

    map_entry(const map_entry < T > & in) : entry_ptr(NULL), lock(NULL)
    {
	*this = in;
    }

    map_entry < T > & operator=(const map_entry < T > & in)
    {
	End();
	entry_ptr = in.entry_ptr;
	Start();
    }

    ~map_entry()
    {
	End();
    }
    T *entry() const
    {
	return entry_ptr;
    }
    T *operator->() const
    {
	return entry_ptr;
    }
};

// extrapolated from the ms's pair<T1,T2> template code
template < class T1, class T2, class T3 > class triplet
{
public:
    typedef T1 first_type;
    typedef T2 second_type;
    typedef T3 third_type;

      triplet() : first(T1()), second(T2()), third(T3())
    {
    }
    triplet(const T1 & _V1, const T2 & _V2, const T3 & _V3) : first(_V1), second(_V2), third(_V3)
    {
    }
    triplet(const triplet < T1, T2, T3 > & in)
    {
	*this = in;
    }
    triplet < T1, T2, T3 > & operator=(const triplet < T1, T2, T3 > & in)
    {
	first = in.first;
	second = in.second;
	third = in.third;
	return *this;
    }
    T1 first;
    T2 second;
    T3 third;
};

template < class T1, class T2, class T3 > inline bool operator==(const triplet < T1, T2, T3 > & X, const triplet < T1, T2,
								 T3 > & Y)
{
    return (X.first == Y.first && X.second == Y.second && X.third == Y.third);
}

template < class T1, class T2, class T3 > inline bool operator!=(const triplet < T1, T2, T3 > & X, const triplet < T1, T2,
								 T3 > & Y)
{
    return (!(X == Y));
}

template < class T1, class T2, class T3 > inline bool operator<(const triplet < T1, T2, T3 > & X, const triplet < T1, T2,
								T3 > & Y)
{
    return (((X.first < Y.first)) || ((X.first == Y.first) && (X.second < Y.second)) ||
	    ((X.first == Y.first) && (X.second == Y.second) && (X.third < Y.third)));
}

template < class T1, class T2, class T3 > inline bool operator<=(const triplet < T1, T2, T3 > & X, const triplet < T1, T2,
								 T3 > & Y)
{
    return !(Y < X);
}

template < class T1, class T2, class T3 > inline bool operator>=(const triplet < T1, T2, T3 > & X, const triplet < T1, T2,
								 T3 > & Y)
{
    return !(X < Y);
}

template < class T1, class T2, class T3 > inline bool operator>(const triplet < T1, T2, T3 > & X, const triplet < T1, T2,
								T3 > & Y)
{
    return !(X <= Y);
}

template < class T1, class T2, class T3 > inline triplet < T1, T2, T3 > make_triplet(const T1 & X, const T2 & Y, const T3 & Z)
{
    return (triplet < T1, T2, T3 > (X, Y, Z));
}

#endif
