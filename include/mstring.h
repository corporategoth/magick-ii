#ifndef _MSTRING_H
#define _MSTRING_H
#include <string>
using namespace std;

class mstring:public string
{
	typedef string inherited;
public:
	char& Last();
	size_t Len();
	const char &operator[](int pos)const;
	char &operator[](int pos);
	const char &operator[](unsigned int pos)const;
	char &operator[](unsigned int pos);
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
	bool IsWord() const;
	bool IsNumber() const;
	bool IsNull() const;
	bool IsEmpty() const;
	bool IsAscii() const;
	size_t Index(const mstring& in, bool caseSensitive=true, bool fromEnd=false)const;
	size_t Index(char ch, int startpos=0)const;
	size_t First(const mstring& in)const;
	size_t First(char c)const;
	int Find(const mstring & in)const;
	int Find(char ch, bool bFromEnd=false)const;
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
	mstring RevAfter(const mstring& in) const;
	mstring RevBefore(const mstring& in) const;
	mstring After(const mstring& in) const;
	mstring Before(const mstring& in) const;
	mstring Right(size_t nCount)const;
	mstring Left(size_t nCount)const;
	 mstring(const string& in);
	mstring SubString(size_t from, size_t to)const;
	mstring Mid(size_t nFirst, size_t nCount=npos)const;
	bool IsSameAs(const mstring& in, bool bCase=true);
	int CmpNoCase(const mstring& in);
	int Cmp(const mstring& in);
	mstring& operator<<(char ch);
	mstring& operator=(char in);
	mstring& operator=(const string& in);
	mstring& operator=(const mstring& in);
	mstring& operator=(const char *in);
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

#endif