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
    long i_Ping;
    long i_Lag;
public:
    Server() {}
    Server(const Server &in) { *this = in; }
    Server(mstring name, int hops, mstring description);
    Server(mstring name, mstring uplink, int hops, mstring description);
    void operator=(const Server &in);
    bool operator==(const Server &in) const
	{ return (i_Name == in.i_Name); }
    bool operator!=(const Server &in) const
	{ return (i_Name == in.i_Name); }
    bool operator<(const Server &in) const
	{ return (i_Name < in.i_Name); }

    mstring Name()		{ return i_Name; }
    mstring Uplink()		{ return i_Uplink; }
    int Hops()			{ return i_Hops; }
    mstring Description()	{ return i_Description; }
    void Ping();
    void Pong();
    double Lag()		{ return i_Lag / 1000.0; }

    vector<mstring> Downlinks();
    vector<mstring> AllDownlinks();
    
    ~Server();
};

class NetworkServ : public mBase
{
    friend class Magick;
private:

public:
    map<mstring,Server> ServerList;
    bool IsServer(mstring server);

    virtual void load_database(void);
    virtual void save_database(void);
    NetworkServ();
    virtual threadtype_enum Get_TType() const { return tt_ServNet; }
    virtual mstring GetInternalName() const { return "NetworkServ"; }
    virtual void execute(const mstring & message);
    void numeric_execute(const mstring & message);
};

#endif