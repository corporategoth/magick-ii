#include "pch.h"
#ifdef _MSC_VER
#pragma hdrstop
#endif

/*  Magick IRC Services
**
** (c) 1997-2000 Preston Elder <prez@magick.tm>
** (c) 1998-2000 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any
** circumstances, however it may be added to if any
** modifications are made to this file.  All modified
** code must be clearly documented and labelled.
**
** ========================================================== */
#ifndef WIN32
#pragma ident "$Id$"
#endif
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.32  2000/02/15 13:27:03  prez
** *** empty log message ***
**
** Revision 1.31  2000/02/15 10:37:49  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */


///////////////////////////////////////////////////////////////////////////////
// Name:        fileconf.cpp
// Purpose:     wxFileConfig derivation of wxConfigBase
// Author:      Vadim Zeitlin
// Modified by: W. King for use with Magick II
// Created:     07.04.98 (adapted from appconf.cpp)
// RCS-ID:      $Id$
// Copyright:   (c) 1997 Karsten Ballnder   &  Vadim Zeitlin
//                       Ballueder@usa.net     <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows license
///////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include "fileconf.h"
#include "utils.h"
#include "trace.h"
#include "magick.h"

#define CONST_CAST ((wxFileConfig *)this)->

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------
#ifndef MAX_PATH
  #define MAX_PATH 512
#endif

// ----------------------------------------------------------------------------
// global functions declarations
// ----------------------------------------------------------------------------

// is 'c' a valid character in group name?
// NB: wxCONFIG_IMMUTABLE_PREFIX and wxCONFIG_PATH_SEPARATOR must be valid chars,
//     but _not_ ']' (group name delimiter)
inline bool IsValid(char c) {
//	FT("IsValid", (c));
//	RET(isalnum(c) || strchr("@_/-!.*%", c));
	return (isalnum(c) || strchr("@_/-!.*%", c));
}

// compare functions for sorting the arrays
static int CompareEntries(ConfigEntry *p1, ConfigEntry *p2);
static int CompareGroups(ConfigGroup *p1, ConfigGroup *p2);

// filter strings
static mstring FilterIn(const mstring& str);
static mstring FilterOut(const mstring& str);

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// static functions
// ----------------------------------------------------------------------------
mstring wxFileConfig::GetGlobalDir()
{
  NFT("wxFileConfig::GetGlobalDir");
  mstring strDir;

  #ifdef WIN32
    char szWinDir[MAX_PATH];
    ::GetWindowsDirectory(szWinDir, MAX_PATH);

    strDir = szWinDir;
    strDir << '\\';
  #elif defined(__WXSTUBS__)
    wxASSERT_MSG( false, "TODO" ) ;
  #else // unix
    strDir = "/etc/";
  #endif

  RET(strDir);
}

mstring wxFileConfig::GetLocalDir()
{
  NFT("WxFileConfig::GetLocalDir");
  mstring strDir;

  wxGetHomeDir(&strDir);

#ifdef WIN32
  if (strDir.Last() != '\\') strDir << '\\';
#else
  if (strDir.Last() != '/') strDir << '/';
#endif

  RET(strDir);
}

mstring wxFileConfig::GetGlobalFileName(const char *szFile)
{
  FT("wxFileConfig::GetGlobalFileName", (szFile));
  mstring str = GetGlobalDir();
  str << szFile;

  if ( strchr(szFile, '.') == NULL )
  #ifdef WIN32
    str << ".ini";
  #else
    str << ".conf";
  #endif

  RET(str);
}

mstring wxFileConfig::GetLocalFileName(const char *szFile)
{
  FT("wxFileConfig::GetLocalFileName", (szFile));
  mstring str = GetLocalDir();

  #ifndef WIN32
    str << '.';
  #endif

  str << szFile;

  #ifdef __WXMSW__
    if ( strchr(szFile, '.') == NULL )
      str << ".ini";
  #endif

  RET(str);
}

// ----------------------------------------------------------------------------
// ctor
// ----------------------------------------------------------------------------

void wxFileConfig::Init()
{
  NFT("wxFileConfig::Init");
  m_pCurrentGroup =
  m_pRootGroup    = new ConfigGroup(NULL, "", this);

  m_linesHead =
  m_linesTail = NULL;

  // it's not an error if (one of the) file(s) doesn't exist

  // parse the global file
  if ( !m_strGlobalFile.IsEmpty() && wxFile::Exists(m_strGlobalFile) ) {
    wxTextFile fileGlobal(m_strGlobalFile);

    if ( fileGlobal.Open() ) {
      Parse(fileGlobal, false /* global */);
      SetRootPath();
    }
    else
      wxLogWarning(Parent->getLogMessage("WX_ERRORS/GLOBAL_CFG"),
                   m_strGlobalFile.c_str());
  }

  // parse the local file
  if ( !m_strLocalFile.IsEmpty() && wxFile::Exists(m_strLocalFile) ) {
    wxTextFile fileLocal(m_strLocalFile);
    if ( fileLocal.Open() ) {
      Parse(fileLocal, true /* local */);
      SetRootPath();
    }
    else
      wxLogWarning(Parent->getLogMessage("WX_ERRORS/USER_CFG"),
                   m_strLocalFile.c_str());
  }
}

// constructor supports creation of wxFileConfig objects of any type
wxFileConfig::wxFileConfig(const mstring& appName, const mstring& vendorName,
                           const mstring& strLocal, const mstring& strGlobal,
                           long style)
            : wxConfigBase(appName, vendorName, strLocal, strGlobal, style),
              m_strLocalFile(strLocal), m_strGlobalFile(strGlobal)
{
  FT("wxFileConfig::wxFileConfig", (appName, vendorName, strLocal, strGlobal, style));
  // Make up an application name if not supplied
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

  if ( !m_strGlobalFile.IsEmpty() && !wxIsAbsolutePath(m_strGlobalFile) )
  {
    mstring strGlobal = m_strGlobalFile;
    m_strGlobalFile = GetGlobalDir();
    m_strGlobalFile << strGlobal;
  }

  Init();
}

void wxFileConfig::CleanUp()
{
  NFT("wxFileConfig::CleanUp");
  delete m_pRootGroup;

  LineList *pCur = m_linesHead;
  while ( pCur != NULL ) {
    LineList *pNext = pCur->Next();
    delete pCur;
    pCur = pNext;
  }
}

wxFileConfig::~wxFileConfig()
{
  NFT("wxFileConfig::~wxFileConfig");
  Flush();

  CleanUp();
}

// ----------------------------------------------------------------------------
// parse a config file
// ----------------------------------------------------------------------------

void wxFileConfig::Parse(wxTextFile& file, bool bLocal)
{
  FT("wxFileConfig::Parse", ("(wxTextFile) file", bLocal));
  const char *pStart;
  const char *pEnd;
  mstring strLine;

  size_t nLineCount = file.GetLineCount();
  for ( size_t n = 0; n < nLineCount; n++ ) {
    strLine = file[n];

    // add the line to linked list
    if ( bLocal )
      LineListAppend(strLine);

    // skip leading spaces
    for ( pStart = strLine; isspace(*pStart); pStart++ )
      ;

    // skip blank/comment lines
    if ( *pStart == '\0'|| *pStart == ';' || *pStart == '#' )
      continue;

    if ( *pStart == '[' ) {          // a new group
      pEnd = pStart;

      while ( *++pEnd != ']' ) {
        if ( !IsValid(*pEnd) && *pEnd != ' ' )  // allow spaces in group names
          break;
      }

      if ( *pEnd != ']' ) {
        wxLogError(Parent->getLogMessage("WX_ERRORS/UNEXPECTED_CHAR"),
                   file.GetName(), *pEnd, n + 1);
        continue; // skip this line
      }

      // group name here is always considered as abs path
      mstring strGroup;
      pStart++;
      strGroup << wxCONFIG_PATH_SEPARATOR << mstring(pStart, pEnd - pStart);

      // will create it if doesn't yet exist
      SetPath(strGroup);

      if ( bLocal )
        m_pCurrentGroup->SetLine(m_linesTail);

      // check that there is nothing except comments left on this line
      bool bCont = true;
      while ( *++pEnd != '\0' && bCont ) {
        switch ( *pEnd ) {
          case '#':
          case ';':
            bCont = false;
            break;

          case ' ':
          case '\t':
            // ignore whitespace ('\n' impossible here)
            break;

          default:
            wxLogWarning(Parent->getLogMessage("WX_ERRORS/LINE_IGNORED"),
                         file.GetName(), n + 1, pEnd);
            bCont = false;
        }
      }
    }
    else {                        // a key
      const char *pEnd = pStart;
      while ( IsValid(*pEnd) )
        pEnd++;

      mstring strKey(pStart, pEnd);

      // skip whitespace
      while ( isspace(*pEnd) )
        pEnd++;

      if ( *pEnd++ != '=' ) {
        wxLogError(Parent->getLogMessage("WX_ERRORS/EQUALS_EXPECTED"),
                   file.GetName(), n + 1);
      }
      else {
        ConfigEntry *pEntry = m_pCurrentGroup->FindEntry(strKey);

        if ( pEntry == NULL ) {
          // new entry
          pEntry = m_pCurrentGroup->AddEntry(strKey, n);

          if ( bLocal )
            pEntry->SetLine(m_linesTail);
        }
        else {
          if ( bLocal && pEntry->IsImmutable() ) {
            // immutable keys can't be changed by user
            wxLogWarning(Parent->getLogMessage("WX_ERRORS/IMMUT_IGNORE"),
                         file.GetName(), n + 1, strKey.c_str());
            continue;
          }
          // the condition below catches the cases (a) and (b) but not (c):
          //  (a) global key found second time in global file
          //  (b) key found second (or more) time in local file
          //  (c) key from global file now found in local one
          // which is exactly what we want.
          else if ( !bLocal || pEntry->IsLocal() ) {
            wxLogWarning(Parent->getLogMessage("WX_ERRORS/DUPLICATE_KEY"),
                         file.GetName(), n + 1, strKey.c_str(), pEntry->Line());

            if ( bLocal )
              pEntry->SetLine(m_linesTail);
          }
        }

        // skip whitespace
        while ( isspace(*pEnd) )
          pEnd++;

        pEntry->SetValue(FilterIn(pEnd), false /* read from file */);
      }
    }
  }
}

// ----------------------------------------------------------------------------
// set/retrieve path
// ----------------------------------------------------------------------------

void wxFileConfig::SetRootPath()
{
  NFT("wxFileConfig::SetRootPath");
  m_strPath.Empty();
  m_pCurrentGroup = m_pRootGroup;
}

void wxFileConfig::SetPath(const mstring& strPath)
{
  FT("wxFileConfig::SetPath", (strPath));
  mArrayString aParts;

  if ( strPath.IsEmpty() ) {
    SetRootPath();
    return;
  }

  if ( strPath[0U] == wxCONFIG_PATH_SEPARATOR ) {
    // absolute path
    wxSplitPath(aParts, strPath);
  }
  else {
    // relative path, combine with current one
    mstring strFullPath = m_strPath;
    strFullPath << wxCONFIG_PATH_SEPARATOR << strPath;
    wxSplitPath(aParts, strFullPath);
  }

  // change current group
  size_t n;
  m_strPath.Empty();
  m_pCurrentGroup = m_pRootGroup;
  for ( n = 0; n < aParts.size(); n++ ) {
    ConfigGroup *pNextGroup = m_pCurrentGroup->FindSubgroup(aParts[n]);
    if ( pNextGroup == NULL )
      if (wxConfigBase::CreateOnDemand() )
      {
        pNextGroup = m_pCurrentGroup->AddSubgroup(aParts[n]);
      }
      else
      {
        n=aParts.size();
      }
    if (n<aParts.size())
    {
      m_pCurrentGroup = pNextGroup;
      m_strPath << wxCONFIG_PATH_SEPARATOR << aParts[n];
    }
  }
}

// ----------------------------------------------------------------------------
// enumeration
// ----------------------------------------------------------------------------

bool wxFileConfig::GetFirstGroup(mstring& str, long& lIndex) const
{
  FT("wxFileConfig::GetFirstGroup", (str, lIndex));
  lIndex = 0;
  bool retval = GetNextGroup(str, lIndex);
  RET(retval);
}

bool wxFileConfig::GetNextGroup (mstring& str, long& lIndex) const
{
  FT("wxFileConfig::GetNextGroup", (str, lIndex));
  if ( size_t(lIndex) < m_pCurrentGroup->Groups().Count() ) {
    str = m_pCurrentGroup->Groups()[lIndex++]->Name();
    RET(true);
  }
  else
    RET(false);
}

bool wxFileConfig::GetFirstEntry(mstring& str, long& lIndex) const
{
  FT("wxFileConfig::GetFirstEntry", (str, lIndex));
  lIndex = 0;
  bool retval = GetNextEntry(str, lIndex);
  RET(retval);
}

bool wxFileConfig::GetNextEntry (mstring& str, long& lIndex) const
{
  FT("wxFileConfig::GetNextEntry", (str, lIndex));
  if ( size_t(lIndex) < m_pCurrentGroup->Entries().Count() ) {
    str = m_pCurrentGroup->Entries()[lIndex++]->Name();
    RET(true);
  }
  else
    RET(false);
}

size_t wxFileConfig::GetNumberOfEntries(bool bRecursive) const
{
  FT("wxFileConfig::GetNumberOfEntries", (bRecursive));
  size_t n = m_pCurrentGroup->Entries().Count();
  if ( bRecursive ) {
    ConfigGroup *pOldCurrentGroup = m_pCurrentGroup;
    size_t nSubgroups = m_pCurrentGroup->Groups().Count();
    for ( size_t nGroup = 0; nGroup < nSubgroups; nGroup++ ) {
      CONST_CAST m_pCurrentGroup = m_pCurrentGroup->Groups()[nGroup];
      n += GetNumberOfEntries(true);
      CONST_CAST m_pCurrentGroup = pOldCurrentGroup;
    }
  }

  RET(n);
}

size_t wxFileConfig::GetNumberOfGroups(bool bRecursive) const
{
  FT("wxFileConfig::GetNumberOfGroups", (bRecursive));
  size_t n = m_pCurrentGroup->Groups().Count();
  if ( bRecursive ) {
    ConfigGroup *pOldCurrentGroup = m_pCurrentGroup;
    size_t nSubgroups = m_pCurrentGroup->Groups().Count();
    for ( size_t nGroup = 0; nGroup < nSubgroups; nGroup++ ) {
      CONST_CAST m_pCurrentGroup = m_pCurrentGroup->Groups()[nGroup];
      n += GetNumberOfGroups(true);
      CONST_CAST m_pCurrentGroup = pOldCurrentGroup;
    }
  }

  RET(n);
}

// ----------------------------------------------------------------------------
// tests for existence
// ----------------------------------------------------------------------------

bool wxFileConfig::HasGroup(const mstring& strName) const
{
  FT("wxFileConfig::HasGroup", (strName));
  wxConfigPathChanger path(this, strName);

  ConfigGroup *pGroup;
  if (strName.Contains("/"))
    pGroup = m_pCurrentGroup->FindSubgroup(path.Name());
  else
    pGroup = m_pCurrentGroup;
  RET(pGroup != NULL);
}

bool wxFileConfig::HasEntry(const mstring& strName) const
{
  FT("wxFileConfig::HasEntry", (strName));
  wxConfigPathChanger path(this, strName);

  ConfigEntry *pEntry = m_pCurrentGroup->FindEntry(path.Name());
  RET(pEntry != NULL);
}

// ----------------------------------------------------------------------------
// read/write values
// ----------------------------------------------------------------------------

bool wxFileConfig::Read(const mstring& key,
                        mstring* pStr) const
{
  FT("wxFileConfig::Read", (key, pStr));
  wxConfigPathChanger path(this, key);

  ConfigEntry *pEntry = m_pCurrentGroup->FindEntry(path.Name());
  if (pEntry == NULL) {
    RET(false);
  }
  else {
    *pStr = ExpandEnvVars(pEntry->Value());
    RET(true);
  }
}

bool wxFileConfig::Read(const mstring& key,
                        mstring* pStr, const mstring& defVal) const
{
  FT("wxFileConfig::Read", (key, pStr, defVal));
  wxConfigPathChanger path(this, key);

  ConfigEntry *pEntry = m_pCurrentGroup->FindEntry(path.Name());
  if (pEntry == NULL) {
    if( IsRecordingDefaults() )
      ((wxFileConfig *)this)->Write(key,defVal);
    *pStr = ExpandEnvVars(defVal);
    RET(false);
  }
  else {
    *pStr = ExpandEnvVars(pEntry->Value());
    RET(true);
  }
}

bool wxFileConfig::Read(const mstring& key, long *val) const
{
  FT("wxFileConfig::Read", (key, val));
  RET(wxConfigBase::Read(key, val));
}

bool wxFileConfig::Read(const mstring& key, long *val, const long& defVal) const
{
  FT("wxFileConfig::Read", (key, val, defVal));
  RET(wxConfigBase::Read(key, val, defVal));
}

bool wxFileConfig::Read(const mstring& key, unsigned long *val) const
{
  FT("wxFileConfig::Read", (key, val));
  RET(wxConfigBase::Read(key, val));
}

bool wxFileConfig::Read(const mstring& key, unsigned long *val, const unsigned long& defVal) const
{
  FT("wxFileConfig::Read", (key, val, defVal));
  RET(wxConfigBase::Read(key, val, defVal));
}

bool wxFileConfig::Read(const mstring& key, int *val) const
{
  FT("wxFileConfig::Read", (key, val));
  RET(wxConfigBase::Read(key, val));
}

bool wxFileConfig::Read(const mstring& key, int *val, const int& defVal) const
{
  FT("wxFileConfig::Read", (key, val, defVal));
  RET(wxConfigBase::Read(key, val, defVal));
}

bool wxFileConfig::Read(const mstring& key, unsigned int *val) const
{
  FT("wxFileConfig::Read", (key, val));
  RET(wxConfigBase::Read(key, val));
}

bool wxFileConfig::Read(const mstring& key, unsigned int *val, const unsigned int& defVal) const
{
  FT("wxFileConfig::Read", (key, val, defVal));
  RET(wxConfigBase::Read(key, val, defVal));
}

bool wxFileConfig::Read(const mstring& key, float *val) const
{
  FT("wxFileConfig::Read", (key, val));
  RET(wxConfigBase::Read(key, val));
}

bool wxFileConfig::Read(const mstring& key, float *val, const float& defVal) const
{
  FT("wxFileConfig::Read", (key, val, defVal));
  RET(wxConfigBase::Read(key, val, defVal));
}

bool wxFileConfig::Read(const mstring& key, bool *val) const
{
  FT("wxFileConfig::Read", (key, val));
  RET(wxConfigBase::Read(key, val));
}

bool wxFileConfig::Read(const mstring& key, bool *val, const bool& defVal) const
{
  FT("wxFileConfig::Read", (key, val, defVal));
  RET(wxConfigBase::Read(key, val, defVal));
}

bool wxFileConfig::Write(const mstring& key, const mstring& szValue)
{
  FT("wxFileConfig::Write", (key, szValue));
  wxConfigPathChanger path(this, key);

  mstring strName = path.Name();
  if ( strName.IsEmpty() ) {
    // setting the value of a group is an error
    wxASSERT_MSG( szValue=="", "can't set value of a group!");

    // ... except if it's empty in which case it's a way to force it's creation
    m_pCurrentGroup->SetDirty();

    // this will add a line for this group if it didn't have it before
    (void)m_pCurrentGroup->GetGroupLine();
  }
  else {
    // writing an entry

    // check that the name is reasonable
    if ( strName[0u] == wxCONFIG_IMMUTABLE_PREFIX ) {
      wxLogError(Parent->getLogMessage("WX_ERRORS/MAYNOTSTART"),
                 wxCONFIG_IMMUTABLE_PREFIX);
      RET(false);
    }

    for ( const char *pc = strName; *pc != '\0'; pc++ ) {
      if ( !IsValid(*pc) ) {
        wxLogError(Parent->getLogMessage("WX_ERRORS/MAYNOTCONTAIN"),
                   *pc);
        RET(false);
      }
    }

    ConfigEntry *pEntry = m_pCurrentGroup->FindEntry(strName);
    if ( pEntry == NULL )
      pEntry = m_pCurrentGroup->AddEntry(strName);

    pEntry->SetValue(szValue);
  }

  RET(true);
}

bool wxFileConfig::Flush(bool bCurrentOnly)
{
  FT("wxFileConfig::Flush",(bCurrentOnly));
  if ( LineListIsEmpty() || !m_pRootGroup->IsDirty() )
    RET(true);

  wxTempFile file(m_strLocalFile);

  if ( !file.IsOpened() ) {
    wxLogError(Parent->getLogMessage("WX_ERRORS/USER_CFG"),
	  m_strLocalFile.c_str());
    RET(false);
  }

  // write all strings to file
  for ( LineList *p = m_linesHead; p != NULL; p = p->Next() ) {
    if ( !file.Write(p->Text() + wxTextFile::GetEOL()) ) {
      wxLogError(Parent->getLogMessage("WX_ERRORS/USER_CFG_WRITE"),
	  m_strLocalFile.c_str());
      RET(false);
    }
  }

  RET(file.Commit());
}

// ----------------------------------------------------------------------------
// renaming groups/entries
// ----------------------------------------------------------------------------

bool wxFileConfig::RenameEntry(const mstring& oldName,
                               const mstring& newName)
{
    FT("wxFileConfig::RenameEntry", (oldName, newName));
    // check that the entry exists
    ConfigEntry *oldEntry = m_pCurrentGroup->FindEntry(oldName);
    if ( !oldEntry )
        RET(false);

    // check that the new entry doesn't already exist
    if ( m_pCurrentGroup->FindEntry(newName) )
        RET(false);

    // delete the old entry, create the new one
    mstring value = oldEntry->Value();
    if ( !m_pCurrentGroup->DeleteEntry(oldName) )
        RET(false);

    ConfigEntry *newEntry = m_pCurrentGroup->AddEntry(newName);
    newEntry->SetValue(value);

    RET(true);
}

bool wxFileConfig::RenameGroup(const mstring& oldName,
                               const mstring& newName)
{
    FT("wxFileConfig::RenameGroup", (oldName, newName));
    // check that the group exists
    ConfigGroup *group = m_pCurrentGroup->FindSubgroup(oldName);
    if ( !group )
        RET(false);

    // check that the new group doesn't already exist
    if ( m_pCurrentGroup->FindSubgroup(newName) )
        RET(false);

    group->Rename(newName);

    RET(true);
}

// ----------------------------------------------------------------------------
// delete groups/entries
// ----------------------------------------------------------------------------

bool wxFileConfig::DeleteEntry(const mstring& key, bool bGroupIfEmptyAlso)
{
  FT("wxFileConfig::DeleteEntry", (key, bGroupIfEmptyAlso));
  wxConfigPathChanger path(this, key);

  if ( !m_pCurrentGroup->DeleteEntry(path.Name()) )
    RET(false);

  if ( bGroupIfEmptyAlso && m_pCurrentGroup->IsEmpty() ) {
    if ( m_pCurrentGroup != m_pRootGroup ) {
      ConfigGroup *pGroup = m_pCurrentGroup;
      SetPath("..");  // changes m_pCurrentGroup!
      m_pCurrentGroup->DeleteSubgroupByName(pGroup->Name());
    }
    //else: never delete the root group
  }

  RET(true);
}

bool wxFileConfig::DeleteGroup(const mstring& key)
{
  FT("wxFileConfig::DeleteGroup", (key));
  wxConfigPathChanger path(this, key);

  RET(m_pCurrentGroup->DeleteSubgroupByName(path.Name()));
}

bool wxFileConfig::DeleteAll()
{
  NFT("wxFileConfig::DeleteAll");
  CleanUp();

  const char *szFile = m_strLocalFile;

  if ( remove(szFile) == -1 )
    wxLogSysError(Parent->getMessage("WX_ERRORS/USER_CFG_DEL"), szFile);

  m_strLocalFile = m_strGlobalFile = "";
  Init();

  RET(true);
}

// ----------------------------------------------------------------------------
// linked list functions
// ----------------------------------------------------------------------------

// append a new line to the end of the list
LineList *wxFileConfig::LineListAppend(const mstring& str)
{
  FT("wxFileConfig::LineListAppend", (str));
  LineList *pLine = new LineList(str);

  if ( m_linesTail == NULL ) {
    // list is empty
    m_linesHead = pLine;
  }
  else {
    // adjust pointers
    m_linesTail->SetNext(pLine);
    pLine->SetPrev(m_linesTail);
  }

  m_linesTail = pLine;
  RET(m_linesTail);
}

// insert a new line after the given one or in the very beginning if !pLine
LineList *wxFileConfig::LineListInsert(const mstring& str,
                                                     LineList *pLine)
{
  FT("wxFileConfig::LineListInsert", (str, pLine));
  if ( pLine == m_linesTail )
    RET(LineListAppend(str));

  LineList *pNewLine = new LineList(str);
  if ( pLine == NULL ) {
    // prepend to the list
    pNewLine->SetNext(m_linesHead);
    m_linesHead->SetPrev(pNewLine);
    m_linesHead = pNewLine;
  }
  else {
    // insert before pLine
    LineList *pNext = pLine->Next();
    pNewLine->SetNext(pNext);
    pNewLine->SetPrev(pLine);
    pNext->SetPrev(pNewLine);
    pLine->SetNext(pNewLine);
  }

  RET(pNewLine);
}

void wxFileConfig::LineListRemove(LineList *pLine)
{
  FT("wxFileConfig::LineListRemove", (pLine));
  LineList *pPrev = pLine->Prev(),
           *pNext = pLine->Next();

  // first entry?
  if ( pPrev == NULL )
    m_linesHead = pNext;
  else
    pPrev->SetNext(pNext);

  // last entry?
  if ( pNext == NULL )
    m_linesTail = pPrev;
  else
    pNext->SetPrev(pPrev);

  delete pLine;
}

bool wxFileConfig::LineListIsEmpty()
{
  NFT("wxFileConfig::LineListIsEmpty");
  RET(m_linesHead == NULL);
}

// ============================================================================
// wxFileConfig::ConfigGroup
// ============================================================================

// ----------------------------------------------------------------------------
// ctor/dtor
// ----------------------------------------------------------------------------

// ctor
ConfigGroup::ConfigGroup(ConfigGroup *pParent,
                                       const mstring& strName,
                                       wxFileConfig *pConfig)
                         : m_aEntries(CompareEntries),
                           m_aSubgroups(CompareGroups),
                           m_strName(strName)
{
  FT("ConfigGroup::ConfigGroup", (pParent, strName, "(wxFileConfig) pConfig"));
  m_pConfig = pConfig;
  m_pParent = pParent;
  m_bDirty  = false;
  m_pLine   = NULL;

  m_pLastEntry = NULL;
  m_pLastGroup = NULL;
}

// dtor deletes all children
ConfigGroup::~ConfigGroup()
{
  NFT("ConfigGroup::~ConfigGroup");
  // entries
  size_t n, nCount = m_aEntries.Count();
  for ( n = 0; n < nCount; n++ )
    delete m_aEntries[n];

  // subgroups
  nCount = m_aSubgroups.Count();
  for ( n = 0; n < nCount; n++ )
    delete m_aSubgroups[n];
}

// ----------------------------------------------------------------------------
// line
// ----------------------------------------------------------------------------

void ConfigGroup::SetLine(LineList *pLine)
{
  FT("ConfigGroup::SetLine", (pLine));
  wxASSERT( m_pLine == NULL ); // shouldn't be called twice

  m_pLine = pLine;
}

/*
  This is a bit complicated, so let me explain it in details. All lines that
  were read from the local file (the only one we will ever modify) are stored
  in a (doubly) linked list. Our problem is to know at which position in this
  list should we insert the new entries/subgroups. To solve it we keep three
  variables for each group: m_pLine, m_pLastEntry and m_pLastGroup.

  m_pLine points to the line containing "[group_name]"
  m_pLastEntry points to the last entry of this group in the local file.
  m_pLastGroup                   subgroup

  Initially, they're NULL all three. When the group (an entry/subgroup) is read
  from the local file, the corresponding variable is set. However, if the group
  was read from the global file and then modified or created by the application
  these variables are still NULL and we need to create the corresponding lines.
  See the following functions (and comments preceding them) for the details of
  how we do it.

  Also, when our last entry/group are deleted we need to find the new last
  element - the code in DeleteEntry/Subgroup does this by backtracking the list
  of lines until it either founds an entry/subgroup (and this is the new last
  element) or the m_pLine of the group, in which case there are no more entries
  (or subgroups) left and m_pLast<element> becomes NULL.

  NB: This last problem could be avoided for entries if we added new entries
      immediately after m_pLine, but in this case the entries would appear
      backwards in the config file (OTOH, it's not that important) and as we
      would still need to do it for the subgroups the code wouldn't have been
      significantly less complicated.
*/

// Return the line which contains "[our name]". If we're still not in the list,
// add our line to it immediately after the last line of our parent group if we
// have it or in the very beginning if we're the root group.
LineList *ConfigGroup::GetGroupLine()
{
  NFT("ConfigGroup::GetGroupLine");
  if ( m_pLine == NULL ) {
    ConfigGroup *pParent = this->Parent();

    // this group wasn't present in local config file, add it now
    if ( pParent != NULL ) {
      mstring strFullName;
      strFullName << "[" << (GetFullName().c_str() + 1) << "]"; // +1: no '/'
      m_pLine = m_pConfig->LineListInsert(strFullName,
                                          pParent->GetLastGroupLine());
      pParent->SetLastGroup(this);  // we're surely after all the others
    }
    else {
      // we return NULL, so that LineListInsert() will insert us in the
      // very beginning
    }
  }

  RET(m_pLine);
}

// Return the last line belonging to the subgroups of this group (after which
// we can add a new subgroup), if we don't have any subgroups or entries our
// last line is the group line (m_pLine) itself.
LineList *ConfigGroup::GetLastGroupLine()
{
  NFT("ConfigGroup::GetLastGroupLine");
  // if we have any subgroups, our last line is the last line of the last
  // subgroup
  if ( m_pLastGroup != NULL ) {
    LineList *pLine = m_pLastGroup->GetLastGroupLine();

    wxASSERT( pLine != NULL );  // last group must have !NULL associated line
    RET(pLine);
  }

  // no subgroups, so the last line is the line of thelast entry (if any)
  RET(GetLastEntryLine());
}

// return the last line belonging to the entries of this group (after which
// we can add a new entry), if we don't have any entries we will add the new
// one immediately after the group line itself.
LineList *ConfigGroup::GetLastEntryLine()
{
  NFT("ConfigGroup::GetLastEntryLine");
  if ( m_pLastEntry != NULL ) {
    LineList *pLine = m_pLastEntry->GetLine();

    wxASSERT( pLine != NULL );  // last entry must have !NULL associated line
    RET(pLine);
  }

  // no entries: insert after the group header
  RET(GetGroupLine());
}

// ----------------------------------------------------------------------------
// group name
// ----------------------------------------------------------------------------

void ConfigGroup::Rename(const mstring& newName)
{
    FT("ConfigGroup::Rename", (newName));
    m_strName = newName;

    LineList *line = GetGroupLine();
    mstring strFullName;
    strFullName << "[" << (GetFullName().c_str() + 1) << "]"; // +1: no '/'
    line->SetText(strFullName);

    SetDirty();
}

mstring ConfigGroup::GetFullName() const
{
  NFT("ConfigGroup::GetFullName");
  if ( this->Parent() )
  {
    RET(this->Parent()->GetFullName() + wxCONFIG_PATH_SEPARATOR + Name());
  }
  else
    RET("");
}

// ----------------------------------------------------------------------------
// find an item
// ----------------------------------------------------------------------------

// use binary search because the array is sorted
ConfigEntry *
ConfigGroup::FindEntry(const char *szName) const
{
  FT("ConfigGroup::FindEntry", (szName));
  size_t i,
       lo = 0,
       hi = m_aEntries.Count();
  int res;
  ConfigEntry *pEntry;

  while ( lo < hi ) {
    i = (lo + hi)/2;
    pEntry = m_aEntries[i];

      res = strcmp(pEntry->Name(), szName);

    if ( res > 0 )
      hi = i;
    else if ( res < 0 )
      lo = i + 1;
    else
      RET(pEntry);
  }

  RET((ConfigEntry *) NULL);
}

ConfigGroup *
ConfigGroup::FindSubgroup(const char *szName) const
{
  FT("ConfigGroup::FindSubgroup", (szName));
  size_t i,
       lo = 0,
       hi = m_aSubgroups.Count();
  int res;
  ConfigGroup *pGroup;

  while ( lo < hi ) {
    i = (lo + hi)/2;
    pGroup = m_aSubgroups[i];

      res = strcmp(pGroup->Name(), szName);

    if ( res > 0 )
      hi = i;
    else if ( res < 0 )
      lo = i + 1;
    else
      RET(pGroup);
  }

  RET((ConfigGroup *) NULL);
}

// ----------------------------------------------------------------------------
// create a new item
// ----------------------------------------------------------------------------

// create a new entry and add it to the current group
ConfigEntry *
ConfigGroup::AddEntry(const mstring& strName, int nLine)
{
  FT("ConfigGroup::AddEntry", (strName, nLine));
  wxASSERT( FindEntry(strName) == NULL );

  ConfigEntry *pEntry = new ConfigEntry(this, strName, nLine);
  m_aEntries.Add(pEntry);

  RET(pEntry);
}

// create a new group and add it to the current group
ConfigGroup *
ConfigGroup::AddSubgroup(const mstring& strName)
{
  FT("ConfigGroup::AddSubgroup", (strName));
  wxASSERT( FindSubgroup(strName) == NULL );

  ConfigGroup *pGroup = new ConfigGroup(this, strName, m_pConfig);
  m_aSubgroups.Add(pGroup);

  RET(pGroup);
}

// ----------------------------------------------------------------------------
// delete an item
// ----------------------------------------------------------------------------

/*
  The delete operations are _very_ slow if we delete the last item of this
  group (see comments before GetXXXLineXXX functions for more details),
  so it's much better to start with the first entry/group if we want to
  delete several of them.
 */

bool ConfigGroup::DeleteSubgroupByName(const char *szName)
{
  FT("ConfigGroup::DeleteSubgroupByName", (szName));
  RET(DeleteSubgroup(FindSubgroup(szName)));
}

// doesn't delete the subgroup itself, but does remove references to it from
// all other data structures (and normally the returned pointer should be
// deleted a.s.a.p. because there is nothing much to be done with it anyhow)
bool ConfigGroup::DeleteSubgroup(ConfigGroup *pGroup)
{
  FT("ConfigGroup::DeleteSubgroup", (pGroup));
  wxCHECK( pGroup != NULL, false ); // deleting non existing group?

  // delete all entries
  size_t nCount = pGroup->m_aEntries.Count();
  for ( size_t nEntry = 0; nEntry < nCount; nEntry++ ) {
    LineList *pLine = pGroup->m_aEntries[nEntry]->GetLine();
    if ( pLine != NULL )
      m_pConfig->LineListRemove(pLine);
  }

  // and subgroups of this sungroup
  nCount = pGroup->m_aSubgroups.Count();
  for ( size_t nGroup = 0; nGroup < nCount; nGroup++ ) {
    pGroup->DeleteSubgroup(pGroup->m_aSubgroups[nGroup]);
  }

  LineList *pLine = pGroup->m_pLine;
  if ( pLine != NULL ) {
    // notice that we may do this test inside the previous "if" because the
    // last entry's line is surely !NULL
    if ( pGroup == m_pLastGroup ) {
      // our last entry is being deleted - find the last one which stays
      wxASSERT( m_pLine != NULL );  // we have a subgroup with !NULL pLine...

      // go back until we find a subgroup or reach the group's line
      ConfigGroup *pNewLast = NULL;
      size_t n, nSubgroups = m_aSubgroups.Count();
      LineList *pl;
      for ( pl = pLine->Prev(); pl != m_pLine; pl = pl->Prev() ) {
        // is it our subgroup?
        for ( n = 0; (pNewLast == NULL) && (n < nSubgroups); n++ ) {
          // do _not_ call GetGroupLine! we don't want to add it to the local
          // file if it's not already there
          if ( m_aSubgroups[n]->m_pLine == m_pLine )
            pNewLast = m_aSubgroups[n];
        }

        if ( pNewLast != NULL ) // found?
          break;
      }

      if ( pl == m_pLine ) {
        wxASSERT( !pNewLast );  // how comes it has the same line as we?

        // we've reached the group line without finding any subgroups
        m_pLastGroup = NULL;
      }
      else
        m_pLastGroup = pNewLast;
    }

    m_pConfig->LineListRemove(pLine);
  }

  SetDirty();

  m_aSubgroups.Remove(pGroup);
  delete pGroup;

  RET(true);
}

bool ConfigGroup::DeleteEntry(const char *szName)
{
  FT("ConfigGroup::DeleteEntry", (szName));
  ConfigEntry *pEntry = FindEntry(szName);
  wxCHECK( pEntry != NULL, false );  // deleting non existing item?

  LineList *pLine = pEntry->GetLine();
  if ( pLine != NULL ) {
    // notice that we may do this test inside the previous "if" because the
    // last entry's line is surely !NULL
    if ( pEntry == m_pLastEntry ) {
      // our last entry is being deleted - find the last one which stays
      wxASSERT( m_pLine != NULL );  // if we have an entry with !NULL pLine...

      // go back until we find another entry or reach the group's line
      ConfigEntry *pNewLast = NULL;
      size_t n, nEntries = m_aEntries.Count();
      LineList *pl;
      for ( pl = pLine->Prev(); pl != m_pLine; pl = pl->Prev() ) {
        // is it our subgroup?
        for ( n = 0; (pNewLast == NULL) && (n < nEntries); n++ ) {
          if ( m_aEntries[n]->GetLine() == m_pLine )
            pNewLast = m_aEntries[n];
        }

        if ( pNewLast != NULL ) // found?
          break;
      }

      if ( pl == m_pLine ) {
        wxASSERT( !pNewLast );  // how comes it has the same line as we?

        // we've reached the group line without finding any subgroups
        m_pLastEntry = NULL;
      }
      else
        m_pLastEntry = pNewLast;
    }

    m_pConfig->LineListRemove(pLine);
  }

  // we must be written back for the changes to be saved
  SetDirty();

  m_aEntries.Remove(pEntry);
  delete pEntry;

  RET(true);
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
void ConfigGroup::SetDirty()
{
  NFT("ConfigGroup::SetDirty");
  m_bDirty = true;
  if ( this->Parent() != NULL )             // propagate upwards
    this->Parent()->SetDirty();
}

// ============================================================================
// wxFileConfig::ConfigEntry
// ============================================================================

// ----------------------------------------------------------------------------
// ctor
// ----------------------------------------------------------------------------
ConfigEntry::ConfigEntry(ConfigGroup *pParent,
                                       const mstring& strName,
                                       int nLine)
                         : m_strName(strName)
{
  FT("ConfigEntry::ConfigEntry", (pParent, strName, nLine));
  wxASSERT( !strName.IsEmpty() );

  m_pParent = pParent;
  m_nLine   = nLine;
  m_pLine   = NULL;

  m_bDirty  = false;

  m_bImmutable = strName[0U] == wxCONFIG_IMMUTABLE_PREFIX;
  if ( m_bImmutable )
    m_strName.erase(0, 1);  // remove first character
}

// ----------------------------------------------------------------------------
// set value
// ----------------------------------------------------------------------------

void ConfigEntry::SetLine(LineList *pLine)
{
  FT("ConfigEntry::SetLine", (pLine));
  if ( m_pLine != NULL ) {
    wxLogWarning(Parent->getLogMessage("WX_ERRORS/DUPLICATE_KEY2"),
                 Name().c_str(), m_pParent->GetFullName().c_str());
  }

  m_pLine = pLine;
  Group()->SetLastEntry(this);
}

// second parameter is false if we read the value from file and prevents the
// entry from being marked as 'dirty'
void ConfigEntry::SetValue(const mstring& strValue, bool bUser)
{
  FT("ConfigEntry::SetValue", (strValue, bUser));
  if ( bUser && IsImmutable() ) {
    wxLogWarning(Parent->getLogMessage("WX_ERRORS/IMMUT_IGNORE2"),
                 Name().c_str());
    return;
  }

  // do nothing if it's the same value
  if ( strValue == m_strValue )
    return;

  m_strValue = strValue;

  if ( bUser ) {
    mstring strVal = FilterOut(strValue);
    mstring strLine;
    strLine << m_strName << " = " << strVal;

    if ( m_pLine != NULL ) {
      // entry was read from the local config file, just modify the line
      m_pLine->SetText(strLine);
    }
    else {
      // add a new line to the file
      wxASSERT( m_nLine == -1 );   // consistency check

      m_pLine = Group()->Config()->LineListInsert(strLine,
                                                  Group()->GetLastEntryLine());
      Group()->SetLastEntry(this);
    }

    SetDirty();
  }
}

void ConfigEntry::SetDirty()
{
  NFT("ConfigEntry::SetDirty");
  m_bDirty = true;
  Group()->SetDirty();
}

// ============================================================================
// global functions
// ============================================================================

// ----------------------------------------------------------------------------
// compare functions for array sorting
// ----------------------------------------------------------------------------

int CompareEntries(ConfigEntry *p1,
                   ConfigEntry *p2)
{
    FT("CompareEntries", (p1, p2));
    RET(strcmp(p1->Name(), p2->Name()));
}

int CompareGroups(ConfigGroup *p1,
                  ConfigGroup *p2)
{
    FT("CompareGroups", (p1, p2));
    RET(strcmp(p1->Name(), p2->Name()));
}

// ----------------------------------------------------------------------------
// filter functions
// ----------------------------------------------------------------------------

// undo FilterOut
mstring FilterIn(const mstring& str)
{
  FT("FilterIn", (str));
  mstring strResult;
//  strResult.resize(str.size(),' ');

  bool bQuoted = !str.IsEmpty() && str[0U] == '"';

  for ( size_t n = bQuoted ? 1 : 0; n < str.size(); n++ ) {
    if ( str[n] == '\\' ) {
      switch ( str[++n] ) {
        case 'n':
          strResult += '\n';
          break;

        case 'r':
          strResult += '\r';
          break;

        case 't':
          strResult += '\t';
          break;

        case '\\':
          strResult += '\\';
          break;

        case '"':
          strResult += '"';
          break;
      }
    }
    else {
      if ( str[n] != '"' || !bQuoted )
        strResult += str[n];
      else if ( n != str.size() - 1 ) {
        wxLogWarning(Parent->getLogMessage("WX_ERRORS/UNEXPECTED_CHAR2"),
                     '\"',n, str.c_str());
      }
      //else: it's the last quote of a quoted string, ok
    }
  }

  RET(strResult);
}

// quote the string before writing it to file
mstring FilterOut(const mstring& str)
{
  FT("FilterOut", (str));
   if(str.IsEmpty())
      RET(str);

  mstring strResult;
//  strResult.resize(str.size(),' ');

  // quoting is necessary to preserve spaces in the beginning of the string
  bool bQuote = isspace(str[0U]) || str[0U] == '"';

  if ( bQuote )
    strResult += '"';

  char c;
  for ( size_t n = 0; n < str.size(); n++ ) {
    switch ( str[n] ) {
      case '\n':
        c = 'n';
        break;

      case '\r':
        c = 'r';
        break;

      case '\t':
        c = 't';
        break;

      case '\\':
        c = '\\';
        break;

      case '"':
        if ( bQuote ) {
          c = '"';
          break;
        }
        //else: fall through

      default:
        strResult += str[n];
        continue;   // nothing special to do
    }

    // we get here only for special characters
    strResult << '\\' << c;
  }

  if ( bQuote )
    strResult += '"';

  RET(strResult);
}

#ifndef max
  #define max(a, b)   (((a) > (b)) ? (a) : (b))
#endif

// ============================================================================
// constants
// ============================================================================

// size increment = max(50% of current size, ARRAY_MAXSIZE_INCREMENT)
#define   ARRAY_MAXSIZE_INCREMENT    4096

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxBaseArray - dynamice array of 'long's
// ----------------------------------------------------------------------------

// ctor
wxBaseArray::wxBaseArray()
{
  NFT("wxBaseArray::wxBaseArray");
  m_nSize  =
  m_nCount = 0;
  m_pItems  = (long *) NULL;
}

// copy ctor
wxBaseArray::wxBaseArray(const wxBaseArray& src)
{
  FT("wxBaseArray::wxBaseArray", ("(wxBaseArray) src"));
  m_nSize  = // not src.m_nSize to save memory
  m_nCount = src.m_nCount;

  if ( m_nSize != 0 ) {
    m_pItems = new long[m_nSize];
    memcpy(m_pItems, src.m_pItems, m_nCount*sizeof(long));
  }
  else
    m_pItems = (long *) NULL;
}

// assignment operator
wxBaseArray& wxBaseArray::operator=(const wxBaseArray& src)
{
  FT("wxBaseArray::operator=", ("(wxBaseArray) src"));
#if 0
  wxDELETEA(m_pItems);
#else
        if ( (m_pItems)) {
                delete (m_pItems);
                (m_pItems) = 0;
        }
#endif

  m_nSize  = // not src.m_nSize to save memory
  m_nCount = src.m_nCount;

  if ( m_nSize != 0 ) {
    m_pItems = new long[m_nSize];
    memcpy(m_pItems, src.m_pItems, m_nCount*sizeof(long));
  }
  else
    m_pItems = (long *) NULL;

  NRET(wxBaseArray, *this);
}

// grow the array
void wxBaseArray::Grow()
{
  NFT("wxBaseArray::Grow");
  // only do it if no more place
  if( m_nCount == m_nSize ) {
    if( m_nSize == 0 ) {
      // was empty, alloc some memory
      m_nSize = WX_ARRAY_DEFAULT_INITIAL_SIZE;
      m_pItems = new long[m_nSize];
    }
    else
    {
      // add 50% but not too much
      size_t nIncrement = m_nSize < WX_ARRAY_DEFAULT_INITIAL_SIZE
                         ? WX_ARRAY_DEFAULT_INITIAL_SIZE : m_nSize >> 1;
      if ( nIncrement > ARRAY_MAXSIZE_INCREMENT )
        nIncrement = ARRAY_MAXSIZE_INCREMENT;
      m_nSize += nIncrement;
      long *pNew = new long[m_nSize];

      // copy data to new location
      memcpy(pNew, m_pItems, m_nCount*sizeof(long));
      delete [] m_pItems;
      m_pItems = pNew;
    }
  }
}

// dtor
wxBaseArray::~wxBaseArray()
{
  NFT("wxBaseArray::~wxBaseArray");
  delete [] m_pItems;
}

// clears the list
void wxBaseArray::Clear()
{
  NFT("wxBaseArray::Clear");
  m_nSize  =
  m_nCount = 0;

  delete [] m_pItems;
}

// pre-allocates memory (frees the previous data!)
void wxBaseArray::Alloc(size_t nSize)
{
  FT("wxBaseArray::Alloc", (nSize));
  wxASSERT( nSize > 0 );

  // only if old buffer was not big enough
  if ( nSize > m_nSize ) {
    delete [] m_pItems;
    m_pItems = new long[nSize];
    m_nSize  = nSize;
  }

  m_nCount = 0;
}

// minimizes the memory usage by freeing unused memory
void wxBaseArray::Shrink()
{
  NFT("wxBaseArray::Shrink");
  // only do it if we have some memory to free
  if( m_nCount < m_nSize ) {
    // allocates exactly as much memory as we need
    long *pNew = new long[m_nCount];

    // copy data to new location
    memcpy(pNew, m_pItems, m_nCount*sizeof(long));
    delete [] m_pItems;
    m_pItems = pNew;
  }
}

// searches the array for an item (forward or backwards)
int wxBaseArray::Index(long lItem, bool bFromEnd) const
{
  FT("wxBaseArray::Index", (lItem, bFromEnd));
  if ( bFromEnd ) {
    if ( m_nCount > 0 ) {
      size_t n = m_nCount;
      do {
        if ( m_pItems[--n] == lItem )
          RET(n);
      }
      while ( n != 0 );
    }
  }
  else {
    for( size_t n = 0; n < m_nCount; n++ ) {
      if( m_pItems[n] == lItem )
        RET(n);
    }
  }

  RET(-1);
}

// search for an item in a sorted array (binary search)
int wxBaseArray::Index(long lItem, CMPFUNC fnCompare) const
{
  FT("wxBaseArray::Index", (lItem, "(CMPFUNC) fnCompare"));
  size_t i,
       lo = 0,
       hi = m_nCount;
  int res;

  while ( lo < hi ) {
    i = (lo + hi)/2;

    res = (*fnCompare)((const void *)lItem, (const void *)m_pItems[i]);
    if ( res < 0 )
      hi = i;
    else if ( res > 0 )
      lo = i + 1;
    else
      RET(i);
  }

  RET(-1);
}
// add item at the end
void wxBaseArray::Add(long lItem)
{
  FT("wxBaseArray::Add", (lItem));
  Grow();
  m_pItems[m_nCount++] = lItem;
}

// add item assuming the array is sorted with fnCompare function
void wxBaseArray::Add(long lItem, CMPFUNC fnCompare)
{
  FT("wxBaseArray::Add", (lItem, "(CMPFUNC) fnCompare"));
  size_t i,
       lo = 0,
       hi = m_nCount;
  int res;

  while ( lo < hi ) {
    i = (lo + hi)/2;

    res = (*fnCompare)((const void *)lItem, (const void *)m_pItems[i]);
    if ( res < 0 )
      hi = i;
    else if ( res > 0 )
      lo = i + 1;
    else {
      lo = hi = i;
      break;
    }
  }

  wxASSERT( lo == hi ); // I hope I got binary search right :-)

  Insert(lItem, lo);
}

// add item at the given position
void wxBaseArray::Insert(long lItem, size_t nIndex)
{
  FT("wxBaseArray::Insert", (lItem, nIndex));
  wxCHECK_RET( (nIndex <= m_nCount), "bad index in wxArray::Insert" );

  Grow();

  memmove(&m_pItems[nIndex + 1], &m_pItems[nIndex],
          (m_nCount - nIndex)*sizeof(long));
  m_pItems[nIndex] = lItem;
  m_nCount++;
}

// removes item from array (by index)
void wxBaseArray::Remove(size_t nIndex)
{
  FT("wxBaseArray::Remove", (nIndex));
  wxCHECK_RET( nIndex <= m_nCount, "bad index in wxArray::Remove" );

  memmove(&m_pItems[nIndex], &m_pItems[nIndex + 1],
          (m_nCount - nIndex - 1)*sizeof(long));
  m_nCount--;
}

// removes item from array (by value)
void wxBaseArray::Remove(long lItem)
{
  FT("wxBaseArray::Remove", (lItem));
  int iIndex = Index(lItem);

  wxCHECK_RET( iIndex != -1,
               "removing inexistent item in wxArray::Remove" );

  Remove((size_t)iIndex);
}

// sort array elements using passed comparaison function
void wxBaseArray::Sort(CMPFUNC fCmp)
{
  FT("wxBaseArray::Sort", ("(CMPFUNC) fCmp"));
  qsort(m_pItems, m_nCount, sizeof(long), fCmp);
}

const char* wxGetHomeDir(mstring *pstr)
{
  FT("wxGetHomeDir", (pstr));
  mstring& strDir = *pstr;

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
          RET(strDir.c_str());
      }

    // 260 was taken from windef.h
    #ifndef MAX_PATH
      #define MAX_PATH  260
    #endif

	char cstrPath[MAX_PATH];
    mstring strPath;
    ::GetModuleFileName(::GetModuleHandle(NULL),
                        cstrPath, MAX_PATH);
    strPath=cstrPath;

    // extract the dir name
    wxSplitPath(strPath, &strDir, NULL, NULL);
  #else
    const char *szHome = getenv("HOME");
    if ( szHome == NULL ) {
      // we're homeless...
      wxLogWarning(Parent->getLogMessage("WX_ERRORS/NOHOMEDIR"));
      strDir = ".";
    }
    else
       strDir = szHome;

    // add a trailing slash if needed
    if ( strDir.Last() != '/' )
      strDir << '/';
  #endif

  RET(strDir.c_str());
}

size_t  wxBaseArray::Count() const   
{
    NFT("wxBaseArray::Count");
    RET(m_nCount);      
}

size_t  wxBaseArray::GetCount() const   
{
    NFT("wxBaseArray::GetCount");
    RET(m_nCount);      
}

bool  wxBaseArray::IsEmpty() const 
{
    NFT("wxBaseArray::IsEmpty");
    RET(m_nCount == 0); 
}

long& wxBaseArray::Item(size_t uiIndex) const 
{
    FT("wxBaseArray::Item", (uiIndex));
    wxASSERT( uiIndex < m_nCount );
    RET(m_pItems[uiIndex]); 
}

long& wxBaseArray::operator[](size_t uiIndex) const 
{
    FT("wxBaseArray::operator[]", (uiIndex));
    RET(Item(uiIndex)); 
}

void LineList::SetNext(LineList *pNext)  
{
    FT("LineList::SetNext", (pNext));
    m_pNext = pNext; 
}

void LineList::SetPrev(LineList *pPrev)  
{
    FT("LineList::SetPrev", (pPrev));
    m_pPrev = pPrev; 
}

LineList *LineList::Next() const              
{
    NFT("LineList::Next");
    RET(m_pNext);  
}

LineList *LineList::Prev() const
{
    NFT("LineList::Prev");
    RET(m_pPrev);  
}

void LineList::SetText(const mstring& str) 
{
    FT("LineList::SetText", (str));
    m_strLine = str;  
}

const mstring& LineList::Text() const
{
    NFT("LineList::Text");
    RET(m_strLine); 
}

const mstring& wxFileConfig::GetPath() const 
{
    NFT("wxFileConfig::GetPath");
    RET(m_strPath); 
}


const mstring& ConfigEntry::Name()        const 
{
    NFT("ConfigEntry::Name");
    RET(m_strName);    
}

const mstring& ConfigEntry::Value()       const 
{
    NFT("ConfigEntry::Value");
    RET(m_strValue);   
}

ConfigGroup    *ConfigEntry::Group()       const 
{
    NFT("ConfigEntry::Group");
    RET(m_pParent);    
}

bool            ConfigEntry::IsDirty()     const 
{
    NFT("ConfigEntry::IsDirty");
    RET(m_bDirty);     
}

bool            ConfigEntry::IsImmutable() const 
{
    NFT("ConfigEntry::IsImmutable");
    RET(m_bImmutable); 
}

bool            ConfigEntry::IsLocal()     const 
{
    NFT("ConfigEntry::IsLocal");
    RET(m_pLine != 0); 
}

int             ConfigEntry::Line()        const 
{
    NFT("ConfigEntry::Line");
    RET(m_nLine);      
}

LineList       *ConfigEntry::GetLine()     const 
{
    NFT("ConfigEntry::GetLine");
    RET(m_pLine);      
}

const mstring& ConfigGroup::Name()    const 
{
    NFT("ConfigGroup::Name");
    RET(m_strName); 
}

ConfigGroup    *ConfigGroup::Parent()  const 
{
    NFT("ConfigGroup::Parent");
    RET(m_pParent); 
}

wxFileConfig   *ConfigGroup::Config()  const 
{
    NFT("ConfigGroup::Config");
    NRET(wxFileConfig, m_pConfig); 
}

bool            ConfigGroup::IsDirty() const 
{
    NFT("ConfigGroup::IsDirty");
    RET(m_bDirty);  
}

const wxFileConfig::ArrayEntries& ConfigGroup::Entries() const 
{
    NFT("ConfigGroup::Entries");
    NRET(wxFileConfig::ArrayEntries, m_aEntries);   
}

const wxFileConfig::ArrayGroups&  ConfigGroup::Groups()  const 
{
    NFT("ConfigGroup::Groups");
    NRET(wxFileConfig::ArrayGroups, m_aSubgroups); 
}

bool  ConfigGroup::IsEmpty() const 
{
    NFT("ConfigGroup::IsEmpty");
    RET(Entries().IsEmpty() && Groups().IsEmpty()); 
}

void ConfigGroup::SetLastEntry(ConfigEntry *pEntry) 
{
    FT("ConfigGroup::SetLastEntry", (pEntry));
    m_pLastEntry = pEntry; 
}

void ConfigGroup::SetLastGroup(ConfigGroup *pGroup) 
{
    FT("ConfigGroup::SetLastGroup", (pGroup));
    m_pLastGroup = pGroup; 
}
