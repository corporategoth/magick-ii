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
#ifndef _SERVMSG_H
#define _SERVMSG_H
#include "pch.h"
RCSID(servmsg_h, "@(#) $Id$");

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

// todo: move this over to a ACE_TASK style architecture
// maybe even use an ACE  message queue for passing data too
// but then again, maybe not.
class ServMsg : public mBase
{
    friend class Magick;

private:
    bool showsync;

    void AddCommands();
    void RemCommands();

public:
    ServMsg();
    ~ServMsg()
    {
    }
    class stats_t
    {
	friend class ServMsg;

	mDateTime i_ClearTime;
	unsigned long i_Global;
	unsigned long i_Ask;
	unsigned long i_Stats;
	unsigned long i_Credits;
	unsigned long i_file_Add;
	unsigned long i_file_Del;
	unsigned long i_file_Send;
	unsigned long i_file_Priv;
	unsigned long i_file_Rename;
	unsigned long i_file_Cancel;

    public:
	stats_t()
	{
	    clear();
	}
	void clear()
	{
	    i_ClearTime = mDateTime::CurrentDateTime();
	    i_Global = i_Ask = i_Stats = i_Credits = i_file_Add = i_file_Del = i_file_Send = i_file_Priv = i_file_Rename =
		i_file_Cancel = 0;
	}
	mDateTime ClearTime() const
	{
	    return i_ClearTime;
	}
	unsigned long Global() const
	{
	    return i_Global;
	}
	unsigned long Ask() const
	{
	    return i_Ask;
	}
	unsigned long Stats() const
	{
	    return i_Stats;
	}
	unsigned long Credits() const
	{
	    return i_Credits;
	}
	unsigned long File_Add() const
	{
	    return i_file_Add;
	}
	unsigned long File_Del() const
	{
	    return i_file_Del;
	}
	unsigned long File_Send() const
	{
	    return i_file_Send;
	}
	unsigned long File_Priv() const
	{
	    return i_file_Priv;
	}
	unsigned long File_Rename() const
	{
	    return i_file_Rename;
	}
	unsigned long File_Cancel() const
	{
	    return i_file_Cancel;
	}
    }
    stats;

    bool ShowSync() const
    {
	return showsync;
    }

    threadtype_enum Get_TType() const
    {
	return tt_OtherServ;
    }
    mstring GetInternalName() const
    {
	return "ServMsg";
    }
    void execute(mstring & source, const mstring & msgtype, const mstring & params);

    static void do_Help(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Credits(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Contrib(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Languages(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_BreakDown(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_BreakDown2(map < mstring, pair < unsigned int, unsigned int > > ServCounts, const mstring & mynick,
			      const mstring & source, const mstring & previndent, const mstring & server);
    static void do_Global(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Ask(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_stats_Nick(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_stats_Channel(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_stats_Oper(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_stats_Other(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_stats_Usage(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_stats_All(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_Stats(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_file_List(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_file_Add(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_file_Del(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_file_Rename(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_file_Priv(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_file_Send(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_file_Dcc(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_file_Cancel(const mstring & mynick, const mstring & source, const mstring & params);
    static void do_file_Lookup(const mstring & mynick, const mstring & source, const mstring & params);
};

#endif
