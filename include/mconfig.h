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
#include "pch.h"
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
** Revision 1.12  2000/12/19 07:24:53  prez
** Massive updates.  Linux works again, added akill reject threshold, and
** lots of other stuff -- almost ready for b6 -- first beta after the
** re-written strings class.  Also now using log adapter!
**
** Revision 1.11  2000/07/28 14:49:34  prez
** Ditched the old wx stuff, mconfig now in use, we're now ready to
** release (only got some conversion tests to do).
**
** Revision 1.10  2000/07/24 17:50:35  ungod
** should be finished... not tested
**
** Revision 1.9  2000/06/23 14:23:37  ungod
** more completion of the ceNode class and more work done on mConfigEngine
**
** Revision 1.8  2000/06/23 12:49:13  ungod
** completion of the ceNode class
**
** Revision 1.7  2000/05/28 05:05:13  prez
** More makefile stuff ... Now we should work on all platforms.
** Added alot of checking for different .h files, functions, etc.
** So now all #define's are config.h based (also added a default
** windows config.h, which will need to be copied on these systems).
**
** Revision 1.6  2000/05/25 11:49:32  ungod
** even more mConfigEngine meaty bits. (should we change it's name to Good-O's?)
**
** Revision 1.5  2000/05/21 14:01:09  ungod
** body for decomment and load/save to files.
**
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
    friend class mConfigEngine;
private:
    mstring i_Name;
    map<mstring,mstring> i_keys;
    map<mstring,ceNode * > i_children;
public:
    ceNode();
    ceNode(ceNode &in);
    ~ceNode();
    ceNode& operator=(const ceNode &in);
    bool operator==(const ceNode &in)const;
    bool operator<(const ceNode &in)const;
    bool SetKey(const mstring &KeyName, const mstring &Value);
    bool DeleteKey(const mstring &KeyName);
    bool CreateNode(const mstring &NodeName);
    bool DeleteNode(const mstring &NodeName);
    bool NodeExists(const mstring &NodeName);
    bool KeyExists(const mstring &KeyName);
    mstring GetKey(const mstring &KeyName, const mstring &DefValue);
    ceNode *GetNode(const mstring &NodeName);
    mstring Write(const mstring &KeyName, const mstring &Value);
    map<mstring,mstring> GetMap();
};

class mConfigEngine
{
private:
    ceNode RootNode;
    mstring i_FileName;
    vector<mstring> DeComment(const vector<mstring> in);
public:
    mConfigEngine();
    mConfigEngine(const mstring& FileName);
    bool LoadFile();
    bool LoadFromString(const mstring& configstring);
    bool LoadFromArray(vector<mstring> configarray);
    bool SaveFile();
    void Empty();

    map<mstring,mstring> GetMap() { return RootNode.GetMap(); }
    mstring Read(const mstring &key, const mstring Defailt="");
    bool Read(const mstring &key, mstring &outvar, mstring Default="");
    bool Read(const mstring &key, bool &outvar, bool Default=false);
    bool Read(const mstring &key, int &outvar, int Default=0);
    bool Read(const mstring &key, unsigned int &outvar, unsigned int Default=0);
    bool Read(const mstring &key, long &outvar, long Default=0);
    bool Read(const mstring &key, unsigned long &outvar, unsigned long Default=0);
    bool Read(const mstring &key, float &outvar, float Default=0.0);
    bool Read(const mstring &key, double &outvar, double Default=0.0);

    mstring Write(const mstring &key,const mstring &value);
    bool Write(const mstring &key,bool value);
    int Write(const mstring &key,int value);
    unsigned int Write(const mstring &key,unsigned int value);
    long Write(const mstring &key,long value);
    unsigned long Write(const mstring &key,unsigned long value);
    double Write(const mstring &key,double value);

    ceNode *GetNode(const mstring& NodeName);
    bool DeleteNode(const mstring& NodeName);
    bool DeleteKey(const mstring& KeyName);
    bool NodeExists(const mstring &NodeName);
};

#endif
