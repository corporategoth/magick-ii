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

bool mstring::IsSameAs(const mstring & in, bool bCase)
{
	if(bCase==true)
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
	return Mid(0,find(in)-1);
}

mstring mstring::After(const mstring & in)
{
	return Mid(find(in)+1);
}

mstring mstring::RevBefore(const mstring & in)
{
	return Mid(0,find_last_of(in)-1);
}

mstring mstring::RevAfter(const mstring & in)
{
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
