#ifndef _DCCENGINE_H
#define _DCCENGINE_H
#include <vector>
using namespace std;

typedef vector<unsigned char> bytevector;
class DccEngine
{
public:
    bytevector lowQuote(bytevector& in);
    bytevector lowDequote(bytevector& in);
    bytevector ctcpQuote(bytevector& in);
    bytevector ctcpDequote(bytevector& in);
    vector<bytevector> ctcpExtract(bytevector& in);
};

#endif