#ifndef _MSTRING_H
#define _MSTRING_H
#include <string>
using namespace std;

class mstring:public string
{
	typedef string inherited;
public:
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