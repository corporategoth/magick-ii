#include "dccengine.h"
#include <algorithm>
using namespace std;

const unsigned char CTCP_DELIM_CHAR='\001';
const unsigned char CTCP_QUOTE_CHAR='\\';
//#define CTCP_QUOTE_EM   "\r\n\001\\"

bytevector DccEngine::lowQuote(bytevector& in)
{
    bytevector Result;

    return Result;
}
bytevector DccEngine::lowDequote(bytevector& in)
{
    bytevector Result;

    return Result;
}
bytevector DccEngine::ctcpQuote(bytevector& in)
{
    bytevector Result;
    bytevector::iterator pos;
    for(pos=in.begin();pos!=in.end();pos++)
    {
	if(*pos==CTCP_DELIM_CHAR)
	{
	    Result.push_back(CTCP_QUOTE_CHAR);
	    Result.push_back('a');
	}
	else if(*pos==CTCP_QUOTE_CHAR)
	{
	    Result.push_back(CTCP_QUOTE_CHAR);
	    Result.push_back(CTCP_QUOTE_CHAR);
	}
	else
	    Result.push_back(*pos);
    }
    return Result;
}
bytevector DccEngine::ctcpDequote(bytevector& in)
{
    bytevector Result;
    bytevector::iterator pos;
    for(pos=in.begin();pos!=in.end();pos++)
    {
	if(*pos==CTCP_DELIM_CHAR&&*(pos+1)=='a')
	{
	    Result.push_back(CTCP_DELIM_CHAR);
	}
	else if(*pos==CTCP_QUOTE_CHAR&&*(pos+1)==CTCP_QUOTE_CHAR)
	{
	    Result.push_back(CTCP_QUOTE_CHAR);
	}
	else
	    Result.push_back(*pos);
    }
    return Result;
}

bytevector DccEngine::ctcpExtract(bytevector& in)
{
    bytevector Result;
    bytevector::iterator delim;
    delim=find(in.begin(),in.end(),CTCP_DELIM_CHAR);
    Result.assign(in.begin(),delim-1);
    return Result;
}
