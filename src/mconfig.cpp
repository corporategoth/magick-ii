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
    i_children.clear();
    i_keys.clear();
}

ceNode& ceNode::operator=(const ceNode &in)
{
    FT("ceNode::operator=", ("(const ceNode &) in"));
}

bool ceNode::operator==(const ceNode &in)const
{
    FT("ceNode::operator==", ("(const ceNode &) in"));
    RET(false);
}

bool ceNode::operator<(const ceNode &in)const
{
    FT("ceNode::operator<", ("(const ceNode &) in"));
    RET(false);
}

bool ceNode::SetKey(const mstring &KeyName, const mstring &Value)
{
    FT("ceNode::SetKey", (KeyName, Value));
    RET(false);
}

bool ceNode::DeleteKey(const mstring &KeyName)
{
    FT("ceNode::DeleteKey", (KeyName));
    RET(false);
}

bool ceNode::CreateNode(const mstring &NodeName)
{
    // strip off the first bit of the path, if not exists, create the node, and pass
    // the rest of the path to it, so it can do the same itself.
    // ie NodeName="blah/test/test2", pull out blah, and pass "test/test2" to the node
    FT("ceNode::CreateNode", (NodeName));
    RET(false);
}

bool ceNode::DeleteNode(const mstring &NodeName)
{
    FT("ceNode::DeleteNode", (NodeName));
    RET(false);
}

bool ceNode::NodeExists(const mstring &NodeName)
{
    // strip off the first bit of the path, if not exists, return false, otherwise pass
    // the rest of the path to it, so it can do the same itself.
    // ie NodeName="blah/test/test2", pull out blah, and pass "test/test2" to the node
    FT("ceNode::NodeExists", (NodeName));
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
    bool retval = RootNode.DeleteNode(NodeName);
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
            Result=RootNode.SetKey(currline.Before("="),currline.After("="));
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

