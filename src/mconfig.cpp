#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
#endif

/*  Magick IRC Services
**
** (c) 1997-2001 Preston Elder <prez@magick.tm>
** (c) 1998-2001 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any
** circumstances, however it may be added to if any
** modifications are made to this file.  All modified
** code must be clearly documented and labelled.
**
** ========================================================== */
#define RCSID(x,y) const char *rcsid_mconfig_cpp_ ## x () { return y; }
RCSID(mconfig_cpp, "@(#)$Id$");

/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.38  2002/01/12 14:42:09  prez
** Pretty-printed all code ... looking at implementing an auto-prettyprint.
**
** Revision 1.37  2001/12/12 03:31:15  prez
** Re-wrote the occurances/find/replace functions in mstring to actually work
** with contents that includes a binary 0.  Also fixed PreParse in mconfig.
**
** Revision 1.36  2001/12/06 01:41:39  prez
** Some fixes to the config file parser -- slightly more flexable now.
**
** Revision 1.35  2001/11/12 01:05:03  prez
** Added new warning flags, and changed code to reduce watnings ...
**
** Revision 1.34  2001/11/03 21:02:53  prez
** Mammoth change, including ALL changes for beta12, and all stuff done during
** the time GOTH.NET was down ... approx. 3 months.  Includes EPONA conv utils.
**
** Revision 1.33  2001/07/15 07:35:38  prez
** Fixed problem of it removing access list entries on slave nickname drop.
** Also fixed it so it wouldnt ignore ini entries that were deliberately blank.
**
** Revision 1.32  2001/06/15 07:20:40  prez
** Fixed windows compiling -- now works with MS Visual Studio 6.0
**
** Revision 1.31  2001/04/02 02:11:23  prez
** Fixed up some inlining, and added better excption handling
**
** Revision 1.30  2001/03/20 14:22:14  prez
** Finished phase 1 of efficiancy updates, we now pass mstring/mDateTime's
** by reference all over the place.  Next step is to stop using operator=
** to initialise (ie. use mstring blah(mstring) not mstring blah = mstring).
**
** Revision 1.29  2001/02/03 02:21:34  prez
** Loads of changes, including adding ALLOW to ini file, cleaning up
** the includes, RCSID, and much more.  Also cleaned up most warnings.
**
** Revision 1.28  2001/01/15 23:31:39  prez
** Added LogChan, HelpOp from helpserv, and changed all string != ""'s to
** !string.empty() to save processing.
**
** Revision 1.27  2001/01/01 05:32:44  prez
** Updated copywrights.  Added 'reversed help' syntax (so ACCESS HELP ==
** HELP ACCESS).
**
** Revision 1.26  2000/12/30 19:53:13  prez
** Fixed problem where it would create keys twice ...
**
** Revision 1.25  2000/12/25 06:36:14  prez
** Added locking around the threadtoself map, and removed a bunch of
** defines from mstring (while keeping it the same!)
**
** Revision 1.24  2000/12/23 23:18:36  prez
** Fixed problem with it using default value always
**
** Revision 1.23  2000/12/23 22:22:24  prez
** 'constified' all classes (ie. made all functions that did not need to
** touch another non-const function const themselves, good for data integrity).
**
** Revision 1.22  2000/12/19 07:24:53  prez
** Massive updates.  Linux works again, added akill reject threshold, and
** lots of other stuff -- almost ready for b6 -- first beta after the
** re-written strings class.  Also now using log adapter!
**
** Revision 1.21  2000/12/09 20:16:41  prez
** Fixed SubString and Left to have correct count/end possitions.  Also
** adjusted rest of source to follow suit.
**
** Revision 1.20  2000/11/09 10:58:19  prez
** THINK I have it working again ... with the free list.
** Will check, still thinking of sorting free list by size.
**
** Revision 1.19  2000/10/10 11:47:52  prez
** mstring is re-written totally ... find or occurances
** or something has a problem, but we can debug that :)
**
** Revision 1.18  2000/09/30 10:48:08  prez
** Some general code cleanups ... got rid of warnings, etc.
**
** Revision 1.17  2000/08/31 06:25:09  prez
** Added our own socket class (wrapper around ACE_SOCK_Stream,
** ACE_SOCK_Connector and ACE_SOCK_Acceptor, with tracing).
**
** Revision 1.16  2000/08/19 15:17:40  ungod
** no message
**
** Revision 1.15  2000/07/28 14:49:35  prez
** Ditched the old wx stuff, mconfig now in use, we're now ready to
** release (only got some conversion tests to do).
**
** Revision 1.14  2000/07/24 17:51:21  ungod
** should be finished... not tested
**
** Revision 1.13  2000/07/21 00:18:49  prez
** Fixed database loading, we can now load AND save databases...
**
** Almost ready to release now :)
**
** Revision 1.12  2000/06/23 14:21:18  ungod
** more completion of the ceNode class and more work done on mConfigEngine
**
** Revision 1.11  2000/06/23 12:49:44  ungod
** completion of the ceNode class
**
** Revision 1.10  2000/06/11 09:30:21  prez
** Added propper MaxLine length, no more hard-coded constants.
**
** Revision 1.9  2000/06/09 13:57:00  prez
** Added tracing to mconfig
**
** Revision 1.8  2000/05/25 11:49:32  ungod
** even more mConfigEngine meaty bits. (should we change it's name to Good-O's?)
**
** Revision 1.7  2000/05/25 08:16:39  prez
** Most of the LOGGING for commands is complete, now have to do mainly
** backend stuff ...
**
** Revision 1.6  2000/05/22 13:25:12  ungod
** making it compilable again after removal of log.cpp/.h
**
** Revision 1.5  2000/05/22 13:00:09  prez
** Updated version.h and some other stuff
**
** Revision 1.4  2000/05/21 14:01:10  ungod
** body for decomment and load/save to files.
**
** Revision 1.3  2000/05/20 01:20:59  ungod
** more meaty goodness in the quest for a better config engine.
**
** Revision 1.2  2000/05/20 00:08:02  ungod
** getting ConfigEngine compiling and cleaning up SXP to stop circular includes of "datetime.h"
**
** Revision 1.1  2000/05/19 13:11:34  ungod
** initial checkin of the new config engine, class structure is in, but no code in it.
**
**
** ========================================================== */

#include "mconfig.h"
#include "mexceptions.h"
#include "filesys.h"

ceNode::~ceNode()
{
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
}

ceNode & ceNode::operator=(const ceNode & in)
{
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
	*(i_children[j->first]) = *(j->second);
    }
    return *this;
}

bool ceNode::operator==(const ceNode & in) const
{
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
	    if (!(*(i_children.find(i->first)->second) == *(i->second)))
		RET(false);
	}
	Result = true;
    }
    RET(Result);
}

bool ceNode::SetKey(const mstring & KeyName, const mstring & Value)
{
    FT("ceNode::SetKey", (KeyName, Value));

    mstring temppath;
    bool Result = false;

    if (KeyName.first() == '/')
	temppath = KeyName.After("/");
    else
	temppath = KeyName;
    temppath.MakeUpper();

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
}

bool ceNode::DeleteKey(const mstring & KeyName)
{
    FT("ceNode::DeleteKey", (KeyName));
    mstring temppath;
    bool Result = false;

    if (KeyName.first() == '/')
	temppath = KeyName.After("/");
    else
	temppath = KeyName;
    temppath.MakeUpper();
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
}

bool ceNode::CreateNode(const mstring & NodeName)
{
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
    temppath.MakeUpper();
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
}

bool ceNode::DeleteNode(const mstring & NodeName)
{
    FT("ceNode::DeleteNode", (NodeName));
    mstring temppath;
    bool Result = false;

    if (NodeName.first() == '/')
	temppath = NodeName.After("/");
    else
	temppath = NodeName;
    temppath.MakeUpper();
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
}

bool ceNode::NodeExists(const mstring & NodeName) const
{
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
    temppath.MakeUpper();
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
}

bool ceNode::KeyExists(const mstring & KeyName) const
{
    FT("ceNode::KeyExists", (KeyName));
    mstring temppath;
    bool Result = false;

    if (KeyName.first() == '/')
	temppath = KeyName.After("/");
    else
	temppath = KeyName;
    temppath.MakeUpper();
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
}

mstring ceNode::GetKey(const mstring & KeyName, const mstring & DefValue) const
{
    FT("ceNode::GetKey", (KeyName, DefValue));
    mstring temppath;
    mstring Result = DefValue;

    if (KeyName.first() == '/')
	temppath = KeyName.After("/");
    else
	temppath = KeyName;
    temppath.MakeUpper();
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
}

ceNode *ceNode::GetNode(const mstring & NodeName)
{
    FT("ceNode::GetNode", (NodeName));
    mstring temppath;
    ceNode *Result = NULL;

    if (NodeName.first() == '/')
	temppath = NodeName.After("/");
    else
	temppath = NodeName;
    temppath.MakeUpper();
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
}

mstring ceNode::Write(const mstring & KeyName, const mstring & Value)
{
    FT("ceNode::Write", (KeyName, Value));
    mstring temppath;
    mstring Result = "";

    if (KeyName.first() == '/')
	temppath = KeyName.After("/");
    else
	temppath = KeyName;
    temppath.MakeUpper();
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
}

map < mstring, mstring > ceNode::GetMap() const
{
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
}


bool mConfigEngine::LoadFile()
{
    NFT("mConfigEngine::LoadFile");

    bool Result = false;

    if (!i_FileName.empty() && mFile::Exists(i_FileName))
    {
	vector < mstring > initialload;
	initialload = mFile::UnDump(i_FileName);
	Result = LoadFromArray(initialload);
    }
    RET(Result);
}

bool mConfigEngine::SaveFile()
{
    NFT("mConfigEngine::SaveFile");
    // bah i'll write this one later
    RET(false);
}

void mConfigEngine::Empty()
{
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
}


mstring mConfigEngine::Read(const mstring & key, const mstring & Default) const
{
    FT("mConfigEngine::Read", (key, Default));
    mstring Result;

    Result = RootNode.GetKey(key, Default);
    RET(Result);
}

bool mConfigEngine::Read(const mstring & key, mstring & outvar, const mstring & Default) const
{
    FT("mConfigEngine::Read", (key, "(mstring &) outvar", Default));
    bool Result = true;

    outvar = RootNode.GetKey(key, Default);
    RET(Result);
}

bool mConfigEngine::Read(const mstring & key, bool & outvar, const bool Default) const
{
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
}

bool mConfigEngine::Read(const mstring & key, int &outvar, const int Default) const
{
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
}

bool mConfigEngine::Read(const mstring & key, unsigned int &outvar, const unsigned int Default) const
{
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
}

bool mConfigEngine::Read(const mstring & key, long &outvar, const long Default) const
{
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
}

bool mConfigEngine::Read(const mstring & key, unsigned long &outvar, const unsigned long Default) const
{
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
}

bool mConfigEngine::Read(const mstring & key, double &outvar, const double Default) const
{
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
}

bool mConfigEngine::Read(const mstring & key, float &outvar, const float Default) const
{
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
}

mstring mConfigEngine::Write(const mstring & key, const mstring & value)
{
    FT("mConfigEngine::Write", (key, value));
    mstring Result = RootNode.Write(key, value);

    RET(Result);
}

bool mConfigEngine::Write(const mstring & key, const bool value)
{
    FT("mConfigEngine::Write", (key, value));
    bool Result;

    Read(key, Result, false);
    if (value == true)
	Write(key, mstring("TRUE"));
    else
	Write(key, mstring("FALSE"));
    RET(Result);
}

int mConfigEngine::Write(const mstring & key, const int value)
{
    FT("mConfigEngine::Write", (key, value));
    mstring tmp;

    tmp << value;
    int Result;

    Read(key, Result, 0);
    Write(key, tmp);
    RET(Result);
}

unsigned int mConfigEngine::Write(const mstring & key, const unsigned int value)
{
    FT("mConfigEngine::Write", (key, value));
    mstring tmp;

    tmp << value;
    unsigned int Result;

    Read(key, Result, 0U);
    Write(key, tmp);
    RET(Result);
}

long mConfigEngine::Write(const mstring & key, const long value)
{
    FT("mConfigEngine::Write", (key, value));
    mstring tmp;

    tmp << value;
    long Result;

    Read(key, Result, 0L);
    Write(key, tmp);
    RET(Result);
}

unsigned long mConfigEngine::Write(const mstring & key, const unsigned long value)
{
    FT("mConfigEngine::Write", (key, value));
    mstring tmp;

    tmp << value;
    unsigned long Result;

    Read(key, Result, 0UL);
    Write(key, tmp);
    RET(Result);
}

double mConfigEngine::Write(const mstring & key, const double value)
{
    FT("mConfigEngine::Write", (key, value));
    mstring tmp;

    tmp << value;
    double Result;

    Read(key, Result, 0.0);
    Write(key, tmp);
    RET(Result);
}


ceNode *mConfigEngine::GetNode(const mstring & NodeName)
{
    // warning *HIGHLY DANGEROUS, as i can't get auto_ptr's working this pointer
    //  could be freed at any time, trust it only as long as you have to
    // but at the moment, i'm just too damn brainfried to redesign this
    FT("mConfigEngine::GetNode", (NodeName));
    ceNode *Result = RootNode.GetNode(NodeName);

    NRET(ceNode *, Result);
}

bool mConfigEngine::DeleteNode(const mstring & NodeName)
{
    FT("mConfigEngine::DeleteNode", (NodeName));
    bool Result = RootNode.DeleteNode(NodeName);

    RET(Result);
}

bool mConfigEngine::DeleteKey(const mstring & KeyName)
{
    FT("mConfigEngine::DeleteKey", (KeyName));
    bool Result = RootNode.DeleteKey(KeyName);

    RET(Result);
}

bool mConfigEngine::LoadFromString(const mstring & configstring)
{
    FT("mConfigEngine::LoadFromString", (configstring));
    vector < mstring > tempstore;
    for (unsigned int i = 1; i <= configstring.WordCount("\n"); i++)
	tempstore.push_back(configstring.ExtractWord(i, "\n"));
    bool Result = LoadFromArray(tempstore);

    RET(Result);
}

bool mConfigEngine::LoadFromArray(const vector < mstring > &configarray)
{
    FT("mConfigEngine::LoadFromArray", ("(vector<mstring>) configarray"));
    bool Result = false;

    vector < mstring > decommented;
    decommented = PreParse(configarray);
    ceNode *currNode = &RootNode;
    mstring currline, currpath;

    for (vector < mstring >::const_iterator i = decommented.begin(); i != decommented.end(); i++)
    {
	currline = *i;
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
}

bool mConfigEngine::NodeExists(const mstring & NodeName) const
{
    FT("mConfigEngine::NodeExists", (NodeName));
    bool Result = RootNode.NodeExists(NodeName);

    RET(Result);
}

vector < mstring > mConfigEngine::PreParse(const vector < mstring > &in)
{
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
}
