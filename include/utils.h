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

#endif