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

///////////////////////////////////////////////////////////////////////////////
// Name:        config.cpp
// Purpose:     implementation of wxConfigBase class
// Author:      Vadim Zeitlin
// Modified by: W. King for use with Magick II
// Created:     07.04.98
// RCS-ID:      $Id$
// Copyright:   (c) 1997 Karsten Ballnder   Ballueder@usa.net
//                       Vadim Zeitlin      <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows license
///////////////////////////////////////////////////////////////////////////////
#include "confbase.h"
#include "fileconf.h"
#include "log.h"

mstring wxExpandEnvVars(const mstring& str)
{
  mstring strResult;
  strResult.reserve(str.length());

  // don't change the values the enum elements: they must be equal
  // to the matching [closing] delimiter.
  enum Bracket
  {
    Bracket_None,
    Bracket_Normal  = ')',
    Bracket_Curly   = '}'
#ifdef  WIN32
    ,Bracket_Windows = '%'     // yeah, Windows people are a bit strange ;-)
#endif
  };

  size_t m;
  for ( size_t n = 0; n < str.length(); n++ ) {
    switch ( str[n] ) {
#ifdef  WIN32
      case '%':
#endif  //WINDOWS
      case '$':
        {
          Bracket bracket;
          #ifdef  WIN32
            if ( str[n] == '%' )
              bracket = Bracket_Windows;
            else
          #endif  //WINDOWS
          if ( n == str.length() - 1 ) {
            bracket = Bracket_None;
          }
          else {
            switch ( str[n + 1] ) {
              case '(':
                bracket = Bracket_Normal;
                n++;                   // skip the bracket
                break;

              case '{':
                bracket = Bracket_Curly;
                n++;                   // skip the bracket
                break;

              default:
                bracket = Bracket_None;
            }
          }

          m = n + 1;

          while ( m < str.length() && (isalnum(str[m]) || str[m] == '_') )
            m++;

          mstring strVarName(str.c_str() + n + 1, m - n - 1);

          const char *pszValue = getenv(strVarName);
          if ( pszValue != NULL ) {
            strResult += pszValue;
          }
          else {
            // variable doesn't exist => don't change anything
            #ifdef  WIN32
              if ( bracket != Bracket_Windows )
            #endif
                if ( bracket != Bracket_None )
                  strResult << str[n - 1];
            strResult << str[n] << strVarName;
          }

          // check the closing bracket
          if ( bracket != Bracket_None ) {
            if ( m == str.length() || str[m] != (char)bracket ) {
              wxLogWarning(_("Environment variables expansion failed: "
                             "missing '%c' at position %d in '%s'."),
                           (char)bracket, m + 1, str.c_str());
            }
            else {
              // skip closing bracket unless the variables wasn't expanded
              if ( pszValue == NULL )
                strResult << (char)bracket;
              m++;
            }
          }

          n = m - 1;  // skip variable name
        }
        break;

      case '\\':
        // backslash can be used to suppress special meaning of % and $
        if ( n != str.length() && (str[n + 1] == '%' || str[n + 1] == '$') ) {
          strResult += str[++n];

          break;
        }
        //else: fall through

      default:
        strResult += str[n];
    }
  }

  return strResult;
}


// this function is used to properly interpret '..' in path
void wxSplitPath(mArrayString& aParts, const mstring &sz)
{
  aParts.clear();

  mstring strCurrent;
  const char *pc = sz.c_str();
  for ( ;; ) {
    if ( *pc == '\0' || *pc == wxCONFIG_PATH_SEPARATOR ) {
      if ( strCurrent == "." ) {
        // ignore
      }
      else if ( strCurrent == ".." ) {
        // go up one level
        if ( aParts.size()==0 )
          wxLogWarning(_("'%s' has extra '..', ignored."), sz);
        else
          aParts.erase(aParts.end() - 1);

        strCurrent.Empty();
      }
      else if ( !strCurrent.IsEmpty() ) {
        aParts.push_back(strCurrent);
        strCurrent.Empty();
      }
      //else:
        // could log an error here, but we prefer to ignore extra '/'

      if ( *pc == '\0' )
        return;
    }
    else
      strCurrent += *pc;

    pc++;
  }
}

// ----------------------------------------------------------------------------
// global and class static variables
// ----------------------------------------------------------------------------

wxConfigBase *wxConfigBase::ms_pConfig     = NULL;
bool          wxConfigBase::ms_bAutoCreate = true;

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxConfigBase
// ----------------------------------------------------------------------------

// Not all args will always be used by derived classes, but
// including them all in each class ensures compatibility.
wxConfigBase::wxConfigBase(const mstring& appName, const mstring& vendorName,
    const mstring& localFilename, const mstring& globalFilename, long style):
        m_appName(appName), m_vendorName(vendorName), m_style(style)
{
    m_bExpandEnvVars = true;
	m_bRecordDefaults = false;
}

wxConfigBase *wxConfigBase::Set(wxConfigBase *pConfig)
{
  wxConfigBase *pOld = ms_pConfig;
  ms_pConfig = pConfig;
  return pOld;
}

wxConfigBase *wxConfigBase::Create()
{
  if ( ms_bAutoCreate && ms_pConfig == NULL ) 
  {
/*    ms_pConfig =
      new wxFileConfig("magick");*/
  }

  return ms_pConfig;
}

mstring wxConfigBase::Read(const mstring& key, const mstring& defVal) const
{
  mstring s;
  Read(key, &s, defVal);
  return s;
}

bool wxConfigBase::Read(const mstring& key, mstring *str, const mstring& defVal) const
{
    if (!Read(key, str))
    {
        *str = ExpandEnvVars(defVal);
        return false;
    }
    else
        return true;
}

bool wxConfigBase::Read(const mstring& key, long *pl, long defVal) const
{
    if (!Read(key, pl))
    {
        *pl = defVal;
        return false;
    }
    else
        return true;
}

bool wxConfigBase::Read(const mstring& key, double* val) const
{
    mstring str;
    if (Read(key, & str))
    {
        *val = atof(str);
        return true;
    }
    else
        return false;
}

bool wxConfigBase::Read(const mstring& key, double* val, double defVal) const
{
    if (!Read(key, val))
    {
        *val = defVal;
        return false;
    }
    else
        return true;
}

bool wxConfigBase::Read(const mstring& key, bool* val) const
{
    long l;
    if (Read(key, & l))
    {
        *val = (l != 0);
        return true;
    }
    else
        return false;
}

bool wxConfigBase::Read(const mstring& key, bool* val, bool defVal) const
{
    if (!Read(key, val))
    {
        *val = defVal;
        return false;
    }
    else
        return true;
}

// Convenience functions

bool wxConfigBase::Read(const mstring& key, int *pi) const
{
    long l;
    bool ret = Read(key, &l);
    if (ret)
        *pi = (int) l;
    return ret;
}

bool wxConfigBase::Read(const mstring& key, int *pi, int defVal) const
{
    long l;
    bool ret = Read(key, &l, (long) defVal);
    *pi = (int) l;
    return ret;
}

bool wxConfigBase::Write(const mstring& key, double val)
{
    mstring str;
    str.Format("%f", val);
    return Write(key, str);
}

bool wxConfigBase::Write(const mstring& key, bool value)
{
    long l = (value ? 1 : 0);
    return Write(key, l);
}

mstring wxConfigBase::ExpandEnvVars(const mstring& str) const
{
    mstring tmp; // Required for BC++
    if (IsExpandingEnvVars())
        tmp = wxExpandEnvVars(str);
    else
        tmp = str;
    return tmp;
}

// ----------------------------------------------------------------------------
// wxConfigPathChanger
// ----------------------------------------------------------------------------

wxConfigPathChanger::wxConfigPathChanger(const wxConfigBase *pContainer,
                                 const mstring& strEntry)
{
  m_pContainer = (wxConfigBase *)pContainer;
  mstring strPath = strEntry.Before(wxCONFIG_PATH_SEPARATOR);

  // special case of "/keyname" when there is nothing before "/"
  if ( strPath.IsEmpty() && ((!strEntry.IsEmpty()) && strEntry[0] == wxCONFIG_PATH_SEPARATOR ))
    strPath = wxCONFIG_PATH_SEPARATOR;

  if ( !strPath.IsEmpty() ) {
    // do change the path
    m_bChanged = true;
    m_strName = strEntry.Right(wxCONFIG_PATH_SEPARATOR);
    m_strOldPath = m_pContainer->GetPath();
    m_strOldPath += wxCONFIG_PATH_SEPARATOR;
    m_pContainer->SetPath(strPath);
  }
  else {
    // it's a name only, without path - nothing to do
    m_bChanged = false;
    m_strName = strEntry;
  }
}

wxConfigPathChanger::~wxConfigPathChanger()
{
  // only restore path if it was changed
  if ( m_bChanged ) {
    m_pContainer->SetPath(m_strOldPath);
  }
}

