#ifndef _MSTRING_H
#define _MSTRING_H
#include <string>
using namespace std;

class mstring:public string
{
	typedef string inherited;
public:
	const char &operator[](int pos)const;
	char &operator[](int pos);
	mstring& operator <<(double d);
	mstring& operator <<(float f);
	mstring& operator<<(int i);
	mstring operator()(size_t start,size_t len);
	void UpperCase();
    // values for first parameter of Strip function
	enum stripType {stLeading, stTrailing , stBoth };
	mstring Strip(stripType s=stTrailing);
	size_t Replace(const mstring& szOld, const mstring& szNew, bool replaceAll=true);
	mstring& RemoveLast();
	mstring& Remove(size_t nStart, size_t nLen);
	mstring& Remove(size_t pos);
	void LowerCase();
	bool IsWord();
	bool IsNumber();
	bool IsNull();
	bool IsEmpty();
	bool IsAscii();
	size_t Index(const mstring& in, bool caseSensitive=true, bool fromEnd=false);
	size_t Index(char ch, int startpos=0);
	size_t First(const mstring& in);
	size_t First(char c);
	int Find(const mstring & in);
	int Find(char ch, bool bFromEnd=false);
	void Empty();
	bool Contains(const mstring& in);
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
	mstring& operator=(const char *in);
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