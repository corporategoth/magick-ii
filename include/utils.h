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
** ==========================================================
#pragma ident "$Id$"
** ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.20  2000/02/15 10:37:47  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#ifndef _UTILS_H
#define _UTILS_H

#include "mstring.h"
#include "datetime.h"
#include "lockable.h"

const char FILE_SEP_EXT = '.';
const char FILE_SEP_DSK = ':';
const char FILE_SEP_PATH_DOS = '\\';
const char FILE_SEP_PATH_UNIX = '/';

extern void wxSplitPath(const char *pszFileName,
                             mstring *pstrPath,
                             mstring *pstrName,
                             mstring *pstrExt);

extern mstring &wxGetHomeDir(mstring &pstr);

extern bool wxIsAbsolutePath (const mstring& filename);

// These are TRANSACTION ID's used for guarenteeing no
// duplicate messages between Magick instances.  They are
// always 10 digits, and with a 4-byte unsigned long,
// you have 3294967295 possible TxnIds.
class TxnIds
{
    static const unsigned long min;
    static const unsigned long keeptime;

    static map<unsigned long, mDateTime> i_Ids;
    static unsigned long i_Current;

public:
    static unsigned long Create();
    static unsigned long Current();
    static bool Register(unsigned long id);
    static void Expire();

};
    

// Ensure subsequent IDs don't clash with this one
extern void wxRegisterId(long id);
#define RegisterId wxRegisterId

// Return the current ID
extern long wxGetCurrentId(void);

// Get current working directory.
// If buf is NULL, allocates space using new, else
// copies into buf.
// IMPORTANT NOTE getcwd is know not to work under some releases
// of Win32s 1.3, according to MS release notes!
char* wxGetWorkingDirectory(char *buf = (char *) NULL,int sz = 1000);
// new and preferred version of wxGetWorkingDirectory
// NB: can't have the same name because of overloading ambiguity
mstring wxGetCwd();

vector<int> ParseNumbers(mstring what);
unsigned long FromHumanTime(mstring in);
mstring ToHumanTime(unsigned long in);
mstring ToHumanNumber(unsigned long in);

// extrapolated from the ms's pair<T1,T2> template code

template<class T1, class T2, class T3> class triplet 
{
#ifdef WIN32
    friend wxOutputStream &operator<<(wxOutputStream& out,const triplet<T1,T2,T3>& in);
    friend wxInputStream &operator>>(wxInputStream& in, triplet<T1,T2,T3>& out);
#else
    friend wxOutputStream &operator<<<T1,T2,T3>(wxOutputStream& out,const triplet<T1,T2,T3>& in);
    friend wxInputStream &operator>><T1,T2,T3>(wxInputStream& in, triplet<T1,T2,T3>& out);
#endif
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
    return (X.first==Y.first && X.second==Y.second && X.third == Y.third);
}
    
template<class T1, class T2, class T3> inline
bool operator!=(const triplet<T1, T2, T3>& X, const triplet<T1, T2, T3>& Y)
{
    return (!(X==Y));
}

template<class T1, class T2, class T3> inline
bool operator<(const triplet<T1, T2, T3>& X, const triplet<T1, T2, T3>& Y)
{
    return (X.first<Y.first
	|| (!(X.first<Y.first) && (X.second<Y.second))
	|| (!(X.first<Y.first) && (!(X.second<Y.second) && (X.third<Y.third))));
}

template<class T1, class T2, class T3> inline
bool operator>(const triplet<T1, T2, T3>& X, const triplet<T1, T2, T3>& Y)
{
    return (Y < X);
}

template<class T1, class T2, class T3> inline
bool operator<=(const triplet<T1, T2, T3>& X, const triplet<T1, T2, T3>& Y)
{
    return !(Y < X);
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


template<class T1, class T2, class T3> inline
wxOutputStream &operator<<(wxOutputStream& out,const triplet<T1,T2,T3>& in)
{
    out<<in.first<<in.second<<in.third;
    return out;
}

template<class T1, class T2, class T3> inline
wxInputStream &operator>>(wxInputStream& in, triplet<T1,T2,T3>& out)
{
    in>>out.first>>out.second>>out.third;
    return in;
}

template<class T1, class T2> inline
wxOutputStream &operator<<(wxOutputStream& out,const pair<T1,T2>& in)
{
    out<<in.first<<in.second;
    return out;
}

template<class T1, class T2> inline
wxInputStream &operator>>(wxInputStream& in, pair<T1,T2>& out)
{
    in>>out.first>>out.second;
    return in;
}

#endif
