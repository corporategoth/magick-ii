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

ceNode::ceNode()
{
}

ceNode::ceNode(ceNode &in)
{
}

ceNode::~ceNode()
{
    // probably not needed, but for safety's sake anyway
    i_children.clear();
    i_keys.clear();
}

ceNode& ceNode::operator=(const ceNode &in)
{
}

bool ceNode::operator==(const ceNode &in)const
{
}

bool ceNode::operator<(const ceNode &in)const
{
}

bool ceNode::SetKey(const mstring &KeyName, const mstring &Value)
{
}

bool ceNode::DeleteKey(const mstring &KeyName)
{
}

bool ceNode::CreateNode(const mstring &NodeName)
{
    // strip off the first bit of the path, if not exists, create the node, and pass
    // the rest of the path to it, so it can do the same itself.
    // ie NodeName="blah/test/test2", pull out blah, and pass "test/test2" to the node
}

bool ceNode::DeleteNode(const mstring &NodeName)
{
}

bool ceNode::NodeExists(const mstring &NodeName)
{
    // strip off the first bit of the path, if not exists, return false, otherwise pass
    // the rest of the path to it, so it can do the same itself.
    // ie NodeName="blah/test/test2", pull out blah, and pass "test/test2" to the node
}

mConfigEngine::mConfigEngine()
{
}

mConfigEngine::mConfigEngine(const mstring& FileName)
{
    i_FileName=FileName;
    LoadFile();
}

bool mConfigEngine::LoadFile()
{
    if(i_FileName=="")
        return false;
    if(mFile::Exists(i_FileName))
    {
        vector<mstring> initialload;
        initialload=mFile::UnDump(i_FileName);
        return LoadFromArray(initialload);
    }
}

bool mConfigEngine::SaveFile()
{
}

void mConfigEngine::Empty()
{
    RootNode.i_children.clear();
    RootNode.i_keys.clear();
}


mstring &mConfigEngine::Read(const mstring &key, const mstring Defailt)
{
}

bool mConfigEngine::Read(const mstring &key, mstring &outvar, mstring Default)
{
}

bool mConfigEngine::Read(const mstring &key, bool &outvar, bool Default)
{
}

bool mConfigEngine::Read(const mstring &key, int &outvar, int Default)
{
}

bool mConfigEngine::Read(const mstring &key, unsigned int &outvar, unsigned int Default)
{
}

bool mConfigEngine::Read(const mstring &key, long &outvar, int Default)
{
}

bool mConfigEngine::Read(const mstring &key, unsigned long &outvar, unsigned int Default)
{
}

bool mConfigEngine::Read(const mstring &key, double &outvar, double Default)
{
}


void mConfigEngine::Write(const mstring &key,const mstring &value)
{
}

void mConfigEngine::Write(const mstring &key,bool value)
{
}

void mConfigEngine::Write(const mstring &key,int value)
{
}

void mConfigEngine::Write(const mstring &key,unsigned int value)
{
}

void mConfigEngine::Write(const mstring &key,long value)
{
}

void mConfigEngine::Write(const mstring &key,unsigned long value)
{
}

void mConfigEngine::Write(const mstring &key,double value)
{
}


auto_ptr<ceNode> mConfigEngine::GetNode(const mstring& NodeName)
{
    auto_ptr<ceNode> Result;

    return Result;
}

bool mConfigEngine::DeleteNode(const mstring& NodeName)
{
    return RootNode.DeleteNode(NodeName);
}

bool mConfigEngine::DeleteKey(const mstring& KeyName)
{
    return RootNode.DeleteKey(KeyName);
}

bool mConfigEngine::LoadFromString(const mstring& configstring)
{
    vector<mstring> tempstore;
    for(unsigned int i=1;i<=configstring.WordCount("\n");i++)
        tempstore.push_back(configstring.ExtractWord(i,"\n"));
    return LoadFromArray(tempstore);
}

bool mConfigEngine::LoadFromArray(vector<mstring> configarray)
{
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
    return Result;
}

bool mConfigEngine::NodeExists(const mstring &NodeName)
{
}

vector<mstring> mConfigEngine::DeComment(const vector<mstring> in)
{
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
    return Result;
}

