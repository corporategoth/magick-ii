// $Id$
//
// Magick IRC Services
// (c) 1997-1999 Preston A. Elder <prez@magick.tm>
// (c) 1998-1999 W. King <ungod@magick.tm>
//
// The above copywright may not be removed under any
// circumstances, however it may be added to if any
// modifications are made to this file.  All modified
// code must be clearly documented and labelled.
//
// ===================================================
#ifndef _UTILS_H
#define _UTILS_H

#include "mstring.h"

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

// Generate a unique ID
extern long wxNewId(void);
#define NewId wxNewId

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

#endif
