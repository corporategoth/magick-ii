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
    NFT("ceNode::cdNode");
}

ceNode::ceNode(ceNode &in)
{
    FT("ceNode::cdNode", ("(ceNode &) in"));
}

ceNode::~ceNode()
{
    NFT("ceNode::~cdNode");
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
    map<mstring,ceNode * >::iterator i;
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
            if(i_children.find(i->first)==i_children.end())
                RET(false);
            // the below line *will* recursively check it's children
            if(!(*(i_children.find(i->first)->second)==*(i->second)))
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
    if(temppath.WordCount("/")==1)
    {
        // end of the line
        if(i_keys.find(temppath)==i_keys.end())
        {
            i_keys[temppath]=Value;
        }
    }
    else
    {
        //pull us out and pass us on
        mstring me,rest;
        me=temppath.Before("/");
        rest=temppath.After("/");
        if((i_children.find(me)==i_children.end())||(i_children[temppath]==NULL))
        {
            i_children[temppath]=new ceNode;
        }
        i_children[temppath]->SetKey(rest,Value);
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
    if(temppath.WordCount("/")==1)
    {
        // end of the line
        if(i_keys.find(temppath)==i_keys.end())
        {
            i_keys.erase(temppath);
            Result=true;
        }
    }
    else
    {
        //pull us out and pass us on
        mstring me,rest;
        me=temppath.Before("/");
        rest=temppath.After("/");
        if((i_children.find(me)==i_children.end())||(i_children[temppath]==NULL))
            Result=false;
        else
            Result=i_children[temppath]->DeleteKey(rest);
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
    if(temppath.WordCount("/")==1)
    {
        // end of the line
        if(i_children.find(temppath)==i_children.end())
        {
            i_children[temppath]=new ceNode;
            i_children[temppath]->i_Name=temppath;
        }
    }
    else
    {
        //pull us out and pass us on
        mstring me,rest;
        me=temppath.Before("/");
        rest=temppath.After("/");
        if((i_children.find(me)==i_children.end())||(i_children[temppath]==NULL))
        {
            i_children[temppath]=new ceNode;
        }
        i_children[temppath]->CreateNode(rest);
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
    if(temppath.WordCount("/")==1)
    {
        // end of the line
        if(i_children.find(temppath)!=i_children.end())
        {
            if(i_children[temppath]!=NULL)
                delete i_children[temppath];
            i_children.erase(temppath);
        }
    }
    else
    {
        //pull us out and pass us on
        mstring me,rest;
        me=temppath.Before("/");
        rest=temppath.After("/");
        if((i_children.find(me)!=i_children.end())&&(i_children[temppath]!=NULL))
        {
            i_children[temppath]->DeleteNode(rest);
        }
    }
    // todo set up the return values, true if it actually deletes it, false if it doesn't
    RET(true);
}

bool ceNode::NodeExists(const mstring &NodeName)
{
    // strip off the first bit of the path, if not exists, return false, otherwise pass
    // the rest of the path to it, so it can do the same itself.
    // ie NodeName="blah/test/test2", pull out blah, and pass "test/test2" to the node
    FT("ceNode::NodeExists", (NodeName));
    mstring temppath;
    bool Result=false;
    if(NodeName[0]=='/')
        temppath=NodeName.After("/");
    if(temppath.WordCount("/")==1)
    {
        // end of the line
        if(i_children.find(temppath)!=i_children.end()&&(i_children[temppath]!=NULL))
            Result=true;
    }
    else
    {
        //pull us out and pass us on
        mstring me,rest;
        me=temppath.Before("/");
        rest=temppath.After("/");
        if((i_children.find(me)!=i_children.end())&&(i_children[temppath]!=NULL))
        {
            Result=i_children[temppath]->NodeExists(rest);
        }
    }
    RET(false);
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

    if(i_FileName=="")
        RET(false);
    if(mFile::Exists(i_FileName))
    {
        vector<mstring> initialload;
        initialload=mFile::UnDump(i_FileName);
        bool retval = LoadFromArray(initialload);
        RET(retval);
    }
}

bool mConfigEngine::SaveFile()
{
    NFT("mConfigEngine::SaveFile");
    RET(false);
}

void mConfigEngine::Empty()
{
    NFT("mConfigEngine::Empty");
    RootNode.i_children.clear();
    RootNode.i_keys.clear();
}


mstring &mConfigEngine::Read(const mstring &key, const mstring Default)
{
    FT("mConfigEngine::Read", (key, Default));
    RET((mstring &) key);
}

bool mConfigEngine::Read(const mstring &key, mstring &outvar, mstring Default)
{
    FT("mConfigEngine::Read", (key, "(mstring &) outvar", Default));
    RET(false);
}

bool mConfigEngine::Read(const mstring &key, bool &outvar, bool Default)
{
    FT("mConfigEngine::Read", (key, "(bool &) outvar", Default));
    RET(false);
}

bool mConfigEngine::Read(const mstring &key, int &outvar, int Default)
{
    FT("mConfigEngine::Read", (key, "(int &) outvar", Default));
    RET(false);
}

bool mConfigEngine::Read(const mstring &key, unsigned int &outvar, unsigned int Default)
{
    FT("mConfigEngine::Read", (key, "(unsigned int &) outvar", Default));
    RET(false);
}

bool mConfigEngine::Read(const mstring &key, long &outvar, int Default)
{
    FT("mConfigEngine::Read", (key, "(long &) outvar", Default));
    RET(false);
}

bool mConfigEngine::Read(const mstring &key, unsigned long &outvar, unsigned int Default)
{
    FT("mConfigEngine::Read", (key, "(unsigned long &) outvar", Default));
    RET(false);
}

bool mConfigEngine::Read(const mstring &key, double &outvar, double Default)
{
    FT("mConfigEngine::Read", (key, "(double &) outvar", Default));
    RET(false);
}


void mConfigEngine::Write(const mstring &key,const mstring &value)
{
    FT("mConfigEngine::Write", (key, value));
}

void mConfigEngine::Write(const mstring &key,bool value)
{
    FT("mConfigEngine::Write", (key, value));
}

void mConfigEngine::Write(const mstring &key,int value)
{
    FT("mConfigEngine::Write", (key, value));
}

void mConfigEngine::Write(const mstring &key,unsigned int value)
{
    FT("mConfigEngine::Write", (key, value));
}

void mConfigEngine::Write(const mstring &key,long value)
{
    FT("mConfigEngine::Write", (key, value));
}

void mConfigEngine::Write(const mstring &key,unsigned long value)
{
    FT("mConfigEngine::Write", (key, value));
}

void mConfigEngine::Write(const mstring &key,double value)
{
    FT("mConfigEngine::Write", (key, value));
}


auto_ptr<ceNode> mConfigEngine::GetNode(const mstring& NodeName)
{
    FT("mConfigEngine::GetNode", (NodeName));
    auto_ptr<ceNode> Result;

    NRET(auto_ptr<ceNode>, Result);
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
    mstring currline;
    mstring currpath;
    for(vector<mstring>::const_iterator i=decommented.begin();i!=decommented.end();i++)
    {
        currline=*i;
        currline.Trim(true).Trim(false);
        if(currline[0]=='['&&currline.Last()==']')
        {
            // new section
            Result=RootNode.CreateNode(currline.After("[").Before("]"));
        }
        else if(currline.First('=')>0)
        {
            // new value
            Result=RootNode.SetKey(currline.Before("=").Trim(true),
					currline.After("=").Trim(false));
        }
    }
    RET(Result);
}

bool mConfigEngine::NodeExists(const mstring &NodeName)
{
    FT("mConfigEngine::NodeExists", (NodeName));
    RET(false);
}

vector<mstring> mConfigEngine::DeComment(const vector<mstring> in)
{
    FT("mConfigEngine::NodeExists", ("(const vector<mstrign>) in"));
    vector<mstring> Result;
    for(vector<mstring>::const_iterator i=in.begin();i!=in.end();i++)
    {
        if(*i[0] != '#' && *i[0] != ';')
        {
            // if we find ; then it's a comment to end of line, but /; is not a comment.
            bool founddelim=false;
            int j=1;
            while(founddelim = false && j < i->Len())
            {
                if(*i[j] == ';' && *i[j-1] != '\\')
                    founddelim=true;
            }
            Result.push_back(i->SubString(0, j-1));
        }
    }
    NRET(vector<mstring>, Result);
}

