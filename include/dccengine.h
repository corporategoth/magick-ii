#ifndef _DCCENGINE_H
#define _DCCENGINE_H
#include "mstring.h"
#include <vector>
using namespace std;

class DccEngine
{
public:
    mstring encode(mstring& in);
    void decode(mstring& in);
    mstring lowQuote(mstring& in);
    mstring lowDequote(mstring& in);
    mstring ctcpQuote(mstring& in);
    mstring ctcpDequote(mstring& in);
    vector<mstring> ctcpExtract(mstring& in);
};

#endif