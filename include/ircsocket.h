#ifndef IRC_SOCKET_H
#define IRC_SOCKET_H

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

class IrcSvcHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM,ACE_MT_SYNCH>
{
    typedef ACE_Svc_Handler<ACE_SOCK_STREAM,ACE_MT_SYNCH> inherited;
    // This takes any characters read from the socket that dont
    // end in \r or \n, and adds them to next read's run.
    mstring flack;
public:
    virtual int close(unsigned long in);
    int send(const mstring& data);
    virtual int open(void *);
    virtual int handle_input(ACE_HANDLE handle);
};

typedef ACE_Connector<IrcSvcHandler,ACE_SOCK_CONNECTOR> IrcServer;

class EventTask : public ACE_Task<ACE_MT_SYNCH>
{
    mDateTime last_expire;
    mDateTime last_save;
    mDateTime last_check;
    mDateTime last_ping;
public:
    void ForceSave() { last_save = mDateTime(0.0); }
    virtual int open(void *in=0);
    virtual int close(unsigned long in);
    virtual int svc(void);
};

#endif
