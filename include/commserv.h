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
#ifndef _COMMSERV_H
#define _COMMSERV_H
#include "pch.h"
static const char *ident_commserv_h = "@(#) $Id$";
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.33  2000/07/21 00:18:46  prez
** Fixed database loading, we can now load AND save databases...
**
** Almost ready to release now :)
**
** Revision 1.32  2000/06/18 12:49:26  prez
** Finished locking, need to do some cleanup, still some small parts
** of magick.cpp/h not locked properly, and need to ensure the case
** is the same every time something is locked/unlocked, but for the
** most part, locks are done, we lock pretty much everything :)
**
** Revision 1.31  2000/06/12 06:07:49  prez
** Added Usage() functions to get ACCURATE usage stats from various
** parts of services.  However bare in mind DONT use this too much
** as it has to go through every data item to grab the usages.
**
** Revision 1.30  2000/05/28 05:05:13  prez
** More makefile stuff ... Now we should work on all platforms.
** Added alot of checking for different .h files, functions, etc.
** So now all #define's are config.h based (also added a default
** windows config.h, which will need to be copied on these systems).
**
** Revision 1.29  2000/05/17 07:47:57  prez
** Removed all save_databases calls from classes, and now using XML only.
** To be worked on: DCC Xfer pointer transferal and XML Loading
**
** Revision 1.28  2000/05/14 04:02:52  prez
** Finished off per-service XML stuff, and we should be ready to go.
**
** Revision 1.27  2000/05/09 09:11:59  prez
** Added XMLisation to non-mapped structures ... still need to
** do the saving stuff ...
**
** Revision 1.26  2000/03/28 09:42:10  prez
** Changed CommServ, ADD/DEL/LIST -> MEMBER ADD/DEL/LIST
** and NEW/KILL -> ADD/DEL and created a new LIST
**
** Revision 1.25  2000/03/15 08:23:51  prez
** Added locking stuff for commserv options, and other stuff
**
** Revision 1.24  2000/02/23 12:21:01  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
**
** ========================================================== */


#include "base.h"

class Committee : public mUserDef, public SXP::IPersistObj
{
    friend class CommServ;

    mstring i_Name;
    mDateTime i_RegTime;
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

    vector<entlist_t *> members_array;

    static SXP::Tag tag_Committee, tag_Name, tag_HeadCom, tag_Head,
	tag_Description, tag_Email, tag_URL, tag_set_Private,
	tag_set_OpenMemos, tag_set_Secure, tag_lock_Private,
	tag_lock_OpenMemos, tag_lock_Secure, tag_Members,
	tag_Messages, tag_UserDef, tag_RegTime;
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
    mDateTime RegTime();
    mstring HeadCom();
    mstring Head();
    mstring Description();
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

    void Email(mstring in);
    mstring Email();
    void URL(mstring in);
    mstring URL();
    void Private(bool in);
    bool Private();
    void L_Private(bool in);
    bool L_Private();
    void OpenMemos(bool in);
    bool OpenMemos();
    void L_OpenMemos(bool in);
    bool L_OpenMemos();
    void Secure(bool in);
    bool Secure();
    void L_Secure(bool in);
    bool L_Secure();

    bool MSG_insert(mstring entry, mstring nick);
    bool MSG_erase();
    entlist_i MSG_begin()	{ return i_Messages.begin(); }
    entlist_i MSG_end()		{ return i_Messages.end(); }
    size_t MSG_size()		{ return i_Messages.size(); }
    bool MSG_find(int num);
    entlist_i message;

    SXP::Tag& GetClassTag() const { return tag_Committee; }
    virtual void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement);
    virtual void EndElement(SXP::IParser * pIn, SXP::IElement * pElement);
    virtual void WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs);

    size_t Usage();
};
wxOutputStream &operator<<(wxOutputStream& out,Committee& in);
wxInputStream &operator>>(wxInputStream& in, Committee& out);

class CommServ : public mBase, public SXP::IPersistObj
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
    static SXP::Tag tag_CommServ;

    vector<Committee *> c_array;

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
	unsigned long i_Lock;
	unsigned long i_Unlock;
    public:
	stats_t() {
	    i_New = i_Kill = i_AddDel = i_Memo =
		i_Logon = i_Set = i_Lock = i_Unlock = 0; }
	unsigned long New()	{ return i_New; }
	unsigned long Kill()	{ return i_Kill; }
	unsigned long AddDel()	{ return i_AddDel; }
	unsigned long Memo()	{ return i_Memo; }
	unsigned long Logon()	{ return i_Logon; }
	unsigned long Set()	{ return i_Set; }
	unsigned long Lock()	{ return i_Lock; }
	unsigned long Unlock()	{ return i_Unlock; }
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

    CommServ();
    virtual threadtype_enum Get_TType() const { return tt_OtherServ; };
    virtual mstring GetInternalName() const { return "CommServ"; };
    virtual void execute(const mstring & message);

    static void do_Help(mstring mynick, mstring source, mstring params);
    static void do_Add(mstring mynick, mstring source, mstring params);
    static void do_Del(mstring mynick, mstring source, mstring params);
    static void do_List(mstring mynick, mstring source, mstring params);
    static void do_Memo(mstring mynick, mstring source, mstring params);
    static void do_Memo2(mstring source, mstring committee, mstring text);
    static void do_Info(mstring mynick, mstring source, mstring params);
    static void do_member_Add(mstring mynick, mstring source, mstring params);
    static void do_member_Del(mstring mynick, mstring source, mstring params);
    static void do_member_List(mstring mynick, mstring source, mstring params);
    static int do_member_List2(mstring mynick, mstring source, mstring committee, bool first, int number);
    static void do_logon_Add(mstring mynick, mstring source, mstring params);
    static void do_logon_Del(mstring mynick, mstring source, mstring params);
    static void do_logon_List(mstring mynick, mstring source, mstring params);
    static void do_set_Head(mstring mynick, mstring source, mstring params);
    static void do_set_Email(mstring mynick, mstring source, mstring params);
    static void do_set_URL(mstring mynick, mstring source, mstring params);
    static void do_set_Secure(mstring mynick, mstring source, mstring params);
    static void do_set_Private(mstring mynick, mstring source, mstring params);
    static void do_set_OpenMemos(mstring mynick, mstring source, mstring params);
    static void do_lock_Secure(mstring mynick, mstring source, mstring params);
    static void do_lock_Private(mstring mynick, mstring source, mstring params);
    static void do_lock_OpenMemos(mstring mynick, mstring source, mstring params);
    static void do_unlock_Secure(mstring mynick, mstring source, mstring params);
    static void do_unlock_Private(mstring mynick, mstring source, mstring params);
    static void do_unlock_OpenMemos(mstring mynick, mstring source, mstring params);

    virtual SXP::Tag& GetClassTag() const { return tag_CommServ; }
    virtual void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement);
    virtual void EndElement(SXP::IParser * pIn, SXP::IElement * pElement);
    virtual void WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs);
    void PostLoad();
};

#endif
