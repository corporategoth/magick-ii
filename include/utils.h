#ifndef WIN32
#pragma interface
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
#ifndef _UTILS_H
#define _UTILS_H
#include "pch.h"
RCSID(utils_h, "@(#) $Id$");

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
mstring sysinfo_node();
mstring sysinfo_type();
mstring sysinfo_rel();
mstring version_string(bool extended = true);

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
    virtual ~ ref_class()
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
    virtual bool validated() const
    {
	return false;
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
	if (entry_ptr != NULL && entry_ptr->validated())
	{
	    lock = new mLOCK(L_Read, entry_ptr->lockData());
	    entry_ptr->addRef();
	}
    }

    void End()
    {
	if (entry_ptr != NULL && entry_ptr->validated())
	{
	    entry_ptr->remRef();
	    if (entry_ptr->doDelete() && entry_ptr->references() == 0)
		delete entry_ptr;
	}
	entry_ptr = NULL;
	if (lock != NULL)
	    delete lock;

	lock = NULL;
    }

public:
    map_entry() : entry_ptr(NULL), lock(NULL)
    {
    }
#ifdef NO_HASH_MAP
    map_entry(const map < mstring, T * > & map_ptr, const mstring & map_key) : entry_ptr(NULL), lock(NULL)
#else
    map_entry(const hash_map < mstring, T * > & map_ptr, const mstring & map_key) : entry_ptr(NULL), lock(NULL)
#endif
    {
	if (map_key.empty())
	    return;

#ifdef NO_HASH_MAP
	typename map < mstring, T * >::const_iterator iter = map_ptr.find(map_key);
#else
	typename hash_map < mstring, T * >::const_iterator iter = map_ptr.find(map_key);
#endif

	if (iter == map_ptr.end())
	    return;

	entry_ptr = iter->second;
	Start();
    }

    map_entry(T * e) : entry_ptr(NULL), lock(NULL)
    {
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
	return *this;
    }

    ~map_entry()
    {
	End();
    }
    T *operator->() const
    {
	return entry_ptr;
    }
    T& operator*() const
    {
	return *entry_ptr;
    }
    operator T*() const
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
