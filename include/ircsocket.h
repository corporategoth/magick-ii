#ifndef IRC_SOCKET_H
#define IRC_SOCKET_H

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include <ace/Reactor.h>
#include <ace/Svc_Handler.h>
#include <ace/Connector.h>
#include <ace/Synch.h>
#include <ace/SOCK_Connector.h>
#include <ace/Activation_Queue.h>
#include <ace/Method_Object.h>
#include "mstring.h"


class IrcSvcHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM,ACE_MT_SYNCH>
{
private:
    ACE_Activation_Queue activation_queue_;
public:
    int open(void *);
    int handle_input(ACE_HANDLE);
    int handle_input_i(const mstring& data);
    int svc(void);
};

class handle_input_MO : public ACE_Method_Object
{
public:
    handle_input_MO(IrcSvcHandler *parent, const mstring& data);
    virtual int call();
private:
    IrcSvcHandler *i_parent;
    mstring i_data;
};

typedef ACE_Connector<IrcSvcHandler,ACE_SOCK_CONNECTOR> IrcServer;

#endif