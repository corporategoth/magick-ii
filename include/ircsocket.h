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
#ifndef _IRCSOCKET_H
#define _IRCSOCKET_H
#include "pch.h"
static const char *ident_ircsocket_h = "@(#) $Id$";
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.35  2000/06/18 12:49:26  prez
** Finished locking, need to do some cleanup, still some small parts
** of magick.cpp/h not locked properly, and need to ensure the case
** is the same every time something is locked/unlocked, but for the
** most part, locks are done, we lock pretty much everything :)
**
** Revision 1.34  2000/05/28 05:05:13  prez
** More makefile stuff ... Now we should work on all platforms.
** Added alot of checking for different .h files, functions, etc.
** So now all #define's are config.h based (also added a default
** windows config.h, which will need to be copied on these systems).
**
** Revision 1.33  2000/05/26 11:21:28  prez
** Implemented HTM (High Traffic Mode) -- Can be used at a later date.
**
** Revision 1.32  2000/05/22 13:00:08  prez
** Updated version.h and some other stuff
**
** Revision 1.31  2000/02/23 12:21:01  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.30  2000/02/15 10:37:47  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */


#include "mstring.h"
#include "datetime.h"

class Reconnect_Handler : public ACE_Event_Handler
{
public:
    virtual int handle_timeout (const ACE_Time_Value &tv, const void *arg);
    mstring Reconnect_Handler::FindNext(mstring server);
};

class KillOnSignon_Handler : public ACE_Event_Handler
{
public:
    virtual int handle_timeout (const ACE_Time_Value &tv, const void *arg);
};

class ToBeSquit_Handler : public ACE_Event_Handler
{
public:
    virtual int handle_timeout (const ACE_Time_Value &tv, const void *arg);
};

class Squit_Handler : public ACE_Event_Handler
{
public:
    virtual int handle_timeout (const ACE_Time_Value &tv, const void *arg);
};

class InFlight_Handler : public ACE_Event_Handler
{
public:
    virtual int handle_timeout (const ACE_Time_Value &tv, const void *arg);
};

class Part_Handler : public ACE_Event_Handler
{
public:
    virtual int handle_timeout (const ACE_Time_Value &tv, const void *arg);
};

class IrcSvcHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM,ACE_MT_SYNCH>
{
    typedef ACE_Svc_Handler<ACE_SOCK_STREAM,ACE_MT_SYNCH> inherited;
    // This takes any characters read from the socket that dont
    // end in \r or \n, and adds them to next read's run.
    mstring flack;

    map<time_t, size_t> traffic;
    mDateTime last_htm_check;
    unsigned short htm_level;
    time_t htm_gap;
    size_t htm_threshold;
public:
    virtual int close(unsigned long in);
    int send(const mstring& data);
    virtual int open(void *);
    virtual int handle_input(ACE_HANDLE handle);

    time_t HTM_Gap();
    unsigned short HTM_Level();
    size_t HTM_Threshold();
    void HTM_Threshold(size_t in);
    void HTM(bool in);
    size_t Average(time_t secs = 0);
};

typedef ACE_Connector<IrcSvcHandler,ACE_SOCK_CONNECTOR> IrcServer;

class EventTask : public ACE_Task<ACE_MT_SYNCH>
{
    mDateTime last_expire;
    mDateTime last_save;
    mDateTime last_check;
    mDateTime last_ping;
public:
    void ForceSave();
    void ForcePing();
    mstring SyncTime();
    virtual int open(void *in=0);
    virtual int close(unsigned long in);
    virtual int svc(void);
};

#endif
