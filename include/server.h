// $Id$
//
// Magick IRC Services
// (c) 1997-1999 Preston A. Elder <prez@magick.tm>
// (c) 1998-1999 W. King <ungod@magick.tm>
//
// The above copywright may not be removed under any
// circumstances, however it may be added to if any
// modifications are made to this file.  All modified
// code must be clearly documented and labelled.
//
// ===================================================
#ifndef _server_h
#define _server_h

#include "mstring.h"
#include "variant.h"
#include "base.h"
#include "mstream.h"

class Server
{
    mstring i_Name;
    mstring i_Uplink;
    int i_Hops;
    mstring i_Description;
    double i_Ping;
    double i_Lag;
public:
    Server();
    Server(const Server &in);
    Server(mstring name, int hops, mstring description);
    Server(mstring name, mstring uplink, int hops, mstring description);
    void operator=(const Server &in);
    bool operator==(const Server &in) const;
    bool operator<(const Server &in) const;

    mstring Name();
    mstring Uplink();
    int Hops();
    mstring Description();
    void Ping();
    void Pong();
    double Lag();

    vector<mstring> Downlinks();
    vector<mstring> AllDownlinks();
    
    ~Server();
};

class NetworkServ : public mBase
{
    friend class Magick;
private:

    // Config Entries ...
    mstring names;		// Names of service (space delimited)
    mstring realname;		// 'Real Name' of service

    // Live Flags
    bool messages;		// Wether to process /MSG, /NOTICE.
    bool automation;		// Wether to do automatic tasks.

public:
    mstring getnames() { return names; }
    map<mstring,Server> ServerList;
    bool IsServer(mstring server);

    bool MSG() { return messages; }
    void MSG(bool on) { messages = on; }
    bool AUTO() { return automation; }
    void AUTO(bool on) { automation = on; }

    NetworkServ();
    virtual threadtype_enum Get_TType() const { return tt_ServNet; }
    virtual mstring GetInternalName() const { return "NetworkServ"; }
    void execute(const mstring & message);
    void numeric_execute(const mstring & message);
};

#endif