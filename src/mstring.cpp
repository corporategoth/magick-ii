#include "pch.h"
#ifdef WIN32
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
#include "rx/rxposix.h"

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

mstring mstring::Mid(size_t nFirst, size_t nCount) const
{
	if (nCount==-1) nCount=npos;
	return substr(nFirst, nCount);
}

mstring mstring::SubString(size_t from, size_t to) const
{
	return Mid(from, (to - from + 1));
}

mstring::mstring(const string & in)
{
	*this=in.c_str();
}

mstring mstring::Left(size_t nCount)const
{
	return Mid(0, nCount);
}

mstring mstring::Right(size_t nCount)const
{
    return Mid(this->size()-nCount-1);
}

mstring mstring::Before(const mstring & in, int count) const
{
    return Mid(0,Find(in,count));
}

mstring mstring::After(const mstring & in,int count) const
{
    return Mid(Find(in,count)+in.Len());
}

mstring mstring::RevBefore(const mstring & in) const
{
	// change find_last_of to Last(in)
	return Mid(0,Index(in,true,true)-1);
}

mstring mstring::RevAfter(const mstring & in) const
{
	// change find_last_of to Last(in)
	return Mid(Index(in,true,true)+in.size());
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
	// change find_last_of to Last(in)
	if(bFromRight==true)
		*this=Mid(0,find_last_not_of(" \t\n"));
	else
		*this=Mid(find_first_not_of(" \t\n"));
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

mstring& mstring::Truncate(size_t uiLine)
{
	*this=Left(uiLine);
	return *this;
}

bool mstring::RxMatches(const mstring & in) const
{

// new code
    bool Result=false;
    regex_t matchcode;
    regmatch_t matches[1];
    if(regcomp(&matchcode,in.c_str(),0)!=0)
    {
	Result=false;
	goto re_cleanup;
    }
    if(regexec(&matchcode,this->c_str(),1,matches,0)!=0)
	Result=false;
    else
    {
	// check the matches value for start offset of 0, length == length of this.
	if(matches[0].rm_so!=0||(matches[0].rm_eo-matches[0].rm_so)!=Len())
	    Result=false;
	else
	    Result=true;
    }
re_cleanup:
    regfree(&matchcode);
    return Result;
/*	const char *psztext,*pszmask;
	for(psztext=c_str(),pszmask=in.c_str();*pszmask!='\0';psztext++,pszmask++)
	{
		switch(*pszmask)
		{
		case '?':
			if(*psztext=='\0')
				return false;
			psztext++;
			pszmask++;
			break;
		case '*':
			{
				while(*pszmask=='*'||*pszmask=='?')
					pszmask++;
				if(*pszmask=='\0')
					return true;
				size_t uiLenMask;
				const char *pEndMask=ACE_OS::strpbrk(pszmask,"*?");
				if(pEndMask!=NULL)
					uiLenMask=pEndMask-pszmask;
				else
					uiLenMask=ACE_OS::strlen(pszmask);
				mstring strtomatch(pszmask,uiLenMask);
				const char *pMatch=ACE_OS::strstr(psztext,strtomatch);
				if(pMatch==NULL)
					return false;
				psztext=pMatch+uiLenMask-1;
				pszmask+=uiLenMask-1;
				break;
			}
		default:
			if(*psztext!=*pszmask)
				return false;
			break;
		}
	}
	return *psztext=='\0';
*/
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

size_t mstring::First(char c) const
{
	return Find(c);
}

size_t mstring::First(const mstring & in) const
{
	return Find(in);
}

size_t mstring::Index(char ch, int startpos) const
{
	return find(ch,startpos);
}

size_t mstring::Index(const mstring & in, bool caseSensitive, bool fromEnd) const
{
	size_t i;
	
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

mstring& mstring::Remove(size_t pos)
{
	return Truncate(pos);
}

mstring& mstring::Remove(size_t nStart, size_t nLen)
{
	erase(nStart,nLen);
	return *this;
}

mstring& mstring::Remove(mstring str)
{
	Remove(Find(str), str.Len());
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
	s.Format("%14.8g",d);
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
	mstring Result=string+mstring(psz);
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

unsigned int mstring::WordCount(const mstring &separators)const
{
    //
    int Result=0;
    size_t i=0;
    mstring S=*this;
    while(i<Len())
    {
	while(i<Len()&&separators.Contains(S[i]))
	    i++;
	if(i<Len())
	    Result++;
	while(i<Len()&&!separators.Contains(S[i]))
	    i++;
    }
    return Result;
}
mstring mstring::ExtractWord(int count,const mstring& separators)const
{
    mstring Result;
    mstring S=*this;
    int i;
    i=WordPosition(count,separators);
    if(i!=-1)
    {
	while(i<Len()&&!separators.Contains(S[(unsigned int)i]))
	{
	    Result<<S[(unsigned int)i];
	    i++;
	}
    }
    return Result;
}
unsigned int mstring::WordPosition(unsigned int N,const mstring& separators)const
{
    unsigned int i=0,count=0;
    mstring S=*this;
    int Result=0;
    while(i<Len()&&count!=N)
    {
	while(i<Len()&&separators.Contains(S[i]))
	    i++;
	if(i<Len())
	    count++;
	if(count!=N)
	{
	    while(i<Len()&&!separators.Contains(S[i]))
		i++;
	}
	else
	    Result=i;
    }
    return Result;
}

pair<int,int> mstring::RegFind(const mstring& pattern)const
{
    pair<int,int> Result;
    regex_t matchcode;
    regmatch_t matches[1];
    if(regcomp(&matchcode,pattern.c_str(),0)!=0)
    {
	Result.first=0;
	Result.second=0;
	goto re_cleanup2;
    }
    if(regexec(&matchcode,this->c_str(),1,matches,0)!=0)
    {
	Result.first=0;
	Result.second=0;
    }
    else
    {
	// check the matches value for start offset of 0, length == length of this.
        Result.first=matches[0].rm_so;
	Result.second=matches[0].rm_eo-matches[0].rm_so;
    }
re_cleanup2:
    regfree(&matchcode);
    return Result;

}

bool mstring::Matches(const mstring& in)const
{
    mstring in2;
    unsigned int i;
    for(i=0;i<in.length();i++)
    {
	if(in[i]=='.')
	    in2=in2+"\\.";
	else if(in[i]=='*')
	    in2=in2+".*";
	else if(in[i]=='?')
	    in2=in2+".";
	else
	    in2=in2+in[i];
    }
    return RxMatches(in2);	
}

#ifdef NEED_ITOA
const char *itoa(int i)
{
    mstring str;
    str << i;
    return str.c_str();
}
#endif

#ifdef NEED_FTOA
const char *ftoa(float f)
{
    mstring str;
    str << f;
    return str.c_str();
}
#endif

#ifdef NEED_LTOA
const char *ltoa(long l)
{
    mstring str;
    str << l;
    return str.c_str();
}
#endif
