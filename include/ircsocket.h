#ifndef IRC_SOCKET_H
#define IRC_SOCKET_H

#include "mstring.h"

class Reconnect_Handler : public ACE_Event_Handler
{
public:
    virtual int handle_timeout (const ACE_Time_Value &tv, const void *arg);
};

class ServerPing_Handler : public ACE_Event_Handler
{
public:
    virtual int handle_timeout (const ACE_Time_Value &tv, const void *arg);
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

class IrcSvcHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM,ACE_MT_SYNCH>
{
    typedef ACE_Svc_Handler<ACE_SOCK_STREAM,ACE_MT_SYNCH> inherited;
    // This takes any characters read from the socket that dont
    // end in \r or \n, and adds them to next read's run.
    mstring flack;
    ServerPing_Handler sph;
public:
    virtual int close(unsigned long in);
    int send(const mstring& data);
    virtual int open(void *);
    virtual int handle_input(ACE_HANDLE handle);
};

typedef ACE_Connector<IrcSvcHandler,ACE_SOCK_CONNECTOR> IrcServer;

#endif
