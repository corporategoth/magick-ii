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

#include "mstring.h"
#include "variant.h"
#include "base.h"

class Committee : public mUserDef
{
    friend wxOutputStream &operator<<(wxOutputStream& out,Committee& in);
    friend wxInputStream &operator>>(wxInputStream& in, Committee& out);
    mstring i_Name;
    // PREZ: why not an mstring? makes late data binding possible on loading.
    // *otherwise* i have to go through a second time and recreate the links
    // rather than just loading and storing the string which can be used to 
    // search the map in commserv anyway.
    mstring i_nameheadcom;
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

    mstring Name()const		{ return i_Name; }
    Committee *HeadCom()const	{ return i_HeadCom; }
    mstring Head()const		{ return i_Head; }
    mstring Description()const	{ return i_Description; }

    bool insert(mstring entry, mstring nick);
    bool erase();
    entlist_ui begin()const	{ return i_Members.begin(); }
    entlist_ui end()const	{ return i_Members.end(); }
    size_t size()const		{ return i_Members.size(); }
    entlist_ui member;

    bool IsIn(mstring nick);
    bool IsHead(mstring nick);

    // If TRUE, all members can do a /MS COMMITTEE 
    void OpenMemos(bool in)	{ i_OpenMemos = in; }
    bool OpenMemos()const	{ return i_OpenMemos; }

    bool MSG_insert(mstring entry, mstring nick);
    bool MSG_erase();
    entlist_i MSG_begin()const	{ return i_Messages.begin(); }
    entlist_i MSG_end()const	{ return i_Messages.end(); }
    size_t MSG_size()const	{ return i_Messages.size(); }
    entlist_i message;

};
wxOutputStream &operator<<(wxOutputStream& out,Committee& in);
wxInputStream &operator>>(wxInputStream& in, Committee& out);


class CommServ : public mBase
{
    friend class Magick;
private:
    bool secure_oper;
protected:
public:
    bool Secure_Oper() { return secure_oper; }

    map<mstring,Committee> list;
    bool IsList(mstring in);

    virtual void load_database(wxInputStream& in);
    virtual void save_database(wxOutputStream& in);

    CommServ() {}
    virtual threadtype_enum Get_TType() const { return tt_OtherServ; };
    virtual mstring GetInternalName() const { return "CommServ"; };
    virtual void execute(const mstring & message);
};

#endif