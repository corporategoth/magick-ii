#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
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
static const char *ident = "@(#)$Id$";
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.67  2000/08/03 13:06:31  prez
** Fixed a bunch of stuff in mstring (caused exceptions on FreeBSD machines).
**
** Revision 1.66  2000/07/28 14:49:35  prez
** Ditched the old wx stuff, mconfig now in use, we're now ready to
** release (only got some conversion tests to do).
**
** Revision 1.65  2000/06/10 07:01:03  prez
** Fixed a bunch of little bugs ...
**
** Revision 1.64  2000/05/28 05:05:14  prez
** More makefile stuff ... Now we should work on all platforms.
** Added alot of checking for different .h files, functions, etc.
** So now all #define's are config.h based (also added a default
** windows config.h, which will need to be copied on these systems).
**
** Revision 1.63  2000/05/19 10:48:14  prez
** Finalized the DCC Sending (now uses the Action map properly)
**
** Revision 1.62  2000/05/17 14:08:12  prez
** More tweaking with DCC, and getting iostream mods working ...
**
** Revision 1.61  2000/05/15 08:43:28  ungod
** fix for recursion issue upon adding of two mstrings on certain platforms.
**
** Revision 1.60  2000/04/30 05:19:39  ungod
** ACE_OS::AC_OS:: hey prez? *smirk*
**
** Revision 1.59  2000/04/30 03:48:29  prez
** Replaced all system calls with ACE_OS equivilants,
** also removed any dependancy on ACE from sxp (xml)
**
** Revision 1.58  2000/02/27 03:58:40  prez
** Fixed the WHAT program, also removed RegEx from Magick.
**
** Revision 1.57  2000/02/23 12:21:03  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.56  2000/02/17 12:55:06  ungod
** still working on borlandization
**
** Revision 1.55  2000/02/16 12:59:40  ungod
** fixing for borland compilability
**
** Revision 1.54  2000/02/15 13:27:04  prez
** *** empty log message ***
**
** Revision 1.53  2000/02/15 10:37:50  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */


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

#include "mstring.h"

#ifdef WIN32
mstring const DirSlash="\\";
#else
mstring const DirSlash="/";
#endif
mstring const Blank("");
mstring const IRC_Bold((char) 2);	// ^B
mstring const IRC_Underline((char) 31);	// ^_
mstring const IRC_Reverse((char) 21);	// ^V
mstring const IRC_Color((char) 3);	// ^C
mstring const IRC_Off((char) 15);	// ^O

mstring::mstring(const mstring& in)
:inherited(in)
{
}
mstring::mstring(const mstring& in, inherited::size_type pos, inherited::size_type n)
:inherited(in,pos,n)
{
}
mstring::mstring(const char *in)
:inherited(in)
{
}
mstring::mstring(const char *in, inherited::size_type pos)
:inherited(in,pos)
{
}
mstring::mstring(inherited::size_type n, char c)
:inherited(n,c)
{
}
mstring::mstring(char c)
:inherited(1,c)
{
}
mstring::mstring(inherited::const_iterator first, inherited::const_iterator last)
:inherited(first,last)
{
}
mstring::mstring()
:inherited("")
{
}

int mstring::Cmp(const mstring & in)
{
	return ACE_OS::strcmp(c_str(),in.c_str());
}

int mstring::CmpNoCase(const mstring & in)
{
	return ACE_OS::strcasecmp(c_str(),in.c_str());
}

bool mstring::IsSameAs(const mstring & in, bool bCaseSensitive)
{
	if(bCaseSensitive==true)
		return Cmp(in)==0;
	else
		return CmpNoCase(in)==0;
}

mstring mstring::Mid(int nFirst, int nCount) const
{
	if (nCount<0) nCount=npos;
	if (nFirst<0) nFirst=0;
	return substr(nFirst, nCount);
}

mstring mstring::SubString(int from, int to) const
{
	return Mid(from, (to - from + 1));
}

mstring::mstring(const string & in)
{
	*this=in.c_str();
}

mstring mstring::Left(int nCount)const
{
	return Mid(0, nCount);
}

mstring mstring::Right(int nCount)const
{
	return Mid(this->size()-nCount-1);
}

mstring mstring::Before(const mstring & in, int count) const
{
	int m_pos = Find(in,count);
	if (m_pos >= 0)
		return Mid(0,m_pos);
	else
		return *this;
}

mstring mstring::After(const mstring & in,int count) const
{
	int m_pos = Find(in,count);
	if (m_pos >= 0)
		return Mid(m_pos+in.Len());
	else
		return *this;
}

mstring mstring::RevBefore(const mstring & in) const
{
	// change find_last_of to Last(in)
	int m_pos = Index(in,true,true);
	if (m_pos >= 0)
		return Mid(0,m_pos);
	else
		return *this;
}

mstring mstring::RevAfter(const mstring & in) const
{
	// change find_last_of to Last(in)
	int m_pos = Index(in,true,true);
	if (m_pos >= 0)
		return Mid(m_pos+in.size());
	else
		return *this;
}

void mstring::MakeUpper()
{
	unsigned int i;
	for(i=0;i<this->size();i++)
		if(islower((*this)[i]))
			at(i)=(char)toupper((*this)[i]);
}

void mstring::MakeLower()
{
	unsigned int i;
	for(i=0;i<this->size();i++)
		if(isupper((*this)[i]))
			at(i)=(char)tolower((*this)[i]);
}

mstring& mstring::Trim(bool bFromRight)
{
	if (!this->size())
		return *this;
	// change find_last_of to Last(in)
	if(bFromRight==true)
	{
		int m_pos = find_last_not_of(" \t\n");
		if (m_pos >= 0)
			*this=Mid(0,m_pos+1);
	}
	else
	{
		int m_pos = find_first_not_of(" \t\n");
		if (m_pos >= 0)
			*this=Mid(m_pos);
	}
	return *this;
}

mstring& mstring::Pad(size_t nCount, char chPad, bool bFromRight)
{
	if(bFromRight==true)
		*this=*this+mstring(nCount,chPad);
	else
		*this=mstring(nCount,chPad)+*this;
	return *this;
}

mstring& mstring::Truncate(int uiLine)
{
	*this=Left(uiLine);
	return *this;
}

int mstring::Format(const char * pszFormat, ...)
{
  va_list argptr;
  va_start(argptr, pszFormat);

  int iLen = FormatV(pszFormat, argptr);

  va_end(argptr);

  return iLen;
}

int mstring::FormatV(const char * pszFormat, va_list argptr)
{
	static char s_szScratch[1024];

	int iLen=ACE_OS::vsprintf(s_szScratch, pszFormat, argptr);
	char *buffer;
	if(iLen<sizeof(s_szScratch)&&iLen!=-1)
		buffer=s_szScratch;
	else
	{
		int size=sizeof(s_szScratch)*2;
		buffer=new char[size];
		while(buffer!=NULL)
		{
			iLen=ACE_OS::vsprintf(buffer, pszFormat, argptr);
			if(iLen<sizeof(s_szScratch)&&iLen!=-1)
				break;
			delete [] buffer;
			size*=2;
			buffer=new char[size];
		}
		if(!buffer)
			return -1;
	}
	*this=buffer;
	if(buffer!=s_szScratch)
		delete [] buffer;
	return iLen;
}

int mstring::CompareTo(const mstring & in, caseCompare cmp)
{
	if(cmp==ccExact)
		return Cmp(in);
	else
		return CmpNoCase(in);
}

mstring& mstring::Append(const mstring & in)
{
	*this=*this+in;
	return *this;
}

mstring& mstring::Append(char ch, int count)
{
	*this=*this+mstring(count,ch);
	return *this;
}

mstring& mstring::Prepend(const mstring & in)
{
	*this=in+*this;
	return *this;
}

mstring& mstring::Prepend(char ch, int count)
{
	*this=mstring(count,ch)+*this;
	return *this;
}

bool mstring::Contains(const mstring & in)const
{
	return Find(in)!=-1;
}

void mstring::Empty()
{
	*this="";
}

int mstring::Find(char ch, bool bFromEnd) const
{
	int i;
	if(bFromEnd==true)
		i=rfind(ch);
	else
		i=find(ch);

	return i;
}

int mstring::Find(const mstring & in, int count) const
{
	//const char *psz=ACE_OS::strstr(c_str(),in);
    mstring::const_iterator i=search(begin(),end(),in.begin(),in.end());
    if(count>1)
    {
	for(int j=1;j<count&&i!=end();j++)
	    i=search(i+1,end(),in.begin(),in.end());
    }
    if(i==end())
	return npos;
    else
	return distance(begin(),i);
}

int mstring::First(char c) const
{
	return Find(c);
}

int mstring::First(const mstring & in) const
{
	return Find(in);
}

int mstring::Index(char ch, int startpos) const
{
	return find(ch,startpos);
}

int mstring::Index(const mstring & in, bool caseSensitive, bool fromEnd) const
{
	int i;
	
	unsigned int start,end;
	if(fromEnd==true)
	{
		start=length()-in.length()-1;
		end=0;
	}
	else
	{
		start=0;
		end=length()-in.length()-1;
	}
	i=start;
	while(i!=end)
	{
		if(Mid(i,in.length()).IsSameAs(in,caseSensitive))
			return i;
		if(fromEnd==false)
			i++;
		else
			i--;
	}
	return npos;
}

bool mstring::IsAscii() const
{
	const char *s=c_str();
	while(*s)
	{
		if(!isascii(*s))
			return false;
		s++;
	}
	return true;
}

bool mstring::IsEmpty() const
{
	return *this=="";
}

bool mstring::IsNull() const
{
	return IsEmpty();
}

bool mstring::IsNumber() const
{
	const char *s=c_str();
	if(IsEmpty())
	    return false;
	if (*s == '-')
	   s++;
	while(*s)
	{
		if(!(isdigit(*s) || *s == '.'))
			return false;
		s++;
	}
	return true;
}

bool mstring::IsWord() const
{
	const char *s=c_str();
	while(*s)
	{
		if(!isalpha(*s))
			return false;
		s++;
	}
	return true;
}

bool mstring::GetBool() const
{
  mstring tmp = *this;
  if (tmp.CmpNoCase("true")==0 || tmp.CmpNoCase("on")==0 || tmp.CmpNoCase("yes")==0 ||
      tmp.CmpNoCase("y")==0 || tmp.CmpNoCase("t")==0 || atoi(tmp.c_str()) == 1)
    return true;
  else
    return false;
}

bool mstring::IsBool() const
{
  mstring tmp = *this;
  if (tmp.CmpNoCase("true")==0 || tmp.CmpNoCase("on")==0 || tmp.CmpNoCase("yes")==0 ||
      tmp.CmpNoCase("y")==0 || tmp.CmpNoCase("t")==0 || tmp == "1" ||
      tmp.CmpNoCase("false")==0 || tmp.CmpNoCase("off")==0 || tmp.CmpNoCase("no")==0 ||
      tmp.CmpNoCase("n")==0 || tmp.CmpNoCase("f")==0 || tmp == "0")
    return true;
  else
    return false;
}

mstring mstring::LowerCase() const
{
    mstring Result=*this;
    Result.MakeLower();
    return Result;
}

mstring& mstring::Remove(int pos)
{
	return Truncate(pos);
}

mstring& mstring::Remove(int nStart, size_t nLen)
{
	erase(nStart,nLen);
	return *this;
}

mstring& mstring::Remove(mstring str)
{
	int m_pos = Find(str);
	if (m_pos >= 0)
		Remove(m_pos, str.Len());
	return *this;
}

mstring& mstring::RemoveLast()
{
	Truncate(length()-1);
	return *this;
}

size_t mstring::Replace(const mstring & szOld, const mstring & szNew, bool replaceAll)
{
	size_t uiCount=0;
	size_t uiOldLen=szOld.length();

	mstring strTemp;
	const char *pCurrent=c_str();
	const char *pSubstr;

	while(*pCurrent!='\0')
	{
		pSubstr=ACE_OS::strstr(pCurrent,szOld.c_str());
		if(pSubstr==NULL)
		{
			if(uiCount==0)
				return 0;
			strTemp<<pCurrent;
			break;
		}
		else
		{
			strTemp<<mstring(pCurrent).Left(pSubstr-pCurrent);
			strTemp<<szNew;
			pCurrent=pSubstr+uiOldLen;
			uiCount++;
			if(!replaceAll)
			{
				strTemp<<pCurrent;
				break;
			}
		}
	}
	*this=strTemp;

	return uiCount;
}

mstring mstring::Strip(stripType s)
{
	mstring str=*this;
	if(s&stLeading) str.Trim(false);
	if(s&stTrailing) str.Trim(true);
	return str;
}

mstring mstring::UpperCase() const
{
    mstring Result=*this;
    Result.MakeUpper();
    return Result;
}

mstring mstring::operator ( )(size_t start, size_t len)
{
	return Mid(start,len);
}

mstring& mstring::operator <<(const mstring & s)
{
	*this=*this+s;
	return *this;
}

mstring& mstring::operator <<(const char * psz)
{
	*this=*this+psz;
	return *this;
}

mstring& mstring::operator <<(char ch)
{
	*this=*this+mstring(ch);
	return *this;
}

mstring& mstring::operator <<(int i)
{
	mstring s;
	s.Format("%d",i);
	return *this<<s;
}

mstring& mstring::operator <<(unsigned int i)
{
	mstring s;
	s.Format("%u",i);
	return *this<<s;
}

mstring& mstring::operator <<(long l)
{
	mstring s;
	s.Format("%d",l);
	return *this<<s;
}

mstring& mstring::operator <<(unsigned long l)
{
	mstring s;
	s.Format("%u",l);
	return *this<<s;
}

mstring& mstring::operator <<(float f)
{
	mstring s;
	s.Format("%f",f);
	return *this<<s;
}

mstring& mstring::operator <<(double d)
{
	mstring s;
	s.Format("%11.5g",d);
	return *this<<s;
}

size_t mstring::Len() const
{
	return length();
}

char mstring::Last()const
{
	mstring Result=*this;
	return Result[Result.Len()-1];
}

mstring operator+(const mstring& string, char ch)
{
	mstring Result=string+mstring(ch);
	return Result;
}

mstring operator+(char ch, const mstring& string)
{
	mstring Result=mstring(ch)+string;
	return Result;
}

mstring operator+(const mstring& string, const char *psz)
{
	mstring Result=std::string(string)+std::string(psz);
	return Result;
}

mstring operator+(const char *psz, const mstring& string)
{
	mstring Result=mstring(psz)+string;
	return Result;
}

/* mstring operator+(const mstring& string, const mstring& s)
{
	mstring Result=string+s;
	return Result;
}

mstring operator+(const mstring& s, const mstring& string)
{
	mstring Result=s+string;
	return Result;
}*/

mstring operator+(const mstring& string, double d)
{
	mstring s;
	s << d;
	mstring Result=string+s;
	return Result;
}

mstring operator+(double d, const mstring& string)
{
	mstring s;
	s << d;
	mstring Result=s+string;
	return Result;
}

mstring operator+(const mstring& string, float f)
{
	mstring s;
	s << f;
	mstring Result=string+s;
	return Result;
}

mstring operator+(float f, const mstring& string)
{
	mstring s;
	s << f;
	mstring Result=s+string;
	return Result;
}

mstring operator+(const mstring& string, long l)
{
	mstring s;
	s << l;
	mstring Result=string+s;
	return Result;
}

mstring operator+(long l, const mstring& string)
{
	mstring s;
	s << l;
	mstring Result=s+string;
	return Result;
}

mstring operator+(const mstring& string, unsigned long l)
{
	mstring s;
	s << l;
	mstring Result=string+s;
	return Result;
}

mstring operator+(unsigned long l, const mstring& string)
{
	mstring s;
	s << l;
	mstring Result=s+string;
	return Result;
}

mstring operator+(const mstring& string, int i)
{
	mstring s;
	s << i;
	mstring Result=string+s;
	return Result;
}

mstring operator+(int i, const mstring& string)
{
	mstring s;
	s << i;
	mstring Result=s+string;
	return Result;
}

mstring operator+(const mstring& string, unsigned int i)
{
	mstring s;
	s << i;
	mstring Result=string+s;
	return Result;
}

mstring operator+(unsigned int i, const mstring& string)
{
	mstring s;
	s << i;
	mstring Result=s+string;
	return Result;
}

unsigned int mstring::WordCount(const mstring &separators, bool assemble)const
{
    //
    int Result=0;
    size_t i=0;
    mstring S=*this;
    while(i<Len())
    {
	while(i<Len()&& assemble && separators.Contains(S[i]))
	    i++;
	if(i<Len())
	    Result++;
	while(i<Len() && !separators.Contains(S[i]))
	    i++;
    }
    return Result;
}
mstring mstring::ExtractWord(int count,const mstring& separators, bool assemble)const
{
    mstring Result;
    mstring S=*this;
    int i;
    i=WordPosition(count,separators, assemble);
    if(i!=-1)
    {
	while(i<Len() && !separators.Contains(S[(unsigned int)i]))
	{
	    Result<<S[(unsigned int)i];
	    i++;
	}
    }
    return Result;
}
unsigned int mstring::WordPosition(unsigned int N,const mstring& separators, bool assemble)const
{
    unsigned int i=0,count=0;
    mstring S=*this;
    int Result=0;
    while(i<Len() && count!=N)
    {
	// Skip past multi-seperators IF we assemble them.
	while (i<Len() && assemble && separators.Contains(S[i]))
	    i++;
	if(i<Len())
	    count++;
	if(count!=N)
	{
	    while(i<Len() && !separators.Contains(S[i]))
		i++;
	    i++;
	}
	else
	    Result=i;
    }
    return Result;
}

bool mstring::Matches(const mstring& in, bool nocase)const
{
    // nocase == non-case-sensative
    return match_wild(in.c_str(), c_str(), nocase);
}

#ifndef HAVE_ITOA
const char *itoa(int i)
{
    mstring str;
    str << i;
    return str.c_str();
}
#endif

#ifndef HAVE_FTOA
const char *ftoa(float f)
{
    mstring str;
    str << f;
    return str.c_str();
}
#endif

#ifndef HAVE_LTOA
const char *ltoa(long l)
{
    mstring str;
    str << l;
    return str.c_str();
}
#endif

#ifndef HAVE_ULTOA
const char *ultoa(unsigned long ul)
{
    mstring str;
    str << ul;
    return str.c_str();
}
#endif



/*  Direct from Magick I, credit to Andy Church for writing this.
 *
 *  match_wild:  Attempt to match a string to a pattern which might contain
 *              '*' or '?' wildcards.  Return 1 if the string matches the
 *              pattern, 0 if not.
 */

bool match_wild (const char *pattern, const char *str, bool docase)
{
    char c;
    const char *s;

    /* This WILL eventually terminate: either by *pattern == 0, or by a
     * trailing '*'. */

    for (;;)
    {
	switch (c = *pattern++)
	{
	case 0:
	    if (!*str)
		return true;
	    return false;
	case '?':
	    if (!*str)
		return false;
	    ++str;
	    break;
	case '*':
	    if (!*pattern)
		return true;	/* trailing '*' matches everything else */
	    s = str;
	    while (*s)
	    {
		if ((docase ? *s == *pattern : (tolower (*s) == tolower (*pattern)))
		    && match_wild (pattern, s, docase))
		    return true;
		++s;
	    }
	    break;
	default:
	    if ((docase ? (*str++ != c) : (tolower (*str++) != tolower (c))))
		return false;
	    break;
	}			/* switch */
    }
}
