#ifndef WIN32
#pragma interface
#endif

/*  Magick IRC Services
**
** (c) 1997-2001 Preston Elder <prez@magick.tm>
** (c) 1998-2001 William King <ungod@magick.tm>
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
RCSID(commserv_h, "@(#) $Id$");

/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.63  2002/01/12 14:42:08  prez
** Pretty-printed all code ... looking at implementing an auto-prettyprint.
**
** Revision 1.62  2002/01/10 19:30:37  prez
** FINALLY finished a MAJOR overhaul ... now have a 'safe pointer', that
** ensures that data being used cannot be deleted while still being used.
**
** Revision 1.61  2001/12/25 08:43:12  prez
** Fixed XML support properly ... it now works again with new version of
** expat (1.95.2) and sxp (1.1).  Also removed some of my const hacks.
**
** Revision 1.60  2001/12/23 20:46:03  prez
** Added cleanup code for committee entries in channel access list.
**
** Revision 1.59  2001/11/12 01:05:01  prez
** Added new warning flags, and changed code to reduce watnings ...
**
** Revision 1.58  2001/11/04 23:43:14  prez
** Updates for MS Visual C++ compilation (it works now!).
**
** Revision 1.57  2001/11/03 21:02:50  prez
** Mammoth change, including ALL changes for beta12, and all stuff done during
** the time GOTH.NET was down ... approx. 3 months.  Includes EPONA conv utils.
**
** Revision 1.56  2001/06/15 07:20:39  prez
** Fixed windows compiling -- now works with MS Visual Studio 6.0
**
** Revision 1.55  2001/05/17 19:18:53  prez
** Added ability to chose GETPASS or SETPASS.
**
** Revision 1.54  2001/05/01 14:00:21  prez
** Re-vamped locking system, and entire dependancy system.
** Will work again (and actually block across threads), however still does not
** work on larger networks (coredumps).  LOTS OF PRINTF's still int he code, so
** DO NOT RUN THIS WITHOUT REDIRECTING STDOUT!  Will remove when debugged.
**
** Revision 1.53  2001/04/05 05:59:50  prez
** Turned off -fno-default-inline, and split up server.cpp, it should
** compile again with no special options, and have default inlines :)
**
** Revision 1.52  2001/04/02 02:13:27  prez
** Added inlines, fixed more of the exception code.
**
** Revision 1.51  2001/03/27 07:04:30  prez
** All maps have been hidden, and are now only accessable via. access functions.
**
** Revision 1.50  2001/03/20 14:22:13  prez
** Finished phase 1 of efficiancy updates, we now pass mstring/mDateTime's
** by reference all over the place.  Next step is to stop using operator=
** to initialise (ie. use mstring blah(mstring) not mstring blah = mstring).
**
** Revision 1.49  2001/03/02 05:24:41  prez
** HEAPS of modifications, including synching up my own archive.
**
** Revision 1.48  2001/02/11 07:41:27  prez
** Enhansed support for server numerics, specifically for Unreal.
**
** Revision 1.46  2001/02/03 03:20:33  prez
** Fixed up some differences in previous committed versions ...
**
** Revision 1.43  2000/12/23 22:22:23  prez
** 'constified' all classes (ie. made all functions that did not need to
** touch another non-const function const themselves, good for data integrity).
**
** Revision 1.42  2000/12/22 19:50:19  prez
** Made all config options const.  Beginnings of securing all non-modifying
** commands to const.  also added serviceschk.
**
** Revision 1.41  2000/12/21 14:18:17  prez
** Fixed AKILL expiry, added limit for chanserv on-join messages and commserv
** logon messages.  Also added ability to clear stats and showing of time
** stats are effective for (ie. time since clear).  Also fixed ordering of
** commands, anything with 2 commands (ie. a space in it) should go before
** anything with 1.
**
** Revision 1.40  2000/12/10 02:56:06  prez
** Added ability to change DESCRIPTION field in committees.
**
** Revision 1.39  2000/09/22 12:26:10  prez
** Fixed that pesky bug with chanserv not seeing modes *sigh*
**
** Revision 1.38  2000/09/02 07:20:44  prez
** Added the DumpB/DumpE functions to all major objects, and put in
** some example T_Modify/T_Changing code in NickServ (set email).
**
** Revision 1.37  2000/08/19 10:59:46  prez
** Added delays between nick/channel registering and memo sending,
** Added limit of channels per reg'd nick
** Added setting of user modes when recognized on hard-coded committees
**
** Revision 1.36  2000/08/08 09:58:55  prez
** Added ModeO to 4 pre-defined committees.
** Also added back some deletes in xml in the hope that it
** will free up some memory ...
**
** Revision 1.35  2000/08/06 08:06:41  prez
** Fixed loading of logon messages in committee ..
**
** Revision 1.34  2000/07/28 14:49:34  prez
** Ditched the old wx stuff, mconfig now in use, we're now ready to
** release (only got some conversion tests to do).
**
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

class Committee_t:public mUserDef, public SXP::IPersistObj, public ref_class
{
    friend class CommServ;

    mstring i_Name;
    mDateTime i_RegTime;
    mstring i_HeadCom;
    mstring i_Head;
    mstring i_Description;
    mstring i_Email;
    mstring i_URL;

    set < entlist_t > i_Members;

    class
    {
	friend class Committee_t;
	bool Private:1;
	bool OpenMemos:1;
	bool Secure:1;
    }
    setting, lock;

    list < entlist_t > i_Messages;

    vector < entlist_t * >members_array;
    vector < entlist_t * >messages_array;

    void defaults();
    static SXP::Tag tag_Committee_t, tag_Name, tag_HeadCom, tag_Head, tag_Description, tag_Email, tag_URL, tag_set_Private,
	tag_set_OpenMemos, tag_set_Secure, tag_lock_Private, tag_lock_OpenMemos, tag_lock_Secure, tag_Members, tag_Messages,
	tag_UserDef, tag_RegTime;
  public:
    Committee_t();
    Committee_t(const Committee_t & in):mUserDef(in), SXP::IPersistObj(in), ref_class()
    {
	*this = in;
    }
    Committee_t(const mstring & name, const mstring & head, const mstring & description);
    Committee_t(const mstring & name, const Committee_t & head, const mstring & description);
    Committee_t(const mstring & name, const mstring & description);

    ~Committee_t()
    {
    }
    Committee_t & operator=(const Committee_t & in);
    bool operator==(const Committee_t & in) const
    {
	return (i_Name == in.i_Name);
    }
    bool operator!=(const Committee_t & in) const
    {
	return (i_Name != in.i_Name);
    }
    bool operator<(const Committee_t & in) const
    {
	return (i_Name < in.i_Name);
    }

    mstring Name() const
    {
	return i_Name;
    }
    mDateTime RegTime() const;
    void Drop();
    mstring HeadCom() const;
    mstring Head() const;
    void Head(const mstring & newhead);

    bool insert(const mstring & entry, const mstring & nick, const mDateTime & modtime = mDateTime::CurrentDateTime());
    bool erase();
    entlist_ui begin()
    {
	return i_Members.begin();
    }
    entlist_ui end()
    {
	return i_Members.end();
    }
    size_t size() const
    {
	return i_Members.size();
    }
    bool find(const mstring & entry);
    entlist_ui member;

    bool IsIn(const mstring & nick) const;
    bool IsHead(const mstring & nick) const;
    bool IsOn(const mstring & nick) const;

    void Description(const mstring & in);
    mstring Description() const;
    void Email(const mstring & in);
    mstring Email() const;
    void URL(const mstring & in);
    mstring URL() const;
    void Private(const bool in);
    bool Private() const;
    void L_Private(const bool in);
    bool L_Private() const;
    void OpenMemos(const bool in);
    bool OpenMemos() const;
    void L_OpenMemos(const bool in);
    bool L_OpenMemos() const;
    void Secure(const bool in);
    bool Secure() const;
    void L_Secure(const bool in);
    bool L_Secure() const;

    bool MSG_insert(const mstring & entry, const mstring & nick, const mDateTime & time = mDateTime::CurrentDateTime());
    bool MSG_erase();
    entlist_i MSG_begin()
    {
	return i_Messages.begin();
    }
    entlist_i MSG_end()
    {
	return i_Messages.end();
    }
    size_t MSG_size() const
    {
	return i_Messages.size();
    }
    bool MSG_find(const int num);
    entlist_i message;

    SXP::Tag & GetClassTag()const
    {
	return tag_Committee_t;
    }
    void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement);
    void EndElement(SXP::IParser * pIn, SXP::IElement * pElement);
    void WriteElement(SXP::IOutStream * pOut, SXP::dict & attribs = SXP::blank_dict);

    size_t Usage() const;
    void DumpB() const;
    void DumpE() const;
};


class CommServ:public mBase, public SXP::IPersistObj
{
    friend class Magick;
  private:
    unsigned int max_logon;

    class
    {
	friend class CommServ;
	friend class Magick;
	bool Private:1;
	bool OpenMemos:1;
	bool Secure:1;
    }
    def, lock;

    class setting_t
    {
	friend class CommServ;
	friend class Magick;
	mstring Name;
	mstring SetMode;
	bool Private:1;
	bool OpenMemos:1;
	bool Secure:1;
	bool ModeO:1;
    }
    sadmin, sop, admin, oper, regd, all;

    mstring ovr_view;
    mstring ovr_owner;
    mstring ovr_cs_mode;
    mstring ovr_cs_op;
    mstring ovr_cs_halfop;
    mstring ovr_cs_voice;
    mstring ovr_cs_invite;
    mstring ovr_cs_unban;
    mstring ovr_cs_clear;
    static SXP::Tag tag_CommServ;

  public:
    typedef map < mstring, Committee_t * >list_t;

  private:

    vector < Committee_t * >c_array;
    list_t i_list;

    void AddCommands();
    void RemCommands();

  public:
    CommServ();
    ~CommServ()
    {
    }
    class stats_t
    {
	friend class CommServ;

	mDateTime i_ClearTime;
	unsigned long i_Add;
	unsigned long i_Del;
	unsigned long i_Member;
	unsigned long i_Logon;
	unsigned long i_Memo;
	unsigned long i_Set;
	unsigned long i_Lock;
	unsigned long i_Unlock;
      public:
	stats_t()
	{
	    clear();
	}
	void clear()
	{
	    i_ClearTime = mDateTime::CurrentDateTime();
	    i_Add = i_Del = i_Member = i_Logon = i_Memo = i_Set = i_Lock = i_Unlock = 0;
	}
	mDateTime ClearTime() const
	{
	    return i_ClearTime;
	}
	unsigned long Add() const
	{
	    return i_Add;
	}
	unsigned long Del() const
	{
	    return i_Del;
	}
	unsigned long Member() const
	{
	    return i_Member;
	}
	unsigned long Logon() const
	{
	    return i_Logon;
	}
	unsigned long Memo() const
	{
	    return i_Memo;
	}
	unsigned long Set() const
	{
	    return i_Set;
	}
	unsigned long Lock() const
	{
	    return i_Lock;
	}
	unsigned long Unlock() const
	{
	    return i_Unlock;
	}
    }
    stats;

    unsigned int Max_Logon() const
    {
	return max_logon;
    }
    bool DEF_Private() const
    {
	return def.Private;
    }
    bool LCK_Private() const
    {
	return lock.Private;
    }
    bool DEF_OpenMemos() const
    {
	return def.OpenMemos;
    }
    bool LCK_OpenMemos() const
    {
	return lock.OpenMemos;
    }
    bool DEF_Secure() const
    {
	return def.Secure;
    }
    bool LCK_Secure() const
    {
	return lock.Secure;
    }
    mstring ALL_Name() const
    {
	return all.Name;
    }
    mstring ALL_SetMode() const
    {
	return all.SetMode;
    }
    mstring REGD_Name() const
    {
	return regd.Name;
    }
    mstring REGD_SetMode() const
    {
	return regd.SetMode;
    }
    mstring SADMIN_Name() const
    {
	return sadmin.Name;
    }
    mstring SADMIN_SetMode() const
    {
	return sadmin.SetMode;
    }
    bool SADMIN_Private() const
    {
	return sadmin.Private;
    }
    bool SADMIN_OpenMemos() const
    {
	return sadmin.OpenMemos;
    }
    bool SADMIN_Secure() const
    {
	return sadmin.Secure;
    }
    bool SADMIN_ModeO() const
    {
	return sadmin.ModeO;
    }
    mstring SOP_Name() const
    {
	return sop.Name;
    }
    mstring SOP_SetMode() const
    {
	return sop.SetMode;
    }
    bool SOP_Private() const
    {
	return sop.Private;
    }
    bool SOP_OpenMemos() const
    {
	return sop.OpenMemos;
    }
    bool SOP_Secure() const
    {
	return sop.Secure;
    }
    bool SOP_ModeO() const
    {
	return sop.ModeO;
    }
    mstring ADMIN_Name() const
    {
	return admin.Name;
    }
    mstring ADMIN_SetMode() const
    {
	return admin.SetMode;
    }
    bool ADMIN_Private() const
    {
	return admin.Private;
    }
    bool ADMIN_OpenMemos() const
    {
	return admin.OpenMemos;
    }
    bool ADMIN_Secure() const
    {
	return admin.Secure;
    }
    bool ADMIN_ModeO() const
    {
	return admin.ModeO;
    }
    mstring OPER_Name() const
    {
	return oper.Name;
    }
    mstring OPER_SetMode() const
    {
	return oper.SetMode;
    }
    bool OPER_Private() const
    {
	return oper.Private;
    }
    bool OPER_OpenMemos() const
    {
	return oper.OpenMemos;
    }
    bool OPER_Secure() const
    {
	return oper.Secure;
    }
    bool OPER_ModeO() const
    {
	return oper.ModeO;
    }
    mstring OVR_View() const
    {
	return ovr_view;
    }
    mstring OVR_Owner() const
    {
	return ovr_owner;
    }
    mstring OVR_CS_Mode() const
    {
	return ovr_cs_mode;
    }
    mstring OVR_CS_Op() const
    {
	return ovr_cs_op;
    }
    mstring OVR_CS_HalfOp() const
    {
	return ovr_cs_halfop;
    }
    mstring OVR_CS_Voice() const
    {
	return ovr_cs_voice;
    }
    mstring OVR_CS_Invite() const
    {
	return ovr_cs_invite;
    }
    mstring OVR_CS_Unban() const
    {
	return ovr_cs_unban;
    }
    mstring OVR_CS_Clear() const
    {
	return ovr_cs_clear;
    }

#ifdef MAGICK_HAS_EXCEPTIONS
    void AddList(Committee_t * in) throw(E_CommServ_List);
    void AddList(const Committee_t & in) throw(E_CommServ_List)
    {
	AddList(new Committee_t(in));
    }
    void AddList(const map_entry < Committee_t > &in) throw(E_CommServ_List)
    {
	AddList(in.entry());
    }
    map_entry < Committee_t > GetList(const mstring & in) const throw(E_CommServ_List);
    void RemList(const mstring & in) throw(E_CommServ_List);
#else
    void AddList(Committee_t * in);
    void AddList(const Committee_t & in)
    {
	AddList(new Committee_t(in));
    }
    void AddList(const map_entry < Committee_t > &in)
    {
	AddList(in.entry());
    }
    map_entry < Committee_t > GetList(const mstring & in);
    void RemList(const mstring & in);
#endif
    list_t::iterator ListBegin()
    {
	return i_list.begin();
    }
    list_t::iterator ListEnd()
    {
	return i_list.end();
    }
    list_t::const_iterator ListBegin()const
    {
	return i_list.begin();
    }
    list_t::const_iterator ListEnd() const
    {
	return i_list.end();
    }
    size_t ListSize() const
    {
	return i_list.size();
    }
    bool IsList(const mstring & in) const;

    threadtype_enum Get_TType() const
    {
	return tt_OtherServ;
    };
    mstring GetInternalName() const
    {
	return "CommServ";
    };
    void execute(mstring & source, const mstring & msgtype, const mstring & params);

    static void do_Help(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Add(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Del(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_List(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Memo(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Memo2(const mstring & source, const mstring & committee, const mstring & text);
    static void do_Info(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_member_Add(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_member_Del(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_member_List(const mstring & mynick, const mstring & source, const mstring & params);
    static int do_member_List2(const mstring & mynick, const mstring & source, const mstring & committee, const bool first,
			       const int number);
    static void do_logon_Add(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_logon_Del(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_logon_List(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_Head(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_Description(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_Email(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_URL(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_Secure(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_Private(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_set_OpenMemos(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_lock_Secure(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_lock_Private(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_lock_OpenMemos(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_unlock_Secure(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_unlock_Private(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_unlock_OpenMemos(const mstring & mynick, const mstring & source, const mstring & params);

    SXP::Tag & GetClassTag()const
    {
	return tag_CommServ;
    }
    void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement);
    void EndElement(SXP::IParser * pIn, SXP::IElement * pElement);
    void WriteElement(SXP::IOutStream * pOut, SXP::dict & attribs = SXP::blank_dict);
    void PostLoad();
};

#endif
