#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
#endif

/* Magick IRC Services
**
** (c) 1997-2002 Preston Elder <prez@magick.tm>
** (c) 1998-2002 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any circumstances,
** however it may be added to if any modifications are made to this
** file.  All modified code must be clearly documented and labelled.
**
** This code is released under the Artistic License v2.0 or better.
** The full text of this license should be contained in a file called
** COPYING distributed with this code.  If this file does not exist,
** it may be viewed here: http://www.magick.tm/m2/license.html
**
** ======================================================================= */
#define RCSID(x,y) const char *rcsid_mconfig_cpp_ ## x () { return y; }
RCSID(mconfig_cpp, "@(#)$Id$");

/* ======================================================================= **
**
** For official changes (by the Magick Development Team),please
** check the ChangeLog* files that come with this distribution.
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** ======================================================================= */

#include "mconfig.h"
#include "mexceptions.h"
#include "filesys.h"

ceNode::~ceNode()
{
    BTCB();
    NFT("ceNode::~ceNode");
    // probably not needed, but for safety's sake anyway
    for (map < mstring, ceNode * >::iterator i = i_children.begin(); i != i_children.end(); i++)
	if (i->second != NULL)
	{
	    delete i->second;

	    i->second = NULL;
	}
    i_children.clear();
    i_keys.clear();
    ETCB();
}

ceNode &ceNode::operator=(const ceNode & in)
{
    BTCB();
    FT("ceNode::operator=", ("(const ceNode &) in"));
    i_Name = in.i_Name;
    i_keys.clear();
    i_keys = in.i_keys;
    for (map < mstring, ceNode * >::iterator i = i_children.begin(); i != i_children.end(); i++)
	if (i->second != NULL)
	{
	    delete i->second;

	    i->second = NULL;
	}
    i_children.clear();
    for (map < mstring, ceNode * >::const_iterator j = in.i_children.begin(); j != in.i_children.end(); j++)
    {
	i_children[j->first] = new ceNode;
	// the below line *will* recursively copy it's children
	*(i_children[j->first]) = * (j->second);
    }
    return *this;
    ETCB();
}

bool ceNode::operator==(const ceNode & in) const
{
    BTCB();
    FT("ceNode::operator==", ("(const ceNode &) in"));
    bool Result = false;

    if (i_keys == in.i_keys)
    {
	map < mstring, ceNode * >::const_iterator i;
	for (i = in.i_children.begin(); i != in.i_children.end(); i++)
	{
	    if (i_children.find(i->first) == i_children.end())
		RET(false);
	    // the below line *will* recursively check it's children
	    if (!(*(i_children.find(i->first)->second) == * (i->second)))
		RET(false);
	}
	Result = true;
    }
    RET(Result);
    ETCB();
}

bool ceNode::SetKey(const mstring & KeyName, const mstring & Value)
{
    BTCB();
    FT("ceNode::SetKey", (KeyName, Value));

    mstring temppath;
    bool Result = false;

    if (KeyName.first() == '/')
	temppath = KeyName.After("/");
    else
	temppath = KeyName;
//    temppath.MakeUpper();

    if (!temppath.Contains("/"))
    {
	i_keys[temppath] = Value;
	Result = true;
    }
    else
    {
	mstring next, rest;

	next = temppath.Before("/");
	rest = temppath.After("/");
	map < mstring, ceNode * >::iterator iter = i_children.find(next);
	if (iter == i_children.end() || iter->second == NULL)
	{
	    i_children[next] = new ceNode;
	    i_children[next]->i_Name = next;
	}
	i_children[next]->SetKey(rest, Value);
    }
    RET(Result);
    ETCB();
}

bool ceNode::DeleteKey(const mstring & KeyName)
{
    BTCB();
    FT("ceNode::DeleteKey", (KeyName));
    mstring temppath;
    bool Result = false;

    if (KeyName.first() == '/')
	temppath = KeyName.After("/");
    else
	temppath = KeyName;
//    temppath.MakeUpper();
    if (!temppath.Contains("/"))
    {
	// end of the line
	map < mstring, mstring >::iterator iter = i_keys.find(temppath);
	if (iter != i_keys.end())
	{
	    i_keys.erase(iter);
	    Result = true;
	}
    }
    else
    {
	mstring next, rest;

	next = temppath.Before("/");
	rest = temppath.After("/");
	map < mstring, ceNode * >::iterator iter = i_children.find(next);
	if (iter == i_children.end() || iter->second == NULL)
	    Result = false;
	else
	    Result = iter->second->DeleteKey(rest);
    }
    RET(Result);
    ETCB();
}

bool ceNode::CreateNode(const mstring & NodeName)
{
    BTCB();
    // strip off the first bit of the path, if not exists, create the node, and pass
    // the rest of the path to it, so it can do the same itself.
    // ie NodeName="blah/test/test2", pull out blah, and pass "test/test2" to the node
    FT("ceNode::CreateNode", (NodeName));
    mstring temppath;
    bool Result = false;

    if (NodeName.first() == '/')
	temppath = NodeName.After("/");
    else
	temppath = NodeName;
//    temppath.MakeUpper();
    if (!temppath.Contains("/"))
    {
	// end of the line
	map < mstring, ceNode * >::iterator iter = i_children.find(temppath);
	if (iter == i_children.end() || iter->second == NULL)
	{
	    i_children[temppath] = new ceNode;
	    i_children[temppath]->i_Name = temppath;
	    Result = true;
	}
    }
    else
    {
	mstring next, rest;

	next = temppath.Before("/");
	rest = temppath.After("/");
	map < mstring, ceNode * >::iterator iter = i_children.find(next);
	if (iter == i_children.end() || iter->second == NULL)
	{
	    i_children[next] = new ceNode;
	    i_children[next]->i_Name = next;
	}
	i_children[next]->CreateNode(rest);
    }
    RET(Result);
    ETCB();
}

bool ceNode::DeleteNode(const mstring & NodeName)
{
    BTCB();
    FT("ceNode::DeleteNode", (NodeName));
    mstring temppath;
    bool Result = false;

    if (NodeName.first() == '/')
	temppath = NodeName.After("/");
    else
	temppath = NodeName;
//    temppath.MakeUpper();
    if (!temppath.Contains("/"))
    {
	// end of the line
	map < mstring, ceNode * >::iterator iter = i_children.find(temppath);
	if (iter != i_children.end())
	{
	    if (iter->second != NULL)
		delete iter->second;

	    i_children.erase(iter);
	    Result = true;
	}
    }
    else
    {
	mstring next, rest;

	next = temppath.Before("/");
	rest = temppath.After("/");
	map < mstring, ceNode * >::iterator iter = i_children.find(next);
	if (iter != i_children.end() && iter->second != NULL)
	{
	    i_children[next]->DeleteNode(rest);
	}
    }
    RET(Result);
    ETCB();
}

bool ceNode::NodeExists(const mstring & NodeName) const
{
    BTCB();
    // strip off the first bit of the path, if not exists, return false, otherwise pass
    // the rest of the path to it, so it can do the same itself.
    // ie NodeName="blah/test/test2", pull out blah, and pass "test/test2" to the node
    FT("ceNode::NodeExists", (NodeName));
    mstring temppath;
    bool Result = false;

    if (NodeName.first() == '/')
	temppath = NodeName.After("/");
    else
	temppath = NodeName;
//    temppath.MakeUpper();
    if (!temppath.Contains("/"))
    {
	// end of the line
	map < mstring, ceNode * >::const_iterator iter = i_children.find(temppath);
	if (iter != i_children.end() && iter->second != NULL)
	    Result = true;
    }
    else
    {
	mstring next, rest;

	next = temppath.Before("/");
	rest = temppath.After("/");
	map < mstring, ceNode * >::const_iterator iter = i_children.find(next);
	if (iter != i_children.end() && iter->second != NULL)
	{
	    Result = iter->second->NodeExists(rest);
	}
    }
    RET(Result);
    ETCB();
}

bool ceNode::KeyExists(const mstring & KeyName) const
{
    BTCB();
    FT("ceNode::KeyExists", (KeyName));
    mstring temppath;
    bool Result = false;

    if (KeyName.first() == '/')
	temppath = KeyName.After("/");
    else
	temppath = KeyName;
//    temppath.MakeUpper();
    if (!temppath.Contains("/"))
    {
	// end of the line
	map < mstring, mstring >::const_iterator iter = i_keys.find(temppath);
	if (iter != i_keys.end())
	    Result = true;
    }
    else
    {
	mstring next, rest;

	next = temppath.Before("/");
	rest = temppath.After("/");
	map < mstring, ceNode * >::const_iterator iter = i_children.find(next);
	if (iter != i_children.end() && iter->second != NULL)
	{
	    Result = iter->second->KeyExists(rest);
	}
    }
    RET(Result);
    ETCB();
}

mstring ceNode::GetKey(const mstring & KeyName, const mstring & DefValue) const
{
    BTCB();
    FT("ceNode::GetKey", (KeyName, DefValue));
    mstring temppath;
    mstring Result = DefValue;

    if (KeyName.first() == '/')
	temppath = KeyName.After("/");
    else
	temppath = KeyName;
//    temppath.MakeUpper();
    if (!temppath.Contains("/"))
    {
	// end of the line
	map < mstring, mstring >::const_iterator iter = i_keys.find(temppath);
	if (iter != i_keys.end())
	    Result = iter->second;
    }
    else
    {
	mstring next, rest;

	next = temppath.Before("/");
	rest = temppath.After("/");
	map < mstring, ceNode * >::const_iterator iter = i_children.find(next);
	if (iter != i_children.end() && iter->second != NULL)
	{
	    Result = iter->second->GetKey(rest, DefValue);
	}
    }
    RET(Result);
    ETCB();
}

ceNode *ceNode::GetNode(const mstring & NodeName)
{
    BTCB();
    FT("ceNode::GetNode", (NodeName));
    mstring temppath;
    ceNode *Result = NULL;

    if (NodeName.first() == '/')
	temppath = NodeName.After("/");
    else
	temppath = NodeName;
//    temppath.MakeUpper();
    if (!temppath.Contains("/"))
    {
	// end of the line
	map < mstring, ceNode * >::const_iterator iter = i_children.find(temppath);
	if (iter == i_children.end() || iter->second == NULL)
	{
	    i_children[temppath] = new ceNode;
	    i_children[temppath]->i_Name = temppath;
	}
	Result = i_children[temppath];
    }
    else
    {
	mstring next, rest;

	next = temppath.Before("/");
	rest = temppath.After("/");
	// note i don't use NodeExists and CreateNode here as this is a recursive function
	// and that would cause it to check if the node exists for every recursion of this function
	map < mstring, ceNode * >::iterator iter = i_children.find(next);
	if (iter == i_children.end() || iter->second == NULL)
	{
	    i_children[next] = new ceNode;
	    i_children[next]->i_Name = next;
	}
	Result = i_children[next]->GetNode(rest);
    }
    NRET(ceNode *, Result);
    ETCB();
}

mstring ceNode::Write(const mstring & KeyName, const mstring & Value)
{
    BTCB();
    FT("ceNode::Write", (KeyName, Value));
    mstring temppath;
    mstring Result = "";

    if (KeyName.first() == '/')
	temppath = KeyName.After("/");
    else
	temppath = KeyName;
//    temppath.MakeUpper();
    if (!temppath.Contains("/"))
    {
	// end of the line
	Result = i_keys[temppath];
	i_keys[temppath] = Value;
    }
    else
    {
	mstring next, rest;

	next = temppath.Before("/");
	rest = temppath.After("/");
	// note i don't use NodeExists and CreateNode here as this is a recursive function
	// and that would cause it to check if the node exists for every recursion of this function
	map < mstring, ceNode * >::iterator iter = i_children.find(next);
	if (iter == i_children.end() || iter->second == NULL)
	{
	    i_children[next] = new ceNode;
	    i_children[next]->i_Name = next;
	}
	Result = i_children[next]->Write(rest, Value);
    }
    RET(Result);
    ETCB();
}

map < mstring, mstring > ceNode::GetMap() const
{
    BTCB();
    NFT("ceNode::GetMap");
    map < mstring, mstring > submap, Result;

    map < mstring, ceNode * >::const_iterator i;
    map < mstring, mstring >::const_iterator j;
    for (i = i_children.begin(); i != i_children.end(); i++)
    {
	if (i->second != NULL)
	{
	    submap = i->second->GetMap();
	    for (j = submap.begin(); j != submap.end(); j++)
	    {
		if (!i_Name.empty())
		    Result[i_Name + "/" + j->first] = j->second;
		else
		    Result[j->first] = j->second;

	    }
	    submap.clear();
	}
    }

    for (j = i_keys.begin(); j != i_keys.end(); j++)
    {
	if (!i_Name.empty())
	    Result[i_Name + "/" + j->first] = j->second;
	else
	    Result[j->first] = j->second;
    }
    NRET(map < mstring_mstring >, Result);
    ETCB();
}

bool mConfigEngine::LoadFile()
{
    BTCB();
    NFT("mConfigEngine::LoadFile");

    bool Result = false;

    if (!i_FileName.empty() && mFile::Exists(i_FileName))
    {
	vector < mstring > initialload;
	initialload = mFile::UnDump(i_FileName);
	Result = LoadFromArray(initialload);
    }
    RET(Result);
    ETCB();
}

bool mConfigEngine::SaveFile()
{
    BTCB();
    NFT("mConfigEngine::SaveFile");
    // bah i'll write this one later
    RET(false);
    ETCB();
}

void mConfigEngine::Empty()
{
    BTCB();
    NFT("mConfigEngine::Empty");
    map < mstring, ceNode * >::iterator i;
    for (i = RootNode.i_children.begin(); i != RootNode.i_children.end(); i++)
	if (i->second != NULL)
	{
	    delete i->second;

	    i->second = NULL;
	}
    RootNode.i_children.clear();
    RootNode.i_keys.clear();
    ETCB();
}

mstring mConfigEngine::Read(const mstring & key, const mstring & Default) const
{
    BTCB();
    FT("mConfigEngine::Read", (key, Default));
    mstring Result;

    Result = RootNode.GetKey(key, Default);
    RET(Result);
    ETCB();
}

bool mConfigEngine::Read(const mstring & key, mstring & outvar, const mstring & Default) const
{
    BTCB();
    FT("mConfigEngine::Read", (key, "(mstring &) outvar", Default));
    bool Result = true;

    outvar = RootNode.GetKey(key, Default);
    RET(Result);
    ETCB();
}

bool mConfigEngine::Read(const mstring & key, bool &outvar, const bool Default) const
{
    BTCB();
    FT("mConfigEngine::Read", (key, "(bool &) outvar", Default));
    mstring tmp;
    bool Result = true;

    tmp = RootNode.GetKey(key, Default ? "true" : "false");

    if (tmp.IsBool())
	outvar = tmp.GetBool();
    else
    {
	outvar = Default;
	Result = false;
    }
    RET(Result);
    ETCB();
}

bool mConfigEngine::Read(const mstring & key, int &outvar, const int Default) const
{
    BTCB();
    FT("mConfigEngine::Read", (key, "(int &) outvar", Default));
    mstring tmpvar;
    bool Result = true;

    tmpvar = RootNode.GetKey(key, Default);
    if (tmpvar.IsNumber())
	outvar = atoi(tmpvar.c_str());
    else
    {
	outvar = Default;
	Result = false;
    }
    RET(Result);
    ETCB();
}

bool mConfigEngine::Read(const mstring & key, unsigned int &outvar, const unsigned int Default) const
{
    BTCB();
    FT("mConfigEngine::Read", (key, "(unsigned int &) outvar", Default));
    mstring tmpvar;
    bool Result = true;

    tmpvar = RootNode.GetKey(key, Default);
    if (tmpvar.IsNumber())
	outvar = atoi(tmpvar.c_str());
    else
    {
	outvar = Default;
	Result = false;
    }
    RET(Result);
    ETCB();
}

bool mConfigEngine::Read(const mstring & key, long &outvar, const long Default) const
{
    BTCB();
    FT("mConfigEngine::Read", (key, "(long &) outvar", Default));
    mstring tmpvar;
    bool Result = true;

    tmpvar = RootNode.GetKey(key, Default);
    if (tmpvar.IsNumber())
	outvar = atol(tmpvar.c_str());
    else
    {
	outvar = Default;
	Result = false;
    }
    RET(Result);
    ETCB();
}

bool mConfigEngine::Read(const mstring & key, unsigned long &outvar, const unsigned long Default) const
{
    BTCB();
    FT("mConfigEngine::Read", (key, "(unsigned long &) outvar", Default));
    mstring tmpvar;
    bool Result = true;

    tmpvar = RootNode.GetKey(key, Default);
    if (tmpvar.IsNumber())
    {
	outvar = atoul(tmpvar.c_str());
    }
    else
    {
	outvar = Default;
	Result = false;
    }
    RET(Result);
    ETCB();
}

bool mConfigEngine::Read(const mstring & key, double &outvar, const double Default) const
{
    BTCB();
    FT("mConfigEngine::Read", (key, "(double &) outvar", Default));
    mstring tmpvar;
    bool Result = true;

    tmpvar = RootNode.GetKey(key, Default);
    if (tmpvar.IsNumber())
	outvar = atod(tmpvar.c_str());
    else
    {
	outvar = Default;
	Result = false;
    }
    RET(Result);
    ETCB();
}

bool mConfigEngine::Read(const mstring & key, float &outvar, const float Default) const
{
    BTCB();
    FT("mConfigEngine::Read", (key, "(double &) outvar", Default));
    mstring tmpvar;
    bool Result = true;

    tmpvar = RootNode.GetKey(key, Default);
    if (tmpvar.IsNumber())
	outvar = atof(tmpvar.c_str());
    else
    {
	outvar = Default;
	Result = false;
    }
    RET(Result);
    ETCB();
}

mstring mConfigEngine::Write(const mstring & key, const mstring & value)
{
    BTCB();
    FT("mConfigEngine::Write", (key, value));
    mstring Result = RootNode.Write(key, value);

    RET(Result);
    ETCB();
}

bool mConfigEngine::Write(const mstring & key, const bool value)
{
    BTCB();
    FT("mConfigEngine::Write", (key, value));
    bool Result;

    Read(key, Result, false);
    if (value == true)
	Write(key, mstring("TRUE"));
    else
	Write(key, mstring("FALSE"));
    RET(Result);
    ETCB();
}

int mConfigEngine::Write(const mstring & key, const int value)
{
    BTCB();
    FT("mConfigEngine::Write", (key, value));
    mstring tmp;

    tmp << value;
    int Result;

    Read(key, Result, 0);
    Write(key, tmp);
    RET(Result);
    ETCB();
}

unsigned int mConfigEngine::Write(const mstring & key, const unsigned int value)
{
    BTCB();
    FT("mConfigEngine::Write", (key, value));
    mstring tmp;

    tmp << value;
    unsigned int Result;

    Read(key, Result, 0U);
    Write(key, tmp);
    RET(Result);
    ETCB();
}

long mConfigEngine::Write(const mstring & key, const long value)
{
    BTCB();
    FT("mConfigEngine::Write", (key, value));
    mstring tmp;

    tmp << value;
    long Result;

    Read(key, Result, 0L);
    Write(key, tmp);
    RET(Result);
    ETCB();
}

unsigned long mConfigEngine::Write(const mstring & key, const unsigned long value)
{
    BTCB();
    FT("mConfigEngine::Write", (key, value));
    mstring tmp;

    tmp << value;
    unsigned long Result;

    Read(key, Result, 0UL);
    Write(key, tmp);
    RET(Result);
    ETCB();
}

double mConfigEngine::Write(const mstring & key, const double value)
{
    BTCB();
    FT("mConfigEngine::Write", (key, value));
    mstring tmp;

    tmp << value;
    double Result;

    Read(key, Result, 0.0);
    Write(key, tmp);
    RET(Result);
    ETCB();
}

ceNode *mConfigEngine::GetNode(const mstring & NodeName)
{
    BTCB();
    // warning *HIGHLY DANGEROUS, as i can't get auto_ptr's working this pointer
    //  could be freed at any time, trust it only as long as you have to
    // but at the moment, i'm just too damn brainfried to redesign this
    FT("mConfigEngine::GetNode", (NodeName));
    ceNode *Result = RootNode.GetNode(NodeName);

    NRET(ceNode *, Result);
    ETCB();
}

bool mConfigEngine::DeleteNode(const mstring & NodeName)
{
    BTCB();
    FT("mConfigEngine::DeleteNode", (NodeName));
    bool Result = RootNode.DeleteNode(NodeName);

    RET(Result);
    ETCB();
}

bool mConfigEngine::DeleteKey(const mstring & KeyName)
{
    BTCB();
    FT("mConfigEngine::DeleteKey", (KeyName));
    bool Result = RootNode.DeleteKey(KeyName);

    RET(Result);
    ETCB();
}

bool mConfigEngine::LoadFromString(const mstring & configstring)
{
    BTCB();
    FT("mConfigEngine::LoadFromString", (configstring));
    vector < mstring > tempstore;
    for (unsigned int i = 1; i <= configstring.WordCount("\n"); i++)
	tempstore.push_back(configstring.ExtractWord(i, "\n"));
    bool Result = LoadFromArray(tempstore);

    RET(Result);
    ETCB();
}

bool mConfigEngine::LoadFromArray(const vector < mstring > & configarray)
{
    BTCB();
    FT("mConfigEngine::LoadFromArray", ("(vector<mstring>) configarray"));
    bool Result = false;

    vector < mstring > decommented;
    decommented = PreParse(configarray);
    ceNode *currNode = & RootNode;
    mstring currline, currpath;

    for (vector < mstring >::const_iterator i = decommented.begin(); i != decommented.end(); i++)
    {
	currline = * i;
	if (currline.first() == '[' && currline.last() == ']')
	{
	    // new section
	    currpath = currline.SubString(1, currline.length() - 2);
	    currpath.Trim(true);
	    currpath.Trim(false);
	    currpath.replace("\255", " ");
	    currpath.replace("\254", "\t");
	    Result = RootNode.CreateNode(currpath);
	    currNode = RootNode.GetNode(currpath);
	}
	else if (currline.find_first_of('=') > 0)
	{
	    // new value
	    if (currNode != NULL)
	    {
		mstring key = currline.Before("=").Strip(true);
		mstring data = currline.After("=").Strip(false);

		key.replace("\255", " ");
		key.replace("\254", "\t");
		data.replace("\255", "");
		data.replace("\254", "\t");
		Result = currNode->SetKey(key, data);
		CSRC(currpath, key, data);
	    }
	}
    }
    RET(Result);
    ETCB();
}

bool mConfigEngine::NodeExists(const mstring & NodeName) const
{
    BTCB();
    FT("mConfigEngine::NodeExists", (NodeName));
    bool Result = RootNode.NodeExists(NodeName);

    RET(Result);
    ETCB();
}

vector < mstring > mConfigEngine::PreParse(const vector < mstring > & in)
{
    BTCB();
    FT("mConfigEngine::PreParse", ("(const vector<mstrign>) in"));
    vector < mstring > Result;
    mstring line;

    for (vector < mstring >::const_iterator i = in.begin(); i != in.end(); i++)
    {
	// Trim our left, thats safe ...
	mstring tmp = i->Strip(false);

	tmp.replace("\r", "");
	tmp.replace("\n", "");

	// If we're non blank, and non-comment ...
	if (tmp.length() && tmp.first() != '#' && tmp.first() != ';')
	{
	    unsigned int j, lnsp = tmp.length();	// lnsp = last non space
	    bool cont = false;

	    for (j = 0; j < tmp.length(); j++)
	    {
		switch (tmp[j])
		{
		case '\\':	// Special backspace handling ...

		    // Leave trailing backslash ...
		    if (j + 1 >= tmp.length())
		    {
			cont = true;
			break;
		    }

		    switch (tmp[j + 1])
		    {
		    case ' ':
			tmp.replace(j, j + 1, "\255", 1);
			lnsp = j;
			break;
		    case '\t':
			tmp.replace(j, j + 1, "\254", 1);
			lnsp = j;
			break;
		    case ';':	// Leave these characters alone ...
		    case '\\':	// (ie. skip them)
			tmp.erase(j, j);
			lnsp = j;
			break;
		    case 'n':	// Insert newline ...
		    case 'N':
			tmp.replace(j, j + 1, "\n", 1);
			lnsp = j;
			break;
		    case 'r':	// Insert return ...
		    case 'R':
			tmp.replace(j, j + 1, "\r", 1);
			lnsp = j;
			break;
		    case 't':	// Insert tab ...
		    case 'T':
			tmp.replace(j, j + 1, "\t", 1);
			lnsp = j;
			break;
		    case 'a':	// Insert CTCP code ...
		    case 'A':
			tmp.replace(j, j + 1, IRC_CTCP);
			j += (IRC_CTCP.length() - 1);
			lnsp = j;
			break;
		    case 'b':	// Insert bold ...
		    case 'B':
			tmp.replace(j, j + 1, IRC_Bold);
			j += (IRC_Bold.length() - 1);
			lnsp = j;
			break;
		    case 'c':	// Insert colour ...
		    case 'C':
			tmp.replace(j, j + 1, IRC_Color);
			j += (IRC_Color.length() - 1);
			lnsp = j;
			break;
		    case 'o':	// Insert off ...
		    case 'O':
			tmp.replace(j, j + 1, IRC_Off);
			j += (IRC_Off.length() - 1);
			lnsp = j;
			break;
		    case 'v':	// Insert reverse ...
		    case 'V':
			tmp.replace(j, j + 1, IRC_Reverse);
			j += (IRC_Reverse.length() - 1);
			lnsp = j;
			break;
		    case '_':	// Insert underline ...
			tmp.replace(j, j + 1, IRC_Underline);
			j += (IRC_Underline.length() - 1);
			lnsp = j;
			break;
		    default:
			tmp.erase(j, j);
			j--;
		    }
		    break;
		case ';':	// Non-backspaced comment
		    tmp.Truncate(j);
		    break;
		case ' ':	// Space characters
		case '\t':
		    break;
		default:
		    lnsp = j;
		}
	    }
	    if (lnsp + 1 < tmp.length())
		tmp.Truncate(lnsp + 1);

	    line += tmp;

	    if (!cont)
	    {
		Result.push_back(line);
		line.erase();
	    }
	}
    }
    NRET(vector < mstring >, Result);
    ETCB();
}
