#ifndef _FILECONF_H
#define _FILECONF_H
///////////////////////////////////////////////////////////////////////////////
// Name:        fileconf.h
// Purpose:     wxFileConfig derivation of wxConfigBase
// Author:      Vadim Zeitlin
// Modified by: Ungod for magick2
// Created:     07.04.98 (adapted from appconf.cpp)
// RCS-ID:      $Id$
// Copyright:   (c) 1997 Karsten Ballnder   &  Vadim Zeitlin
//                       Ballueder@usa.net     <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows license
///////////////////////////////////////////////////////////////////////////////
#include "confbase.h"
#include "mstring.h"
#include <vector>
#include <list>
#include <set>
using namespace std;

// ----------------------------------------------------------------------------
// wxFileConfig
// ----------------------------------------------------------------------------

/*
  wxFileConfig derives from base Config and implements file based config class,
  i.e. it uses ASCII disk files to store the information. These files are
  alternatively called INI, .conf or .rc in the documentation. They are
  organized in groups or sections, which can nest (i.e. a group contains
  subgroups, which contain their own subgroups &c). Each group has some
  number of entries, which are "key = value" pairs. More precisely, the format
  is:

  # comments are allowed after either ';' or '#' (Win/UNIX standard)

  # blank lines (as above) are ignored

  # global entries are members of special (no name) top group
  written_for = Windows
  platform    = Linux

  # the start of the group 'Foo'
  [Foo]                           # may put comments like this also
  # following 3 lines are entries
  key = value
  another_key = "  strings with spaces in the beginning should be quoted, \
                   otherwise the spaces are lost"
  last_key = but you don't have to put " normally (nor quote them, like here)

  # subgroup of the group 'Foo'
  # (order is not important, only the name is: separator is '/', as in paths)
  [Foo/Bar]
  # entries prefixed with "!" are immutable, i.e. can't be changed if they are
  # set in the system-wide config file
  !special_key = value
  bar_entry = whatever

  [Foo/Bar/Fubar]   # depth is (theoretically :-) unlimited
  # may have the same name as key in another section
  bar_entry = whatever not

  You have {read/write/delete}Entry functions (guess what they do) and also
  setCurrentPath to select current group. enum{Subgroups/Entries} allow you
  to get all entries in the config file (in the current group). Finally,
  flush() writes immediately all changed entries to disk (otherwise it would
  be done automatically in dtor)

  wxFileConfig manages not less than 2 config files for each program: global
  and local (or system and user if you prefer). Entries are read from both of
  them and the local entries override the global ones unless the latter is
  immutable (prefixed with '!') in which case a warning message is generated
  and local value is ignored. Of course, the changes are always written to local
  file only.

  The names of these files can be specified in a number of ways. First of all,
  you can use the standard convention: using the ctor which takes 'strAppName'
  parameter will probably be sufficient for 90% of cases. If, for whatever
  reason you wish to use the files with some other names, you can always use the
  second ctor.

  wxFileConfig also may automatically expand the values of environment variables
  in the entries it reads: for example, if you have an entry
    score_file = $HOME/.score
  a call to Read(&str, "score_file") will return a complete path to .score file
  unless the expansion was previousle disabled with SetExpandEnvVars(FALSE) call
  (it's on by default, the current status can be retrieved with
   IsExpandingEnvVars function).
*/

  class wxFileConfig; //linea nueva

typedef list<mstring> LineList;

class wxFileConfig : public wxConfigBase
{
public:
  // construct the "standard" full name for global (system-wide) and
  // local (user-specific) config files from the base file name.
  //
  // the following are the filenames returned by this functions:
  //            global                local
  // Unix   /etc/file.ext           ~/.file
  // Win    %windir%\file.ext   %USERPROFILE%\file.ext
  //
  // where file is the basename of szFile, ext is it's extension
  // or .conf (Unix) or .ini (Win) if it has none
  static mstring GetGlobalFileName(const mstring &szFile);
  static mstring GetLocalFileName(const mstring &szFile);

   // New constructor: one size fits all. Specify wxCONFIG_USE_LOCAL_FILE
   // or wxCONFIG_USE_GLOBAL_FILE to say which files should be used.
  wxFileConfig(const mstring& appName, const mstring& vendorName = "",
               const mstring& localFilename = "", const mstring& globalFilename = "",
               long style = wxCONFIG_USE_LOCAL_FILE);

	virtual void SetPath(const mstring& strPath);
	virtual const mstring& GetPath() const { return m_strPath; }

private:
  // GetXXXFileame helpers: return ('/' terminated) directory names
  static mstring GetGlobalDir();
  static mstring GetLocalDir();
private:
	void Init();
  mstring     m_strLocalFile,     // local  file name passed to ctor
              m_strGlobalFile;    // global
  mstring     m_strPath;          // current path (not '/' terminated)
};



#endif