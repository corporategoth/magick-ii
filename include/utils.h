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

#endif