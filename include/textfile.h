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
** ==========================================================
#pragma ident "$Id$"
** ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.11  2000/02/15 10:37:47  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#ifndef _TEXTFILE_H
#define _TEXTFILE_H

///////////////////////////////////////////////////////////////////////////////
// Name:        textfile.h
// Purpose:     class wxTextFile to work with text files of _small_ size
//              (file is fully loaded in memory) and which understands CR/LF
//              differences between platforms.
// Author:      Vadim Zeitlin
// Modified by: Ungod for magick2
// Created:     03.04.98
// RCS-ID:      $Id$
// Copyright:   (c) 1998 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows license
///////////////////////////////////////////////////////////////////////////////

#include "mstring.h"
#include "mstream.h"
//#include "log.h"

enum wxTextFileType
{
    wxTextFileType_None,  // incomplete (the last line of the file only)
    wxTextFileType_Unix,  // line is terminated with 'LF' = 0xA = 10 = '\n'
    wxTextFileType_Dos,   //                         'CR' 'LF'
    wxTextFileType_Mac    //                         'CR' = 0xD = 13 = '\r'
};

typedef vector<wxTextFileType> ArrayFileType;

class wxTextFile  
{
public:
  // default type for current platform (determined at compile time)
  static const wxTextFileType typeDefault;

  // ctors
    // def ctor, use Open(string)
  wxTextFile() { }
    //
  wxTextFile(const mstring& strFile);

  // file operations
    // file exists?
  bool Exists() const;
    // Open() also loads file in memory on success
  bool Open();
    // same as Open() but with (another) file name
  bool Open(const mstring& strFile);
    // closes the file and frees memory, losing all changes
  bool Close();
    // is file currently opened?
  bool IsOpened() const;

  // accessors
    // get the number of lines in the file
  size_t GetLineCount() const;
    // the returned line may be modified (but don't add CR/LF at the end!)
  mstring& GetLine(size_t n)    const;
  mstring& operator[](size_t n) const;

    // the current line has meaning only when you're using
    // GetFirstLine()/GetNextLine() functions, it doesn't get updated when
    // you're using "direct access" i.e. GetLine()
  size_t GetCurrentLine() const;
  void GoToLine(size_t n);
  bool Eof() const;

    // these methods allow more "iterator-like" traversal of the list of
    // lines, i.e. you may write something like:
    //  for ( str = GetFirstLine(); !Eof(); str = GetNextLine() ) { ... }

    // @@@ const is commented out because not all compilers understand
    //     'mutable' keyword yet (m_nCurLine should be mutable)
  mstring& GetFirstLine() /* const */;
  mstring& GetNextLine()  /* const */;
  mstring& GetPrevLine()  /* const */;
  mstring& GetLastLine() /* const */;

    // get the type of the line (see also GetEOL)
  wxTextFileType GetLineType(size_t n) const;
    // guess the type of file (m_file is supposed to be opened)
  wxTextFileType GuessType() const;
    // get the name of the file
  const char *GetName() const;

  // add/remove lines
    // add a line to the end
  void AddLine(const mstring& str, wxTextFileType type = typeDefault);
    // insert a line before the line number n
  void InsertLine(const mstring& str,
                  size_t n,
                  wxTextFileType type = typeDefault);
    // delete one line
  void RemoveLine(size_t n);

  // change the file on disk (default argument means "don't change type")
  // possibly in another format
  bool Write(wxTextFileType typeNew = wxTextFileType_None);

  // get the file termination string
  // Note: implementation moved to textfile to prevent warning due to switch.
  static const char *GetEOL(wxTextFileType type = typeDefault);

  // dtor
  ~wxTextFile();

private:
  // copy ctor/assignment operator not implemented
  wxTextFile(const wxTextFile&);
  wxTextFile& operator=(const wxTextFile&);

  // read the file in memory (m_file is supposed to be just opened)
  bool Read();

  wxFile        m_file;     // current file
  ArrayFileType m_aTypes;   // type of each line
  vector<mstring> m_aLines;   // lines of file

  size_t        m_nCurLine; // number of current line in the file

  mstring      m_strFile;  // name of the file

};

#endif
