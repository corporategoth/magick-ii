#include "dccengine.h"
#include <algorithm>
using namespace std;
#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

const char CTCP_DELIM_CHAR='\001';
const char CTCP_QUOTE_CHAR='\\';
const char CTCP_MQUOTE_CHAR='\020';
//#define CTCP_QUOTE_EM   "\r\n\001\\"

mstring DccEngine::lowQuote(mstring& in)
{
    mstring Result;
    mstring::iterator pos;
    for(pos=in.begin();pos!=in.end();pos++)
    {
	if(*pos=='\0')
	    Result<<CTCP_MQUOTE_CHAR<<'0';
	else if(*pos=='\n')
	    Result<<CTCP_MQUOTE_CHAR<<'n';
	else if(*pos=='\r')
	    Result<<CTCP_MQUOTE_CHAR<<'r';
	else if(*pos==CTCP_MQUOTE_CHAR)
	    Result<<CTCP_MQUOTE_CHAR<<CTCP_MQUOTE_CHAR;
	else
	    Result<<*pos;
    }
    return Result;
}

mstring DccEngine::lowDequote(mstring& in)
{
    mstring Result;
    mstring::iterator pos;
    for(pos=in.begin();pos!=in.end();pos++)
    {
	if(*pos==CTCP_MQUOTE_CHAR)
	{
	    if(*(pos+1)=='0')
    	    {
		Result<<'\0';
		pos++;
	    }
	    else if(*(pos+1)=='n')
    	    {
		Result<<'\n';
		pos++;
	    }
	    else if(*(pos+1)=='r')
	    {
		Result<<'\r';
		pos++;
	    }
	    else if(*(pos+1)==CTCP_MQUOTE_CHAR)
	    {
		Result<<CTCP_MQUOTE_CHAR;
		pos++;
	    }
	    else
	    {
		Result<<*(pos+1);
		pos++;
	    }
	}
	else
	    Result<<*pos;
    }
    return Result;
}

mstring DccEngine::ctcpQuote(mstring& in)
{
    mstring Result;
    mstring::iterator pos;
    for(pos=in.begin();pos!=in.end();pos++)
    {
	if(*pos==CTCP_DELIM_CHAR)
	    Result<<CTCP_QUOTE_CHAR<<'a';
	else if(*pos==CTCP_QUOTE_CHAR)
	    Result<<CTCP_QUOTE_CHAR<<CTCP_QUOTE_CHAR;
	else
	    Result<<*pos;
    }
    return Result;
}

mstring DccEngine::ctcpDequote(mstring& in)
{
    mstring Result;
    mstring::iterator pos;
    for(pos=in.begin();pos!=in.end();pos++)
    {
	if(*pos==CTCP_DELIM_CHAR)
	{
	    if(*(pos+1)=='a')
	    {
		Result<<CTCP_DELIM_CHAR;
		pos++;
	    }
	    else if(*(pos+1)==CTCP_QUOTE_CHAR)
	    {
		Result<<CTCP_QUOTE_CHAR;
		pos++;
	    }
	    else
	    {
		Result<<*(pos+1);
		pos++;
	    }
	}
	else
	    Result<<*pos;
    }
    return Result;
}

vector<mstring> DccEngine::ctcpExtract(mstring& in)
{
    // pull out /001...../001 pairs
    vector<mstring> Result;
    mstring tmpstring;
    mstring::iterator start,end;
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

void DccEngine::decode(mstring & in)
{
    vector<mstring> ResVector;
    if(count(in.begin(),in.end(),CTCP_DELIM_CHAR)<2)
	return;
    ResVector=ctcpExtract(in);

}

mstring DccEngine::encode(mstring & in)
{
    mstring Result;


    return Result;
}
