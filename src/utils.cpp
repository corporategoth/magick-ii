#include "pch.h"
#ifdef _MSC_VER
#pragma hdrstop
#endif
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
#ifdef WIN32
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
  wxCHECK_RET( pszFileName, "NULL file name in wxSplitPath" );

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

  #ifdef WIN32
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

  #else
    const char *szHome = getenv("HOME");
    if ( szHome == NULL ) {
      // we're homeless...
      wxLogWarning("can't find user's HOME, using current directory.");
      strDir = ".";
    }
    else
       strDir = szHome;

    // add a trailing slash if needed
    if ( strDir.Last() != '/' )
      strDir << '/';
  #endif

  RET(strDir);
}

bool
wxIsAbsolutePath (const mstring& filename)
{
  FT("wxIsAbsolutePath", (filename));
  if (filename != "")
    {
      if (filename[0U] == '/'
#ifdef __VMS__
      || (filename[0U] == '[' && filename[1U] != '.')
#endif
#ifdef WIN32
      /* MSDOS */
      || filename[0U] == '\\' || (isalpha (filename[0U]) && filename[1U] == ':')
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

// Get current working directory.
// If buf is NULL, allocates space using new, else
// copies into buf.
char *wxGetWorkingDirectory(char *buf, int sz)
{
  if (!buf)
    buf = new char[sz+1];
  ACE_OS::getcwd(buf,sz);
  buf[sz]=0;
  return buf;
}

mstring wxGetCwd()
{
    char buf[1024];

    wxGetWorkingDirectory(buf, 1023);
    mstring str(buf);
    return str;
}

vector<int> ParseNumbers(mstring what)
{
    FT("ParseNumbers", (what));
    vector<int> numbers;
    int i;
    mstring tmp;

    what.Replace(",", " ");
    for (i=1; i<=what.WordCount(" "); i++)
    {
	tmp = what.ExtractWord(i, " ");
	if (!tmp.IsNumber() || tmp[0U] == '-' ||
	    tmp[tmp.size()-1] == '-' || tmp.WordCount("-") > 2)
	{
	    numbers.push_back(-1);
	}
	else if (tmp.Contains("-"))
	{
	    int j, limit;
	    j = atoi(tmp.Before("-").c_str());
	    limit = atoi(tmp.After("-").c_str());
	    if (limit >= j)
	    {
		for (; j<=limit; j++)
		    numbers.push_back(j);
	    }
	    else
	    {
		for (; j>=limit; j--)
		    numbers.push_back(j);
	    }
	}
	else
	{
	    numbers.push_back(atoi(tmp.c_str()));
	}
    }
    NRET(vector<int>, numbers);
}

unsigned long FromHumanTime(mstring in)
{
    FT("FromHumanTime", (in));

    unsigned int i;
    unsigned long number = 0, total = 0;

    for (i=0; i<in.size(); i++)
    {
	switch(in[i])
	{
	case 'Y':
	case 'y':
	    if (number != 0)
	    {
		total += (unsigned long) (number * 60 * 60 * 24 * 365.25);
		number = 0;
	    }
	    break;
	case 'W':
	case 'w':
	    if (number != 0)
	    {
		total += (number * 60 * 60 * 24 * 7);
		number = 0;
	    }
	    break;
	case 'D':
	case 'd':
	    if (number != 0)
	    {
		total += (number * 60 * 60 * 24);
		number = 0;
	    }
	    break;
	case 'H':
	case 'h':
	    if (number != 0)
	    {
		total += (number * 60 * 60);
		number = 0;
	    }
	    break;
	case 'M':
	case 'm':
	    if (number != 0)
	    {
		total += (number * 60);
		number = 0;
	    }
	    break;
	case 'S':
	case 's':
	    if (number != 0)
	    {
		total += number;
		number = 0;
	    }
	    break;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	    number *= 10;
	    number += (unsigned long) (in[i] - '0');
	    break;
	default:
	    RET(0);
	    break;
	}
    }
    if (number != 0)
	total += number;

    RET(total);
}

mstring ToHumanTime(unsigned long in)
{
    FT("ToHumanTime", (in));

    if (in==0)
    {
	RET("unlimited");
    }

    mDateTime mytime = Now() - ((double) (in+1) / (60.0 * 60.0 * 24.0));
    RET(mytime.Ago());
}
