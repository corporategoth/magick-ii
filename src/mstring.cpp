#include "mstring.h"
#include "ace/OS.h"
#include <ctype.h>
#include <stdarg.h>

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
mstring::mstring(inherited::const_iterator first, inherited::const_iterator last)
:inherited(first,last)
{
}
mstring::mstring()
:inherited("")
{
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

mstring& mstring::operator =(const mstring & in)
{
	*this=string(in);
	return *this;
}

mstring& mstring::operator =(const string & in)
{
	*this=in;
	return *this;
}

mstring& mstring::operator <<(char ch)
{
	*this=*this+mstring(1,ch);
	return *this;
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

mstring mstring::Mid(size_t nFirst, size_t nCount)
{
	return substr(nFirst, nCount);
}

mstring mstring::SubString(size_t from, size_t to)
{
	return Mid(from, (to - from + 1));
}

mstring::mstring(const string & in)
{
	*this=mstring(in.c_str());
}

mstring mstring::Left(size_t nCount)
{
	return Mid(0, nCount);
}

mstring mstring::Right(size_t nCount)
{
	return Mid(size()-nCount-1);
}

mstring mstring::Before(const mstring & in)
{
	return Mid(0,First(in)-1);
}

mstring mstring::After(const mstring & in)
{
	return Mid(First(in)+1);
}

mstring mstring::RevBefore(const mstring & in)
{
	// change find_last_of to Last(in)
	return Mid(0,find_last_of(in)-1);
}

mstring mstring::RevAfter(const mstring & in)
{
	// change find_last_of to Last(in)
	return Mid(find_last_of(in)+1);
}

void mstring::MakeUpper()
{
	int i;
	for(i=0;i<size();i++)
		if(islower(*this[i]))
			at(i)=(char)toupper(*this[i]);
}

void mstring::MakeLower()
{
	int i;
	for(i=0;i<size();i++)
		if(isupper(*this[i]))
			at(i)=(char)tolower(*this[i]);
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

bool mstring::Matches(const mstring & in)
{
	const char *psztext,*pszmask;
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
	*this=mstring(buffer);
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

bool mstring::Contains(const mstring & in)
{
	return Find(in)!=-1;
}

void mstring::Empty()
{
	*this=mstring("");
}

int mstring::Find(char ch, bool bFromEnd)
{
	const char *psz=bFromEnd?ACE_OS::strrchr(c_str(),ch) : ACE_OS::strchr(c_str(),ch);

	return (psz==NULL)?-1:psz-c_str();
}

int mstring::Find(const mstring & in)
{
	const char *psz=ACE_OS::strstr(c_str(),in);

	return (psz==NULL)?-1:psz-c_str();
}

size_t mstring::First(char c)
{
	return Find(c);
}

size_t mstring::First(const mstring & in)
{
	return Find(in);
}

size_t mstring::Index(char ch, int startpos)
{
	return find(ch,startpos);
}

size_t mstring::Index(const mstring & in, bool caseSensitive, bool fromEnd)
{
	size_t i;
	
	int start,end;
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
	return -1;
}

bool mstring::IsAscii()
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

bool mstring::IsEmpty()
{
	return *this==mstring("");
}

bool mstring::IsNull()
{
	return IsEmpty();
}

bool mstring::IsNumber()
{
	const char *s=c_str();
	while(*s)
	{
		if(!isdigit(*s))
			return false;
		s++;
	}
	return true;
}

bool mstring::IsWord()
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

void mstring::LowerCase()
{
	MakeLower();
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
