#ifndef IRC_SOCKET_H
#define IRC_SOCKET_H

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include "mstring.h"

class Magick;
class handle_input_MO;
class send_MO;
class IrcSvcHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM,ACE_MT_SYNCH>
{
    friend handle_input_MO;
    friend send_MO;
private:
    ACE_Activation_Queue activation_queue_;
    int handle_input_i(const mstring& data);
    int send_i(const mstring& data);
public:
    Magick *Parent;
    int close(unsigned long in);
    int send(const mstring& data);
    int open(void *);
    int handle_input(ACE_HANDLE);
    int svc(void);
    int shutdown();
};

typedef ACE_Connector<IrcSvcHandler,ACE_SOCK_CONNECTOR> IrcServer;

#endif