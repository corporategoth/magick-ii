///////////////////////////////////////////////////////////////////////////////
// Name:        fileconf.cpp
// Purpose:     wxFileConfig derivation of wxConfigBase
// Author:      Vadim Zeitlin
// Modified by: Ungod for magick2
// Created:     07.04.98 (adapted from appconf.cpp)
// RCS-ID:      $Id$
// Copyright:   (c) 1997 Karsten Ballnder   &  Vadim Zeitlin
//                       Ballueder@usa.net     <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows license
///////////////////////////////////////////////////////////////////////////////
#include "fileconf.h"
#include "utils.h"
#include <ctype.h>
#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------
#ifndef MAX_PATH
  #define MAX_PATH 512
#endif

// is 'c' a valid character in group name?
// NB: wxCONFIG_IMMUTABLE_PREFIX and wxCONFIG_PATH_SEPARATOR must be valid chars
//     but _not_ ']' (group name delimiter)
inline bool IsValid(char c) { return isalnum(c) || strchr("@_/-!.*%", c); }

// filter strings
static mstring FilterIn(const mstring& str);
static mstring FilterOut(const mstring& str);

mstring wxFileConfig::GetGlobalFileName(const mstring &szFile)
{
	mstring str=GetGlobalDir();

	str<<szFile;
	if(strchr(szFile,'.')==NULL)
	{
#ifdef __UNIX__
		str<<".conf";
#else
		str<<".ini";
#endif
	}
	return str;
}
mstring wxFileConfig::GetLocalFileName(const mstring &szFile)
{
  mstring str = GetLocalDir();

  #ifdef  __UNIX__
    str << '.';
  #endif

  str << szFile;

  #ifdef WIN32
    if ( strchr(szFile, '.') == NULL )
      str << ".ini";
  #endif

  return str;
}

mstring wxFileConfig::GetGlobalDir()
{
  mstring strDir;

  #ifdef __UNIX__
    strDir = "/etc/";
  #else // Windows
    char szWinDir[MAX_PATH];
    ::GetWindowsDirectory(szWinDir, MAX_PATH);

    strDir = szWinDir;
    strDir << '\\';
  #endif // Unix/Windows

  return strDir;
}

mstring wxFileConfig::GetLocalDir()
{
  mstring strDir;

  wxGetHomeDir(strDir);

#ifdef  __UNIX__
  if (strDir.Last() != '/') strDir << '/';
#else
  if (strDir.Last() != '\\') strDir << '\\';
#endif

  return strDir;
}

// constructor supports creation of wxFileConfig objects of any type
wxFileConfig::wxFileConfig(const mstring& appName, const mstring& vendorName,
                           const mstring& strLocal, const mstring& strGlobal,
                           long style)
            : wxConfigBase(appName, vendorName, strLocal, strGlobal, style),
              m_strLocalFile(strLocal), m_strGlobalFile(strGlobal)
{
	if(appName.IsEmpty())
		SetAppName("magick");
	// Make up names for files if empty
	if ( m_strLocalFile.IsEmpty() && (style & wxCONFIG_USE_LOCAL_FILE) )
	{
		m_strLocalFile = GetLocalFileName(GetAppName());
	}
	if ( m_strGlobalFile.IsEmpty() && (style & wxCONFIG_USE_GLOBAL_FILE) )
	{
		m_strGlobalFile = GetGlobalFileName(GetAppName());
	}
	// Check if styles are not supplied, but filenames are, in which case
	// add the correct styles.
	if ( !m_strLocalFile.IsEmpty() )
		SetStyle(GetStyle() | wxCONFIG_USE_LOCAL_FILE);

	if ( !m_strGlobalFile.IsEmpty() )
		SetStyle(GetStyle() | wxCONFIG_USE_GLOBAL_FILE);

	// if the path is not absolute, prepend the standard directory to it
	if ( !m_strLocalFile.IsEmpty() && !wxIsAbsolutePath(m_strLocalFile) )
	{
		mstring strLocal = m_strLocalFile;
		m_strLocalFile = GetLocalDir();
		m_strLocalFile << strLocal;
	}
	
	//Init();
}

void wxFileConfig::Init()
{

}

void wxFileConfig::SetPath(const mstring & strPath)
{

}

mstring& wxFileConfig::GetPath() const
{

}
