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

// extrapolated from the ms's pair<T1,T2> template code

template<class _T1, class _T2, class _T3> class triplet 
{
public:
    typedef _T1 first_type;
    typedef _T2 second_type;
    typedef _T3 third_type;
    triplet() 
	: first(_T1()), second(_T2()), third(_T3()) {}
    triplet(const _T1& _V1, const _T2& _V2, const _T3& _V3)
	: first(_V1), second(_V2), third(_V3) {}
    _T1 first;
    _T2 second;
    _T3 third;
};

template<class _T1, class _T2, class _T3> inline
bool operator==(const triplet<_T1, _T2, _T3>& _X, const triplet<_T1, _T2, _T3>& _Y)
{
    return (_X.first==_Y.first && _X.second==_Y.second && _X.third == _Y.third);
}
    
template<class _T1, class _T2, class _T3> inline
bool operator!=(const triplet<_T1, _T2, _T3>& _X, const triplet<_T1, _T2, _T3>& _Y)
{
    return (!(_X==_Y));
}

template<class _T1, class _T2, class _T3> inline
bool operator<(const triplet<_T1, _T2, _T3>& _X, const triplet<_T1, _T2, _T3>& _Y)
{
    return (_X.first<_Y.first
	|| (!(_X.first<_Y.first) && (_X.second<_Y.second))
	|| (!(_X.first<_Y.first) && (!(_X.second<_Y.second) && (_X.third<_Y.third))));
}

template<class _T1, class _T2, class _T3> inline
bool operator>(const triplet<_T1, _T2, _T3>& _X, const triplet<_T1, _T2, _T3>& _Y)
{
    return (_Y < _X);
}

template<class _T1, class _T2, class _T3> inline
bool operator<=(const triplet<_T1, _T2, _T3>& _X, const triplet<_T1, _T2, _T3>& _Y)
{
    return !(_Y < _X);
}

template<class _T1, class _T2, class _T3> inline
bool operator>=(const triplet<_T1, _T2, _T3>& _X, const triplet<_T1, _T2, _T3>& _Y)
{
    return !(_X < _Y);
}

template<class _T1, class _T2, class _T3> inline
triplet<_T1,_T2,_T3> make_triplet(const _T1& _X, const _T2& _Y, const _T3& _Z)
{
    return (triplet<_T1,_T2,_T3>(_X,_Y,_Z));
}

#endif