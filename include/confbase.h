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
** Revision 1.15  2000/02/15 10:37:47  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#ifndef _CONFBASE_H
#define _CONFBASE_H

///////////////////////////////////////////////////////////////////////////////
// Name:        confbase.h
// Purpose:     declaration of the base class of all config implementations
//              (see also: fileconf.h and msw/regconf.h)
// Author:      Karsten Ballnder & Vadim Zeitlin
// Modified by: Ungod for use in Magick2
// Created:     07.04.98 (adapted from appconf.h)
// RCS-ID:      $Id$
// Copyright:   (c) 1997 Karsten Ballnder   Ballueder@usa.net
//                       Vadim Zeitlin      <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows license
///////////////////////////////////////////////////////////////////////////////

#include "mstring.h"

/// shall we be case sensitive in parsing variable names?
const bool  wxCONFIG_CASE_SENSITIVE       =false;

/// separates group and entry names (probably shouldn't be changed)
const char  wxCONFIG_PATH_SEPARATOR       ='/';

/// introduces immutable entries
// (i.e. the ones which can't be changed from the local config file)
const char   wxCONFIG_IMMUTABLE_PREFIX    ='!';

// Style flags for constructor style parameter
const int wxCONFIG_USE_LOCAL_FILE         =1;
const int wxCONFIG_USE_GLOBAL_FILE        =2;

/*
  Replace environment variables ($SOMETHING) with their values. The format is
  $VARNAME or ${VARNAME} where VARNAME contains alphanumeric characters and
  '_' only. '$' must be escaped ('\$') in order to be taken literally.
 */
extern mstring wxExpandEnvVars(const mstring &sz);

/*
  Split path into parts removing '..' in progress
 */
typedef vector<mstring> mArrayString;
extern void wxSplitPath(mArrayString& aParts, const mstring& sz);

// ----------------------------------------------------------------------------
// abstract base class wxConfigBase which defines the interface for derived
// classes
//
// wxConfig organizes the items in a tree-like structure (modeled after the
// Unix/Dos filesystem). There are groups (directories) and keys (files).
// There is always one current group given by the current path.
//
// Keys are pairs "key_name = value" where value may be of string or integer
// (long) type (@@@ doubles and other types such as wxDate coming soon).
// ----------------------------------------------------------------------------
class wxConfigBase
{
public:
  // static functions
    // sets the config object, returns the previous pointer
  static wxConfigBase *Set(wxConfigBase *pConfig);
    // get the config object, creates it on demand unless DontCreateOnDemand
    // was called
  static wxConfigBase *Get();
    // create a new config object: this function will create the "best"
    // implementation of wxConfig available for the current platform, see
    // comments near definition wxCONFIG_WIN32_NATIVE for details. It returns
    // the created object and also sets it as ms_pConfig.
  static wxConfigBase *Create();
    // should Get() try to create a new log object if the current one is NULL?
  static void CreateOnDemand(bool in);
    // Return status of above command
  static bool CreateOnDemand();

  // ctor & virtual dtor
    // environment variable expansion is on by default
//  wxConfigBase() { m_bExpandEnvVars = true; m_bRecordDefaults = false; }

  // ctor
  // Not all args will always be used by derived classes, but
  // including them all in each class ensures compatibility.
  // If appName is empty, uses wxApp name
  wxConfigBase(const mstring& appName = "", const mstring& vendorName = "",
               const mstring& localFilename = "", const mstring& globalFilename = "",
               long style = 0);

    // empty but ensures that dtor of all derived classes is virtual
  virtual ~wxConfigBase() { }

  // path management
    // set current path: if the first character is '/', it's the absolute path,
    // otherwise it's a relative path. '..' is supported. If the strPath
    // doesn't exist it is created.
  virtual void SetPath(const mstring& strPath) = 0;
    // retrieve the current path (always as absolute path)
  virtual const mstring& GetPath() const = 0;

  // enumeration: all functions here return false when there are no more items.
  // you must pass the same lIndex to GetNext and GetFirst (don't modify it)
    // enumerate subgroups
  virtual bool GetFirstGroup(mstring& str, long& lIndex) const = 0;
  virtual bool GetNextGroup (mstring& str, long& lIndex) const = 0;
    // enumerate entries
  virtual bool GetFirstEntry(mstring& str, long& lIndex) const = 0;
  virtual bool GetNextEntry (mstring& str, long& lIndex) const = 0;
    // get number of entries/subgroups in the current group, with or without
    // it's subgroups
  virtual size_t GetNumberOfEntries(bool bRecursive = false) const = 0;
  virtual size_t GetNumberOfGroups(bool bRecursive = false) const = 0;

  // tests of existence
    // returns true if the group by this name exists
  virtual bool HasGroup(const mstring& strName) const = 0;
    // same as above, but for an entry
  virtual bool HasEntry(const mstring& strName) const = 0;
    // returns true if either a group or an entry with a given name exist
  bool Exists(const mstring& strName) const;

  // read a string from the key
  virtual mstring Read(const mstring& key, const mstring& defVal= "") const;

  // key access: returns true if value was really read, false if default used
  // (and if the key is not found the default value is returned.)

  // mstring
  virtual bool Read(const mstring& key, mstring* pStr) const = 0;
  virtual bool Read(const mstring& key, mstring* pStr, const mstring& defVal) const;

  // long
  virtual bool Read(const mstring& key, long* val) const;
  virtual bool Read(const mstring& key, long* val, const long& defVal) const;
  virtual bool Read(const mstring& key, unsigned long* val) const;
  virtual bool Read(const mstring& key, unsigned long* val, const unsigned long& defVal) const;

  // int
  virtual bool Read(const mstring& key, int* val) const;
  virtual bool Read(const mstring& key, int* val, const int& defVal) const;
  virtual bool Read(const mstring& key, unsigned int* val) const;
  virtual bool Read(const mstring& key, unsigned int* val, const unsigned int& defVal) const;

  // float
  virtual bool Read(const mstring& key, float* val) const;
  virtual bool Read(const mstring& key, float* val, const float& defVal) const;

  // bool
  virtual bool Read(const mstring& key, bool* val) const;
  virtual bool Read(const mstring& key, bool* val, const bool& defVal) const;

    // write the value (return true on success)
  virtual bool Write(const mstring& key, const mstring& value) = 0;

    // permanently writes all changes
  virtual bool Flush(bool bCurrentOnly = false) = 0;

  // delete entries/groups
    // deletes the specified entry and the group it belongs to if
    // it was the last key in it and the second parameter is true
  virtual bool DeleteEntry(const mstring& key,
                           bool bDeleteGroupIfEmpty = true) = 0;
    // delete the group (with all subgroups)
  virtual bool DeleteGroup(const mstring& key) = 0;
    // delete the whole underlying object (disk file, registry key, ...)
    // primarly for use by desinstallation routine.
  virtual bool DeleteAll() = 0;

  // options
    // we can automatically expand environment variables in the config entries
    // (this option is on by default, you can turn it on/off at any time)
  bool IsExpandingEnvVars() const;
  void SetExpandEnvVars(bool bDoIt = true);
    // recording of default values
  void SetRecordDefaults(bool bDoIt = true);
  bool IsRecordingDefaults() const;
  // does expansion only if needed
  mstring ExpandEnvVars(const mstring& str) const;

    // misc accessors
  mstring GetAppName() const;
  mstring GetVendorName() const;

  void SetAppName(const mstring& appName);
  void SetVendorName(const mstring& vendorName);
  void SetStyle(long style);
  long GetStyle() const;

protected:
  static bool IsImmutable(const mstring& key);

private:
  // are we doing automatic environment variable expansion?
  bool m_bExpandEnvVars;
  // do we record default values?
  bool m_bRecordDefaults;

  // static variables
  static wxConfigBase *ms_pConfig;
  static bool          ms_bAutoCreate;

  // Application name and organisation name
  mstring          m_appName;
  mstring          m_vendorName;

  // Style flag
  long              m_style;
};

// a handy little class which changes current path to the path of given entry
// and restores it in dtor: so if you declare a local variable of this type,
// you work in the entry directory and the path is automatically restored
// when the function returns
// Taken out of wxConfig since not all compilers can cope with nested classes.
class wxConfigPathChanger
{
public:
  // ctor/dtor do path changing/restorin
  wxConfigPathChanger(const wxConfigBase *pContainer, const mstring& strEntry);
 ~wxConfigPathChanger();

  // get the key name
  const mstring& Name() const;

private:
  wxConfigBase *m_pContainer;   // object we live in
  mstring      m_strName,      // name of entry (i.e. name only)
                m_strOldPath;   // saved path
  bool          m_bChanged;     // was the path changed?
};

#endif
