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
static const char *ident_config_h = "@(#) $Id$";
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.1  2000/05/19 13:08:27  ungod
** no message
**
** Revision 1.3  2000/02/23 12:21:01  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
**
** ========================================================== */

class ceNode;

class ceNode
{
private:
    mstring i_Name;
    map<mstring,mstring> i_keys;
    map<mstring,ceNode> i_children;
public:
    ceNode();
    ceNode(ceNode &in);
    ceNode& operator=(const ceNode &in);
    bool operator==(const ceNode &in)const;
    bool operator<(const ceNode &in)const;
    void SetKey(const mstring &KeyName, const mstring &Value);
    void DeleteKey(const mstring &KeyName);
    void CreateNode(const mstring &NodeName);
    void DeleteNode(const mstring *NodeName);
};

class mConfigEngine
{
private:
    ceNode RootNode;
    mstring i_FileName;
public:
    mConfigEngine();
    mConfigEngine(const mstring& FileName);
    bool LoadFile();
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
    bool DeleteKey(const nstring& KeyName);
};

#endif
