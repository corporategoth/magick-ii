#ifndef IRC_SOCKET_H
#define IRC_SOCKET_H

#include <ace/Task.h>
#include <ace/Activation_Queue.h>
#include <ace/Method_Object.h>
#include "mstring.h"

class IrcSocket : public ACE_Task<ACE_MT_SYNCH>
{
public:
    IrcSocket();
    virtual ~IrcSocket();
    virtual int open(void *);
    virtual int close(unsigned long flags = 0);
    virtual int svc(void);


// todo here the socket stuff which basically splits the data off to the appropriate server
    void process(mstring input);
    void shutdown();

    void process_i(mstring input);
private:
    ACE_Activation_Queue activation_queue;
};

class process_MO : public ACE_Method_Object
{
public:
    process_MO(IrcSocket *socket, mstring input);
    virtual ~process_MO();
    virtual int call();

private:
    mstring input_i;
    IrcSocket *socket_i;
};

class shutdown_MO : public ACE_Method_Object
{
public:
    shutdown_MO();
    virtual ~shutdown_MO();
    virtual int call();
};

#endif