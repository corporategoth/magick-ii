#include "dccengine.h"
#include <algorithm>
using namespace std;
#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

const unsigned char CTCP_DELIM_CHAR='\001';
const unsigned char CTCP_QUOTE_CHAR='\\';
const unsigned char CTCP_MQUOTE_CHAR='\020';
//#define CTCP_QUOTE_EM   "\r\n\001\\"

bytevector DccEngine::lowQuote(bytevector& in)
{
    bytevector Result;
    bytevector::iterator pos;
    for(pos=in.begin();pos!=in.end();pos++)
    {
	if(*pos=='\0')
	{
	    Result.push_back(CTCP_MQUOTE_CHAR);
	    Result.push_back('0');
	}
	else if(*pos=='\n')
	{
	    Result.push_back(CTCP_MQUOTE_CHAR);
	    Result.push_back('n');
	}
	else if(*pos=='\r')
	{
	    Result.push_back(CTCP_MQUOTE_CHAR);
	    Result.push_back('r');
	}
	else if(*pos==CTCP_MQUOTE_CHAR)
	{
	    Result.push_back(CTCP_MQUOTE_CHAR);
	    Result.push_back(CTCP_MQUOTE_CHAR);
	}
	else
	    Result.push_back(*pos);
    }
    return Result;
}
bytevector DccEngine::lowDequote(bytevector& in)
{
    bytevector Result;
    bytevector::iterator pos;
    for(pos=in.begin();pos!=in.end();pos++)
    {
	if(*pos==CTCP_MQUOTE_CHAR)
	{
	    if(*(pos+1)=='0')
    	    {
		Result.push_back('\0');
		pos++;
	    }
	    else if(*(pos+1)=='n')
    	    {
		Result.push_back('\n');
		pos++;
	    }
	    else if(*(pos+1)=='r')
	    {
		Result.push_back('\r');
		pos++;
	    }
	    else if(*(pos+1)==CTCP_MQUOTE_CHAR)
	    {
		Result.push_back(CTCP_MQUOTE_CHAR);
		pos++;
	    }
	    else
	    {
		Result.push_back(*(pos+1));
		pos++;
	    }
	}
	else
	    Result.push_back(*pos);
    }
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
	if(*pos==CTCP_DELIM_CHAR)
	{
	    if(*(pos+1)=='a')
	    {
		Result.push_back(CTCP_DELIM_CHAR);
		pos++;
	    }
	    else if(*(pos+1)==CTCP_QUOTE_CHAR)
	    {
		Result.push_back(CTCP_QUOTE_CHAR);
		pos++;
	    }
	    else
	    {
		Result.push_back(*(pos+1));
		pos++;
	    }
	}
	else
	    Result.push_back(*pos);
    }
    return Result;
}

vector<bytevector> DccEngine::ctcpExtract(bytevector& in)
{
    // pull out /001...../001 pairs
    vector<bytevector> Result;
    bytevector tmpstring;
    bytevector::iterator start,end;
    start=find(in.begin(),in.end(),CTCP_DELIM_CHAR);
    if(start==in.end())
	return Result;
    end=find(start+1,in.end(),CTCP_DELIM_CHAR);
    while(start!=in.end()&&end!=in.end())
    {
	tmpstring.assign(start,end);
	Result.push_back(tmpstring);
	start=end;
	if(start!=in.end())
	    end=find(start+1,in.end(),CTCP_DELIM_CHAR);
    }
    return Result;
}
