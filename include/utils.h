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

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

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

#if 0
template <class T>
class magick_allocator
{
public:
    typedef unsigned long size_type;
    typedef long difference_type;
    typedef T * pointer;
    typedef const T * const_pointer;
    typedef T & reference;
    typedef const T & const_reference;
    typedef T value_type;
    template <class U>
	struct rebind { typedef allocator<U> other; };
    magick_allocator();
    template <class U>
	magick_allocator(const magick_allocator<U>&);
    template <class U>
	operator=(const magick_allocator<U>&);
    ~magick_allocator();
    pointer address(reference r)const{return &r;};
    const_pointer address(const_reference r)const{return &r;};
    pointer allocate(size_type n, const_pointer hint=0);
    void deallocate(pointer);
    size_type max_size();
    void construct(pointer p, const T& val);
    void destroy(pointer p);
};

template <class T>
magick_allocator::pointer 
operator new(magick_allocator::size_type, magick_allocator&);

template <class T, class U>
bool 
operator==(const magick_allocator<T>& a, 
          const magick_allocator<U>& b);

template <class T, class U>
bool 
operator!=(const magick_allocator<T>& a, 
	   const magcik_allocator<U>& b) { return !(a==b);};
#endif

#endif