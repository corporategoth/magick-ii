#ifndef _MSTRING_H
#define _MSTRING_H
#include <string>
using namespace std;

class mstring:public string
{
	typedef string inherited;
public:
	int FormatV(const char *pszFormat, va_list argptr);
	int Format(const char *pszFormat, ...);
	// Matches using '*' and '?' rules
	bool Matches(const mstring& in);
	mstring& Truncate(size_t uiLine);
	mstring& Pad(size_t nCount, char chPad=' ', bool bFromRight=true);
	mstring& Trim(bool bFromRight=true);
	void MakeLower();
	void MakeUpper();
	mstring RevAfter(const mstring& in);
	mstring RevBefore(const mstring& in);
	mstring After(const mstring& in);
	mstring Before(const mstring& in);
	mstring Right(size_t nCount);
	mstring Left(size_t nCount);
	 mstring(const string& in);
	mstring SubString(size_t from, size_t to);
	mstring Mid(size_t nFirst, size_t nCount=npos);
	bool IsSameAs(const mstring& in, bool bCase=true);
	int CmpNoCase(const mstring& in);
	int Cmp(const mstring& in);
	mstring& operator<<(char ch);
	mstring& operator=(const string& in);
	mstring& operator=(const mstring& in);
	mstring& operator<<(const char *psz);
	mstring& operator<<(const mstring& s);
	mstring(const mstring& in);
	mstring(const mstring& in, inherited::size_type pos, inherited::size_type n);
	mstring(const char *in);
	mstring(const char *in, inherited::size_type pos);
	mstring(inherited::size_type n, char c);
	mstring(inherited::const_iterator first, inherited::const_iterator last);
	mstring();

	operator const char *() const { return c_str(); }
};

#endif