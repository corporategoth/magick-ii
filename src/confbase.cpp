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
#include "trace.h"

mstring wxExpandEnvVars(const mstring& str)
{
  FT("wxExpandEnvVars", (str));
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
              wxLogWarning("Environment variables expansion failed: "
                             "missing '%c' at position %d in '%s'.",
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

  RET(strResult);
}


// this function is used to properly interpret '..' in path
void wxSplitPath(mArrayString& aParts, const mstring &sz)
{
  FT("wxSplitPath", ("(mArrayStrings) aParts", sz));
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
          wxLogWarning("'%s' has extra '..', ignored.", sz.c_str());
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
  FT("wxConfigBase::wxConfigBase", (appName, vendorName, localFilename, globalFilename, style));
    m_bExpandEnvVars = true;
	m_bRecordDefaults = false;
}

wxConfigBase *wxConfigBase::Set(wxConfigBase *pConfig)
{
  FT("wxConfigBase::Set", ("(wxConfigBase *) pConfig"));
  wxConfigBase *pOld = ms_pConfig;
  ms_pConfig = pConfig;
  NRET(wxConfBase, pOld);
}

wxConfigBase *wxConfigBase::Create()
{
  NFT("wxConfigBase::Create");
  if ( ms_bAutoCreate && ms_pConfig == NULL ) 
  {
    ms_pConfig =
      new wxFileConfig("magick");
  }

  NRET(wxConfBase, ms_pConfig);
}

mstring wxConfigBase::Read(const mstring& key, const mstring& defVal) const
{
  FT("wxConfigBase::Read", (key, defVal));
  mstring s;
  Read(key, &s, defVal);
  RET(s);
}

bool wxConfigBase::Read(const mstring& key, mstring *str, const mstring& defVal) const
{
    FT("wxConfigBase::Read", (key, str, defVal));
    if (!Read(key, str))
    {
        *str = ExpandEnvVars(defVal);
        RET(false);
    }
    else
        RET(true);
}

bool wxConfigBase::Read(const mstring& key, long* val) const
{
    FT("wxConfigBase::Read", (key, val));
    mstring str;
    if (Read(key, & str))
    {
        *val = atol(str);
        RET(true);
    }
    else
        RET(false);
}

bool wxConfigBase::Read(const mstring& key, long* val, const long& defVal) const
{
    FT("wxConfigBase", (key, val, defVal));
    mstring str, defStr;
    defStr << defVal;
    bool retval = Read(key, &str, defVal);
    *val = atol(str);
    RET(retval);
}

bool wxConfigBase::Read(const mstring& key, int* val) const
{
    FT("wxConfigBase::Read", (key, val));
    mstring str;
    if (Read(key, & str))
    {
        *val = atoi(str);
        RET(true);
    }
    else
        RET(false);
}

bool wxConfigBase::Read(const mstring& key, int* val, const int& defVal) const
{
    FT("wxConfigBase", (key, val, defVal));
    mstring str, defStr;
    defStr << defVal;
    bool retval = Read(key, &str, defVal);
    *val = atoi(str);
    RET(retval);
}

bool wxConfigBase::Read(const mstring& key, float* val) const
{
    FT("wxConfigBase::Read", (key, val));
    mstring str;
    if (Read(key, & str))
    {
        *val = atof(str);
        RET(true);
    }
    else
        RET(false);
}

bool wxConfigBase::Read(const mstring& key, float* val, const float& defVal) const
{
    FT("wxConfigBase", (key, val, defVal));
    mstring str, defStr;
    defStr << defVal;
    bool retval = Read(key, &str, defVal);
    *val = atof(str);
    RET(retval);
}

static bool makebool(mstring intext)
{
  if (intext.CmpNoCase("true")==0 || intext.CmpNoCase("on")==0 || intext.CmpNoCase("yes")==0 ||
      intext.CmpNoCase("y")==0 || intext.CmpNoCase("t")==0)
    return true;
  else
    return false;
}

bool wxConfigBase::Read(const mstring& key, bool* val) const
{
    FT("wxConfigBase::Read", (key, val));
    mstring str;
    if (Read(key, & str))
    {
        *val = makebool(str);
        RET(true);
    }
    else
        RET(false);
}

bool wxConfigBase::Read(const mstring& key, bool* val, const bool& defVal) const
{
    FT("wxConfigBase", (key, val, defVal));
    mstring str, defStr;
    defStr << (defVal ? "TRUE" : "FALSE");
    bool retval = Read(key, &str, defVal);
    *val = makebool(str);
    RET(retval);
}

mstring wxConfigBase::ExpandEnvVars(const mstring& str) const
{
    FT("wxConfigBase::ExpandEnvVars", (str));
    mstring tmp; // Required for BC++
    if (IsExpandingEnvVars())
        tmp = wxExpandEnvVars(str);
    else
        tmp = str;
    RET(tmp);
}

// ----------------------------------------------------------------------------
// wxConfigPathChanger
// ----------------------------------------------------------------------------

wxConfigPathChanger::wxConfigPathChanger(const wxConfigBase *pContainer,
                                 const mstring& strEntry)
{
  FT("wxConfigPathChanger::wxConfigPathChanger", ("(wxConfigBase *) pContainer", strEntry));
  m_pContainer = (wxConfigBase *)pContainer;
  mstring strPath = strEntry.Before(wxCONFIG_PATH_SEPARATOR);

  // special case of "/keyname" when there is nothing before "/"
  if ( strPath.IsEmpty() && ((!strEntry.IsEmpty()) && strEntry[0U] == wxCONFIG_PATH_SEPARATOR ))
    strPath = wxCONFIG_PATH_SEPARATOR;

  if ( !strPath.IsEmpty() ) {
    // do change the path
    m_bChanged = true;
    m_strName = strEntry.After(wxCONFIG_PATH_SEPARATOR);
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
  NFT("wxConfigPathChanger::~wxConfigPathChanger");
  // only restore path if it was changed
  if ( m_bChanged ) {
    m_pContainer->SetPath(m_strOldPath);
  }
}

wxConfigBase *wxConfigBase::Get() 
{
    NFT("wxConfigBase::Get");
    if ( !ms_pConfig ) 
	Create();
    NRET(wxConfigBase, ms_pConfig); 
}

void wxConfigBase::DontCreateOnDemand() 
{
    NFT("wxConfigBase::DontCreateOnDemand");
    ms_bAutoCreate = false; 
}

bool wxConfigBase::Exists(const mstring& strName) const 
{
    FT("wxConfigBase::Exists", (strName));
    RET(HasGroup(strName) || HasEntry(strName)); 
}

bool wxConfigBase::IsExpandingEnvVars() const 
{
    NFT("wxConfigBase::IsExpandingEnvVars");
    RET(m_bExpandEnvVars); 
}

void wxConfigBase::SetExpandEnvVars(bool bDoIt) 
{
    FT("wxConfigBase::SetExpandEnvVars", (bDoIt));
    m_bExpandEnvVars = bDoIt; 
}

void wxConfigBase::SetRecordDefaults(bool bDoIt) 
{
    FT("wxConfigBase::SetRecordDefaults", (bDoIt));
    m_bRecordDefaults = bDoIt; 
}

bool wxConfigBase::IsRecordingDefaults() const 
{
    NFT("wxConfigBase::IsRecordingDefaults");
    RET(m_bRecordDefaults); 
}

mstring wxConfigBase::GetAppName() const 
{
    NFT("wxConfigBase::GetAppName");
    RET(m_appName); 
}

mstring wxConfigBase::GetVendorName() const
{
    NFT("wxConfigBase::GetVendorName");
    RET(m_vendorName); 
}

void wxConfigBase::SetAppName(const mstring& appName) 
{
    FT("wxConfigBase::SetAppName", (appName));
    m_appName = appName; 
}

void wxConfigBase::SetVendorName(const mstring& vendorName) 
{
    FT("wxConfigBase::SetVendorName", (vendorName));
    m_vendorName = vendorName; 
}

void wxConfigBase::SetStyle(long style) 
{
    FT("wxConfigBase::SetStyle", (style));
    m_style = style; 
}

long wxConfigBase::GetStyle() const 
{
    NFT("wxConfigBase::GetStyle");
    RET(m_style); 
}

bool wxConfigBase::IsImmutable(const mstring& key) 
{
    FT("wxConfigBase::IsImmutable", (key));
    RET(!key.IsEmpty() && key[0U] == wxCONFIG_IMMUTABLE_PREFIX); 
}

const mstring& wxConfigPathChanger::Name() const 
{
    NFT("wxConfigPathChanger::Name");
    RET(m_strName); 
}