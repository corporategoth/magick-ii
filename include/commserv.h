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

#include "base.h"

class Committee : public mUserDef
{
    friend wxOutputStream &operator<<(wxOutputStream& out,Committee& in);
    friend wxInputStream &operator>>(wxInputStream& in, Committee& out);
    mstring i_Name;
    mstring i_HeadCom;
    mstring i_Head;
    mstring i_Description;
    mstring i_Email;
    mstring i_URL;

    set<entlist_t> i_Members;
    bool i_Private;
    bool l_Private;
    bool i_OpenMemos;
    bool l_OpenMemos;
    bool i_Secure;
    bool l_Secure;
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
    mstring HeadCom()const	{ return i_HeadCom; }
    mstring Head()const		{ return i_Head; }
    mstring Description()const	{ return i_Description; }
    void Head(mstring newhead);

    bool insert(mstring entry, mstring nick, mDateTime modtime = Now());
    bool erase();
    entlist_ui begin()		{ return i_Members.begin(); }
    entlist_ui end()		{ return i_Members.end(); }
    size_t size()const		{ return i_Members.size(); }
    bool find(mstring entry);
    entlist_ui member;

    bool IsIn(mstring nick);
    bool IsHead(mstring nick);
    bool IsOn(mstring nick);

    // If TRUE, all members can do a /MS COMMITTEE 
    void Email(mstring in)	{ i_Email = in; }
    mstring Email()const	{ return i_Email; }
    void URL(mstring in)	{ i_URL = in; }
    mstring URL()const		{ return i_URL; }
    void Private(bool in)	{ i_Private = in; }
    bool Private()const		{ return i_Private; }
    void L_Private(bool in)	{ l_Private = in; }
    bool L_Private()const	{ return l_Private; }
    void OpenMemos(bool in)	{ i_OpenMemos = in; }
    bool OpenMemos()const	{ return i_OpenMemos; }
    void L_OpenMemos(bool in)	{ l_OpenMemos = in; }
    bool L_OpenMemos()const	{ return l_OpenMemos; }
    void Secure(bool in)	{ i_Secure = in; }
    bool Secure()const		{ return i_Secure; }
    void L_Secure(bool in)	{ l_Secure = in; }
    bool L_Secure()const	{ return l_Secure; }

    bool MSG_insert(mstring entry, mstring nick);
    bool MSG_erase();
    entlist_i MSG_begin()	{ return i_Messages.begin(); }
    entlist_i MSG_end()		{ return i_Messages.end(); }
    size_t MSG_size()		{ return i_Messages.size(); }
    bool MSG_find(int num);
    entlist_i message;

};
wxOutputStream &operator<<(wxOutputStream& out,Committee& in);
wxInputStream &operator>>(wxInputStream& in, Committee& out);

class CommServ : public mBase
{
    friend class Magick;
private:
    bool    def_openmemos;
    bool    lck_openmemos;
    bool    def_private;
    bool    lck_private;
    bool    def_secure;
    bool    lck_secure;
    mstring all_name;
    mstring regd_name;
    mstring sadmin_name;
    bool    sadmin_secure;
    bool    sadmin_private;
    bool    sadmin_openmemos;
    mstring sop_name;
    bool    sop_secure;
    bool    sop_private;
    bool    sop_openmemos;
    mstring admin_name;
    bool    admin_secure;
    bool    admin_private;
    bool    admin_openmemos;
    mstring oper_name;
    bool    oper_secure;
    bool    oper_private;
    bool    oper_openmemos;

    void AddCommands();
    void RemCommands();
public:
    class stats_t
    {
	friend class CommServ;

	unsigned long i_New;
	unsigned long i_Kill;
	unsigned long i_AddDel;
	unsigned long i_Memo;
	unsigned long i_Logon;
	unsigned long i_Set;
    public:
	stats_t() {
	    i_New = i_Kill = i_AddDel = i_Memo =
		i_Logon = i_Set = 0; }
	unsigned long New()	{ return i_New; }
	unsigned long Kill()	{ return i_Kill; }
	unsigned long AddDel()	{ return i_AddDel; }
	unsigned long Memo()	{ return i_Memo; }
	unsigned long Logon()	{ return i_Logon; }
	unsigned long Set()	{ return i_Set; }
    } stats;

    bool    DEF_OpenMemos()	{ return def_openmemos; }
    bool    LCK_OpenMemos()	{ return lck_openmemos; }
    bool    DEF_Private()	{ return def_private; }
    bool    LCK_Private()	{ return lck_private; }
    bool    DEF_Secure()	{ return def_secure; }
    bool    LCK_Secure()	{ return lck_secure; }
    mstring ALL_Name()		{ return all_name; }
    mstring REGD_Name()		{ return regd_name; }
    mstring SADMIN_Name()	{ return sadmin_name; }
    bool    SADMIN_Secure()	{ return sadmin_secure; }
    bool    SADMIN_Private()	{ return sadmin_private; }
    bool    SADMIN_OpenMemos()	{ return sadmin_openmemos; }
    mstring SOP_Name()		{ return sop_name; }
    bool    SOP_Secure()	{ return sop_secure; }
    bool    SOP_Private()	{ return sop_private; }
    bool    SOP_OpenMemos()	{ return sop_openmemos; }
    mstring ADMIN_Name()	{ return admin_name; }
    bool    ADMIN_Secure()	{ return admin_secure; }
    bool    ADMIN_Private()	{ return admin_private; }
    bool    ADMIN_OpenMemos()	{ return admin_openmemos; }
    mstring OPER_Name()		{ return oper_name; }
    bool    OPER_Secure()	{ return oper_secure; }
    bool    OPER_Private()	{ return oper_private; }
    bool    OPER_OpenMemos()	{ return oper_openmemos; }

    map<mstring,Committee> list;
    bool IsList(mstring in);

    virtual void load_database(wxInputStream& in);
    virtual void save_database(wxOutputStream& in);

    CommServ();
    virtual threadtype_enum Get_TType() const { return tt_OtherServ; };
    virtual mstring GetInternalName() const { return "CommServ"; };
    virtual void execute(const mstring & message);

    static void do_Help(mstring mynick, mstring source, mstring params);
    static void do_AddComm(mstring mynick, mstring source, mstring params);
    static void do_DelComm(mstring mynick, mstring source, mstring params);
    static void do_Add(mstring mynick, mstring source, mstring params);
    static void do_Del(mstring mynick, mstring source, mstring params);
    static void do_Memo(mstring mynick, mstring source, mstring params);
    static void do_Memo2(mstring source, mstring committee, mstring text);
    static void do_List(mstring mynick, mstring source, mstring params);
    static int do_List2(mstring mynick, mstring source, mstring committee, bool first, int number);
    static void do_Info(mstring mynick, mstring source, mstring params);
    static void do_logon_Add(mstring mynick, mstring source, mstring params);
    static void do_logon_Del(mstring mynick, mstring source, mstring params);
    static void do_logon_List(mstring mynick, mstring source, mstring params);
    static void do_set_Head(mstring mynick, mstring source, mstring params);
    static void do_set_Email(mstring mynick, mstring source, mstring params);
    static void do_set_URL(mstring mynick, mstring source, mstring params);
    static void do_set_Secure(mstring mynick, mstring source, mstring params);
    static void do_set_Private(mstring mynick, mstring source, mstring params);
    static void do_set_OpenMemos(mstring mynick, mstring source, mstring params);
};

#endif
