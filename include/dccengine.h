#ifndef _DCCENGINE_H
#define _DCCENGINE_H
#include <ace/INET_Addr.h>
#include "mstring.h"
#include <vector>
using namespace std;

class DccEngine
{
public:
    void DoDccSend(ACE_INET_Addr addr, size_t size);
    void DoDccChat(ACE_INET_Addr addr);
    void GotDCC(const mstring& in);
    mstring encode(mstring& in);
    void decode(mstring& in);
    mstring lowQuote(mstring& in);
    mstring lowDequote(mstring& in);
    mstring ctcpQuote(mstring& in);
    mstring ctcpDequote(mstring& in);
    vector<mstring> ctcpExtract(mstring& in);
};

#endif