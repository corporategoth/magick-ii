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
#include "des/des_locl.h"
#endif

vector<int> ParseNumbers(mstring what);
unsigned long FromHumanTime(const mstring &in);
mstring ToHumanTime(const unsigned long in, const mstring &source = "");
mstring ToHumanNumber(const unsigned long in);
unsigned long FromHumanSpace(const mstring &in);
mstring ToHumanSpace(const unsigned long in);
void mDES(unsigned char *in, unsigned char *out, size_t size,
	des_key_schedule key1, des_key_schedule key2, const int enc);
void mHASH(unsigned char *in, const size_t size, unsigned char *out);
unsigned long str_to_base64(const mstring &in);
mstring base64_to_str(unsigned long in);
mstring sysinfo_node();
mstring sysinfo_type();
mstring sysinfo_rel();

// extrapolated from the ms's pair<T1,T2> template code

template<class T1, class T2, class T3> class triplet 
{
public:
    typedef T1 first_type;
    typedef T2 second_type;
    typedef T3 third_type;
    triplet() 
	: first(T1()), second(T2()), third(T3()) {}
    triplet(const T1& _V1, const T2& _V2, const T3& _V3)
	: first(_V1), second(_V2), third(_V3) {}
    T1 first;
    T2 second;
    T3 third;
};

template<class T1, class T2, class T3> inline
bool operator==(const triplet<T1, T2, T3>& X, const triplet<T1, T2, T3>& Y)
{
    return (X.first==Y.first && X.second==Y.second && X.third==Y.third);
}
    
template<class T1, class T2, class T3> inline
bool operator!=(const triplet<T1, T2, T3>& X, const triplet<T1, T2, T3>& Y)
{
    return (!(X==Y));
}

template<class T1, class T2, class T3> inline
bool operator<(const triplet<T1, T2, T3>& X, const triplet<T1, T2, T3>& Y)
{
    return (((X.first<Y.first)) || 
	((X.first==Y.first) && (X.second<Y.second)) || 
	((X.first==Y.first) && (X.second==Y.second) && (X.third<Y.third)));
}

template<class T1, class T2, class T3> inline
bool operator>(const triplet<T1, T2, T3>& X, const triplet<T1, T2, T3>& Y)
{
    return !((Y < X) || (Y == X));
}

template<class T1, class T2, class T3> inline
bool operator<=(const triplet<T1, T2, T3>& X, const triplet<T1, T2, T3>& Y)
{
    return !(Y > X);
}

template<class T1, class T2, class T3> inline
bool operator>=(const triplet<T1, T2, T3>& X, const triplet<T1, T2, T3>& Y)
{
    return !(X < Y);
}

template<class T1, class T2, class T3> inline
triplet<T1,T2,T3> make_triplet(const T1& X, const T2& Y, const T3& Z)
{
    return (triplet<T1,T2,T3>(X,Y,Z));
}

#endif
