#ifndef WIN32
#pragma interface
#endif

/* Magick IRC Services
**
** (c) 1997-2002 Preston Elder <prez@magick.tm>
** (c) 1998-2002 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any circumstances,
** however it may be added to if any modifications are made to this
** file.  All modified code must be clearly documented and labelled.
**
** This code is released under the Artistic License v2.0 or better.
** The full text of this license should be contained in a file called
** COPYING distributed with this code.  If this file does not exist,
** it may be viewed here: http://www.magick.tm/m2/license.html
**
** ======================================================================= */
#ifndef _COMMSERV_H
#define _COMMSERV_H
#include "pch.h"
RCSID(commserv_h, "@(#) $Id$");

/* ======================================================================= **
**
** For official changes (by the Magick Development Team),please
** check the ChangeLog* files that come with this distribution.
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** ======================================================================= */

#include "base.h"

class Committee_t : public mUserDef, public SXP::IPersistObj, public ref_class
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

    vector < entlist_t * > members_array;
    vector < entlist_t * > messages_array;

    void defaults();
    static SXP::Tag tag_Committee_t, tag_Name, tag_HeadCom, tag_Head, tag_Description, tag_Email, tag_URL, tag_set_Private,
	tag_set_OpenMemos, tag_set_Secure, tag_lock_Private, tag_lock_OpenMemos, tag_lock_Secure, tag_Members, tag_Messages,
	tag_UserDef, tag_RegTime;
public:
    Committee_t();
    Committee_t(const Committee_t & in) : mUserDef(in), SXP::IPersistObj(in), ref_class()
    {
	*this = in;
    }
    Committee_t(const mstring & name, const mstring & head, const mstring & description);
    Committee_t(const mstring & name, const Committee_t & head, const mstring & description);
    Committee_t(const mstring & name, const mstring & description);

    ~Committee_t()
    {
    }
    Committee_t &operator=(const Committee_t & in);
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

    bool validated() const
    {
	return (i_Name.length() != 0 && i_Name.validated());
    }
    mstring Name() const
    {
	return i_Name;
    }
    mDateTime RegTime() const;
    unsigned long Drop();
    mstring HeadCom() const;
    void HeadCom(const mstring & newhead);
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

    SXP::Tag & GetClassTag() const
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

class CommServ : public mBase, public SXP::IPersistObj
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
    typedef map < mstring, Committee_t * > list_t;

private:

    vector < Committee_t * > c_array;
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
    void AddList(const map_entry < Committee_t > & in) throw(E_CommServ_List)
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
    void AddList(const map_entry < Committee_t > & in)
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
    list_t::const_iterator ListBegin() const
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

    SXP::Tag & GetClassTag() const
    {
	return tag_CommServ;
    }
    void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement);
    void EndElement(SXP::IParser * pIn, SXP::IElement * pElement);
    void WriteElement(SXP::IOutStream * pOut, SXP::dict & attribs = SXP::blank_dict);
    void PostLoad();
};

#endif
