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

// based upon

/////////////////////////////////////////////////////////////////////////////
// Name:        string.cpp
// Purpose:     wxString class
// Author:      Vadim Zeitlin
// Modified by:
// Created:     29/01/98
// RCS-ID:      $Id$
// Copyright:   (c) 1998 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef _MSTRING_H
#define _MSTRING_H

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

class mstring:public string
{
	typedef string inherited;
public:
	char& Last();
	size_t Len() const;
	// below are fast convert_to_string+append functions
	mstring& operator <<(double d);
	mstring& operator <<(float f);
	mstring& operator <<(long l);
	mstring& operator<<(int i);
	mstring operator()(size_t start,size_t len);
	mstring UpperCase() const;
    // values for first parameter of Strip function
	enum stripType {stLeading, stTrailing , stBoth };
	mstring Strip(stripType s=stTrailing);
	size_t Replace(const mstring& szOld, const mstring& szNew, bool replaceAll=true);
	mstring& RemoveLast();
	mstring& Remove(size_t nStart, size_t nLen);
	mstring& Remove(size_t pos);
	mstring LowerCase() const;
	bool IsWord() const;
	bool IsNumber() const;
	bool IsNull() const;
	bool IsEmpty() const;
	bool IsAscii() const;
	size_t Index(const mstring& in, bool caseSensitive=true, bool fromEnd=false)const;
	size_t Index(char ch, int startpos=0)const;
	size_t First(const mstring& in)const;
	size_t First(char c)const;
	int Find(const mstring & in, int count=1)const;
	int Find(char ch, bool bFromEnd=false)const;
	void Empty();
	bool Contains(const mstring& in)const;
	mstring& Prepend(char ch, int count=1);
	mstring& Prepend(const mstring& in);
	mstring& Append(char ch, int count=1);
	mstring& Append(const mstring& in);
	// values for second parameter of CompareTo function
	enum caseCompare {ccExact, ccIgnoreCase};

	int CompareTo(const mstring& in, caseCompare cmp=ccExact);
	int FormatV(const char *pszFormat, va_list argptr);
	int Format(const char *pszFormat, ...);
	// Matches using '*' and '?' rules
	bool Matches(const mstring& in);
	mstring& Truncate(size_t uiLine);
	mstring& Pad(size_t nCount, char chPad=' ', bool bFromRight=true);
	mstring& Trim(bool bFromRight=true);
	void MakeLower();
	void MakeUpper();
	mstring RevAfter(const mstring& in) const;
	mstring RevBefore(const mstring& in) const;
	mstring After(const mstring& in, int count=1) const;
	mstring Before(const mstring& in,int count=1) const;
	mstring Right(size_t nCount)const;
	mstring Left(size_t nCount)const;
	 mstring(const string& in);
	mstring SubString(size_t from, size_t to)const;
	mstring Mid(size_t nFirst, size_t nCount=npos)const;
	bool IsSameAs(const mstring& in, bool bCase=true);
	int CmpNoCase(const mstring& in);
	int Cmp(const mstring& in);
	mstring& operator<<(char ch);
	mstring& operator<<(const char *psz);
	mstring& operator<<(const mstring& s);
	mstring(const mstring& in);
	mstring(const mstring& in, inherited::size_type pos, inherited::size_type n);
	mstring(const char *in);
	mstring(const char *in, inherited::size_type pos);
	mstring(char c); 
	mstring(inherited::size_type n, char c);
	mstring(inherited::const_iterator first, inherited::const_iterator last);
	mstring();
	int WordCount(const mstring &separators)const;
	mstring ExtractWord(int count,const mstring& separators)const;
	int WordPosition(int count,const mstring& separators)const;
	pair<int,int> RegFind(const mstring& pattern)const;

	operator const char *() const { return c_str(); }

    // non-destructive concatenation
      //
  friend mstring operator+(const mstring& string, char ch);
      //
  friend mstring operator+(char ch, const mstring& string);
      //
  friend mstring operator+(const mstring& string, const char *psz);
      //
  friend mstring operator+(const char *psz, const mstring& string);

};

extern const mstring DirSlash;

#endif