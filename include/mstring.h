#ifndef WIN32
#pragma interface
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
#ifndef _MSTRING_H
#define _MSTRING_H
#include "pch.h"
static const char *ident_mstring_h = "@(#) $Id$";
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.49  2000/10/07 11:00:11  ungod
** no message
**
** Revision 1.48  2000/09/30 10:48:06  prez
** Some general code cleanups ... got rid of warnings, etc.
**
** Revision 1.47  2000/08/03 13:06:29  prez
** Fixed a bunch of stuff in mstring (caused exceptions on FreeBSD machines).
**
** Revision 1.46  2000/07/29 21:58:52  prez
** Fixed XML loading of weird characters ...
** 2 known bugs now, 1) last_seen dates are loaded incorrectly on alot
** of nicknames, which means we expire lots of nicknames.  2) services
** wont rejoin a +i/+k channel when last user exits.
**
** Revision 1.45  2000/07/28 14:49:34  prez
** Ditched the old wx stuff, mconfig now in use, we're now ready to
** release (only got some conversion tests to do).
**
** Revision 1.44  2000/05/28 05:05:13  prez
** More makefile stuff ... Now we should work on all platforms.
** Added alot of checking for different .h files, functions, etc.
** So now all #define's are config.h based (also added a default
** windows config.h, which will need to be copied on these systems).
**
** Revision 1.43  2000/05/17 14:08:11  prez
** More tweaking with DCC, and getting iostream mods working ...
**
** Revision 1.42  2000/02/27 03:58:39  prez
** Fixed the WHAT program, also removed RegEx from Magick.
**
** Revision 1.41  2000/02/23 12:21:02  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
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


class mstring:public string
{
	typedef string inherited;
public:
	mstring(const mstring& in);
	mstring(const string& in);
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
	friend mstring operator+(const mstring& inp_string, char ch);
	friend mstring operator+(char ch, const mstring& inp_string);
	friend mstring operator+(const mstring& inp_string, const char *psz);
	friend mstring operator+(const char *psz, const mstring& inp_string);
	/* friend mstring operator+(const mstring& inp_string, const mstring& s);
	friend mstring operator+(const mstring& s, const mstring& inp_string); */
	friend mstring operator+(const mstring& inp_string, double d);
	friend mstring operator+(double d, const mstring& inp_string);
	friend mstring operator+(const mstring& inp_string, float f);
	friend mstring operator+(float f, const mstring& inp_string);
	friend mstring operator+(const mstring& inp_string, long l);
	friend mstring operator+(long l, const mstring& inp_string);
	friend mstring operator+(const mstring& inp_string, unsigned long l);
	friend mstring operator+(unsigned long l, const mstring& inp_string);
	friend mstring operator+(const mstring& inp_string, int i);
	friend mstring operator+(int i, const mstring& inp_string);
	friend mstring operator+(const mstring& inp_string, unsigned int i);
	friend mstring operator+(unsigned int i, const mstring& inp_string);
	mstring operator()(size_t start,size_t len);
	operator const char *() const { return c_str(); }

	mstring UpperCase() const;
    // values for first parameter of Strip function
	enum stripType {stLeading, stTrailing , stBoth };
	mstring Strip(stripType s=stTrailing);
	size_t Replace(const mstring& szOld, const mstring& szNew, bool replaceAll=true);
	mstring& RemoveLast();
	mstring& Remove(int nStart, size_t nLen);
	mstring& Remove(int pos);
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
	int Index(const mstring& in, bool caseSensitive=true, bool fromEnd=false)const;
	int Index(char ch, int startpos=0)const;
	int First(const mstring& in)const;
	int First(char c)const;
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
	bool Matches(const mstring& in, bool nocase = false)const;
	mstring& Truncate(int uiLine);
	mstring& Pad(size_t nCount, char chPad=' ', bool bFromRight=true);
	mstring& Trim(bool bFromRight=true);
	void MakeLower();
	void MakeUpper();
	mstring RevAfter(const mstring& in) const;
	mstring RevBefore(const mstring& in) const;
	mstring After(const mstring& in, int count=1) const;
	mstring Before(const mstring& in,int count=1) const;
	mstring Right(int nCount)const;
	mstring Left(int nCount)const;
	mstring SubString(int from, int to)const;
	mstring Mid(int nFirst, int nCount=npos)const;
	bool IsSameAs(const mstring& in, bool bCase=true);
	int CmpNoCase(const mstring& in);
	int Cmp(const mstring& in);
	unsigned int WordCount(const mstring &separators, bool assemble = true)const;
	mstring ExtractWord(int count,const mstring& separators, bool assemble = true)const;
	unsigned int WordPosition(unsigned int count,const mstring& separators, bool assemble = true)const;
};

#ifndef HAVE_ITOA
const char *itoa(int i);
#endif
#ifndef HAVE_LTOA
const char *ltoa(long l);
#endif
#ifndef HAVE_FTOA
const char *ftoa(float f);
#endif
#ifndef HAVE_DTOA
const char *dtoa(double d);
#endif
#ifndef HAVE_ULTOA
const char *ultoa(unsigned long ul);
#endif
#ifndef HAVE_UITOA
const char *uitoa(unsigned int ui);
#endif

extern const mstring DirSlash;
extern const mstring Blank;
extern const mstring IRC_Bold;
extern const mstring IRC_Underline;
extern const mstring IRC_Reverse;
extern const mstring IRC_Colour;
extern const mstring IRC_Off;

extern bool match_wild (const char *pattern, const char *str, bool nocase = false);

#endif
