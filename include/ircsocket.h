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
#include "mstring.h"


// change this shit over to ACE_Connector<IrcSvcHandler,ACE_SOCK_CONNECTOR>
class IrcSvcHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM,ACE_MT_SYNCH>
{
public:
    int open(void *);
    int handle_input(ACE_HANDLE);
    int svc(void);
};

typedef ACE_Connector<IrcSvcHandler,ACE_SOCK_CONNECTOR> IrcServer;

#endif