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
// Name:        textfile.cpp
// Purpose:     class wxTextFile to work with text files of _small_ size
//              (file is fully loaded in memory) and which understands CR/LF
//              differences between platforms.
// Author:      Vadim Zeitlin
// Modified by: W. King for use with Magick II
// Created:     03.04.98
// RCS-ID:      $Id$
// Copyright:   (c) 1998 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows license
///////////////////////////////////////////////////////////////////////////////

#include "textfile.h"
#include "log.h"
#include "trace.h"

// default type is the native one
const wxTextFileType wxTextFile::typeDefault =
#if   defined(WIN32)
  wxTextFileType_Dos;
#elif defined(__UNIX__)
  wxTextFileType_Unix;
#else
  wxTextFileType_None;
  #error  "wxTextFile: unsupported platform."
#endif

// ----------------------------------------------------------------------------
// ctors & dtor
// ----------------------------------------------------------------------------

wxTextFile::wxTextFile(const mstring& strFile) : m_strFile(strFile)
{
}

wxTextFile::~wxTextFile()
{
  // m_file dtor called automatically
}

// ----------------------------------------------------------------------------
// file operations
// ----------------------------------------------------------------------------

bool wxTextFile::Open(const mstring& strFile)
{
  FT("wxTextFile::Open", (strFile));
  m_strFile = strFile;
  RET(Open());
}

bool wxTextFile::Open()
{
  NFT("wxTextFile::Open");
  // file name must be either given in ctor or in Open(const mstring&)
  wxASSERT( !m_strFile.IsEmpty() );

  // open file in read-only mode
  if ( !m_file.Open(m_strFile) )
    RET(false);

  // read file into memory
  bool bRet = Read();

  m_file.Close();

  RET(bRet);
}

// analyse some lines of the file trying to guess it's type.
// if it fails, it assumes the native type for our platform.
wxTextFileType wxTextFile::GuessType() const
{
  NFT("wxTextFile::GuessType");
  // file should be opened and we must be in it's beginning
  wxASSERT( m_file.IsOpened() && m_file.Tell() == 0 );

  // scan the file lines
  size_t nUnix = 0,     // number of '\n's alone
       nDos  = 0,     // number of '\r\n'
       nMac  = 0;     // number of '\r's

  // we take MAX_LINES_SCAN in the beginning, middle and the end of file
  #define MAX_LINES_SCAN    (10)
  size_t nCount = m_aLines.size() / 3,
       nScan =  nCount > 3*MAX_LINES_SCAN ? MAX_LINES_SCAN : nCount / 3;

  #define   AnalyseLine(n)              \
    switch ( m_aTypes[n] ) {            \
      case wxTextFileType_Unix: nUnix++; break;   \
      case wxTextFileType_Dos:  nDos++;  break;   \
      case wxTextFileType_Mac:  nMac++;  break;   \
      default: wxFAIL_MSG(_("unknown line terminator")); \
    }

  size_t n;
  for ( n = 0; n < nScan; n++ )     // the beginning
    AnalyseLine(n);
  for ( n = (nCount - nScan)/2; n < (nCount + nScan)/2; n++ )
    AnalyseLine(n);
  for ( n = nCount - nScan; n < nCount; n++ )
    AnalyseLine(n);

  #undef   AnalyseLine

  // interpret the results (@@ far from being even 50% fool proof)
  if ( nDos + nUnix + nMac == 0 ) 
  {
    // no newlines at all
    wxLogWarning(_("'%s' is probably a binary file."), m_strFile.c_str());
  }
  else 
  {
    #define   GREATER_OF(t1, t2) n##t1 == n##t2 ? typeDefault               \
                                                : n##t1 > n##t2             \
                                                    ? wxTextFileType_##t1     \
                                                    : wxTextFileType_##t2

// Watcom C++ doesn't seem to be able to handle the macro
#if defined(__WATCOMC__)
    RET(typeDefault);
#else
    if ( nDos > nUnix )
    {
      RET(GREATER_OF(Dos, Mac));
    }
    else if ( nDos < nUnix )
    {
      RET(GREATER_OF(Unix, Mac));
    }
    else 
    {
      // nDos == nUnix
      RET((nMac > nDos ? wxTextFileType_Mac : typeDefault));
    }
#endif

    #undef    GREATER_OF
  }

  RET(typeDefault);
}

bool wxTextFile::Read()
{
  NFT("wxTextFile::Read");
  // file should be opened and we must be in it's beginning
  wxASSERT( m_file.IsOpened() && m_file.Tell() == 0 );

  mstring str;
  char ch, chLast = '\0';
  char buf[1024];
  int n, nRead;
  while ( !m_file.Eof() ) {
    nRead = m_file.Read(buf, sizeof(buf));
    if ( nRead == -1 ) {
      // read error (error message already given in wxFile::Read)
      m_file.Close();
      RET(false);
    }

    for ( n = 0; n < nRead; n++ ) {
      ch = buf[n];
      switch ( ch ) {
        case '\n':
          // Dos/Unix line termination
          m_aLines.push_back(str);
          m_aTypes.push_back(chLast == '\r' ? wxTextFileType_Dos
                                      : wxTextFileType_Unix);
          str.Empty();
          chLast = '\n';
          break;

        case '\r':
          if ( chLast == '\r' ) {
            // Mac empty line
            m_aLines.push_back("");
            m_aTypes.push_back(wxTextFileType_Mac);
          }
          else
            chLast = '\r';
          break;

        default:
          if ( chLast == '\r' ) {
            // Mac line termination
            m_aLines.push_back(str);
            m_aTypes.push_back(wxTextFileType_Mac);
            chLast = ch;
            str = ch;
          }
          else {
            // add to the current line
            str += ch;
          }
      }
    }
  }

  // anything in the last line?
  if ( !str.IsEmpty() ) {
    m_aTypes.push_back(wxTextFileType_None);  // no line terminator
    m_aLines.push_back(str);
  }

  RET(true);
}

bool wxTextFile::Write(wxTextFileType typeNew)
{
  FT("wxTextFile::Write", ("(wxTextFileType) typeNew"));
  wxTempFile fileTmp(m_strFile);

  if ( !fileTmp.IsOpened() ) {
    wxLogError(_("can't write file '%s' to disk."), m_strFile.c_str());
    RET(false);
  }

  size_t nCount = m_aLines.size();
  for ( size_t n = 0; n < nCount; n++ ) {
    fileTmp.Write(m_aLines[n] +
                  GetEOL(typeNew == wxTextFileType_None ? m_aTypes[n]
                                                        : typeNew));
  }

  // replace the old file with this one
  RET(fileTmp.Commit());
}

const char *wxTextFile::GetEOL(wxTextFileType type)
{
    FT("wxTextFile::GetEOL", ("(wxTextFileType) type"));
    switch ( type ) {
      case wxTextFileType_None: RET("");
      case wxTextFileType_Unix: RET("\n");
      case wxTextFileType_Dos:  RET("\r\n");
      case wxTextFileType_Mac:  RET("\r");

      default:
        wxFAIL_MSG("bad file type in wxTextFile::GetEOL.");
        RET((const char *) NULL);
    }
}

bool wxTextFile::IsOpened() const
{
    NFT("wxTextFile::IsOpened");
    RET(m_file.IsOpened()); 
}

size_t wxTextFile::GetLineCount() const 
{
    NFT("wxTextFile::GetLineCount");
    RET(m_aLines.size()); 
}

mstring& wxTextFile::GetLine(size_t n)    const 
{
    FT("wxTextFile::GetLine", (n));
    RET(((mstring&)m_aLines[n])); 
}

mstring& wxTextFile::operator[](size_t n) const 
{
    FT("wxTextFile::operator[]", (n));
    RET(((mstring&)m_aLines[n])); 
}

size_t wxTextFile::GetCurrentLine() const 
{
    NFT("wxTextFile::GetCurrentLine");
    RET(m_nCurLine); 
}

void wxTextFile::GoToLine(size_t n) 
{
    FT("wxTextFile::GoToLine", (n));
    m_nCurLine = n; 
}

bool wxTextFile::Eof() const 
{
    NFT("wxTextFile::Eof");
    RET(m_nCurLine == m_aLines.size()); 
}

mstring& wxTextFile::GetFirstLine() /* const */ 
{
    NFT("wxTextFile::GetFirstLine");
    RET(m_aLines[m_nCurLine = 0]); 
}

mstring& wxTextFile::GetNextLine()  /* const */ 
{
    NFT("wxTextFile::GetNextLine");
    RET(m_aLines[++m_nCurLine]);   
}

mstring& wxTextFile::GetPrevLine()  /* const */ 
{
    NFT("wxTextFile::GetPrevLine");
    wxASSERT(m_nCurLine > 0);
    RET(m_aLines[--m_nCurLine]);   
}

mstring& wxTextFile::GetLastLine() /* const */ 
{
    NFT("wxTextFile::GetLastLine");
    RET(m_aLines[m_nCurLine = m_aLines.size() - 1]); 
}

wxTextFileType wxTextFile::GetLineType(size_t n) const 
{
    FT("wxTextFile::GetLineType", (n));
    RET(m_aTypes[n]); 
}

const char *wxTextFile::GetName() const 
{
    NFT("wxTextFile::GetName");
    RET(m_strFile.c_str()); 
}

void wxTextFile::AddLine(const mstring& str, wxTextFileType type) 
{
    FT("wxTextFile::AddLine", (str, type));
    m_aLines.push_back(str);
    m_aTypes.push_back(type); 
}

void wxTextFile::InsertLine(const mstring& str,
                  size_t n,
                  wxTextFileType type) 
{
    FT("wxTextFile::InsertLine", (str, n, type));
    m_aLines.insert(m_aLines.begin()+n, str);
    m_aTypes.insert(m_aTypes.begin()+n, type); 
}

void wxTextFile::RemoveLine(size_t n) 
{
    FT("wxTextFile::RemoveLine", (n));
    m_aLines.erase(m_aLines.begin()+n);
    m_aTypes.erase(m_aTypes.begin()+n); 
}
