#ifndef WIN32
#pragma interface
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
#ifndef _MCONFIG_H
#define _MCONFIG_H
static const char *ident_mconfig_h = "@(#) $Id$";
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.4  2000/05/20 15:17:00  prez
** Changed LOG system to use ACE's log system, removed wxLog, and
** added wrappers into pch.h and magick.cpp.
**
** Revision 1.3  2000/05/20 01:20:59  ungod
** more meaty goodness in the quest for a better config engine.
**
** Revision 1.2  2000/05/20 00:08:01  ungod
** getting ConfigEngine compiling and cleaning up SXP to stop circular includes of "datetime.h"
**
** Revision 1.1  2000/05/19 13:11:34  ungod
** initial checkin of the new config engine, class structure is in, but no code in it.
**
**
** ========================================================== */

#include "mstring.h"

class ceNode;

class ceNode
{
private:
    mstring i_Name;
    map<mstring,mstring> i_keys;
    map<mstring,auto_ptr<ceNode> > i_children;
public:
    ceNode();
    ceNode(ceNode &in);
    ~ceNode();
    ceNode& operator=(const ceNode &in);
    bool operator==(const ceNode &in)const;
    bool operator<(const ceNode &in)const;
    void SetKey(const mstring &KeyName, const mstring &Value);
    void DeleteKey(const mstring &KeyName);
    void CreateNode(const mstring &NodeName);
    void DeleteNode(const mstring &NodeName);
    bool NodeExists(const mstring &NodeName);
};

class mConfigEngine
{
private:
    ceNode RootNode;
    mstring i_FileName;
    vector<mstring> &DeComment(const vector<mstring> in);
public:
    mConfigEngine();
    mConfigEngine(const mstring& FileName);
    bool LoadFile();
    bool LoadFromString(const mstring& configstring);
    bool LoadFromArray(vector<mstring> configarray);
    bool SaveFile();
    void Empty();

    mstring &Read(const mstring &key, const mstring Defailt="");
    bool Read(const mstring &key, mstring &outvar, mstring Default="");
    bool Read(const mstring &key, bool &outvar, bool Default=true);
    bool Read(const mstring &key, int &outvar, int Default=0);
    bool Read(const mstring &key, unsigned int &outvar, unsigned int Default=0);
    bool Read(const mstring &key, long &outvar, int Default=0);
    bool Read(const mstring &key, unsigned long &outvar, unsigned int Default=0);
    bool Read(const mstring &key, double &outvar, double Default=0.0);

    void Write(const mstring &key,const mstring &value);
    void Write(const mstring &key,bool value);
    void Write(const mstring &key,int value);
    void Write(const mstring &key,unsigned int value);
    void Write(const mstring &key,long value);
    void Write(const mstring &key,unsigned long value);
    void Write(const mstring &key,double value);

    ceNode &GetNode(const mstring& NodeName);
    bool DeleteNode(const mstring& NodeName);
    bool DeleteKey(const mstring& KeyName);
    bool NodeExists(const mstring &NodeName);
};

#endif
