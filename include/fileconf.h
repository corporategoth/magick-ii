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
#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif
#include "confbase.h"
#include "mstring.h"
#include "textfile.h"

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

class wxFileConfig;
class ConfigGroup;
class ConfigEntry;

const char* wxGetHomeDir(mstring *pstr);

// ----------------------------------------------------------------------------
/**
 base class managing data having size of type 'long' (not used directly)

 NB: for efficiency this often used class has no virtual functions (hence no
     VTBL), even dtor is <B>not</B> virtual. If used as expected it won't
     create any problems because ARRAYs from DEFINE_ARRAY have no dtor at all,
     so it's not too important if it's not called (this happens when you cast
     "SomeArray *" as "BaseArray *" and then delete it)

  @memo Base class for template array classes
*/
// ----------------------------------------------------------------------------

#ifdef  __VISUALC__
  #define   CMPFUNC_CONV    _cdecl
#else   // !Visual C++
  #define   CMPFUNC_CONV
#endif  // compiler
typedef int (CMPFUNC_CONV *CMPFUNC)(const void* pItem1, const void* pItem2);

#define   WX_ARRAY_DEFAULT_INITIAL_SIZE    (16)

class wxBaseArray
{
public:
  /** @name ctors and dtor */
  //@{
    /// default ctor
  wxBaseArray();
    /// copy ctor
  wxBaseArray(const wxBaseArray& array);
    /// assignment operator
  wxBaseArray& operator=(const wxBaseArray& src);
    /// not virtual, see above
    /// EXCEPT for Gnu compiler to reduce warnings...
#ifdef __GNUG__
 virtual
#endif
  ~wxBaseArray();
  //@}

  /** @name memory management */
  //@{
    /// empties the array, but doesn't release memory
  void Empty() { m_nCount = 0; }
    /// empties the array and releases memory
  void Clear();
    /// preallocates memory for given number of items
  void Alloc(size_t uiSize);
    /// minimizes the memory used by the array (frees unused memory)
  void Shrink();
  //@}
  /** @name simple accessors */
  //@{
    /// number of elements in the array
  size_t  Count() const   { return m_nCount;      }
  size_t  GetCount() const   { return m_nCount;      }
    /// is it empty?
  bool  IsEmpty() const { return m_nCount == 0; }
  //@}

protected:
  // these methods are protected because if they were public one could
  // mistakenly call one of them instead of DEFINE_ARRAY's or OBJARRAY's
  // type safe methods

  /** @name items access */
  //@{
    /// get item at position uiIndex (range checking is done in debug version)
  long& Item(size_t uiIndex) const
    { wxASSERT( uiIndex < m_nCount ); return m_pItems[uiIndex]; }
    /// same as Item()
  long& operator[](size_t uiIndex) const { return Item(uiIndex); }
  //@}

  /** @name item management */
  //@{
    /**
      Search the element in the array, starting from the either side
      @param bFromEnd if TRUE, start from the end
      @return index of the first item matched or -1
      @see -1
     */
  int Index(long lItem, bool bFromEnd = FALSE) const;
    /// search for an item using binary search in a sorted array
  int Index(long lItem, CMPFUNC fnCompare) const;
    /// add new element at the end
  void Add(long lItem);
    /// add item assuming the array is sorted with fnCompare function
  void Add(long lItem, CMPFUNC fnCompare);
    /// add new element at given position (it becomes Item[uiIndex])
  void Insert(long lItem, size_t uiIndex);
    /// remove first item matching this value
  void Remove(long lItem);
    /// remove item by index
  void Remove(size_t uiIndex);
  //@}

  /// sort array elements using given compare function
  void Sort(CMPFUNC fnCompare);

private:
  void    Grow();     // makes array bigger if needed

  size_t  m_nSize,    // current size of the array
          m_nCount;   // current number of elements

  long   *m_pItems;   // pointer to data
};

// ----------------------------------------------------------------------------
// This is the same as the previous macro, but it defines a sorted array.
// Differences:
//  1) it must be given a COMPARE function in ctor which takes 2 items of type
//     T* and should return -1, 0 or +1 if the first one is less/greater
//     than/equal to the second one.
//  2) the Add() method inserts the item in such was that the array is always
//     sorted (it uses the COMPARE function)
//  3) it has no Sort() method because it's always sorted
//  4) Index() method is much faster (the sorted arrays use binary search
//     instead of linear one), but Add() is slower.
//
// Summary: use this class when the speed of Index() function is important, use
// the normal arrays otherwise.
//
// NB: it has only inline functions => takes no space at all
// Mod by JACS: Salford C++ doesn't like 'var->operator=' syntax, as in:
//    { ((wxBaseArray *)this)->operator=((const wxBaseArray&)src);
// so using a temporary variable instead.
// ----------------------------------------------------------------------------
#define  _WX_DEFINE_SORTED_ARRAY(T, name)                           \
typedef int (CMPFUNC_CONV *SCMPFUNC##T)(T pItem1, T pItem2);        \
class name : public wxBaseArray									    \
{                                                                   \
public:                                                             \
  name(SCMPFUNC##T fn)                                              \
    { wxASSERT( sizeof(T) <= sizeof(long) ); m_fnCompare = fn; }    \
                                                                    \
  name& operator=(const name& src)                                  \
    { wxBaseArray* temp = (wxBaseArray*) this;                      \
      (*temp) = ((const wxBaseArray&)src);                          \
      m_fnCompare = src.m_fnCompare;                                \
      return *this; }                                               \
                                                                    \
  T& operator[](size_t uiIndex) const                               \
    { return (T&)(wxBaseArray::Item(uiIndex)); }                    \
  T& Item(size_t uiIndex) const                                     \
    { return (T&)(wxBaseArray::Item(uiIndex)); }                    \
  T& Last() const                                                   \
    { return (T&)(wxBaseArray::Item(Count() - 1)); }                \
                                                                    \
  int Index(T Item) const                                           \
    { return wxBaseArray::Index((long)Item, (CMPFUNC)m_fnCompare); }\
                                                                    \
  void Add(T Item)                                                  \
    { wxBaseArray::Add((long)Item, (CMPFUNC)m_fnCompare); }         \
                                                                    \
  void Remove(size_t uiIndex) { wxBaseArray::Remove(uiIndex); }     \
  void Remove(T Item)                                               \
    { int iIndex = Index(Item);                                     \
      wxCHECK2_MSG( iIndex != -1, return,                  \
        "removing inexisting element in wxArray::Remove" );         \
      wxBaseArray::Remove((size_t)iIndex); }                        \
                                                                    \
private:                                                            \
  SCMPFUNC##T m_fnCompare;                                          \
}

  /**
   This macro does the same as WX_DEFINE_ARRAY except that the array will be
   sorted with the specified compare function.
   */
#define WX_DEFINE_SORTED_ARRAY(T, name)  typedef T _A##name;                 \
                                  _WX_DEFINE_SORTED_ARRAY(_A##name, name)


// we store all lines of the local config file as a linked list in memory
class LineList
{
public:
  void      SetNext(LineList *pNext)  { m_pNext = pNext; }
  void      SetPrev(LineList *pPrev)  { m_pPrev = pPrev; }

  // ctor
  LineList(const mstring& str, LineList *pNext = (LineList *) NULL) : m_strLine(str)
    { SetNext(pNext); SetPrev((LineList *) NULL); }

  //
  LineList *Next() const              { return m_pNext;  }
  LineList *Prev() const              { return m_pPrev;  }

  //
  void SetText(const mstring& str) { m_strLine = str;  }
  const mstring& Text() const      { return m_strLine; }

private:
  mstring  m_strLine;      // line contents
  LineList *m_pNext,        // next node
           *m_pPrev;        // previous one
};

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
  static mstring GetGlobalFileName(const char *szFile);
  static mstring GetLocalFileName(const char *szFile);

  // ctor & dtor

#if 0
    // the names of local and global (if not disabled) config files are
    // constructed using Get{Local|Global}FileName functions described above
    // (szAppName is just the (short) name of your application)
  wxFileConfig(const char *szAppName, bool bLocalOnly = FALSE);
    // this ctor allows you to specify custom names for both files (if strGlobal
    // isn't a full path, it's considered to be relative to the standard
    // directory, i.e. /etc under Unix and %windir% under Windows, if strLocal
    // is not an absolute path, it's considered to be relative to the user's
    // directory). If either of strings is empty, the corresponding file is not
    // used.
  wxFileConfig(const mstring& strLocal, const mstring& strGlobal);
#endif

   // New constructor: one size fits all. Specify wxCONFIG_USE_LOCAL_FILE
   // or wxCONFIG_USE_GLOBAL_FILE to say which files should be used.
  wxFileConfig(const mstring& appName, const mstring& vendorName = "",
               const mstring& localFilename = "", const mstring& globalFilename = "",
               long style = wxCONFIG_USE_LOCAL_FILE);

    // dtor will save unsaved data
  virtual ~wxFileConfig();

  // implement inherited pure virtual functions
  virtual void SetPath(const mstring& strPath);
  virtual const mstring& GetPath() const { return m_strPath; }

  virtual bool GetFirstGroup(mstring& str, long& lIndex) const;
  virtual bool GetNextGroup (mstring& str, long& lIndex) const;
  virtual bool GetFirstEntry(mstring& str, long& lIndex) const;
  virtual bool GetNextEntry (mstring& str, long& lIndex) const;

  virtual size_t GetNumberOfEntries(bool bRecursive = FALSE) const;
  virtual size_t GetNumberOfGroups(bool bRecursive = FALSE) const;

  virtual bool HasGroup(const mstring& strName) const;
  virtual bool HasEntry(const mstring& strName) const;

  virtual bool Read(const mstring& key, mstring *pStr) const;
  virtual bool Read(const mstring& key, mstring *pStr, const mstring& defValue) const;
  virtual bool Read(const mstring& key, long *pl) const;

  // The following are necessary to satisfy the compiler
  mstring Read(const mstring& key, const mstring& defVal) const
    { return wxConfigBase::Read(key, defVal); }
  bool Read(const mstring& key, long *pl, long defVal) const
    { return wxConfigBase::Read(key, pl, defVal); }
  long Read(const mstring& key, long defVal) const
    { return wxConfigBase::Read(key, defVal); }
  bool Read(const mstring& key, int *pi, int defVal) const
    { return wxConfigBase::Read(key, pi, defVal); }
  bool Read(const mstring& key, int *pi) const
    { return wxConfigBase::Read(key, pi); }
  bool Read(const mstring& key, double* val) const
    { return wxConfigBase::Read(key, val); }
  bool Read(const mstring& key, double* val, double defVal) const
    { return wxConfigBase::Read(key, val, defVal); }
  bool Read(const mstring& key, bool* val) const
    { return wxConfigBase::Read(key, val); }
  bool Read(const mstring& key, bool* val, bool defVal) const
    { return wxConfigBase::Read(key, val, defVal); }

  virtual bool Write(const mstring& key, const mstring& szValue);
  virtual bool Write(const mstring& key, long lValue);
  bool Write(const mstring& key, double value)
    { return wxConfigBase::Write(key, value); }
  bool Write(const mstring& key, bool value)
    { return wxConfigBase::Write(key, value); }

  virtual bool Flush(bool bCurrentOnly = FALSE);

  virtual bool RenameEntry(const mstring& oldName, const mstring& newName);
  virtual bool RenameGroup(const mstring& oldName, const mstring& newName);

  virtual bool DeleteEntry(const mstring& key, bool bGroupIfEmptyAlso);
  virtual bool DeleteGroup(const mstring& szKey);
  virtual bool DeleteAll();

public:
  // functions to work with this list
  LineList *LineListAppend(const mstring& str);
  LineList *LineListInsert(const mstring& str,
                           LineList *pLine);    // NULL => Prepend()
  void      LineListRemove(LineList *pLine);
  bool      LineListIsEmpty();

private:
  // GetXXXFileName helpers: return ('/' terminated) directory names
  static mstring GetGlobalDir();
  static mstring GetLocalDir();

  // common part of all ctors (assumes that m_str{Local|Global}File are already
  // initialized
  void Init();

  // common part of from dtor and DeleteAll
  void CleanUp();

  // parse the whole file
  void Parse(wxTextFile& file, bool bLocal);

  // the same as SetPath("/")
  void SetRootPath();

  // member variables
  // ----------------
  LineList   *m_linesHead,        // head of the linked list
             *m_linesTail;        // tail

  mstring    m_strLocalFile,     // local  file name passed to ctor
              m_strGlobalFile;    // global
  mstring    m_strPath;          // current path (not '/' terminated)

  ConfigGroup *m_pRootGroup,      // the top (unnamed) group
              *m_pCurrentGroup;   // the current group

public:
  WX_DEFINE_SORTED_ARRAY(ConfigEntry *, ArrayEntries);
  WX_DEFINE_SORTED_ARRAY(ConfigGroup *, ArrayGroups);
};

class ConfigEntry
{
private:
  ConfigGroup  *m_pParent;      // group that contains us
  mstring      m_strName,      // entry name
                m_strValue;     //       value
  bool          m_bDirty,       // changed since last read?
                m_bImmutable;   // can be overriden locally?
  int           m_nLine;        // used if m_pLine == NULL only
  LineList     *m_pLine;        // pointer to our line in the linked list
                                // or NULL if it was found in global file

public:
  ConfigEntry(ConfigGroup *pParent, const mstring& strName, int nLine);

  // simple accessors
  const mstring& Name()        const { return m_strName;    }
  const mstring& Value()       const { return m_strValue;   }
  ConfigGroup    *Group()       const { return m_pParent;    }
  bool            IsDirty()     const { return m_bDirty;     }
  bool            IsImmutable() const { return m_bImmutable; }
  bool            IsLocal()     const { return m_pLine != 0; }
  int             Line()        const { return m_nLine;      }
  LineList       *GetLine()     const { return m_pLine;      }

  // modify entry attributes
  void SetValue(const mstring& strValue, bool bUser = TRUE);
  void SetDirty();
  void SetLine(LineList *pLine);
};

class ConfigGroup
{
private:
  wxFileConfig *m_pConfig;        // config object we belong to
  ConfigGroup  *m_pParent;        // parent group (NULL for root group)
  wxFileConfig::ArrayEntries  m_aEntries;       // entries in this group
  wxFileConfig::ArrayGroups   m_aSubgroups;     // subgroups
  mstring      m_strName;        // group's name
  bool          m_bDirty;         // if FALSE => all subgroups are not dirty
  LineList     *m_pLine;          // pointer to our line in the linked list
  ConfigEntry  *m_pLastEntry;     // last entry/subgroup of this group in the
  ConfigGroup  *m_pLastGroup;     // local file (we insert new ones after it)

  // DeleteSubgroupByName helper
  bool DeleteSubgroup(ConfigGroup *pGroup);

public:
  // ctor
  ConfigGroup(ConfigGroup *pParent, const mstring& strName, wxFileConfig *);

  // dtor deletes all entries and subgroups also
  ~ConfigGroup();

  // simple accessors
  const mstring& Name()    const { return m_strName; }
  ConfigGroup    *Parent()  const { return m_pParent; }
  wxFileConfig   *Config()  const { return m_pConfig; }
  bool            IsDirty() const { return m_bDirty;  }

  const wxFileConfig::ArrayEntries& Entries() const { return m_aEntries;   }
  const wxFileConfig::ArrayGroups&  Groups()  const { return m_aSubgroups; }
  bool  IsEmpty() const { return Entries().IsEmpty() && Groups().IsEmpty(); }

  // find entry/subgroup (NULL if not found)
  ConfigGroup *FindSubgroup(const char *szName) const;
  ConfigEntry *FindEntry   (const char *szName) const;

  // delete entry/subgroup, return FALSE if doesn't exist
  bool DeleteSubgroupByName(const char *szName);
  bool DeleteEntry(const char *szName);

  // create new entry/subgroup returning pointer to newly created element
  ConfigGroup *AddSubgroup(const mstring& strName);
  ConfigEntry *AddEntry   (const mstring& strName, int nLine = -1);

  // will also recursively set parent's dirty flag
  void SetDirty();
  void SetLine(LineList *pLine);

  // rename: no checks are done to ensure that the name is unique!
  void Rename(const mstring& newName);

  //
  mstring GetFullName() const;

  // get the last line belonging to an entry/subgroup of this group
  LineList *GetGroupLine();     // line which contains [group]
  LineList *GetLastEntryLine(); // after which our subgroups start
  LineList *GetLastGroupLine(); // after which the next group starts

  // called by entries/subgroups when they're created/deleted
  void SetLastEntry(ConfigEntry *pEntry) { m_pLastEntry = pEntry; }
  void SetLastGroup(ConfigGroup *pGroup) { m_pLastGroup = pGroup; }
};


#endif