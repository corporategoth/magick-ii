#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
#endif

/*  Magick IRC Services
**
** (c) 1997-2000 Preston Elder <prez@magick.tm>
** (c) 1998-2000 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any
** circumstances, however it may be added to if any
** modifications are made to this file.  All modified
** code must be clearly documented and labelled.
**
** ========================================================== */
static const char *ident = "@(#)$Id$";
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
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
#include "filesys.h"
#include "trace.h"

ceNode::ceNode()
{
    NFT("ceNode::ceNode");
}

ceNode::ceNode(ceNode &in)
{
    FT("ceNode::ceNode", ("(ceNode &) in"));
}

ceNode::~ceNode()
{
    NFT("ceNode::~ceNode");
    // probably not needed, but for safety's sake anyway
    for(map<mstring,ceNode * >::iterator i=i_children.begin();i!=i_children.end();i++)
        if(i->second!=NULL)
        {
            delete i->second;
            i->second=NULL;
        }
    i_children.clear();
    i_keys.clear();
}

ceNode& ceNode::operator=(const ceNode &in)
{
    FT("ceNode::operator=", ("(const ceNode &) in"));
    i_Name=in.i_Name;
    i_keys.clear();
    i_keys=in.i_keys;
    for(map<mstring,ceNode * >::iterator i=i_children.begin();i!=i_children.end();i++)
        if(i->second!=NULL)
        {
            delete i->second;
            i->second=NULL;
        }
    i_children.clear();
    for(map<mstring,ceNode * >::const_iterator j=in.i_children.begin();j!=in.i_children.end();j++)
    {
        i_children[j->first]=new ceNode;
        // the below line *will* recursively copy it's children
        *(i_children[j->first])=*(j->second);
    }
    return *this;
}

bool ceNode::operator==(const ceNode &in)const
{
    FT("ceNode::operator==", ("(const ceNode &) in"));
    bool Result=false;
    if(i_keys==in.i_keys)
    {
        map<mstring,ceNode * >::const_iterator i;
        for(i=in.i_children.begin();i!=in.i_children.end();i++)
        {
            if(i_children.find(i->first) == i_children.end())
                RET(false);
            // the below line *will* recursively check it's children
            if(!(*(i_children.find(i->first)->second) == *(i->second)))
                RET(false);
        }
        Result=true;
    }
    RET(Result);
}

bool ceNode::operator<(const ceNode &in)const
{
    FT("ceNode::operator<", ("(const ceNode &) in"));
    RET(i_Name<in.i_Name);
}

bool ceNode::SetKey(const mstring &KeyName, const mstring &Value)
{
    FT("ceNode::SetKey", (KeyName, Value));

    mstring temppath;
    if(KeyName[0]=='/')
        temppath=KeyName.After("/");
    else
	temppath=KeyName;
    temppath.MakeUpper();

    if(!temppath.Contains("/"))
    {
	i_keys[temppath]=Value;
    }
    else
    {
        mstring next,rest;
	next = temppath.Before("/");
	rest = temppath.After("/");
        if (i_children.find(next) == i_children.end() ||
	    i_children[next] == NULL)
        {
            i_children[next]=new ceNode;
            i_children[next]->i_Name=next;
        }
        i_children[next]->SetKey(rest,Value);
    }
    // todo set up the return values, true if it actually sets it, false if it doesn't
    RET(false);
}

bool ceNode::DeleteKey(const mstring &KeyName)
{
    FT("ceNode::DeleteKey", (KeyName));
    mstring temppath;
    bool Result=false;
    if(KeyName[0]=='/')
        temppath=KeyName.After("/");
    else
	temppath=KeyName;
    temppath.MakeUpper();
    if(!temppath.Contains("/"))
    {
        // end of the line
        if (i_keys.find(temppath) == i_keys.end())
        {
            i_keys.erase(temppath);
            Result=true;
        }
    }
    else
    {
        mstring next,rest;
	next = temppath.Before("/");
	rest = temppath.After("/");
        if (i_children.find(next) == i_children.end() ||
	    i_children[next] == NULL)
            Result=false;
        else
            Result=i_children[next]->DeleteKey(rest);
    }
    RET(Result);
}

bool ceNode::CreateNode(const mstring &NodeName)
{
    // strip off the first bit of the path, if not exists, create the node, and pass
    // the rest of the path to it, so it can do the same itself.
    // ie NodeName="blah/test/test2", pull out blah, and pass "test/test2" to the node
    FT("ceNode::CreateNode",(NodeName));
    mstring temppath;
    if(NodeName[0]=='/')
        temppath=NodeName.After("/");
    else
	temppath=NodeName;
    temppath.MakeUpper();
    if(!temppath.Contains("/"))
    {
        // end of the line
        if (i_children.find(temppath) == i_children.end())
        {
            i_children[temppath]=new ceNode;
            i_children[temppath]->i_Name=temppath;
        }
    }
    else
    {
        mstring next,rest;
	next = temppath.Before("/");
	rest = temppath.After("/");
        if (i_children.find(next) == i_children.end() ||
	    i_children[next] == NULL)
        {
            i_children[next]=new ceNode;
	    i_children[next]->i_Name=next;
        }
        i_children[next]->CreateNode(rest);
    }
    // todo set up the return values, true if it actually creates it, false if it doesn't
    RET(true);
}

bool ceNode::DeleteNode(const mstring &NodeName)
{
    FT("ceNode::DeleteNode",(NodeName));
    mstring temppath;
    if(NodeName[0]=='/')
        temppath=NodeName.After("/");
    else
	temppath=NodeName;
    temppath.MakeUpper();
    if(!temppath.Contains("/"))
    {
        // end of the line
        if (i_children.find(temppath) != i_children.end())
        {
            if(i_children[temppath]!=NULL)
                delete i_children[temppath];
            i_children.erase(temppath);
        }
    }
    else
    {
        mstring next,rest;
	next = temppath.Before("/");
	rest = temppath.After("/");
        if (i_children.find(next) != i_children.end() &&
	    i_children[next] != NULL)
        {
            i_children[next]->DeleteNode(rest);
        }
    }
    // todo set up the return values, true if it actually deletes it, false if it doesn't
    RET(true);
}

bool ceNode::NodeExists(const mstring &NodeName) const
{
    // strip off the first bit of the path, if not exists, return false, otherwise pass
    // the rest of the path to it, so it can do the same itself.
    // ie NodeName="blah/test/test2", pull out blah, and pass "test/test2" to the node
    FT("ceNode::NodeExists", (NodeName));
    mstring temppath;
    bool Result=false;
    if(NodeName[0]=='/')
        temppath=NodeName.After("/");
    else
	temppath=NodeName;
    temppath.MakeUpper();
    if(!temppath.Contains("/"))
    {
        // end of the line
	map<mstring,ceNode * >::const_iterator iter = i_children.find(temppath);
	if (iter != i_children.end() && iter->second != NULL)
            Result=true;
    }
    else
    {
        mstring next,rest;
	next = temppath.Before("/");
	rest = temppath.After("/");
	map<mstring,ceNode * >::const_iterator iter = i_children.find(temppath);
	if (iter != i_children.end() && iter->second != NULL)
        {
            Result=iter->second->NodeExists(rest);
        }
    }
    RET(Result);
}

bool ceNode::KeyExists(const mstring &KeyName) const
{
    FT("ceNode::KeyExists", (KeyName));
    mstring temppath;
    bool Result=false;
    if(KeyName[0]=='/')
        temppath=KeyName.After("/");
    else
	temppath=KeyName;
    temppath.MakeUpper();
    if(!temppath.Contains("/"))
    {
        // end of the line
	map<mstring,mstring>::const_iterator iter = i_keys.find(temppath);
	if(iter != i_keys.end() && iter->second != "")
            Result=true;
    }
    else
    {
        mstring next,rest;
	next = temppath.Before("/");
	rest = temppath.After("/");
	map<mstring,ceNode * >::const_iterator iter = i_children.find(temppath);
	if (iter != i_children.end() && iter->second != NULL)
        {
            Result=iter->second->KeyExists(rest);
        }
    }
    RET(Result);
}

mstring ceNode::GetKey(const mstring &KeyName, const mstring &DefValue) const
{
    FT("ceNode::GetKey", (KeyName,DefValue));
    mstring temppath;
    mstring Result=DefValue;
    if(KeyName[0]=='/')
        temppath=KeyName.After("/");
    else
	temppath=KeyName;
    temppath.MakeUpper();
    if(!temppath.Contains("/"))
    {
        // end of the line
	map<mstring,mstring>::const_iterator iter = i_keys.find(temppath);
	if(iter != i_keys.end() && iter->second != "")
            Result=iter->second;
	else
	    Result=DefValue;
    }
    else
    {
        mstring next,rest;
	next = temppath.Before("/");
	rest = temppath.After("/");
	map<mstring,ceNode * >::const_iterator iter = i_children.find(temppath);
	if (iter != i_children.end() && iter->second != NULL)
        {
            Result=iter->second->GetKey(rest,DefValue);
        }
    }
    RET(Result);
}

ceNode *ceNode::GetNode(const mstring &NodeName)
{
    FT("ceNode::GetNode", (NodeName));
    mstring temppath;
    ceNode *Result=NULL;
    if(NodeName[0]=='/')
        temppath=NodeName.After("/");
    else
	temppath=NodeName;
    temppath.MakeUpper();
    if(!temppath.Contains("/"))
    {
        // end of the line
	map<mstring,ceNode * >::const_iterator iter = i_children.find(temppath);
	if (iter != i_children.end() && iter->second != NULL)
            Result=iter->second;
    }
    else
    {
        mstring next,rest;
	next = temppath.Before("/");
	rest = temppath.After("/");
        // note i don't use NodeExists and CreateNode here as this is a recursive function
        // and that would cause it to check if the node exists for every recursion of this function
	map<mstring,ceNode * >::iterator iter = i_children.find(temppath);
	if (iter == i_children.end() || iter->second != NULL)
        {
            i_children[next]=new ceNode;
            i_children[next]->i_Name=next;
        }
        Result=i_children[next]->GetNode(rest);
    }
    NRET(ceNode *,Result);
}

mstring ceNode::Write(const mstring &KeyName, const mstring &Value)
{
    FT("ceNode::Write", (KeyName,Value));
    mstring temppath;
    mstring Result="";
    if(KeyName[0]=='/')
        temppath=KeyName.After("/");
    else
	temppath=KeyName;
    temppath.MakeUpper();
    if(!temppath.Contains("/"))
    {
        // end of the line
        Result=i_keys[temppath];
        i_keys[temppath]=Value;
    }
    else
    {
        mstring next,rest;
	next = temppath.Before("/");
	rest = temppath.After("/");
        // note i don't use NodeExists and CreateNode here as this is a recursive function
        // and that would cause it to check if the node exists for every recursion of this function
	map<mstring,ceNode * >::iterator iter = i_children.find(temppath);
	if (iter == i_children.end() || iter->second != NULL)
        {
            i_children[next]=new ceNode;
            i_children[next]->i_Name=next;
        }
        Result=i_children[next]->Write(rest,Value);
    }
    RET(Result);
}

map<mstring,mstring> ceNode::GetMap() const
{
    NFT("ceNode::GetMap");
    map<mstring,mstring> submap, retval;

    map<mstring,ceNode * >::const_iterator i;
    map<mstring,mstring>::const_iterator j;
    for(i = i_children.begin(); i != i_children.end(); i++)
    {
        if(i->second!=NULL)
        {
	    submap = i->second->GetMap();
	    for (j=submap.begin(); j!=submap.end(); j++)
	    {
		if (i_Name != "")
		    retval[i_Name + "/" + j->first] = j->second;
		else
		    retval[j->first] = j->second;

	    }
	    submap.clear();
        }
    }

    for (j=i_keys.begin(); j!=i_keys.end(); j++)
    {
	if (i_Name != "")
	    retval[i_Name + "/" + j->first] = j->second;
	else
	    retval[j->first] = j->second;
    }
    NRET(map<mstring_mstring>, retval);
}

mConfigEngine::mConfigEngine()
{
    NFT("mConfigEngine::mConfigEngine");
}

mConfigEngine::mConfigEngine(const mstring& FileName)
{
    FT("mConfigEngine::mConfigEngine", (FileName));
    i_FileName=FileName;
    LoadFile();
}

bool mConfigEngine::LoadFile()
{
    NFT("mConfigEngine::LoadFile");

    if(i_FileName != "" && mFile::Exists(i_FileName))
    {
        vector<mstring> initialload;
        initialload=mFile::UnDump(i_FileName);
        bool retval = LoadFromArray(initialload);
        RET(retval);
    }
    RET(false);
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
    map<mstring, ceNode *>::iterator i;
    for(i=RootNode.i_children.begin();i!=RootNode.i_children.end();i++)
        if(i->second!=NULL)
	{
            delete i->second;
	    i->second = NULL;
	}
    RootNode.i_children.clear();
    RootNode.i_keys.clear();
}


mstring mConfigEngine::Read(const mstring &key, const mstring Default) const
{
    FT("mConfigEngine::Read", (key, Default));
    mstring Result;
    Result=RootNode.GetKey(key,Default);
    RET(Result);
}

bool mConfigEngine::Read(const mstring &key, mstring &outvar, mstring Default) const
{
    FT("mConfigEngine::Read", (key, "(mstring &) outvar", Default));
    bool Result=true;
    outvar=RootNode.GetKey(key,Default);
    RET(Result);
}

bool mConfigEngine::Read(const mstring &key, bool &outvar, bool Default) const
{
    FT("mConfigEngine::Read", (key, "(bool &) outvar", Default));
    mstring tmp;
    bool Result=true;
    tmp=RootNode.GetKey(key,Default ? "true" : "false");
    if (tmp.IsBool())
	outvar=tmp.GetBool();
    else
    {
        outvar=Default;
        Result=false;
    }
    RET(Result);
}

bool mConfigEngine::Read(const mstring &key, int &outvar, int Default) const
{
    FT("mConfigEngine::Read", (key, "(int &) outvar", Default));
    mstring tmpvar;
    bool Result=true;
    tmpvar=RootNode.GetKey(key,Default);
    if(tmpvar.IsNumber())
        outvar=atoi(tmpvar.c_str());
    else
    {
        outvar=Default;
        Result=false;
    }
    RET(Result);
}

bool mConfigEngine::Read(const mstring &key, unsigned int &outvar, unsigned int Default) const
{
    FT("mConfigEngine::Read", (key, "(unsigned int &) outvar", Default));
    mstring tmpvar;
    bool Result=true;
    tmpvar=RootNode.GetKey(key,Default);
    if(tmpvar.IsNumber())
        outvar=atoi(tmpvar.c_str());
    else
    {
        outvar=Default;
        Result=false;
    }
    RET(Result);
}

bool mConfigEngine::Read(const mstring &key, long &outvar, long Default) const
{
    FT("mConfigEngine::Read", (key, "(long &) outvar", Default));
    mstring tmpvar;
    bool Result=true;
    tmpvar=RootNode.GetKey(key,Default);
    if(tmpvar.IsNumber())
        outvar=atol(tmpvar.c_str());
    else
    {
        outvar=Default;
        Result=false;
    }
    RET(Result);
}

bool mConfigEngine::Read(const mstring &key, unsigned long &outvar, unsigned long Default) const
{
    FT("mConfigEngine::Read", (key, "(unsigned long &) outvar", Default));
    mstring tmpvar;
    bool Result=true;
    tmpvar=RootNode.GetKey(key,Default);
    if(tmpvar.IsNumber())
    {
        outvar=atoul(tmpvar.c_str());
    }
    else
    {
        outvar=Default;
        Result=false;
    }
    RET(Result);
}

bool mConfigEngine::Read(const mstring &key, double &outvar, double Default) const
{
    FT("mConfigEngine::Read", (key, "(double &) outvar", Default));
    mstring tmpvar;
    bool Result=true;
    tmpvar=RootNode.GetKey(key,Default);
    if(tmpvar.IsNumber())
        outvar=atod(tmpvar.c_str());
    else
    {
        outvar=Default;
        Result=false;
    }
    RET(Result);
}

bool mConfigEngine::Read(const mstring &key, float &outvar, float Default) const
{
    FT("mConfigEngine::Read", (key, "(double &) outvar", Default));
    mstring tmpvar;
    bool Result=true;
    tmpvar=RootNode.GetKey(key,Default);
    if(tmpvar.IsNumber())
        outvar=atof(tmpvar.c_str());
    else
    {
        outvar=Default;
        Result=false;
    }
    RET(Result);
}

mstring mConfigEngine::Write(const mstring &key,const mstring &value)
{
    FT("mConfigEngine::Write", (key, value));
    mstring Result=RootNode.Write(key,value);
    RET(Result);
}

bool mConfigEngine::Write(const mstring &key,bool value)
{
    FT("mConfigEngine::Write", (key, value));
    bool Result;
    Read(key,Result,false);
    if(value==true)
        Write(key,"True");
    else
        Write(key,"False");
    RET(Result);
}

int mConfigEngine::Write(const mstring &key,int value)
{
    FT("mConfigEngine::Write", (key, value));
    mstring tmp;
    tmp<<value;
    int Result;
    Read(key,Result,0);
    Write(key,tmp);
    RET(Result);
}

unsigned int mConfigEngine::Write(const mstring &key,unsigned int value)
{
    FT("mConfigEngine::Write", (key, value));
    mstring tmp;
    tmp<<value;
    unsigned int Result;
    Read(key,Result,0U);
    Write(key,tmp);
    RET(Result);
}

long mConfigEngine::Write(const mstring &key,long value)
{
    FT("mConfigEngine::Write", (key, value));
    mstring tmp;
    tmp<<value;
    long Result;
    Read(key,Result,0L);
    Write(key,tmp);
    RET(Result);
}

unsigned long mConfigEngine::Write(const mstring &key,unsigned long value)
{
    FT("mConfigEngine::Write", (key, value));
    mstring tmp;
    tmp<<value;
    unsigned long Result;
    Read(key,Result,0UL);
    Write(key,tmp);
    RET(Result);
}

double mConfigEngine::Write(const mstring &key,double value)
{
    FT("mConfigEngine::Write", (key, value));
    mstring tmp;
    tmp<<value;
    double Result;
    Read(key,Result,0.0);
    Write(key,tmp);
    RET(Result);
}


ceNode *mConfigEngine::GetNode(const mstring& NodeName)
{
    // warning *HIGHLY DANGEROUS, as i can't get auto_ptr's working this pointer
    //  could be freed at any time, trust it only as long as you have to
    // but at the moment, i'm just too damn brainfried to redesign this
    FT("mConfigEngine::GetNode", (NodeName));
    ceNode *retval = RootNode.GetNode(NodeName);
    NRET(ceNode *, retval);
}

bool mConfigEngine::DeleteNode(const mstring& NodeName)
{
    FT("mConfigEngine::DeleteNode", (NodeName));
    bool retval=RootNode.DeleteNode(NodeName);
    RET(retval);
}

bool mConfigEngine::DeleteKey(const mstring& KeyName)
{
    FT("mConfigEngine::DeleteKey", (KeyName));
    bool retval = RootNode.DeleteKey(KeyName);
    RET(retval);
}

bool mConfigEngine::LoadFromString(const mstring& configstring)
{
    FT("mConfigEngine::LoadFromString", (configstring));
    vector<mstring> tempstore;
    for(unsigned int i=1;i<=configstring.WordCount("\n");i++)
        tempstore.push_back(configstring.ExtractWord(i,"\n"));
    bool retval = LoadFromArray(tempstore);
    RET(retval);
}

bool mConfigEngine::LoadFromArray(vector<mstring> configarray)
{
    FT("mConfigEngine::LoadFromArray", ("(vector<mstring>) configarray"));
    bool Result=false;
    vector<mstring> decommented;
    decommented=DeComment(configarray);
    ceNode *currNode=&RootNode;
    mstring currline, currpath;
    for(vector<mstring>::const_iterator i=decommented.begin();i!=decommented.end();i++)
    {
        currline=i->Strip(true).Strip(false);
        if(currline[0]=='[' && currline.last()==']')
        {
            // new section
            Result=RootNode.CreateNode(currline.After("[").Before("]"));
            currNode=RootNode.GetNode(currline.After("[").Before("]"));
            currpath = currline.After("[").Before("]");
        }
        else if(currline.find_first_of('=')>0)
        {
            // new value
            if(currNode!=NULL)
            {
		mstring data = currline.After("=").Strip(false);
		data.replace("\\ ", " ");
                Result=currNode->SetKey(currline.Before("=").Strip(true), data);
                CSRC(currpath,currline.Before("=").Strip(true), data);
	    }
        }
    }
    RET(Result);
}

bool mConfigEngine::NodeExists(const mstring &NodeName) const
{
    FT("mConfigEngine::NodeExists", (NodeName));
    RET(RootNode.NodeExists(NodeName));
}

vector<mstring> mConfigEngine::DeComment(const vector<mstring> in)
{
    FT("mConfigEngine::NodeExists", ("(const vector<mstrign>) in"));
    vector<mstring> Result;
    for(vector<mstring>::const_iterator i=in.begin();i!=in.end();i++)
    {
	mstring tmp = *i;
        if(tmp.length() && tmp[0] != '#' && tmp[0] != ';')
        {
            // if we find ; then it's a comment to end of line, but /; is not a comment.
            bool founddelim=false;
            unsigned int j=1;
            for (; founddelim == false && j < tmp.length(); j++)
            {
                if(tmp[j] == ';' && tmp[j-1] != '\\')
                {
		    j-=2;
                    founddelim=true;
		}
            }
            Result.push_back(tmp.SubString(0, j-1));
        }
    }
    NRET(vector<mstring>, Result);
}
