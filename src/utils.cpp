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
#include "magickmemory.hpp"
#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include "utils.h"
#include "log.h"
#include "trace.h"


void wxSplitPath(const char *pszFileName,
                             mstring *pstrPath,
                             mstring *pstrName,
                             mstring *pstrExt)
{
  FT("wxSplitPath", (pszFileName, pstrPath, pstrName, pstrExt));
  wxCHECK_RET( pszFileName, _("NULL file name in wxSplitPath") );

  const char *pDot = strrchr(pszFileName, FILE_SEP_EXT);
  const char *pSepUnix = strrchr(pszFileName, FILE_SEP_PATH_UNIX);
  const char *pSepDos = strrchr(pszFileName, FILE_SEP_PATH_DOS);

  // take the last of the two
  size_t nPosUnix = pSepUnix ? pSepUnix - pszFileName : 0;
  size_t nPosDos = pSepDos ? pSepDos - pszFileName : 0;
  if ( nPosDos > nPosUnix )
    nPosUnix = nPosDos;
//  size_t nLen = Strlen(pszFileName);

  if ( pstrPath )
    *pstrPath = mstring(pszFileName, nPosUnix);
  if ( pDot ) {
    size_t nPosDot = pDot - pszFileName;
    if ( pstrName )
      *pstrName = mstring(pszFileName + nPosUnix + 1, nPosDot - nPosUnix);
    if ( pstrExt )
      *pstrExt = mstring(pszFileName + nPosDot + 1);
  }
  else {
    if ( pstrName )
      *pstrName = mstring(pszFileName + nPosUnix + 1);
    if ( pstrExt )
      pstrExt->Empty();
  }
}

mstring &wxGetHomeDir(mstring &pstr)
{
  FT("wxGetHomeDir", (pstr));
  mstring& strDir = pstr;

  #if defined(__UNIX__) || defined(__linux__)
    const char *szHome = getenv("HOME");
    if ( szHome == NULL ) {
      // we're homeless...
      wxLogWarning(_("can't find user's HOME, using current directory."));
      strDir = ".";
    }
    else
       strDir = szHome;

    // add a trailing slash if needed
    if ( strDir.Last() != '/' )
      strDir << '/';
  #else   // Windows
      const char *szHome = getenv("HOMEDRIVE");
      if ( szHome != NULL )
        strDir << szHome;
      szHome = getenv("HOMEPATH");
      if ( szHome != NULL ) {
        strDir << szHome;

        // the idea is that under NT these variables have default values
        // of "%systemdrive%:" and "\\". As we don't want to create our
        // config files in the root directory of the system drive, we will
        // create it in our program's dir. However, if the user took care
        // to set HOMEPATH to something other than "\\", we suppose that he
        // knows what he is doing and use the supplied value.
        if ( strcmp(szHome, "\\") != 0 )
          RET(strDir);
      }


    // 260 was taken from windef.h
    #ifndef MAX_PATH
      #define MAX_PATH  260
    #endif

    mstring strPath;
	char cstrPath[MAX_PATH+1];
    ::GetModuleFileName(::GetModuleHandle(NULL),
                        cstrPath, MAX_PATH);
    strPath=cstrPath;

    // extract the dir name
    wxSplitPath(strPath, &strDir, NULL, NULL);

  #endif  // UNIX/Win

  RET(strDir);
}

bool
wxIsAbsolutePath (const mstring& filename)
{
  FT("wxIsAbsolutePath", (filename));
  if (filename != "")
    {
      if (filename[0] == '/'
#ifdef __VMS__
      || (filename[0] == '[' && filename[1] != '.')
#endif
#ifdef WIN32
      /* MSDOS */
      || filename[0] == '\\' || (isalpha (filename[0]) && filename[1] == ':')
#endif
        )
        RET(true);
    }
  RET(false);
}

// Id generation
static long wxCurrentId = 100;

long
wxNewId (void)
{
  NFT("wxNewId");
  RET(wxCurrentId++);
}

long
wxGetCurrentId(void)
{
  NFT("wxGetCurrentId");
  RET(wxCurrentId);
}

void
wxRegisterId (long id)
{
  FT("wxRegisterId", (id));
  if (id >= wxCurrentId)
    wxCurrentId = id + 1;
}

