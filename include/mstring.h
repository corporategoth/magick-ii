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
** Revision 1.40  2000/02/17 12:55:03  ungod
** still working on borlandization
**
** Revision 1.39  2000/02/15 10:37:47  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#ifndef _MSTRING_H
#define _MSTRING_H

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

// Need these to make SunOS happy
#include <stdio.h>
#include <stdarg.h>

class mstring:public string
{
	typedef string inherited;
public:
	mstring(const mstring& in);
	mstring(const mstring& in, inherited::size_type pos, inherited::size_type n);
	mstring(const char *in);
	mstring(const char *in, inherited::size_type pos);
	mstring(char c); 
/*	mstring(double d); 
	mstring(float f); 
	mstring(long l); 
	mstring(unsigned long l); 
	mstring(int i); 
	mstring(unsigned int i);
*/
	mstring(inherited::size_type n, char c);
	mstring(inherited::const_iterator first, inherited::const_iterator last);
	mstring();

	// below are fast convert_to_string+append functions
	mstring& operator <<(char ch);
	mstring& operator <<(const char *psz);
	mstring& operator <<(const mstring& s);
	mstring& operator <<(double d);
	mstring& operator <<(float f);
	mstring& operator <<(long l);
	mstring& operator <<(unsigned long l);
	mstring& operator <<(int i);
	mstring& operator <<(unsigned int i);
	// non-destructive operators
	friend mstring operator+(const mstring& string, char ch);
	friend mstring operator+(char ch, const mstring& string);
	friend mstring operator+(const mstring& string, const char *psz);
	friend mstring operator+(const char *psz, const mstring& string);
	/* friend mstring operator+(const mstring& string, const mstring& s);
	friend mstring operator+(const mstring& s, const mstring& string); */
	friend mstring operator+(const mstring& string, double d);
	friend mstring operator+(double d, const mstring& string);
	friend mstring operator+(const mstring& string, float f);
	friend mstring operator+(float f, const mstring& string);
	friend mstring operator+(const mstring& string, long l);
	friend mstring operator+(long l, const mstring& string);
	friend mstring operator+(const mstring& string, unsigned long l);
	friend mstring operator+(unsigned long l, const mstring& string);
	friend mstring operator+(const mstring& string, int i);
	friend mstring operator+(int i, const mstring& string);
	friend mstring operator+(const mstring& string, unsigned int i);
	friend mstring operator+(unsigned int i, const mstring& string);
	mstring operator()(size_t start,size_t len);
	operator const char *() const { return c_str(); }

	mstring UpperCase() const;
    // values for first parameter of Strip function
	enum stripType {stLeading, stTrailing , stBoth };
	mstring Strip(stripType s=stTrailing);
	size_t Replace(const mstring& szOld, const mstring& szNew, bool replaceAll=true);
	mstring& RemoveLast();
	mstring& Remove(size_t nStart, size_t nLen);
	mstring& Remove(size_t pos);
	mstring& Remove(mstring str);
	mstring LowerCase() const;
	bool IsWord() const;
	bool IsNumber() const;
	bool IsNull() const;
	bool IsEmpty() const;
	bool IsBool() const;
	bool GetBool() const;
	char Last() const;
	size_t Len() const;
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
	bool RxMatches(const mstring& in)const;
	bool Matches(const mstring& in)const;
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
	unsigned int WordCount(const mstring &separators)const;
	mstring ExtractWord(int count,const mstring& separators)const;
	unsigned int WordPosition(unsigned int count,const mstring& separators)const;
	pair<int,int> RegFind(const mstring& pattern)const;
};

#define NEED_ITOA
#define NEED_FTOA
#define NEED_LTOA

#ifdef NEED_ITOA
const char *itoa(int i);
#endif
#ifdef NEED_FTOA
const char *ftoa(float f);
#endif
#ifdef NEED_LTOA
const char *ltoa(long l);
#endif

extern const mstring DirSlash;
extern const mstring Blank;
extern const mstring IRC_Bold;
extern const mstring IRC_Underline;
extern const mstring IRC_Reverse;
extern const mstring IRC_Colour;
extern const mstring IRC_Off;

#endif
