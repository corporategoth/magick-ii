// $Id$
//
// Magick IRC Services
// (c) 1997-1999 Preston A. Elder <prez@magick.tm>
// (c) 1998-1999 W. King <ungod@magick.tm>
//
// The above copywright may not be removed under any
// circumstances, however it may be added to if any
// modifications are made to this file.  All modified
// code must be clearly documented and labelled.
//
// ===================================================
#ifndef _commserv_h
#define _commserv_h

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include "mstring.h"
#include "variant.h"
#include "base.h"

class Committee : public mUserDef
{
    mstring i_Name;
    Committee *i_HeadCom;
    mstring i_Head;
    mstring i_Description;

    set<entlist_t> i_Members;
    bool i_OpenMemos;
    list<entlist_t> i_Messages;

public:
    Committee() {}
    Committee(const Committee &in) { *this = in; }
    Committee(mstring name, mstring head, mstring description);
    Committee(mstring name, Committee *head, mstring description);
    Committee(mstring name, mstring description);
    void operator=(const Committee &in);
    bool operator==(const Committee &in) const
    	{ return (i_Name == in.i_Name); }
    bool operator!=(const Committee &in) const
    	{ return (i_Name != in.i_Name); }
    bool operator<(const Committee &in) const
    	{ return (i_Name < in.i_Name); }

    mstring Name()		{ return i_Name; }
    Committee *HeadCom()	{ return i_HeadCom; }
    mstring Head()		{ return i_Head; }
    mstring Description()	{ return i_Description; }

    bool insert(mstring entry, mstring nick);
    bool erase();
    entlist_ui begin()		{ return i_Members.begin(); }
    entlist_ui end()		{ return i_Members.end(); }
    size_t size()		{ return i_Members.size(); }
    entlist_ui member;

    bool IsIn(mstring nick);
    bool IsHead(mstring nick);

    // If TRUE, all members can do a /MS COMMITTEE 
    void OpenMemos(bool in)	{ i_OpenMemos = in; }
    bool OpenMemos()		{ return i_OpenMemos; }

    bool MSG_insert(mstring entry, mstring nick);
    bool MSG_erase();
    entlist_i MSG_begin()	{ return i_Messages.begin(); }
    entlist_i MSG_end()		{ return i_Messages.end(); }
    size_t MSG_size()		{ return i_Messages.size(); }
    entlist_i message;

};

class CommServ : public mBase
{
    friend class Magick;
private:
    bool secure_oper;
protected:
public:
    bool Secure_Oper() { return secure_oper; }

    map<mstring,Committee> stored;
    bool IsStored(mstring in);

    virtual void load_database(void);
    virtual void save_database(void);

    CommServ();
    virtual threadtype_enum Get_TType() const { return tt_OtherServ; };
    virtual mstring GetInternalName() const { return "CommServ"; };
    virtual void execute(const mstring & message);
};

#endif