#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
#pragma implementation "version.h"
#endif

/*  Magick IRC Services
**
** (c) 1997-2001 Preston Elder <prez@magick.tm>
** (c) 1998-2001 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any
** circumstances, however it may be added to if any
** modifications are made to this file.  All modified
** code must be clearly documented and labelled.
**
** ========================================================== */
#define RCSID(x,y) const char *rcsid_server_cpp_ ## x () { return y; }
RCSID(server_cpp, "@(#)$Id$");
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.163  2001/04/13 00:46:38  prez
** Fixec channel registering
**
** Revision 1.162  2001/04/05 05:59:51  prez
** Turned off -fno-default-inline, and split up server.cpp, it should
** compile again with no special options, and have default inlines :)
**
** Revision 1.161  2001/04/02 02:11:23  prez
** Fixed up some inlining, and added better excption handling
**
** Revision 1.160  2001/03/27 16:09:43  prez
** Fixed chanserv internal maps problem (inserted with incorrect case)
**
** Revision 1.159  2001/03/27 07:04:32  prez
** All maps have been hidden, and are now only accessable via. access functions.
**
** Revision 1.158  2001/03/20 14:22:15  prez
** Finished phase 1 of efficiancy updates, we now pass mstring/mDateTime's
** by reference all over the place.  Next step is to stop using operator=
** to initialise (ie. use mstring blah(mstring) not mstring blah = mstring).
**
** Revision 1.157  2001/03/03 00:30:34  prez
** Fixed compile error
**
** Revision 1.156  2001/03/02 05:24:42  prez
** HEAPS of modifications, including synching up my own archive.
**
** Revision 1.155  2001/02/11 07:41:28  prez
** Enhansed support for server numerics, specifically for Unreal.
**
** Revision 1.154  2001/02/03 02:21:35  prez
** Loads of changes, including adding ALLOW to ini file, cleaning up
** the includes, RCSID, and much more.  Also cleaned up most warnings.
**
** Revision 1.153  2001/01/16 15:47:40  prez
** Fixed filesys not generating first entry in maps, fixed chanserv level
** changes (could confuse set) and fixed idle times on whois user user
**
** Revision 1.152  2001/01/16 12:47:37  prez
** Fixed mlock setting in live channels (also fixed helpop)
**
** Revision 1.151  2001/01/15 23:31:39  prez
** Added LogChan, HelpOp from helpserv, and changed all string != ""'s to
** !string.empty() to save processing.
**
** Revision 1.150  2001/01/01 05:32:45  prez
** Updated copywrights.  Added 'reversed help' syntax (so ACCESS HELP ==
** HELP ACCESS).
**
** Revision 1.149  2000/12/31 17:54:29  prez
** Added checking to see if 'http://' was entered in the SET URL commands.
**
** Revision 1.148  2000/12/31 12:00:32  prez
** JOIN no longer uses PushMessage
**
** Revision 1.147  2000/12/31 03:11:10  prez
** some changes to the flushing users, etc.
**
** Revision 1.146  2000/12/23 22:22:24  prez
** 'constified' all classes (ie. made all functions that did not need to
** touch another non-const function const themselves, good for data integrity).
**
** Revision 1.145  2000/12/22 03:30:26  prez
** Fixed bug in nickserv ident.
**
** Revision 1.144  2000/12/21 14:18:18  prez
** Fixed AKILL expiry, added limit for chanserv on-join messages and commserv
** logon messages.  Also added ability to clear stats and showing of time
** stats are effective for (ie. time since clear).  Also fixed ordering of
** commands, anything with 2 commands (ie. a space in it) should go before
** anything with 1.
**
** Revision 1.142  2000/12/19 08:29:06  prez
** Added EOB
**
** Revision 1.140  2000/12/12 06:01:59  prez
** Typos
**
** Revision 1.139  2000/12/10 13:06:12  prez
** Ditched alot of the *toa's since mstring can do it internally now.
**
** Revision 1.138  2000/12/10 12:47:36  prez
** Fixed it joining all forbidden channels on startup
**
** Revision 1.137  2000/12/10 12:27:57  prez
** Added SETHOST, and fixed possibility of servers being processed out
** of order (causing a server to be missing!)
**
** Revision 1.136  2000/12/09 10:17:14  prez
** Added +h to unreal IRCD profile, and made variant more efficiant
** string wise by cutting out many operator= calls.
**
** Revision 1.135  2000/10/10 11:47:52  prez
** mstring is re-written totally ... find or occurances
** or something has a problem, but we can debug that :)
**
** Revision 1.134  2000/10/03 05:36:27  prez
** Updated some makefiles, helper stuff, and headers -- nothing
** too earth shattering.
**
** Revision 1.133  2000/09/30 10:48:09  prez
** Some general code cleanups ... got rid of warnings, etc.
**
** Revision 1.132  2000/09/27 11:21:39  prez
** Added a BURST mode ...
**
** Revision 1.131  2000/09/22 12:26:12  prez
** Fixed that pesky bug with chanserv not seeing modes *sigh*
**
** Revision 1.130  2000/09/18 08:17:58  prez
** Intergrated mpatrol into the xml/des sublibs, and did
** some minor fixes as a result of mpatrol.
**
** Revision 1.129  2000/09/13 12:45:34  prez
** Added intergration of mpatrol (memory leak finder).  Default is set OFF,
** must enable with --enable-mpatrol in configure (and have mpatrol in system).
**
** Revision 1.128  2000/09/12 21:17:02  prez
** Added IsLiveAll (IsLive now checks to see if user is SQUIT).
**
** Revision 1.127  2000/09/10 09:53:43  prez
** Added functionality to ensure the order of messages is kept.
**
** Revision 1.126  2000/09/09 02:17:49  prez
** Changed time functions to actuallt accept the source nick as a param
** so that the time values (minutes, etc) can be customized.  Also added
** weeks to the time output.
**
** Revision 1.125  2000/09/07 08:13:17  prez
** Fixed some of the erronous messages (SVSHOST, SQLINE, etc).
** Also added CPU statistics and fixed problem with socket deletions.
**
** Revision 1.124  2000/09/06 11:27:33  prez
** Finished the T_Modify / T_Changing traces, fixed a bug in clone
** adding (was adding clone liimt as the mask length), updated docos
** a little more, and added a response to SIGINT to signon clients.
**
** Revision 1.123  2000/09/02 07:20:46  prez
** Added the DumpB/DumpE functions to all major objects, and put in
** some example T_Modify/T_Changing code in NickServ (set email).
**
** Revision 1.122  2000/08/28 10:51:39  prez
** Changes: Locking mechanism only allows one lock to be set at a time.
** Activation_Queue removed, and use pure message queue now, mBase::init()
** now resets us back to the stage where we havnt started threads, and is
** called each time we re-connect.  handle_close added to ircsvchandler.
** Also added in locking for all accesses of ircsvchandler, and checking
** to ensure it is not null.
**
** Revision 1.121  2000/08/22 08:43:41  prez
** Another re-write of locking stuff -- this time to essentially make all
** locks re-entrant ourselves, without relying on implementations to do it.
** Also stops us setting the same lock twice in the same thread.
**
** Revision 1.120  2000/08/19 14:45:03  prez
** Fixed mode settings upon commitee recognitition syntax checking
**
** Revision 1.119  2000/08/19 10:59:47  prez
** Added delays between nick/channel registering and memo sending,
** Added limit of channels per reg'd nick
** Added setting of user modes when recognized on hard-coded committees
**
** Revision 1.118  2000/08/10 22:44:24  prez
** Added 'binding to IP' options for shell servers, etc.  Also added akick
** triggers for when a user changes their nick and suddenly matches akick.
**
** Revision 1.117  2000/08/06 21:56:14  prez
** Fixed some small problems in akill/clone protection
**
** Revision 1.116  2000/08/06 08:06:41  prez
** Fixed loading of logon messages in committee ..
**
** Revision 1.115  2000/08/06 05:55:55  prez
** Added rudimentary UnderNet 2.8.10 support.
**
** Revision 1.114  2000/08/06 05:27:47  prez
** Fixed akill, and a few other minor bugs.  Also made trace TOTALLY optional,
** and infact disabled by default due to it interfering everywhere.
**
** Revision 1.113  2000/08/03 13:06:32  prez
** Fixed a bunch of stuff in mstring (caused exceptions on FreeBSD machines).
**
** Revision 1.112  2000/08/02 20:08:58  prez
** Minor code cleanups, added ACE installation instructions, updated the
** suggestions file and stopped people doing a whole bunch of stuff to
** forbidden nicknames.
**
** Revision 1.111  2000/07/29 21:58:54  prez
** Fixed XML loading of weird characters ...
** 2 known bugs now, 1) last_seen dates are loaded incorrectly on alot
** of nicknames, which means we expire lots of nicknames.  2) services
** wont rejoin a +i/+k channel when last user exits.
**
** Revision 1.110  2000/07/28 14:49:36  prez
** Ditched the old wx stuff, mconfig now in use, we're now ready to
** release (only got some conversion tests to do).
**
** Revision 1.109  2000/07/21 00:18:50  prez
** Fixed database loading, we can now load AND save databases...
**
** Almost ready to release now :)
**
** Revision 1.108  2000/06/25 11:58:03  prez
** Fixed problem where messages from nickserv about killing user would not
** be sent out (people would not know a nick was forbidden).
**
** Revision 1.107  2000/06/25 07:58:50  prez
** Added Bahamut support, listing of languages, and fixed some minor bugs.
**
** Revision 1.106  2000/06/18 13:31:48  prez
** Fixed the casings, now ALL locks should set 'dynamic' values to the
** same case (which means locks will match eachother, yippee!)
**
** Revision 1.105  2000/06/18 12:49:27  prez
** Finished locking, need to do some cleanup, still some small parts
** of magick.cpp/h not locked properly, and need to ensure the case
** is the same every time something is locked/unlocked, but for the
** most part, locks are done, we lock pretty much everything :)
**
** Revision 1.104  2000/06/12 06:07:51  prez
** Added Usage() functions to get ACCURATE usage stats from various
** parts of services.  However bare in mind DONT use this too much
** as it has to go through every data item to grab the usages.
**
** Revision 1.103  2000/06/11 09:30:21  prez
** Added propper MaxLine length, no more hard-coded constants.
**
** Revision 1.102  2000/06/10 07:01:04  prez
** Fixed a bunch of little bugs ...
**
** Revision 1.101  2000/06/08 13:07:34  prez
** Added Secure Oper and flow control to DCC's.
** Also added DCC list and cancel ability
**
** Revision 1.100  2000/06/06 08:57:58  prez
** Finished off logging in backend processes except conver (which I will
** leave for now).  Also fixed some minor bugs along the way.
**
** Revision 1.99  2000/05/27 07:06:03  prez
** HTM actually does something now ... wooo :)
**
** Revision 1.98  2000/05/22 13:00:10  prez
** Updated version.h and some other stuff
**
** Revision 1.97  2000/05/21 04:49:40  prez
** Removed all wxLog tags, now totally using our own logging.
**
** Revision 1.96  2000/05/20 03:28:12  prez
** Implemented transaction based tracing (now tracing wont dump its output
** until logical 'transactions' are done, which are ended by the thread
** being re-attached to another type, ending, or an explicit FLUSH() call).
**
** Revision 1.95  2000/05/19 10:48:15  prez
** Finalized the DCC Sending (now uses the Action map properly)
**
** Revision 1.94  2000/05/18 10:13:15  prez
** Finished off the mFile structure, and the DCC system, it all works.
**
** Revision 1.93  2000/05/10 11:47:00  prez
** added back memo timers
**
** Revision 1.92  2000/04/30 03:48:30  prez
** Replaced all system calls with ACE_OS equivilants,
** also removed any dependancy on ACE from sxp (xml)
**
** Revision 1.91  2000/04/23 02:31:50  prez
** *** empty log message ***
**
** Revision 1.90  2000/04/18 14:34:23  prez
** Fixed the HELP system, it now loads into memory, and can be unloaded
** with the OS unload command.  The stats however are inaccurate.
**
** Revision 1.89  2000/04/06 12:52:50  prez
** Various code changes, but mainly added AUTOMAKE/AUTOCONF files :)
**
** Revision 1.88  2000/04/04 03:21:36  prez
** Added support for SVSHOST where applicable.
**
** Revision 1.87  2000/04/04 03:13:51  prez
** Added support for masking hostnames.
**
** Revision 1.86  2000/04/03 09:45:24  prez
** Made use of some config entries that were non-used, and
** removed some redundant ones ...
**
** Revision 1.85  2000/04/02 13:06:04  prez
** Fixed the channel TOPIC and MODE LOCK stuff ...
**
** Also fixed the setting of both on join...
**
** Revision 1.84  2000/03/28 16:20:59  prez
** LOTS of RET() fixes, they should now be safe and not do double
** calculations.  Also a few bug fixes from testing.
**
** Revision 1.83  2000/03/27 21:26:13  prez
** More bug fixes due to testing, also implemented revenge.
**
** Revision 1.82  2000/03/26 14:59:37  prez
** LOADS of bugfixes due to testing in the real-time environment
** Also enabled the SECURE OperServ option in the CFG file.
**
** Revision 1.81  2000/03/24 15:35:18  prez
** Fixed establishment of DCC transfers, and some other misc stuff
** (eg. small bug in trace, etc).  Still will not send or receive
** any data through DCC tho (will time out, but not receive data,
** error 14 - "Bad Access" -- to be investigated).
**
** Revision 1.80  2000/03/19 08:50:56  prez
** More Borlandization -- Added WHAT project, and fixed a bunch
** of minor warnings that appear in borland.
**
** Revision 1.79  2000/03/15 14:42:59  prez
** Added variable AKILL types (including GLINE)
**
** Revision 1.78  2000/03/15 08:23:52  prez
** Added locking stuff for commserv options, and other stuff
**
** Revision 1.77  2000/03/14 15:10:16  prez
** OK -- more stuff with SJOIN/SNICK -- but it WORKS!!!
**
** Revision 1.76  2000/03/14 13:36:46  prez
** Finished P12 compliance (SJOIN, etc).
**
** Revision 1.75  2000/03/14 10:05:17  prez
** Added Protocol class (now we can accept multi IRCD's)
**
** Revision 1.74  2000/03/14 10:02:48  prez
** Added SJOIN and SNICK
**
** Revision 1.73  2000/03/13 09:36:18  prez
** Completed help file, we now have full help text.
**
** Revision 1.72  2000/03/08 23:38:37  prez
** Added LIVE to nickserv/chanserv, added help funcitonality to all other
** services, and a bunch of other small changes (token name changes, etc)
**
** Revision 1.71  2000/02/27 03:58:40  prez
** Fixed the WHAT program, also removed RegEx from Magick.
**
** Revision 1.70  2000/02/23 12:21:04  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.69  2000/02/17 12:55:07  ungod
** still working on borlandization
**
** Revision 1.68  2000/02/16 12:59:40  ungod
** fixing for borland compilability
**
** Revision 1.67  2000/02/15 13:27:04  prez
** *** empty log message ***
**
** Revision 1.66  2000/02/15 10:37:50  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#include "magick.h"
#include "dccengine.h"

#ifndef MAGICK_HAS_EXCEPTIONS
static Server_t GLOB_Server_t;
#endif

void Protocol::SetTokens(const unsigned int type)
{
    FT("Protocol::SetTokens", (type));

    tokens.clear();
    switch (type)
    {
    case 1000: // ircu >= 2.10.x
	tokens["AB"] = "ADMIN";
	tokens["A"]  = "AWAY";
	tokens["CN"] = "CNOTICE";
	tokens["CO"] = "CONNECT";
	tokens["CP"] = "CPRIVMSG";
	tokens["C"]  = "CREATE";
	tokens["DE"] = "DESTRUCT";
	tokens["DS"] = "DESYNCH";
	tokens["EB"] = "END_OF_BURST";
	tokens["EA"] = "EOB_ACK";
	tokens["Y"]  = "ERROR";
	tokens["GL"] = "GLINE";
	tokens["F"]  = "INFO";
	tokens["I"]  = "INVITE";
	tokens["J"]  = "JOIN";
	tokens["K"]  = "KICK";
	tokens["D"]  = "KILL";
	tokens["LI"] = "LINKS";
	tokens["LU"] = "LUSERS";
	tokens["M"]  = "MODE";
	tokens["MO"] = "MOTD";
	tokens["E"]  = "NAMES";
	tokens["N"]  = "NICK";
	tokens["O"]  = "NOTICE";
	tokens["L"]  = "PART";
	tokens["G"]  = "PING";
	tokens["Z"]  = "PONG";
	tokens["P"]  = "PRIVMSG";
	tokens["Q"]  = "QUIT";
	tokens["RI"] = "RPING";
	tokens["RO"] = "RPONG";
	tokens["SE"] = "SETTIME";
	tokens["U"]  = "SILENCE";
	tokens["SQ"] = "SQUIT";
	tokens["R"]  = "STATS";
	tokens["TI"] = "TIME";
	tokens["TR"] = "TOPIC";
	tokens["V"]  = "VERSION";
	tokens["WC"] = "WALLCHOPS";
	tokens["WA"] = "WALLOPS";
	tokens["H"]  = "WHO";
	tokens["W"]  = "WHOIS";
	tokens["X"]  = "WHOWAS";
//	tokens["ACTION"] = CTCP_DELIM_CHAR + "ACTION";
//	tokens["VERSION"] = CTCP_DELIM_CHAR + "VERSION";
//	tokens["PING"] = CTCP_DELIM_CHAR + "PING";
//	tokens["PONG"] = CTCP_DELIM_CHAR + "PONG";
//	tokens["CLIENTINFO"] = CTCP_DELIM_CHAR + "CLIENTINFO";
//	tokens["USERINFO"] = CTCP_DELIM_CHAR + "USERINFO";
//	tokens["TIME"] = CTCP_DELIM_CHAR + "TIME";
//	tokens["ERRMSG"] = CTCP_DELIM_CHAR + "ERRMSG";
	break;

    case 0001: // DAL >= 4.4.15
	tokens["!"] = "PRIVMSG";
	tokens["\\"] = "WHO";
	tokens["#"] = "WHOIS";
	tokens["$"] = "WHOWAS";
	tokens["%"] = "USER";
	tokens["&"] = "NICK";
	tokens["'"] = "SERVER";
	tokens["("] = "LIST";
	tokens[")"] = "TOPIC";
	tokens["*"] = "INVITE";
	tokens["+"] = "VERSION";
	tokens[","] = "QUIT";
	tokens["-"] = "SQUIT";
	tokens["."] = "KILL";
	tokens["/"] = "INFO";
	tokens["0"] = "LINKS";
	tokens["1"] = "SUMMON";
	tokens["2"] = "STATS";
	tokens["3"] = "USERS";
	tokens["4"] = "HELP";
	tokens["5"] = "ERROR";
	tokens["6"] = "AWAY";
	tokens["7"] = "CONNECT";
	tokens["8"] = "PING";
	tokens["9"] = "PONG";
	tokens[";"] = "OPER";
	tokens["<"] = "PASS";
	tokens[">"] = "TIME";
	tokens["="] = "WALLOPS";
	tokens["?"] = "NAMES";
	tokens["@"] = "ADMIN";
	tokens["B"] = "NOTICE";
	tokens["C"] = "JOIN";
	tokens["D"] = "PART";
	tokens["E"] = "LUSERS";
	tokens["F"] = "MOTD";
	tokens["G"] = "MODE";
	tokens["H"] = "KICK";
	tokens["I"] = "SERVICE";
	tokens["J"] = "USERHOST";
	tokens["K"] = "ISON";
	tokens["L"] = "SQUERY";
	tokens["M"] = "SERVLIST";
	tokens["N"] = "SERVSET";
	tokens["O"] = "REHASH";
	tokens["P"] = "RESTART";
	tokens["Q"] = "CLOSE";
	tokens["R"] = "DIE";
	tokens["S"] = "HASH";
	tokens["T"] = "DNS";
	tokens["U"] = "SILENCE";
	tokens["V"] = "AKILL";
	tokens["W"] = "KLINE";
	tokens["X"] = "UNKLINE";
	tokens["Y"] = "RAKILL";
	tokens["Z"] = "GNOTICE";
	tokens["["] = "GOPER";
	tokens["]"] = "GLOBOPS";
	tokens["^"] = "LOCOPS";
	tokens["_"] = "PROTOCTL";
	tokens["`"] = "WATCH";
	tokens["b"] = "TRACE";
	tokens["c"] = "SQLINE";
	tokens["d"] = "UNSQLINE";
	tokens["e"] = "SVSNICK";
	tokens["f"] = "SVSNOOP";
	tokens["g"] = "PRIVMSG NickServ :IDENTIFY";
	tokens["h"] = "SVSKILL";
	tokens["i"] = "PRIVMSG NickServ";
	tokens["j"] = "PRIVMSG ChanServ";
	tokens["k"] = "PRIVMSG OperServ";
	tokens["l"] = "PRIVMSG MemoServ";
	tokens["m"] = "SERVICES";
	tokens["n"] = "SVSMODE";
	tokens["o"] = "SAMODE";
	tokens["p"] = "CHATOPS";
	tokens["q"] = "ZLINE";
	tokens["r"] = "UNZLINE";
	tokens["s"] = "PRIVMSG HelpServ";
	break;

    case 0002: // Relic >= 2.1
	SetTokens(0001);
	tokens.erase("1");
	tokens.erase("3");
	tokens.erase("=");
	tokens["^"] = "WALLOPS";
	tokens["LO"] = "LOCOPS";
	tokens.erase("c");
	tokens.erase("d");
	tokens["QL"] = "SQLINE";
	tokens["Uq"] = "UNSQLINE";
	tokens["o"] = "OMODE";
	tokens.erase("p");
	tokens["s"] = "OPERMOTD";
	tokens["v"] = "RPING";
	tokens["w"] = "RPONG";
	tokens["{"] = "MAP";
	tokens["|"] = "SJOIN";
	tokens["}"] = "SNICK";
	tokens["~"] = "GLINE";
	tokens["y"] = "SETTIME";
	tokens["HM"] = "HTM";
	tokens["z"] = "ADCHAT";
	tokens["Rz"] = "RW";
	break;

    case 0003: // Bahamut
	SetTokens(0001);
	tokens["e"] = "MODNICK";
	break;

    case 0004: // Unreal
	SetTokens(0001);
	tokens["t"] = "RULES";
	tokens["u"] = "MAP";
	tokens["v"] = "SVS2MODE";
	tokens["w"] = "DALINFO";
	tokens["x"] = "ADCHAT";
	tokens["y"] = "MKPASSWD";
	tokens["z"] = "ADDLINE";
	tokens["}"] = "GLINE";
	tokens["~"] = "SJOIN";
	tokens["AA"] = "SETHOST";
	tokens["AB"] = "TECHAT";
	tokens["AC"] = "NACHAT";
	tokens["AD"] = "SETIDENT";
	tokens["AE"] = "SETNAME";
	tokens["AF"] = "LAG";
	tokens["AG"] = "SDESC";
	tokens["AH"] = "PRIVMSG StatServ";
	tokens["AI"] = "KNOCK";
	tokens["AJ"] = "CREDITS";
	tokens["AK"] = "LICENSE";
	tokens["AL"] = "CHGHOST";
	tokens["AM"] = "RPING";
	tokens["AN"] = "RPONG";
	tokens["AO"] = "NETINFO";
	tokens["AP"] = "SENDUMODE";
	tokens["AQ"] = "ADDMOTD";
	tokens["AR"] = "ADDOMOTD";
	tokens["AS"] = "SVSMOTD";
	tokens["AU"] = "SMO";
	tokens["AV"] = "OPERMOTD";
	tokens["AW"] = "TSCTL";
	tokens["AX"] = "SVSJOIN";
	tokens["AY"] = "SVSPART";
	tokens["AZ"] = "CHGIDENT";
	tokens["BA"] = "SWHOIS";
	tokens["BB"] = "SVSO";
	tokens["BC"] = "SVSFLINE";
	tokens["BD"] = "TKL";
	tokens["BE"] = "VHOST";
	tokens["BF"] = "BOTMOTD";
	tokens["BG"] = "REMGLINE";
	tokens["BH"] = "HTM";
	tokens["|"] = "UMODE2";
	tokens["BI"] = "DCCDENY";
	tokens["BJ"] = "UNDCCDENY";
	tokens["BK"] = "CHGNAME";
	tokens["BL"] = "SHUN";
	tokens["BM"] = "CRYPTO";
	tokens["BN"] = "POST";
	tokens["BO"] = "PRIVMSG InfoServ";
	break;
    default:
	tokens.clear();
    }
}

Protocol::Protocol()
    : i_Number(0), i_NickLen(9), i_MaxLine(450), i_Globops(false),
      i_Helpops(false), i_Tokens(false), i_P12(false), i_TSora(false),
      i_Akill(0), i_Signon(0), i_Modes(3), i_ChanModeArg("ovbkl"),
      i_Server("SERVER %s %d :%s"), i_Numeric(0)
{
    NFT("Protocol::Protocol");
    DumpB();
}

void Protocol::Set(const unsigned int in)
{
    FT("Protocol::Set", (in));

    // WE NEVER set 'i_Tokens', thats set with the PROTCTL line.

    DumpB();
    switch (in)
    {
    case 0: // RFC
	i_Signon = 0000;
	i_Akill = 2000;
	SetTokens(0000);
	break;

    case 1: // RFC with TS8
	i_Signon = 0001;
	i_Akill = 2000;
	SetTokens(0000);
	break;

    case 10: // DAL < 4.4.15
	i_NickLen = 32;
	i_Signon = 1000;
	i_Globops = true;
	i_Akill = 1000;
	i_Modes = 4;
	SetTokens(0000);
	break;

    case 11: // DAL >= 4.4.15
	i_NickLen = 32;
	i_Signon = 1001;
	i_Globops = true;
	i_Helpops = true;
	i_Akill = 1000;
	i_Modes = 6;
	i_Protoctl = "PROTOCTL NOQUIT TOKEN SAFELIST";
	i_SVSNICK = "SVSNICK";
	i_SVSMODE = "SVSMODE";
	i_SVSKILL = "SVSKILL";
	i_SVSNOOP = "SVSNOOP";
	i_SQLINE = "SQLINE";
	i_UNSQLINE = "UNSQLINE";
	SetTokens(0001);
	break;

    case 12: // Bahamut
	i_NickLen = 32;
	i_Signon = 2001;
	i_Globops = true;
	i_Helpops = true;
	i_Akill = 1001;
	i_Modes = 6;
	i_TSora = true;
	i_Protoctl = "CAPAB NOQUIT TS3 SSJOIN BURST UNCONNECT";
	i_SVSNICK = "MODNICK";
	i_SVSMODE = "SVSMODE";
	i_SVSKILL = "SVSKILL";
	i_SVSNOOP = "SVSNOOP";
	i_SQLINE = "SQLINE";
	i_UNSQLINE = "UNSQLINE";
	i_Burst = "BURST";
	i_EndBurst = "BURST END";
	SetTokens(0003);
	break;

    case 20: // UnderNet < 2.10.x
	i_Signon = 1000;
	i_Akill = 2000;
	SetTokens(0000);
	break;

    case 21: // UnderNet >= 2.10.x
	i_Signon = 1000;
	i_Akill = 2000;
	i_Server = "SERVER %s %d 0 0 P10 %d :%s";
	i_Numeric = 6;
	i_Burst = "BURST";
	i_EndBurst = "END_OF_BURST";
	SetTokens(1000);
	break;

    case 30: // Hybrid
	i_Signon = 2000;
	SetTokens(0000);
	i_TSora = true;
	i_Protoctl = "CAPAB EX DE";
	break;

    case 40: // Elite
	i_NickLen = 32;
	i_Signon = 1001;
	i_Globops = true;
	i_Helpops = true;
	i_Akill = 1000;
	i_SVSNICK = "SVSNICK";
	i_SVSMODE = "SVSMODE";
	i_SVSKILL = "SVSKILL";
	i_SVSNOOP = "SVSNOOP";
	i_SQLINE = "SQLINE";
	i_UNSQLINE = "UNSQLINE";
	SetTokens(0001);
	break;

    case 50: // Relic 2.0
	i_NickLen = 32;
	i_Globops = true;
	i_Helpops = true;
	i_Signon = 1001;
	i_Akill = 1000;
	i_Modes = 6;
	i_Server = "SERVER %s %d relic2.0 :%s";
	i_Protoctl = "PROTOCTL NOQUIT TOKEN SAFELIST";
	i_SVSNICK = "SVSNICK";
	i_SVSMODE = "SVSMODE";
	i_SVSKILL = "SVSKILL";
	i_SVSNOOP = "SVSNOOP";
	i_SQLINE = "SQLINE";
	i_UNSQLINE = "UNSQLINE";
	SetTokens(0001);
	break;

    case 51: // Relic 2.1
	i_NickLen = 32;
	i_Globops = true;
	i_Helpops = true;
	i_P12 = true;
	i_Signon = 1003;
	i_Akill = 2002;
	i_Modes = 6;
	i_ChanModeArg = "ovbekl";
	i_Server = "SERVER %s %d relic2.1 :%s";
	i_Protoctl = "PROTOCTL NOQUIT TOKEN SAFELIST";
	i_SVSNICK = "SVSNICK";
	i_SVSMODE = "SVSMODE";
	i_SVSKILL = "SVSKILL";
	i_SVSNOOP = "SVSNOOP";
	i_SQLINE = "SQLINE";
	i_UNSQLINE = "UNSQLINE";
	SetTokens(0002);
	break;

    case 53: // Relic 4.0
	i_NickLen = 32;
	i_Signon = 2002;
	i_Globops = true;
	i_Akill = 2002;
	i_Modes = 6;
	i_TSora = true;
	i_Protoctl = "CAPAB NOQUIT TS3 SSJOIN BURST UNCONNECT";
	i_SVSNICK = "MODNICK";
	i_SVSMODE = "SVSMODE";
	i_SVSKILL = "SVSKILL";
	i_SVSNOOP = "SVSNOOP";
	i_SQLINE = "SQLINE";
	i_UNSQLINE = "UNSQLINE";
	i_Burst = "BURST";
	i_EndBurst = "EOB";
	SetTokens(0003);
	break;

    case 60: // Aurora
	i_NickLen = 32;
	i_Signon = 1002;
	i_Globops = true;
	i_Helpops = true;
	i_Akill = 1000;
	i_Modes = 6;
	i_Protoctl = "PROTOCTL NOQUIT TOKEN SAFELIST";
	i_SVSNICK = "SVSNICK";
	i_SVSMODE = "SVSMODE";
	i_SVSKILL = "SVSKILL";
	i_SVSNOOP = "SVSNOOP";
	i_SQLINE = "SQLINE";
	i_UNSQLINE = "UNSQLINE";
	i_SVSHOST = "SVSHOST";
	SetTokens(0001);
	break;

    case 70: // Unreal
	i_NickLen = 32;
	i_Signon = 2003;
	i_Globops = true;
	i_Helpops = true;
	i_Akill = 1000;
	i_Modes = 6;
	i_Protoctl = "PROTOCTL NOQUIT TOKEN NICKv2 SJOIN SJOIN2 UMODE2 SJ3 NS VHP";
	// Check serveropts in s_debug.c for what the letters are
	i_Server = "SERVER %s %d :U2301-CFhiIpnXS-%d %s";
	i_Numeric = 3;
	i_ChanModeArg = "ovbehklLf";
	i_SVSNICK = "SVSNICK";
	i_SVSMODE = "SVSMODE";
	i_SVSKILL = "SVSKILL";
	i_SVSNOOP = "SVSNOOP";
	i_SQLINE = "SQLINE";
	i_UNSQLINE = "UNSQLINE";
	SetTokens(0004);
	break;

    case 80: // UltimateIRCD
	i_NickLen = 32;
	i_Signon = 1001;
	i_Globops = true;
	i_Helpops = true;
	i_Akill = 1000;
	i_Modes = 6;
	i_Protoctl = "PROTOCTL NOQUIT TOKEN SAFELIST";
	i_SVSNICK = "SVSNICK";
	i_SVSMODE = "SVSMODE";
	i_SVSKILL = "SVSKILL";
	i_SVSNOOP = "SVSNOOP";
	i_SQLINE = "SQLINE";
	i_UNSQLINE = "UNSQLINE";
	SetTokens(0001);
	break;

    default:
	DumpE();
	return;
    }
    i_Number = in;
    DumpE();
}

mstring Protocol::GetToken(const mstring& in) const
{
    FT("Protocol::GetToken", (in));
    map<mstring,mstring>::const_iterator iter = tokens.find(in);
    if (iter == tokens.end())
	RET("");
    RET(iter->second);
}

mstring Protocol::GetNonToken(const mstring& in) const
{
    FT("Protocol::GetNonToken", (in));
    mstring retval;
    map<mstring,mstring>::const_iterator iter;
    for (iter = tokens.begin(); iter != tokens.end(); iter++)
    {
	if (iter->second.IsSameAs(in))
	{
	    retval = iter->first;
	    break;
	}
    }
    RET(retval);
}

void Protocol::DumpB() const
{
    MB(0, (i_Number, i_NickLen, i_MaxLine, i_Globops, i_Helpops,
	i_Tokens, i_P12, i_TSora, i_Akill, i_Signon, i_Modes,
	i_ChanModeArg, i_Server, i_Numeric, i_Burst, i_EndBurst));
    MB(16, (i_Protoctl, i_SVSNICK, i_SVSMODE, i_SVSKILL, i_SVSNOOP,
	i_SQLINE, i_UNSQLINE, i_SVSHOST, tokens.size()));
}

void Protocol::DumpE() const
{
    ME(0, (i_Number, i_NickLen, i_MaxLine, i_Globops, i_Helpops,
	i_Tokens, i_P12, i_TSora, i_Akill, i_Signon, i_Modes,
	i_ChanModeArg, i_Server, i_Numeric, i_Burst, i_EndBurst));
    ME(16, (i_Protoctl, i_SVSNICK, i_SVSMODE, i_SVSKILL, i_SVSNOOP,
	i_SQLINE, i_UNSQLINE, i_SVSHOST, tokens.size()));
}

Server_t::Server_t(const mstring& name, const mstring& description,
	const unsigned long numeric)
    : i_Name(name.LowerCase()), i_AltName(name.LowerCase()), 
      i_Numeric(numeric), i_Uplink(Parent->startup.Server_Name().LowerCase()),
      i_Hops(0), i_Description(description), i_Ping(0), i_Lag(0), i_Jupe(true)
{
    FT("Server_t::Server_t", (name, description, numeric));
    MLOCK(("Server", "list", name.LowerCase()));
    DumpE();
}

Server_t::Server_t(const mstring& name, const int hops, const mstring& description,
	const unsigned long numeric)
    : i_Name(name.LowerCase()), i_AltName(name.LowerCase()), 
      i_Numeric(numeric), i_Uplink(Parent->startup.Server_Name().LowerCase()),
      i_Hops(hops), i_Description(description), i_Ping(0), i_Lag(0), i_Jupe(false)
{
    FT("Server_t::Server_t", (name, hops, description, numeric));
    MLOCK(("Server", "list", name.LowerCase()));
    Parent->server.OurUplink(i_Name);
    DumpE();
}

Server_t::Server_t(const mstring& name, const mstring& uplink, const int hops,
	const mstring& description, const unsigned long numeric)
    : i_Name(name.LowerCase()), i_AltName(name.LowerCase()), 
      i_Numeric(numeric), i_Uplink(uplink.LowerCase()), i_Hops(hops),
      i_Description(description), i_Ping(0), i_Lag(0), i_Jupe(false)
{
    FT("Server::Server", (name, uplink, hops, description, numeric));
    MLOCK(("Server", "list", name.LowerCase()));
    DumpE();
}

void Server_t::operator=(const Server_t &in)
{
    FT("Server_t::operator=", ("(const Server_t &) in"));
    MLOCK(("Server", "list", in.i_Name.LowerCase()));
    i_Name = in.i_Name;
    i_AltName = in.i_AltName;
    i_Numeric = in.i_Numeric;
    i_Uplink = in.i_Uplink;
    i_Hops = in.i_Hops;
    i_Description = in.i_Description;
    i_Ping = in.i_Ping;
    i_Lag = in.i_Lag;
    i_Jupe = in.i_Jupe;
}

mstring Server_t::AltName() const
{
    NFT("Server_t::AltName");
    RLOCK(("Server", "list", i_Name.LowerCase(), "i_AltName"));
    RET(i_AltName);
}

void Server_t::AltName(const mstring& in)
{
    FT("Server_t::AltName", (in));
    WLOCK(("Server", "list", i_Name.LowerCase(), "i_AltName"));
    MCB(i_AltName);
    i_AltName = in;
    MCE(i_AltName);
}

unsigned long Server_t::Numeric() const
{
    NFT("Server_t::Numeric");
    RLOCK(("Server", "list", i_Name.LowerCase(), "i_Numeric"));
    RET(i_Numeric);
}

void Server_t::Numeric(const unsigned long in)
{
    FT("Server_t::Numeric", (in));
    WLOCK(("Server", "list", i_Name.LowerCase(), "i_Numeric"));
    MCB(i_Numeric);
    i_Numeric = in;
    MCE(i_Numeric);
}

mstring Server_t::Uplink() const
{
    NFT("Server_t::Uplink");
    RLOCK(("Server", "list", i_Name.LowerCase(), "i_Uplink"));
    RET(i_Uplink);
}

int Server_t::Hops() const
{
    NFT("Server_t::Hops");
    RLOCK(("Server", "list", i_Name.LowerCase(), "i_Hops"));
    RET(i_Hops);
}

mstring Server_t::Description() const
{
    NFT("Server_t::Description");
    RLOCK(("Server", "list", i_Name.LowerCase(), "i_Description"));
    RET(i_Description);
}

void Server_t::Ping()
{
    NFT("Server_t::Ping");

    WLOCK(("Server", "list", i_Name.LowerCase(), "i_Ping"));
    if (!i_Ping)
    {
	Parent->server.sraw(((Parent->server.proto.Tokens() && !Parent->server.proto.GetNonToken("PING").empty()) ?
		Parent->server.proto.GetNonToken("PING") : mstring("PING")) +
		" " + Parent->startup.Server_Name() + " :" + i_Name);
	MCB(i_Ping);
	i_Ping = ACE_OS::gettimeofday().msec();
	MCE(i_Ping);
    }
}

void Server_t::Pong()
{
    NFT("Server_t::Pong");
    WLOCK(("Server", "list", i_Name.LowerCase(), "i_Ping"));
    if (i_Ping)
    {
	WLOCK2(("Server", "list", i_Name.LowerCase(), "i_Lag"));
	MCB(i_Lag);
	CB(1, i_Ping);
	i_Lag = ACE_OS::gettimeofday().msec() - i_Ping;
	COM(("The lag time of %s is %3.3f seconds.", i_Name.c_str(), i_Lag / 1000.0));
	i_Ping = 0;
	CE(1, i_Ping);
	MCE(i_Lag);
    }
}

float Server_t::Lag() const
{
    NFT("Server_t::Lag");
    RLOCK(("Server", "list", i_Name.LowerCase(), "i_Lag"));
    float retval = i_Lag / 1000.0;
    RET(retval);
}

bool Server_t::Jupe() const
{
    NFT("Server_t::Jupe");
    RLOCK(("Server", "list", i_Name.LowerCase(), "i_Jupe"));
    RET(i_Jupe);
}

unsigned int Server_t::Users() const
{
    NFT("Server_t::Users");

    unsigned int count = 0;
    NickServ::live_t::iterator k;
    RLOCK(("NickServ", "live"));
    for (k=Parent->nickserv.LiveBegin(); k!=Parent->nickserv.LiveEnd(); k++)
	if (k->second.Server() == i_Name)
	    count++;
    RET(count);
}

unsigned int Server_t::Opers() const
{
    NFT("Server_t::Opers");

    unsigned int count = 0;
    NickServ::live_t::iterator k;
    RLOCK(("NickServ", "live"));
    for (k=Parent->nickserv.LiveBegin(); k!=Parent->nickserv.LiveEnd(); k++)
	if (k->second.Server() == i_Name && k->second.HasMode("o"))
	    count++;
    RET(count);
}

vector<mstring> Server_t::Downlinks() const
{
    NFT("Server_t::Downlinks");
    vector<mstring> downlinks;
    Server::list_t::iterator serv;

    RLOCK(("Server", "list"));
    for(serv=Parent->server.ListBegin(); serv!=Parent->server.ListEnd(); serv++)
    {
	if (serv->second.Uplink() == i_Name && !i_Name.empty())
	    downlinks.push_back(serv->first);
    }
    NRET(vector<mstring>, downlinks);
}

vector<mstring> Server_t::AllDownlinks() const
{
    NFT("Server_t::AllDownlinks");
    vector<mstring> downlinks, uplinks, uplinks2;
    Server::list_t::iterator serv;
    bool found = false;

    RLOCK(("Server", "list"));
    for(serv=Parent->server.ListBegin(); serv!=Parent->server.ListEnd(); serv++)
    {
	if (serv->second.Uplink() == i_Name)
	{
	    downlinks.push_back(serv->first);
	    uplinks.push_back(serv->first);
	    found = true;
	}
    }

    while (found)
    {
	found = false;
	for (unsigned int i=0; i<uplinks.size(); i++) 
	{
	    for(serv=Parent->server.ListBegin(); serv!=Parent->server.ListEnd(); serv++)
	    {
		if (serv->second.Uplink() == uplinks[i])
		{
		    downlinks.push_back(serv->first);
		    uplinks2.push_back(serv->first);
		    found = true;
		}
	    }
	}
	uplinks.clear();
	uplinks = uplinks2;
	uplinks2.clear();
    }

    NRET(vector<mstring>, downlinks);
}

Server_t::~Server_t()
{
    NFT("Server_t::~Server_t");

    WLOCK(("Server", "list", i_Name.LowerCase()));
    if (Parent->Shutdown())
	return;

    // Take my sublinks with me (who will take theirs ...)
    vector<mstring> Kill = Downlinks();
    COM(("Destroying %d more servers", Kill.size()));
    for (unsigned int i=0; i<Kill.size(); i++)
	Parent->server.RemList(Kill[i]);
    if (Kill.size() && Parent->server.OurUplink() == i_Name)
	Parent->server.OurUplink("");
}

size_t Server_t::Usage() const
{
    size_t retval = 0;

    WLOCK(("Server", "list", i_Name.LowerCase()));
    retval += i_Name.capacity();
    retval += i_AltName.capacity();
    retval += i_Uplink.capacity();
    retval += sizeof(i_Hops);
    retval += i_Description.capacity();
    retval += sizeof(i_Ping);
    retval += sizeof(i_Lag);
    retval += sizeof(i_Jupe);

    return retval;
}

void Server_t::DumpB() const
{
    MB(0, (i_Name, i_AltName, i_Numeric, i_Uplink, i_Hops, i_Description,
	i_Ping, i_Lag, i_Jupe));
}

void Server_t::DumpE() const
{
    ME(0, (i_Name, i_AltName, i_Numeric, i_Uplink, i_Hops, i_Description,
	i_Ping, i_Lag, i_Jupe));
}


void Server::raw(const mstring& text) const
{
    FT("Server::raw", (text));
    Parent->send(text);
}

void Server::sraw(const mstring& text) const
{
    mstring out;
//  if (proto.Numeric())
//	out << "@" << base64_to_str(GetOurNumeric()) << " ";
//  else
    if (!proto.Numeric())
	out << ":" << Parent->startup.Server_Name() << " ";
    out << text;
    raw(out);
}

void Server::SignOnAll()
{
    NFT("Server::SignOnAll");

    mstring doison;
    unsigned int i;
    WLOCK(("Server", "WaitIsOn"));
    for (i=1; i<=Parent->operserv.GetNames().WordCount(" "); i++)
    {
	if (!Parent->nickserv.IsLive(Parent->operserv.GetNames().ExtractWord(i, " ")))
	{
	    doison += Parent->operserv.GetNames().ExtractWord(i, " ") + " ";
	    WaitIsOn.insert(Parent->operserv.GetNames().ExtractWord(i, " "));
	}
    }
    for (i=1; i<=Parent->nickserv.GetNames().WordCount(" "); i++)
    {
	if (!Parent->nickserv.IsLive(Parent->nickserv.GetNames().ExtractWord(i, " ")))
	{
	    doison += Parent->nickserv.GetNames().ExtractWord(i, " ") + " ";
	    WaitIsOn.insert(Parent->nickserv.GetNames().ExtractWord(i, " "));
	}
    }
    for (i=1; i<=Parent->chanserv.GetNames().WordCount(" "); i++)
    {
	if (!Parent->nickserv.IsLive(Parent->chanserv.GetNames().ExtractWord(i, " ")))
	{
	    doison += Parent->chanserv.GetNames().ExtractWord(i, " ") + " ";
	    WaitIsOn.insert(Parent->chanserv.GetNames().ExtractWord(i, " "));
	}
    }
    for (i=1; i<=Parent->memoserv.GetNames().WordCount(" "); i++)
    {
	if (!Parent->nickserv.IsLive(Parent->memoserv.GetNames().ExtractWord(i, " ")))
	{
	    doison += Parent->memoserv.GetNames().ExtractWord(i, " ") + " ";
	    WaitIsOn.insert(Parent->memoserv.GetNames().ExtractWord(i, " "));
	}
    }
    for (i=1; i<=Parent->commserv.GetNames().WordCount(" "); i++)
    {
	if (!Parent->nickserv.IsLive(Parent->commserv.GetNames().ExtractWord(i, " ")))
	{
	    doison += Parent->commserv.GetNames().ExtractWord(i, " ") + " ";
	    WaitIsOn.insert(Parent->commserv.GetNames().ExtractWord(i, " "));
	}
    }
    for (i=1; i<=Parent->servmsg.GetNames().WordCount(" "); i++)
    {
	if (!Parent->nickserv.IsLive(Parent->servmsg.GetNames().ExtractWord(i, " ")))
	{
	    doison += Parent->servmsg.GetNames().ExtractWord(i, " ") + " ";
	    WaitIsOn.insert(Parent->servmsg.GetNames().ExtractWord(i, " "));
	}
    }

    if (!doison.empty())
	sraw(((proto.Tokens() && !proto.GetNonToken("ISON").empty()) ?
		proto.GetNonToken("ISON") : mstring("ISON")) + " :" +
		doison);
}


Server::Server()
{
    NFT("Server::Server");
    messages=true;
    WLOCK(("Server", "i_UserMax"));
    i_UserMax = 0;
    proc_SERVER = proc_NICK = proc_CHAN = 0;
    DumpE();
}

size_t Server::UserMax() const
{
    NFT("Server::UserMax");
    RLOCK(("Server", "i_UserMax"));
    RET(i_UserMax);
}

void Server::OurUplink(const mstring& server)
{
    FT("Server::OurUplink", (server));
    WLOCK(("Server", "i_OurUplink"));
    MCB(i_OurUplink);
    i_OurUplink = server;
    MCE(i_OurUplink);
}

mstring Server::OurUplink() const
{
    NFT("Server::OurUplink");
    RLOCK(("Server", "i_OurUplink"));
    RET(i_OurUplink);
}

void Server::FlushMsgs(const mstring& nick)
{
    FT("Server::FlushMsgs", (nick));

    map<mstring, list<triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> > > >::iterator i;
    list<triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> > >::iterator j;

    // Dont report this, thats the point of the queue...
    if (!Parent->nickserv.IsLive(nick))
	return;

    RLOCK(("Server", "ToBeSent", nick.LowerCase()));
    MCB(ToBeSent.size());
    if ((i = ToBeSent.find(nick.LowerCase())) != ToBeSent.end())
    {
	WLOCK(("Server", "ToBeSent", i->first));
	for (j=i->second.begin(); j!=i->second.end(); j++)
	{
	    if (j->second.SecondsSince() > Parent->config.Squit_Protect())
		continue;

	    switch (j->first)
	    {
	    case t_GLOBOPS:
		GLOBOPS(nick, j->third.first);
		break;
	    case t_HELPOPS:
		HELPOPS(nick, j->third.first);
		break;
	    case t_INVITE:
		INVITE(nick, j->third.first, j->third.second);
		break;
	    case t_KICK:
		KICK(nick, j->third.first, j->third.second, j->third.third);
		break;
	    case t_KILL:
		KILL(nick, j->third.first, j->third.second);
		break;
	    case t_NOTICE:
		NOTICE(nick, j->third.first, j->third.second);
		break;
	    case t_PRIVMSG:
		PRIVMSG(nick, j->third.first, j->third.second);
		break;
	    case t_SQLINE:
		SQLINE(nick, j->third.first, j->third.second);
		break;
	    case t_SVSMODE:
		SVSMODE(nick, j->third.first, j->third.second);
		break;
	    case t_SVSNICK:
		SVSNICK(nick, j->third.first, j->third.second);
		break;
	    case t_SVSKILL:
		SVSKILL(nick, j->third.first, j->third.second);
		break;
	    case t_SVSHOST:
		SVSHOST(nick, j->third.first, j->third.second);
		break;
	    case t_TOPIC:
		TOPIC(nick, j->third.first, j->third.second, j->third.third);
		break;
	    case t_UNSQLINE:
		UNSQLINE(nick, j->third.first);
		break;
	    case t_WALLOPS:
		WALLOPS(nick, j->third.first);
		break;
	    default:
		LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_UNKNOWN"),
			static_cast<int>(j->first), i->first.c_str(), j->third.first.c_str(),
			j->third.second.c_str(), j->third.third.c_str(),
			ToHumanTime(j->second.SecondsSince()).c_str()));
		break;
	    }
	}
	ToBeSent.erase(i->first);
    }
    MCE(ToBeSent.size());
}

void Server::FlushUser(const mstring& nick, const mstring& channel)
{
    FT("Server::FlushUser", (nick, channel));

    map<mstring, list<triplet<mDateTime, mstring, mstring> > >::iterator i;

    list<triplet<mDateTime, mstring, mstring> >::iterator j;
    list<triplet<mDateTime, mstring, mstring> > WorkList, WorkList2;
    vector<mstring> LastProc;

    // Dont report this, thats the point of the queue...
    if (!Parent->nickserv.IsLive(nick) ||
	Parent->nickserv.GetLive(nick).IsServices())
	return;

    WLOCK(("Server", "ToBeDone", nick.LowerCase()));
    MCB(ToBeDone.size());
    if ((i = ToBeDone.find(nick.LowerCase())) != ToBeDone.end())
    {
	WorkList = i->second;
	{ WLOCK(("Server", "ToBeDone"));
	ToBeDone.erase(i);
	}
	for (j=WorkList.begin(); j!=WorkList.end(); j++)
	{
	    if (j->first.SecondsSince() > Parent->config.Squit_Protect())
		continue;

	    // If its not a channel record and we're not doing
	    // a channel 'run', do it -- else if its a channel
	    // run and the this record is for that channel and
	    // the channel exists with the user in it, do it.
	    if (((channel.LowerCase() == j->third)) && ((channel.empty()) ||
		(Parent->chanserv.IsLive(channel) &&
		 Parent->chanserv.GetLive(channel).IsIn(nick))))
	    {
		if (j->second.Contains(" JOIN "))
		{
		    LastProc.push_back(j->second);
		}
		else
		{
		    execute(j->second);
		}
	    }
	    else
	    {
		WorkList2.push_back(*j);
	    }
	}
	if (WorkList2.size())
	{
	    WLOCK(("Server", "ToBeDone"));
	    ToBeDone[nick.LowerCase()] = WorkList2;
	}
	if (LastProc.size())
	{
	    for (unsigned int i=0; i<LastProc.size(); i++)
	    {
		execute(LastProc[i]);
	    }
	}
    }
    MCE(ToBeDone.size());

}

void Server::PushUser(const mstring& nick, const mstring& message,
	const mstring& channel)
{
    FT("Server::PushUser", (nick, message, channel));
    // If the nick is reg'd and either a channel is not specified
    // or the channel exists and the user is in it, just do it,
    // else queue it.
    if (((Parent->nickserv.IsLive(nick))) &&
	((channel.empty()) || (Parent->chanserv.IsLive(channel) &&
	Parent->chanserv.GetLive(channel).IsIn(nick))))
    {
	execute(message);
    }
    else
    {
	WLOCK(("Server", "ToBeDone", nick.LowerCase()));
	WLOCK2(("Server", "ToBeDone"));
	MCB(ToBeDone.size());
	ToBeDone[nick.LowerCase()].push_back(
		triplet<mDateTime, mstring, mstring>(
		mDateTime::CurrentDateTime(), message, channel.LowerCase()));
	MCE(ToBeDone.size());
    }
}

void Server::PopUser(const mstring& nick, const mstring& channel)
{
    FT("Server::PopUser", (nick, channel));

    map<mstring, list<triplet<mDateTime, mstring, mstring> > >::iterator i;

    list<triplet<mDateTime, mstring, mstring> >::iterator j;
    list<triplet<mDateTime, mstring, mstring> > WorkList, WorkList2;

    WLOCK(("Server", "ToBeDone", nick.LowerCase()));
    MCB(ToBeDone.size());
    if ((i = ToBeDone.find(nick.LowerCase())) != ToBeDone.end())
    {
	WorkList = i->second;
	{ WLOCK(("Server", "ToBeDone"));
	ToBeDone.erase(i->first);
	}
	for (j=WorkList.begin(); j!=WorkList.end(); j++)
	{
	    if (j->third != channel.LowerCase())
		WorkList2.push_back(*j);
	}
	if (WorkList2.size())
	{
	    WLOCK(("Server", "ToBeDone"));
	    ToBeDone[nick.LowerCase()] = WorkList2;
	}
    }
    MCE(ToBeDone.size());
}


#ifdef MAGICK_HAS_EXCEPTIONS
void Server::AddList(Server_t *in) throw(E_Server_List)
#else
void Server::AddList(Server_t *in)
#endif
{
    FT("Server::AddList", ("(Server_t *) in"));

    if (in == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_Server_List(E_Server_List::W_Add, E_Server_List::T_Invalid));
#else
	LOG((LM_CRITICAL, "Exception - Server:List:Add:Invalid"));
	return;
#endif
    }

    if (in->Name().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_Server_List(E_Server_List::W_Add, E_Server_List::T_Blank));
#else
	LOG((LM_CRITICAL, "Exception - Server:List:Add:Blank"));
	return;
#endif
    }

    WLOCK(("Server", "list"));
    // i_list[in->Name().LowerCase()] = in;
    i_list[in->Name().LowerCase()] = *in;
}


#ifdef MAGICK_HAS_EXCEPTIONS
Server_t &Server::GetList(const mstring &in) const throw(E_Server_List)
#else
Server_t &Server::GetList(const mstring &in) const
#endif
{
    FT("Server::GetList", (in));

    RLOCK(("Server", "list", in.LowerCase()));
    Server::list_t::const_iterator iter = i_list.find(in.LowerCase());
    if (iter == i_list.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_Server_List(E_Server_List::W_Get, E_Server_List::T_NotFound, in.c_str()));
#else
	LOG((LM_EMERGENCY, "Exception - Server:List:Get:NotFound - %s", in.c_str()));
	NRET(Server_t &, GLOB_Server_t);
#endif
    }
//  if (iter->second == NULL)
//  {
#ifdef MAGICK_HAS_EXCEPTIONS
//	throw(E_Server_List(E_Server_List::W_Get, E_Server_List::T_Invalid, in.c_str()));
#else
//	LOG((LM_EMERGENCY, "Exception - Server:List:Get:Invalid - %s", in.c_str()));
//	NRET(Server_t &, GLOB_Server_t);
#endif
//  }
//  if (iter->second->Name().empty())
    if (iter->second.Name().empty())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_Server_List(E_Server_List::W_Get, E_Server_List::T_Blank, in.c_str()));
#else
	LOG((LM_EMERGENCY, "Exception - Server:List:Get:Blank - %s", in.c_str()));
	NRET(Server_t &, GLOB_Server_t);
#endif
    }

    // NRET(Server_t &, const_cast<Server_t &>(*iter->second));
    NRET(Server_t &, const_cast<Server_t &>(iter->second));
}


#ifdef MAGICK_HAS_EXCEPTIONS
void Server::RemList(const mstring &in) throw(E_Server_List)
#else
void Server::RemList(const mstring &in)
#endif
{
    FT("Server::RemList", (in));

    WLOCK(("Server", "list"));
    Server::list_t::iterator iter = i_list.find(in.LowerCase());
    if (iter == i_list.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_Server_List(E_Server_List::W_Rem, E_Server_List::T_NotFound, in.c_str()));
#else
	LOG((LM_CRITICAL, "Exception - Server:List:Rem:NotFound - %s", in.c_str()));
	return;
#endif
    }
    WLOCK2(("Server", "list", iter->first));
//  if (iter->second != NULL)
//  {
//	delete iter->second;
//  }
    i_list.erase(iter);
}


bool Server::IsList(const mstring& server) const
{
    FT("Server::IsList", (server));
    RLOCK(("Server", "list"));
    bool retval = (i_list.find(server.LowerCase()) != i_list.end());
    RET(retval);
}


mstring Server::ServerNumeric(const unsigned long num) const
{
    FT("Server::ServerNumeric", (num));
    mstring retval;
    RLOCK(("Server", "list"));
    Server::list_t::const_iterator iter;
    for (iter=ListBegin(); iter!=ListEnd(); iter++)
	if (iter->second.Numeric() == num)
	{
	    retval = iter->second.Name();
	    break;
	}
    RET(retval);
}


mstring Server::GetServer(const mstring& server) const
{
    FT("Server::GetServer", (server));
    mstring retval;
    RLOCK(("Server", "list"));
    if (proto.Numeric())
    {
	retval = ServerNumeric(str_to_base64(server));
	if (retval.empty() && IsList(server))
	    retval = server;
    }
    else if (IsList(server))
	retval = server;
    RET(retval);
}


unsigned long Server::GetOurNumeric() const
{
    NFT("Server::GetOurNumeric");
    unsigned long retval = Parent->startup.Server(Parent->CurrentServer()).second.third;
    RET(retval);
}


bool Server::IsSquit(const mstring& server) const
{
    FT("Server::IsSquit", (server));
    RLOCK(("Server", "ServerSquit"));
    bool retval = (ServerSquit.find(server.LowerCase()) != ServerSquit.end());
    RET(retval);
}

void Server::Jupe(const mstring& server, const mstring& reason)
{
    FT("Server::Jupe", (server, reason));
    if (IsList(server))
	raw(((proto.Tokens() && !proto.GetNonToken("SQUIT").empty()) ?
		proto.GetNonToken("SQUIT") : mstring("SQUIT")) +
		" " + server.LowerCase() + " :JUPE command used.");
	    // SERVER downlink hops :description
	    // :uplink SERVER downlink hops :description
    mstring tmp;
    tmp.Format(proto.Server().c_str(),
	    server.LowerCase().c_str(), 2,
	    ("JUPED (" + reason + ")").c_str());
    raw(tmp);
    Server_t jupe(server.LowerCase(), "JUPED (" + reason + ")");
    Parent->server.AddList(&jupe);
}

void Server::AKILL(const mstring& host, const mstring& reason,
	const unsigned long time, const mstring& killer)
{
    FT("Server::AKILL", (host, reason, time, killer));

    if (!host.Contains("@"))
	return;

    mstring line;
    switch (proto.Akill())
    {
    case 0000:
	break;
    case 1000:
	if (proto.Tokens() && !proto.GetNonToken("AKILL").empty())
	    line << proto.GetNonToken("AKILL");
	else
	    line << "AKILL";
	line << " " << host.After("@") << " " << host.Before("@") <<
		" :" << reason;
	break;
    case 1001:
	if (proto.Tokens() && !proto.GetNonToken("AKILL").empty())
	    line << proto.GetNonToken("AKILL");
	else
	    line << "AKILL";
	line << " " << host.After("@") << " " << host.Before("@") <<
		" " << time << " " << ((!killer.empty()) ? killer :
		Parent->operserv.FirstName()) << " " <<
		static_cast<time_t>(mDateTime::CurrentDateTime()) <<
		" :" << reason;
	break;
    case 2000:
	if (proto.Tokens() && !proto.GetNonToken("GLINE").empty())
	    line << proto.GetNonToken("GLINE");
	else
	    line << "GLINE";
	line << " * +" << time << " " << host << " :" << reason;
	break;
    case 2001:
	if (proto.Tokens() && !proto.GetNonToken("GLINE").empty())
	    line << proto.GetNonToken("GLINE");
	else
	    line << "GLINE";
	line << " * +" << host << " " << time << " :" << reason;
	break;
    case 2002:
	if (proto.Tokens() && !proto.GetNonToken("GLINE").empty())
	    line << proto.GetNonToken("GLINE");
	else
	    line << "GLINE";
	line << " +" << host << " " << time << " :" << reason;
	break;
    }

    // GLINING clients do this for us ...
    if (proto.Akill() < 2000)
    {
	NickServ::live_t::iterator nlive;
	vector<mstring> killusers;
	{ RLOCK(("NickServ", "live"));
	for (nlive = Parent->nickserv.LiveBegin(); nlive != Parent->nickserv.LiveEnd(); nlive++)
	{
	    if (nlive->second.Mask(Nick_Live_t::U_P_H).After("!").Matches(host, true))
		killusers.push_back(nlive->first);
	}}

	unsigned int j;
	for (j=0; j<killusers.size(); j++)
	    Parent->server.KILL(killer, killusers[j], reason);
    }

    if (!line.empty())
	sraw(line);
}

void Server::ANONKILL(const mstring& nick, const mstring& dest,
	const mstring& reason)
{
    FT("Server::ANONKILL", (nick, dest, reason));

    if (!Parent->nickserv.IsLive(nick))
    {
	WLOCK(("Server", "ToBeSent", nick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[nick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_KILL, mDateTime::CurrentDateTime(), triplet<mstring, mstring, mstring>(
		dest, reason, "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Parent->nickserv.GetLive(nick).IsServices())
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"KILL", nick.c_str()));
    }
    else if (!Parent->nickserv.IsLive(dest))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONUSER"),
		"KILL", nick.c_str(), dest.c_str()));
    }
    else
    {
	Parent->nickserv.GetLive(dest).Quit(
		"Killed (" + reason + ")");
	WLOCK(("NickServ", "live"));
	Parent->nickserv.RemLive(dest);
	raw(":" + nick + " " +
		((proto.Tokens() && !proto.GetNonToken("KILL").empty()) ?
			proto.GetNonToken("KILL") : mstring("KILL")) +
		" " + dest + " :" + reason);
    }
}


void Server::AWAY(const mstring& nick, const mstring& reason)
{
    FT("Server::AWAY", (nick, reason));

    if (!Parent->nickserv.IsLive(nick))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONUSER"),
		"AWAY", nick.c_str()));
    }
    else if (!Parent->nickserv.GetLive(nick).IsServices())
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"AWAY", nick.c_str()));
    }
    else
    {
	mstring line;
	Parent->nickserv.GetLive(nick).Away(reason);
	if (proto.Tokens() && !proto.GetNonToken("AWAY").empty())
	    line << proto.GetNonToken("AWAY");
	else
	    line << "AWAY";
	if(!reason.empty())
	    line=" : " + reason;
	raw(":" + nick + " " +  line);
    }
}


void Server::GLOBOPS(const mstring& nick, const mstring& message)
{
    FT("Server::GLOBOPS", (nick, message));

    if (!Parent->nickserv.IsLive(nick))
    {
	WLOCK(("Server", "ToBeSent", nick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[nick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_GLOBOPS, mDateTime::CurrentDateTime(), triplet<mstring, mstring, mstring>(
		message, "", "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Parent->nickserv.GetLive(nick).IsServices())
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"GLOBOPS", nick.c_str()));
    }
    else
    {
	Parent->nickserv.GetLive(nick).Action();
	if (proto.Globops())
	    raw(":" + nick + " " +
		((proto.Tokens() && !proto.GetNonToken("GLOBOPS").empty()) ?
			proto.GetNonToken("GLOBOPS") : mstring("GLOBOPS")) +
		" :" + message);
	else
	    raw(":" + nick + " " +
		((proto.Tokens() && !proto.GetNonToken("WALLOPS").empty()) ?
			proto.GetNonToken("WALLOPS") : mstring("WALLOPS")) +
		" :" + message);
    }
}


void Server::HELPOPS(const mstring& nick, const mstring& message)
{
    FT("Server::HELPOPS", (nick, message));

    if (!Parent->nickserv.IsLive(nick))
    {
	WLOCK(("Server", "ToBeSent", nick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[nick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_HELPOPS, mDateTime::CurrentDateTime(), triplet<mstring, mstring, mstring>(
		message, "", "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Parent->nickserv.GetLive(nick).IsServices())
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"HELPOPS", nick.c_str()));
    }
    else
    {
	Parent->nickserv.GetLive(nick).Action();
	if (proto.Helpops())
	    raw(":" + nick + " " +
		((proto.Tokens() && !proto.GetNonToken("HELPOP").empty()) ?
			proto.GetNonToken("HELPOP") : mstring("HELPOP")) +
		" :" + message);
    }
}


void Server::INVITE(const mstring& nick, const mstring& dest,
	const mstring& channel)
{
    FT("Server::INVITE", (nick, dest, channel));

    if (!Parent->nickserv.IsLive(nick))
    {
	WLOCK(("Server", "ToBeSent", nick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[nick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_INVITE, mDateTime::CurrentDateTime(), triplet<mstring, mstring, mstring>(
		dest, channel, "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Parent->nickserv.GetLive(nick).IsServices())
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"INVITE", nick.c_str()));
    }
    else if (!Parent->nickserv.IsLive(dest))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONUSER"),
		"INVITE", nick.c_str(), dest.c_str()));
    }
    else if (!Parent->chanserv.IsLive(channel))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONCHAN"),
		"INVITE", nick.c_str(), channel.c_str()));
    }
    else
    {
	raw(":" + nick + " " +
		((proto.Tokens() && !proto.GetNonToken("INVITE").empty()) ?
			proto.GetNonToken("INVITE") : mstring("INVITE")) +
		" " + dest + " :" + channel);
    }
}


void Server::JOIN(const mstring& nick, const mstring& channel)
{
    FT("Server::JOIN", (nick, channel));

    if (!Parent->nickserv.IsLive(nick))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONUSER"),
		"JOIN", nick.c_str()));
    }
    else if (!Parent->nickserv.GetLive(nick).IsServices())
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"JOIN", nick.c_str()));
    }
    else
    {
	for (unsigned int i=0; i<channel.WordCount(", "); i++)
	    Parent->nickserv.GetLive(nick).Join(channel.ExtractWord(i+1, ", "));
	raw(":" + nick + " " +
		((proto.Tokens() && !proto.GetNonToken("JOIN").empty()) ?
			proto.GetNonToken("JOIN") : mstring("JOIN")) +
		" :" + channel);
    }
}


void Server::KICK(const mstring& nick, const mstring& dest,
	const mstring& channel, const mstring& reason)
{
    FT("Server::KICK", (nick, dest, channel, reason));

    if (!Parent->nickserv.IsLive(nick))
    {
	WLOCK(("Server", "ToBeSent", nick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[nick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_KICK, mDateTime::CurrentDateTime(), triplet<mstring, mstring, mstring>(
		dest, channel, reason)));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Parent->nickserv.GetLive(nick).IsServices())
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"KICK", nick.c_str()));
    }
    else if (!Parent->nickserv.IsLive(dest))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONUSER"),
		"KICK", nick.c_str(), dest.c_str()));
    }
    else if (!Parent->chanserv.IsLive(channel))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONCHAN"),
		"KICK", nick.c_str(), channel.c_str()));
    }
    else if (!Parent->chanserv.GetLive(channel).IsIn(dest))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_NOTINCHAN"),
		"KICK", nick.c_str(), dest.c_str(), channel.c_str()));
    }
    else
    {
	Parent->nickserv.GetLive(dest).Kick(nick, channel);
	raw(":" + nick + " " +
		((proto.Tokens() && !proto.GetNonToken("KICK").empty()) ?
			proto.GetNonToken("KICK") : mstring("KICK")) +
		" " + channel + " " + dest + " :" + reason);
    }
}


void Server::KILL(const mstring& nick, const mstring& dest,
	const mstring& reason)
{
    FT("Server::KILL", (nick, dest, reason));

    if (!Parent->nickserv.IsLive(nick))
    {
	WLOCK(("Server", "ToBeSent", nick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[nick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_KILL, mDateTime::CurrentDateTime(), triplet<mstring, mstring, mstring>(
		dest, reason, "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Parent->nickserv.GetLive(nick).IsServices())
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"KILL", nick.c_str()));
    }
    else if (!Parent->nickserv.IsLive(dest))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONUSER"),
		"KILL", nick.c_str(), dest.c_str()));
    }
    else
    {
	WLOCK(("NickServ", "live"));
	Parent->nickserv.GetLive(dest).Quit(
		"Killed (" + nick + " (" + reason + "))");
	Parent->nickserv.RemLive(dest);
	raw(":" + nick + " " +
		((proto.Tokens() && !proto.GetNonToken("KILL").empty()) ?
			proto.GetNonToken("KILL") : mstring("KILL")) +
		" " + dest + " :" + Parent->nickserv.GetLive(nick).Host() +
		"!" + nick + " (" + reason + ")");
    }
}


void Server::MODE(const mstring& nick, const mstring& mode)
{
    FT("Server::MODE", (nick, mode));

    if (!Parent->nickserv.IsLive(nick))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONUSER"),
		"MODE", nick.c_str()));
    }
    else if (!Parent->nickserv.GetLive(nick).IsServices())
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"MODE", nick.c_str()));
    }
    else
    {
	Parent->nickserv.GetLive(nick).Mode(mode);
	raw(":" + nick + " " +
		((proto.Tokens() && !proto.GetNonToken("MODE").empty()) ?
			proto.GetNonToken("MODE") : mstring("MODE")) +
		" " + nick + " :" + mode);
    }
}


void Server::MODE(const mstring& nick, const mstring& channel,
	const mstring& mode) 
{
    FT("Server::MODE", (nick, channel, mode));

    if (!Parent->nickserv.IsLive(nick))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONUSER"),
		"MODE", nick.c_str()));
    }
    else if (!Parent->nickserv.GetLive(nick).IsServices())
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"MODE", nick.c_str()));
    }
    else if (!Parent->chanserv.IsLive(channel))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONCHAN"),
		"MODE", nick.c_str(), channel.c_str()));
    }
    else
    {
	Parent->chanserv.GetLive(channel).Mode(nick, mode);
	raw(":" + nick + " " +
		((proto.Tokens() && !proto.GetNonToken("MODE").empty()) ?
			proto.GetNonToken("MODE") : mstring("MODE")) +
		" " + channel + " " + mode.Before(" ") + " " +
		mode.After(" "));
    }
}


void Server::NICK(const mstring& nick, const mstring& user,
	const mstring& host, const mstring& server, const mstring& realname)
{
    FT("Server::NICK", (nick, user, host, server, realname));

    if (Parent->nickserv.IsLive(nick))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_TOUSER"),
		"NICK", server.c_str(), nick.c_str()));
    }
    else
    {
	mstring send, token;
	switch (proto.Signon())
	{
	case 0000:
	    token = "USER";
	    if (proto.Tokens() && !proto.GetNonToken(token).empty())
		send << proto.GetNonToken(token);
	    else
		send << token;
	    send << " " << nick  << " " << user << " " << host <<
		" " << server << " :" << realname;
	    break;
	case 0001:
	    token = "USER";
	    if (proto.Tokens() && !proto.GetNonToken(token).empty())
		send << proto.GetNonToken(token);
	    else
		send << token;
	    send << " " << nick  << mDateTime::CurrentDateTime().timetstring() <<
		" " << user << " " << host << " " << server <<
		" :" << realname;
	    break;
	case 1000:
	    token = "NICK";
	    if (proto.P12())
		token = "SNICK";
	    if (proto.Tokens() && !proto.GetNonToken(token).empty())
		send << proto.GetNonToken(token);
	    else
		send << token;
	    send << " " << nick << " 1 " << mDateTime::CurrentDateTime().timetstring() <<
		" " << user << " " << host << " " << server;
	    if (proto.P12())
		send << " +" << Parent->startup.Setmode();
	    send << " :" << realname;
	    break;
	case 1001:
	    token = "NICK";
	    if (proto.P12())
		token = "SNICK";
	    if (proto.Tokens() && !proto.GetNonToken(token).empty())
		send << proto.GetNonToken(token);
	    else
		send << token;
	    send << " " << nick << " 1 " << mDateTime::CurrentDateTime().timetstring() <<
		" " << user << " " << host << " " << server << " 1";
	    if (proto.P12())
		send << " +" << Parent->startup.Setmode();
	    send << " :" << realname;
	    break;
	case 1002:
	    token = "NICK";
	    if (proto.P12())
		token = "SNICK";
	    if (proto.Tokens() && !proto.GetNonToken(token).empty())
		send << proto.GetNonToken(token);
	    else
		send << token;
	    send << " " << nick << " 1 " << mDateTime::CurrentDateTime().timetstring() <<
		" " << user << " " << host << " " << server << " 1 "
		<< host;
	    if (proto.P12())
		send << " +" << Parent->startup.Setmode();
	    send << " :" << realname;
	    break;
	case 1003:
	    token = "NICK";
	    if (proto.P12())
		token = "SNICK";
	    if (proto.Tokens() && !proto.GetNonToken(token).empty())
		send << proto.GetNonToken(token);
	    else
		send << token;
	    send << " " << nick << " 1 " << mDateTime::CurrentDateTime().timetstring() <<
		" " << user << " " << host << " " << host << " " <<
		server << " 1";
	    if (proto.P12())
		send << " +" << Parent->startup.Setmode();
	    send << " :" << realname;
	    break;
	case 2000:
	    token = "NICK";
	    if (proto.P12())
		token = "SNICK";
	    if (proto.Tokens() && !proto.GetNonToken(token).empty())
		send << proto.GetNonToken(token);
	    else
		send << token;
	    send << " " << nick << " 1 " << mDateTime::CurrentDateTime().timetstring() <<
		" +" << Parent->startup.Setmode() << " " << user <<
		" " << host << " " << server << " :" << realname;
	    break;
	case 2001:
	    token = "NICK";
	    if (proto.P12())
		token = "SNICK";
	    if (proto.Tokens() && !proto.GetNonToken(token).empty())
		send << proto.GetNonToken(token);
	    else
		send << token;
	    send << " " << nick << " 1 " << mDateTime::CurrentDateTime().timetstring() <<
		" +" << Parent->startup.Setmode() << " " << user <<
		" " << host << " " << server << " 1 :" << realname;
	    break;
	case 2002:
	    token = "NICK";
	    if (proto.P12())
		token = "SNICK";
	    if (proto.Tokens() && !proto.GetNonToken(token).empty())
		send << proto.GetNonToken(token);
	    else
		send << token;
	    send << " " << nick << " 1 " << mDateTime::CurrentDateTime().timetstring() <<
		" +" << Parent->startup.Setmode() << " " << user <<
		" " << host << " " << host << " " << server << " 1 :" <<
		realname;
	    break;
	case 2003:
	    token = "NICK";
	    if (proto.P12())
		token = "SNICK";
	    if (proto.Tokens() && !proto.GetNonToken(token).empty())
		send << proto.GetNonToken(token);
	    else
		send << token;
	    send << " " << nick << " 1 " << mDateTime::CurrentDateTime().timetstring() <<
		" +" << user << " " << host << " " << server << " 1 +" <<
		Parent->startup.Setmode() << " " << host << " :" << realname;
	    break;
	}
	// Sign ourselves in ...

	if (Parent->nickserv.IsLiveAll(nick.LowerCase()))
	{
	    Parent->nickserv.GetLive(nick).Quit("SQUIT - " + Parent->nickserv.GetLive(nick).Server());
	    Parent->nickserv.RemLive(nick);
	}
	Nick_Live_t tmp(nick, user, host, realname);
	if (proto.P12() || (proto.Signon() >= 2000 && proto.Signon() < 3000))
		tmp.Mode(Parent->startup.Setmode());
	Parent->nickserv.AddLive(&tmp);
	{ WLOCK2(("Server", "i_UserMax"));
	if (i_UserMax < Parent->nickserv.LiveSize())
	{
	    MCB(i_UserMax);
	    i_UserMax = Parent->nickserv.LiveSize();
	    MCE(i_UserMax);
	}}
	raw(send);
    }
}


void Server::NICK(const mstring& oldnick, const mstring& newnick)
{
    FT("Server::NICK", (oldnick, newnick));

    if (!Parent->nickserv.IsLive(oldnick))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONUSER"),
		"NICK", oldnick.c_str()));
    }
    else if (!Parent->nickserv.GetLive(oldnick).IsServices())
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"NICK", oldnick.c_str()));
    }
    else
    {
	Nick_Live_t tmp(Parent->nickserv.GetLive(oldnick));
	Parent->nickserv.RemLive(oldnick);
	tmp.Name(newnick);
	if (Parent->nickserv.IsLiveAll(newnick))
	{
	    Parent->nickserv.GetLive(newnick).Quit("SQUIT - " + Parent->nickserv.GetLive(newnick).Server());
	    Parent->nickserv.RemLive(newnick);
	}
	Parent->nickserv.AddLive(&tmp);
	raw(":" + oldnick + " " +
		((proto.Tokens() && !proto.GetNonToken("NICK").empty()) ?
			proto.GetNonToken("NICK") : mstring("NICK")) +
		" " + newnick + (proto.TSora() ?
			" :" + mDateTime::CurrentDateTime().timetstring() :
			mstring("")));
    }
}


void Server::NOTICE(const mstring& nick, const mstring& dest,
	const mstring& text)
{
    FT("Server::NOTICE", (nick, dest, text));

    if (!Parent->nickserv.IsLive(nick))
    {
	WLOCK(("Server", "ToBeSent", nick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[nick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_NOTICE, mDateTime::CurrentDateTime(), triplet<mstring, mstring, mstring>(
		dest, text, "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Parent->nickserv.GetLive(nick).IsServices())
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"NOTICE", nick.c_str()));
    }
    else if (!(dest[0u] == '$' || dest[0u] == '#' ||
		Parent->nickserv.IsLive(dest) ||
		Parent->chanserv.IsLive(dest)))
    {
	if (IsChan(dest))
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONCHAN"),
		"NOTICE", nick.c_str(), dest.c_str()));
	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONUSER"),
		"NOTICE", nick.c_str(), dest.c_str()));
	}
    }
    else
    {
	Parent->nickserv.GetLive(nick).Action();
	raw(":" + nick + " " +
		((proto.Tokens() && !proto.GetNonToken("NOTICE").empty()) ?
			proto.GetNonToken("NOTICE") : mstring("NOTICE")) +
		" " + dest + " :" + text);
    }
}


void Server::PART(const mstring& nick, const mstring& channel,
	const mstring& reason)
{
    FT("Server::PART", (nick, channel, reason));

    if (!Parent->nickserv.IsLive(nick))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONUSER"),
		"PART", nick.c_str()));
    }
    else if (!Parent->nickserv.GetLive(nick).IsServices())
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"PART", nick.c_str()));
    }
    else if (!Parent->chanserv.IsLive(channel))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONCHAN"),
		"PART", nick.c_str(), channel.c_str()));
    }
    else if (!Parent->chanserv.GetLive(channel).IsIn(nick))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_NOTINCHAN"),
		"KICK", nick.c_str(), nick.c_str(), channel.c_str()));
    }
    else
    {
	Parent->nickserv.GetLive(nick).Part(channel);
	mstring tmpResult;
	if(!reason.empty())
	    tmpResult=" :"+reason;
	else
	    tmpResult="";
	raw(":" + nick + " " +
		((proto.Tokens() && !proto.GetNonToken("PART").empty()) ?
			proto.GetNonToken("PART") : mstring("PART")) +
		" " + channel + tmpResult);
    }
}


void Server::PRIVMSG(const mstring& nick, const mstring& dest,
	const mstring& text)
{
    FT("Server::PRIVMSG", (nick, dest, text));

    if (!Parent->nickserv.IsLive(nick))
    {
	WLOCK(("Server", "ToBeSent", nick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[nick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_PRIVMSG, mDateTime::CurrentDateTime(), triplet<mstring, mstring, mstring>(
		dest, text, "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Parent->nickserv.GetLive(nick).IsServices())
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"PRIVMSG", nick.c_str()));
    }
    else if (!(dest[0u] == '$' || dest[0u] == '#' ||
		Parent->nickserv.IsLive(dest) ||
		Parent->chanserv.IsLive(dest)))
    {
	if (IsChan(dest))
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONCHAN"),
		"PRIVMSG", nick.c_str(), dest.c_str()));
	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONUSER"),
		"PRIVMSG", nick.c_str(), dest.c_str()));
	}
    }
    else
    {
	Parent->nickserv.GetLive(nick).Action();
	raw(":" + nick + " " +
		((proto.Tokens() && !proto.GetNonToken("PRIVMSG").empty()) ?
			proto.GetNonToken("PRIVMSG") : mstring("PRIVMSG")) +
		" " + dest + " :" + text);
    }
}


void Server::SQLINE(const mstring& nick, const mstring& target,
	const mstring& reason)
{
    FT("Server::SQLINE", (nick, target,reason));

    if (proto.SQLINE().empty())
	return;

    if (!Parent->nickserv.IsLive(nick))
    {
	WLOCK(("Server", "ToBeSent", nick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[nick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_SQLINE, mDateTime::CurrentDateTime(), triplet<mstring, mstring, mstring>(
		target, reason, "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Parent->nickserv.GetLive(nick).IsServices())
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		proto.SQLINE().c_str(), nick.c_str()));
    }
    else
    {
	mstring output;
	output << ":" << nick << " ";
	if (proto.Tokens() && !proto.GetNonToken(proto.SQLINE()).empty())
	    output << proto.GetNonToken(proto.SQLINE());
	else
	    output << proto.SQLINE();
	output << " " << target << " :" << reason;
	raw(output);
    }
}


void Server::QUIT(const mstring& nick, const mstring& reason)
{
    FT("Server::QUIT", (nick, reason));

    if (!Parent->nickserv.IsLive(nick))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONUSER"),
		"QUIT", nick.c_str()));
    }
    else if (!Parent->nickserv.GetLive(nick).IsServices())
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"QUIT", nick.c_str()));
    }
    else
    {
	Parent->nickserv.GetLive(nick).Quit(reason);
	WLOCK(("NickServ", "live"));
	Parent->nickserv.RemLive(nick);
	raw(":" + nick + " " +
		((proto.Tokens() && !proto.GetNonToken("QUIT").empty()) ?
			proto.GetNonToken("QUIT") : mstring("QUIT")) +
		" :" + reason);
    }
}


void Server::RAKILL(const mstring& host)
{
    FT("Server::RAKILL", (host));

    if (!host.Contains("@"))
	return;

    mstring line;
    switch (proto.Akill())
    {
    case 0000:
	break;
    case 1000:
	if (proto.Tokens() && !proto.GetNonToken("RAKILL").empty())
	    line << proto.GetNonToken("RAKILL");
	else
	    line << "RAKILL";
	line << " " << host.After("@") << " " << host.Before("@");
	break;
    case 1001:
	if (proto.Tokens() && !proto.GetNonToken("RAKILL").empty())
	    line << proto.GetNonToken("RAKILL");
	else
	    line << "RAKILL";
	line << " " << host.After("@") << " " << host.Before("@");
	break;
    case 2000:
	if (proto.Tokens() && !proto.GetNonToken("UNGLINE").empty())
	    line << proto.GetNonToken("UNGLINE");
	else
	    line << "UNGLINE";
	line << " * " << host;
	break;
    case 2001:
	if (proto.Tokens() && !proto.GetNonToken("GLINE").empty())
	    line << proto.GetNonToken("GLINE");
	else
	    line << "GLINE";
	line << " * -" << host;
	break;
    case 2002:
	if (proto.Tokens() && !proto.GetNonToken("GLINE").empty())
	    line << proto.GetNonToken("GLINE");
	else
	    line << "GLINE";
	line << " -" << host;
	break;
    }
    if (!line.empty())
	sraw(line);
}

void Server::SVSHOST(const mstring& mynick, const mstring& nick,
	const mstring& newhost)
{
    FT("Server::SVSHOST", (mynick, nick, newhost));

    if (proto.SVSHOST().empty())
	return;

    if (!Parent->nickserv.IsLive(mynick))
    {
	WLOCK(("Server", "ToBeSent", mynick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[mynick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_SVSHOST, mDateTime::CurrentDateTime(), triplet<mstring, mstring, mstring>(
		nick, newhost, "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Parent->nickserv.GetLive(mynick).IsServices())
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		proto.SVSHOST().c_str(), nick.c_str()));
    }
    else if (!Parent->nickserv.IsLive(nick))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONUSER"),
		proto.SVSHOST().c_str(), mynick.c_str(), nick.c_str()));
    }
    else
    {
	Parent->nickserv.GetLive(nick).AltHost(newhost);
	mstring output;
	output << ":" << mynick << " ";
	if (proto.Tokens() && !proto.GetNonToken(proto.SVSHOST()).empty())
	    output << proto.GetNonToken(proto.SVSHOST());
	else
	    output << proto.SVSHOST();
	output << " " << nick << " " << newhost << " :" << mDateTime::CurrentDateTime().timetstring();
	raw(output);
    }
}


void Server::SVSKILL(const mstring& mynick, const mstring& nick,
	const mstring& reason)
{
    FT("Server::SVSKILL", (mynick, nick, reason));

    if (proto.SVSKILL().empty())
	return;

    if (!Parent->nickserv.IsLive(mynick))
    {
	WLOCK(("Server", "ToBeSent", mynick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[mynick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_SVSKILL, mDateTime::CurrentDateTime(), triplet<mstring, mstring, mstring>(
		nick, reason, "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Parent->nickserv.GetLive(mynick).IsServices())
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		proto.SVSKILL().c_str(), nick.c_str()));
    }
    else if (!Parent->nickserv.IsLive(nick))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONUSER"),
		proto.SVSKILL().c_str(), mynick.c_str(), nick.c_str()));
    }
    else
    {
	Parent->nickserv.GetLive(nick).Quit(reason);
	WLOCK(("NickServ", "live"));
	Parent->nickserv.RemLive(nick);
	mstring output;
	output << ":" << mynick << " ";
	if (proto.Tokens() && !proto.GetNonToken(proto.SVSKILL()).empty())
	    output << proto.GetNonToken(proto.SVSKILL());
	else
	    output << proto.SVSKILL();
	output << " " << nick << " " << reason;
	raw(output);
    }
}


void Server::SVSNICK(const mstring& mynick, const mstring& nick,
	const mstring& newnick)
{
    FT("Server::SVSNICK", (mynick, nick, newnick));

    if (proto.SVSNICK().empty())
	return;

    if (!Parent->nickserv.IsLive(mynick))
    {
	WLOCK(("Server", "ToBeSent", mynick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[mynick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_SVSNICK, mDateTime::CurrentDateTime(), triplet<mstring, mstring, mstring>(
		nick, newnick, "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Parent->nickserv.GetLive(mynick).IsServices())
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		proto.SVSNICK().c_str(), nick.c_str()));
    }
    else if (!Parent->nickserv.IsLive(nick))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONUSER"),
		proto.SVSNICK().c_str(), mynick.c_str(), nick.c_str()));
    }
    else if (Parent->nickserv.IsLive(newnick))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_TOUSER"),
		proto.SVSNICK().c_str(), mynick.c_str(), newnick.c_str()));
    }
    else
    {
	mstring output;
	output << ":" << mynick << " ";
	if (proto.Tokens() && !proto.GetNonToken(proto.SVSNICK()).empty())
	    output << proto.GetNonToken(proto.SVSNICK());
	else
	    output << proto.SVSNICK();
	output << " " << nick << " " <<
		    newnick << " :" << time_t(NULL);
	raw(output);
    }
}


void Server::SVSNOOP(const mstring& nick, const mstring& server,
	const bool onoff)
{
    FT("Server::SVSNOOP", (nick, server, onoff));

    if (proto.SVSNOOP().empty())
	return;

    if (!Parent->nickserv.IsLive(nick))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONUSER"),
		proto.SVSNOOP().c_str(), nick.c_str()));
    }
    else if (!Parent->nickserv.GetLive(nick).IsServices())
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		proto.SVSNOOP().c_str(), nick.c_str()));
    }
    else if (!IsList(server))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONSERVER"),
		proto.SVSNOOP().c_str(), nick.c_str(), server.c_str()));
    }
    else
    {
	mstring output;
	output << ":" << nick << " ";
	if (proto.Tokens() && !proto.GetNonToken(proto.SVSNOOP()).empty())
	    output << proto.GetNonToken(proto.SVSNOOP());
	else
	    output << proto.SVSNOOP();
	output << " " + server + " " + mstring(onoff ? "+" : "-");
	raw(output);
    }
}


void Server::SVSMODE(const mstring& mynick, const mstring& nick,
	const mstring& mode)
{
    FT("Server::SVSMODE", (mynick, nick, mode));

    if (proto.SVSMODE().empty())
	return;

    if (!Parent->nickserv.IsLive(mynick))
    {
	WLOCK(("Server", "ToBeSent", mynick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[mynick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_SVSMODE, mDateTime::CurrentDateTime(), triplet<mstring, mstring, mstring>(
		nick, mode, "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Parent->nickserv.GetLive(mynick).IsServices())
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		proto.SVSMODE().c_str(), nick.c_str()));
    }
    else if (!Parent->nickserv.IsLive(nick))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONUSER"),
		proto.SVSMODE().c_str(), mynick.c_str(), nick.c_str()));
    }
    else
    {
	Parent->nickserv.GetLive(nick).Mode(mode);
	mstring output;
	output << ":" << mynick << " ";
	if (proto.Tokens() && !proto.GetNonToken(proto.SVSMODE()).empty())
	    output << proto.GetNonToken(proto.SVSMODE());
	else
	    output << proto.SVSMODE();
	output << " " << nick << " " << mode;
	raw(output);
    }
}


void Server::TOPIC(const mstring& nick, const mstring& setter,
	const mstring& channel, const mstring& topic, const mDateTime& time)
{
    FT("Server::TOPIC", (nick, setter, channel, topic, time));

    
    if (!Parent->nickserv.IsLive(nick))
    {
	WLOCK(("Server", "ToBeSent", nick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[nick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_TOPIC, mDateTime::CurrentDateTime(), triplet<mstring, mstring, mstring>(
		setter, channel, topic)));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Parent->nickserv.GetLive(nick).IsServices())
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"TOPIC", nick.c_str()));
    }
    else if (!Parent->chanserv.IsLive(channel))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONCHAN"),
		"TOPIC", nick.c_str(), channel.c_str()));
    }
    else
    {
	CP(("%s is %s", nick.c_str(),
		Parent->nickserv.GetLive(nick).Mask(Nick_Live_t::N_U_P_H).c_str()));
	mstring send;
	send << ":" << nick << " ";
	if (proto.Tokens() && !proto.GetNonToken("TOPIC").empty())
	    send << proto.GetNonToken("TOPIC");
	else
	    send << "TOPIC";
	if (topic.empty())
	    send << " " << channel << " " << setter;
	else
	    send << " " << channel << " " <<
		setter << " " << time.timetstring() << " :" << topic;

	Parent->chanserv.GetLive(channel).Topic(nick, topic, setter, time);
	raw(send);
    }
}


void Server::UNSQLINE(const mstring& nick, const mstring& target)
{
    FT("Server::UNSQLINE", (nick, target));

    if (!proto.UNSQLINE().empty())
	return;

    if (!Parent->nickserv.IsLive(nick))
    {
	WLOCK(("Server", "ToBeSent", nick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[nick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_UNSQLINE, mDateTime::CurrentDateTime(), triplet<mstring, mstring, mstring>(
		target, "", "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Parent->nickserv.GetLive(nick).IsServices())
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		proto.UNSQLINE().c_str(), nick.c_str()));
    }
    else
    {
	mstring output;
	output << ":" << nick << " ";
	if (proto.Tokens() && !proto.GetNonToken(proto.UNSQLINE()).empty())
	    output << proto.GetNonToken(proto.UNSQLINE());
	else
	    output << proto.UNSQLINE();
	output << " " << target;
	raw(output);
    }
}


void Server::WALLOPS(const mstring& nick, const mstring& message)
{
    FT("Server::WALLOPS", (nick, message));

    if (!Parent->nickserv.IsLive(nick))
    {
	WLOCK(("Server", "ToBeSent", nick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[nick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_WALLOPS, mDateTime::CurrentDateTime(), triplet<mstring, mstring, mstring>(
		message, "", "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Parent->nickserv.GetLive(nick).IsServices())
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"WALLOPS", nick.c_str()));
    }
    else
    {
	Parent->nickserv.GetLive(nick).Action();
	raw(":" + nick + " " +
		((proto.Tokens() && !proto.GetNonToken("WALLOPS").empty()) ?
			proto.GetNonToken("WALLOPS") : mstring("WALLOPS")) +
		" :" + message);
    }
}


void Server::KillUnknownUser(const mstring& user) const
{
    FT("Server::KillUnknownUser", (user));
    sraw(((proto.Tokens() && !proto.GetNonToken("KILL").empty()) ?
		proto.GetNonToken("KILL") : mstring("KILL")) +
		" " + user + " :" + Parent->startup.Server_Name() +
		" (" + user + "(?) <- " + Parent->CurrentServer() + ")");
    LOG((LM_ERROR, Parent->getLogMessage("OTHER/KILL_UNKNOWN"),
	user.c_str()));
}


unsigned int Server::SeenMessage(const mstring& data)
{
    FT("Server::SeenMessage", (data));

    map<mstring, pair<unsigned int, mDateTime> >::iterator iter;
    vector<mstring> chunked;
    unsigned int times = 0;

    WLOCK(("Server", "ReDoMessages"));
    for (iter=ReDoMessages.begin(); iter!=ReDoMessages.end(); iter++)
    {
	if (iter->second.second.SecondsSince() > Parent->config.MSG_Seen_Time())
	    chunked.push_back(iter->first);
    }
    for (unsigned int i=0; i<chunked.size(); i++)
	ReDoMessages.erase(chunked[i]);

    if (ReDoMessages.find(data) != ReDoMessages.end())
    {
	times = ReDoMessages[data].first;
	times++;
    }

    ReDoMessages[data] = pair<unsigned int,mDateTime>(times, mDateTime::CurrentDateTime());

    RET(times);
}

void Server::execute(const mstring & data)
{
    //okay this is the main networkserv command switcher
    FT("Server::execute", (data));

    mstring msgtype;
    if (data[0u] == ':' || data[0u] == '@')
        msgtype = data.ExtractWord(2,": ").UpperCase();
    else
        msgtype = data.ExtractWord(1,": ").UpperCase();

    // Message names direct from RFC1459, with DAL4.4.15+
    // extensions.  Will add to for other ircd's.
    switch (msgtype[0U])
    {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
	numeric_execute(data);
	break;
    case 'A':
	parse_A(data);
	break;
    case 'B':
	parse_B(data);
	break;
    case 'C':
	parse_C(data);
	break;
    case 'D':
	parse_D(data);
	break;
    case 'E':
	parse_E(data);
	break;
    case 'F':
	parse_F(data);
	break;
    case 'G':
	parse_G(data);
	break;
    case 'H':
	parse_H(data);
	break;
    case 'I':
	parse_I(data);
	break;
    case 'J':
	parse_J(data);
	break;
    case 'K':
	parse_K(data);
	break;
    case 'L':
	parse_L(data);
	break;
    case 'M':
	parse_M(data);
	break;
    case 'N':
	parse_N(data);
	break;
    case 'O':
	parse_O(data);
	break;
    case 'P':
	parse_P(data);
	break;
    case 'Q':
	parse_Q(data);
	break;
    case 'R':
	parse_R(data);
	break;
    case 'S':
	parse_S(data);
	break;
    case 'T':
	parse_T(data);
	break;
    case 'U':
	parse_U(data);
	break;
    case 'V':
	parse_V(data);
	break;
    case 'W':
	parse_W(data);
	break;
    case 'X':
	parse_X(data);
	break;
    case 'Y':
	parse_Y(data);
	break;
    case 'Z':
	parse_Z(data);
	break;
    default:
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	break;
    }
}

void Server::parse_A(const mstring &data)
{
    FT("Server::parse_A", (data));
    mstring source, msgtype, params;

    if (data[0u] == ':' || data[0u] == '@')
    {
	if (data[0u] == '@' && proto.Numeric())
	{
	    RLOCK(("ServerLinking"));
	    source = GetServer(data.ExtractWord(1, "@ "));
	    for (unsigned int i = 0; source.empty() && i < 10; i++)
	    {
		ACE_OS::sleep(1);
		source = GetServer(data.ExtractWord(1, "@ "));
	    }
	}
	else
	    source = data.ExtractWord(1,": ");
	if (!(source.Contains(".") || Parent->nickserv.IsLive(source)))
	{
	    PushUser(source, data);
	    return;
	}
        msgtype = data.ExtractWord(2,": ").UpperCase();
	if (data.WordCount(" ") > 2)
	    params = data.After(" ", 2);
    }
    else
    {
	source = OurUplink();
        msgtype = data.ExtractWord(1,": ").UpperCase();
	if (data.WordCount(" ") > 1)
	    params = data.After(" ");
    }
    if (source.empty())
	source = Parent->startup.Server_Name();
    mstring sourceL(source.LowerCase());


	if (msgtype=="ADCHAT")
	{
	    // Useless admin chatter.
	}
	else if (msgtype=="ADMIN")
	{
	    { RLOCK(("IrcSvcHandler"));
	    if (Parent->ircsvchandler != NULL &&
		Parent->ircsvchandler->HTM_Level() > 3)
	    {
		mstring tmp;
		tmp.Format(Parent->getMessage(source, "MISC/HTM").c_str(),
								msgtype.c_str());
		raw(((proto.Tokens() && !proto.GetNonToken("NOTICE").empty()) ?
			proto.GetNonToken("NOTICE") : mstring("NOTICE")) +
			" " + source + " :" + tmp.c_str());
		return;
	    }}
	    // :source ADMIN
	    sraw("256 " + source + " :Administrative info about " +
		Parent->startup.Server_Name());
	    sraw("257 " + source + " :" + Parent->startup.Server_Desc());
	    sraw("258 " + source + " :Admins - " + Parent->operserv.Services_Admin());
	    sraw("259 " + source + " :" + FULLNAME + " - " + EMAIL);
	}
	else if (msgtype=="AKILL")
	{
	    // We will ignore AKILLS because they're not relivant to us.
	    // we will not be akilling our own clients ;P
	}
	else if (msgtype=="AWAY")
	{
	    if (source.Contains("."))
		return;

	    RLOCK(("UsingNick", sourceL));
	    // :source AWAY
	    // :source AWAY :This is my reason
	    if (params.empty())
	    {
		Parent->nickserv.GetLive(sourceL).Away("");

		// HAS to be AFTER the nickname is added to map.
		CommServ::list_t::iterator iter;
		{ RLOCK2(("CommServ", "list"));
		for (iter = Parent->commserv.ListBegin();
				    iter != Parent->commserv.ListEnd();
				    iter++)
		{
		    if (iter->second.IsOn(sourceL))
		    {
			MLOCK(("CommServ", "list", iter->first, "message"));
			for (iter->second.message = iter->second.MSG_begin();
			    iter->second.message != iter->second.MSG_end();
			    iter->second.message++)
			{
			    Parent->servmsg.send(source, "[" + IRC_Bold +
					    iter->first + IRC_Off + "] " +
					    iter->second.message->Entry());
			}
		    }
		}}

		if (Parent->nickserv.IsStored(source))
		{
		    mstring who = Parent->nickserv.GetStored(sourceL).Host().LowerCase();
		    if (who.empty())
			who = sourceL;
		    if (Parent->memoserv.IsNick(who))
		    {
			size_t count = Parent->memoserv.NickMemoCount(who);
			if (count)
			    send(Parent->memoserv.FirstName(), source,
				Parent->getMessage(source, "MS_STATUS/NS_UNREAD"),
				count, Parent->memoserv.FirstName().c_str());
		    }
		}
	    }
	    else
		Parent->nickserv.GetLive(sourceL).Away(params.After(":"));
	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
}

void Server::parse_B(const mstring &data)
{
    FT("Server::parse_B", (data));
    mstring source, msgtype, params;

    if (data[0u] == ':' || data[0u] == '@')
    {
	if (data[0u] == '@' && proto.Numeric())
	{
	    RLOCK(("ServerLinking"));
	    source = GetServer(data.ExtractWord(1, "@ "));
	    for (unsigned int i = 0; source.empty() && i < 10; i++)
	    {
		ACE_OS::sleep(1);
		source = GetServer(data.ExtractWord(1, "@ "));
	    }
	}
	else
	    source = data.ExtractWord(1,": ");
	if (!(source.Contains(".") || Parent->nickserv.IsLive(source)))
	{
	    PushUser(source, data);
	    return;
	}
        msgtype = data.ExtractWord(2,": ").UpperCase();
	if (data.WordCount(" ") > 2)
	    params = data.After(" ", 2);
    }
    else
    {
	source = OurUplink();
        msgtype = data.ExtractWord(1,": ").UpperCase();
	if (data.WordCount(" ") > 1)
	    params = data.After(" ");
    }
    if (source.empty())
	source = Parent->startup.Server_Name();
    mstring sourceL(source.LowerCase());


	if (msgtype=="BURST")
	{
	    // Thanks to bahamut :)
	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
}

void Server::parse_C(const mstring &data)
{
    FT("Server::parse_C", (data));
    mstring source, msgtype, params;

    if (data[0u] == ':' || data[0u] == '@')
    {
	if (data[0u] == '@' && proto.Numeric())
	{
	    RLOCK(("ServerLinking"));
	    source = GetServer(data.ExtractWord(1, "@ "));
	    for (unsigned int i = 0; source.empty() && i < 10; i++)
	    {
		ACE_OS::sleep(1);
		source = GetServer(data.ExtractWord(1, "@ "));
	    }
	}
	else
	    source = data.ExtractWord(1,": ");
	if (!(source.Contains(".") || Parent->nickserv.IsLive(source)))
	{
	    PushUser(source, data);
	    return;
	}
        msgtype = data.ExtractWord(2,": ").UpperCase();
	if (data.WordCount(" ") > 2)
	    params = data.After(" ", 2);
    }
    else
    {
	source = OurUplink();
        msgtype = data.ExtractWord(1,": ").UpperCase();
	if (data.WordCount(" ") > 1)
	    params = data.After(" ");
    }
    if (source.empty())
	source = Parent->startup.Server_Name();
    mstring sourceL(source.LowerCase());


	if (msgtype=="CAPAB")
	{
	    // Bahamut version of the PROTOCTL line
	    if ((data + " ").Contains(" TOKEN "))
		proto.Tokens(true);
	}
	else if (msgtype=="CHATOPS")
	{
	}
	else if (msgtype=="CONNECT")
	{
	    // :soul.darker.net 481 ChanServ :Permission Denied- You do not have the correct IRC operator privileges
	    // :source CONNECT some.server port :our.server
	    if (IsList(params.ExtractWord(1, ": ")) ||
	    	params.ExtractWord(1, ": ").LowerCase() == Parent->startup.Server_Name().LowerCase())
	    {
		sraw(((proto.Tokens() && !proto.GetNonToken("NOTICE").empty()) ?
			proto.GetNonToken("NOTICE") : mstring("NOTICE")) +
			" " + source + " :Connect: Server " + params.ExtractWord(1, ": ") +
			" already exists from " + Parent->startup.Server_Name());
	    }
	    else
	    {
		sraw(((proto.Tokens() && !proto.GetNonToken("NOTICE").empty()) ?
			proto.GetNonToken("NOTICE") : mstring("NOTICE")) +
			" " + source + " :Connect: Host " +
			params.ExtractWord(1, ": ") + " not listed in irc.conf");
	    }
	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
}

void Server::parse_D(const mstring &data)
{
    FT("Server::parse_D", (data));
    mstring source, msgtype, params;

    if (data[0u] == ':' || data[0u] == '@')
    {
	if (data[0u] == '@' && proto.Numeric())
	{
	    RLOCK(("ServerLinking"));
	    source = GetServer(data.ExtractWord(1, "@ "));
	    for (unsigned int i = 0; source.empty() && i < 10; i++)
	    {
		ACE_OS::sleep(1);
		source = GetServer(data.ExtractWord(1, "@ "));
	    }
	}
	else
	    source = data.ExtractWord(1,": ");
	if (!(source.Contains(".") || Parent->nickserv.IsLive(source)))
	{
	    PushUser(source, data);
	    return;
	}
        msgtype = data.ExtractWord(2,": ").UpperCase();
	if (data.WordCount(" ") > 2)
	    params = data.After(" ", 2);
    }
    else
    {
	source = OurUplink();
        msgtype = data.ExtractWord(1,": ").UpperCase();
	if (data.WordCount(" ") > 1)
	    params = data.After(" ");
    }
    if (source.empty())
	source = Parent->startup.Server_Name();
    mstring sourceL(source.LowerCase());


	LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
}

void Server::parse_E(const mstring &data)
{
    FT("Server::parse_E", (data));
    mstring source, msgtype, params;

    if (data[0u] == ':' || data[0u] == '@')
    {
	if (data[0u] == '@' && proto.Numeric())
	{
	    RLOCK(("ServerLinking"));
	    source = GetServer(data.ExtractWord(1, "@ "));
	    for (unsigned int i = 0; source.empty() && i < 10; i++)
	    {
		ACE_OS::sleep(1);
		source = GetServer(data.ExtractWord(1, "@ "));
	    }
	}
	else
	    source = data.ExtractWord(1,": ");
	if (!(source.Contains(".") || Parent->nickserv.IsLive(source)))
	{
	    PushUser(source, data);
	    return;
	}
        msgtype = data.ExtractWord(2,": ").UpperCase();
	if (data.WordCount(" ") > 2)
	    params = data.After(" ", 2);
    }
    else
    {
	source = OurUplink();
        msgtype = data.ExtractWord(1,": ").UpperCase();
	if (data.WordCount(" ") > 1)
	    params = data.After(" ");
    }
    if (source.empty())
	source = Parent->startup.Server_Name();
    mstring sourceL(source.LowerCase());


	if (msgtype=="END_OF_BURST" || msgtype=="EOB")
	{
	    // Tis only nice, afterall ...
	    Parent->server.sraw(((proto.Tokens() &&
		!proto.GetNonToken("EOB_ACK").empty()) ?
		proto.GetNonToken("EOB_ACK") :
		mstring("EOB_ACK")));
	}
	else if (msgtype=="EOB_ACK")
	{
	    // Thank you, drive through ...
	}
	else if (msgtype=="ERROR")
	{
	    // ERROR :This is my error
	    LOG((LM_NOTICE, Parent->getLogMessage("OTHER/SERVER_MSG"),
		    msgtype.c_str(), params.After(":").c_str()));
	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
}

void Server::parse_F(const mstring &data)
{
    FT("Server::parse_F", (data));
    mstring source, msgtype, params;

    if (data[0u] == ':' || data[0u] == '@')
    {
	if (data[0u] == '@' && proto.Numeric())
	{
	    RLOCK(("ServerLinking"));
	    source = GetServer(data.ExtractWord(1, "@ "));
	    for (unsigned int i = 0; source.empty() && i < 10; i++)
	    {
		ACE_OS::sleep(1);
		source = GetServer(data.ExtractWord(1, "@ "));
	    }
	}
	else
	    source = data.ExtractWord(1,": ");
	if (!(source.Contains(".") || Parent->nickserv.IsLive(source)))
	{
	    PushUser(source, data);
	    return;
	}
        msgtype = data.ExtractWord(2,": ").UpperCase();
	if (data.WordCount(" ") > 2)
	    params = data.After(" ", 2);
    }
    else
    {
	source = OurUplink();
        msgtype = data.ExtractWord(1,": ").UpperCase();
	if (data.WordCount(" ") > 1)
	    params = data.After(" ");
    }
    if (source.empty())
	source = Parent->startup.Server_Name();
    mstring sourceL(source.LowerCase());


	LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
}

void Server::parse_G(const mstring &data)
{
    FT("Server::parse_G", (data));
    mstring source, msgtype, params;

    if (data[0u] == ':' || data[0u] == '@')
    {
	if (data[0u] == '@' && proto.Numeric())
	{
	    RLOCK(("ServerLinking"));
	    source = GetServer(data.ExtractWord(1, "@ "));
	    for (unsigned int i = 0; source.empty() && i < 10; i++)
	    {
		ACE_OS::sleep(1);
		source = GetServer(data.ExtractWord(1, "@ "));
	    }
	}
	else
	    source = data.ExtractWord(1,": ");
	if (!(source.Contains(".") || Parent->nickserv.IsLive(source)))
	{
	    PushUser(source, data);
	    return;
	}
        msgtype = data.ExtractWord(2,": ").UpperCase();
	if (data.WordCount(" ") > 2)
	    params = data.After(" ", 2);
    }
    else
    {
	source = OurUplink();
        msgtype = data.ExtractWord(1,": ").UpperCase();
	if (data.WordCount(" ") > 1)
	    params = data.After(" ");
    }
    if (source.empty())
	source = Parent->startup.Server_Name();
    mstring sourceL(source.LowerCase());


	if (msgtype=="GLINE")
	{
	    // We will ignore GLINES because they're not relivant to us.
	    // we will not be glining our own clients ;P
	}
	else if (msgtype=="GLOBOPS")
	{
	    // :source GLOBOPS :This message
	    // useless chatter ... can be ignored.
	}
	else if (msgtype=="GNOTICE")
	{
	    // :server GNOTICE :This message
	    LOG((LM_NOTICE, Parent->getLogMessage("OTHER/SERVER_MSG"),
		msgtype.c_str(), params.After(":").c_str()));
	}
	else if (msgtype=="GOPER")
	{
	    // useless chatter ... can be ignored.
	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
}

void Server::parse_H(const mstring &data)
{
    FT("Server::parse_H", (data));
    mstring source, msgtype, params;

    if (data[0u] == ':' || data[0u] == '@')
    {
	if (data[0u] == '@' && proto.Numeric())
	{
	    RLOCK(("ServerLinking"));
	    source = GetServer(data.ExtractWord(1, "@ "));
	    for (unsigned int i = 0; source.empty() && i < 10; i++)
	    {
		ACE_OS::sleep(1);
		source = GetServer(data.ExtractWord(1, "@ "));
	    }
	}
	else
	    source = data.ExtractWord(1,": ");
	if (!(source.Contains(".") || Parent->nickserv.IsLive(source)))
	{
	    PushUser(source, data);
	    return;
	}
        msgtype = data.ExtractWord(2,": ").UpperCase();
	if (data.WordCount(" ") > 2)
	    params = data.After(" ", 2);
    }
    else
    {
	source = OurUplink();
        msgtype = data.ExtractWord(1,": ").UpperCase();
	if (data.WordCount(" ") > 1)
	    params = data.After(" ");
    }
    if (source.empty())
	source = Parent->startup.Server_Name();
    mstring sourceL(source.LowerCase());


	if (msgtype=="HELP")
	{
	    // ignore ...
	}
	else if (msgtype=="HELPOP")
	{
	    // ignore ...
	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
}

void Server::parse_I(const mstring &data)
{
    FT("Server::parse_I", (data));
    mstring source, msgtype, params;

    if (data[0u] == ':' || data[0u] == '@')
    {
	if (data[0u] == '@' && proto.Numeric())
	{
	    RLOCK(("ServerLinking"));
	    source = GetServer(data.ExtractWord(1, "@ "));
	    for (unsigned int i = 0; source.empty() && i < 10; i++)
	    {
		ACE_OS::sleep(1);
		source = GetServer(data.ExtractWord(1, "@ "));
	    }
	}
	else
	    source = data.ExtractWord(1,": ");
	if (!(source.Contains(".") || Parent->nickserv.IsLive(source)))
	{
	    PushUser(source, data);
	    return;
	}
        msgtype = data.ExtractWord(2,": ").UpperCase();
	if (data.WordCount(" ") > 2)
	    params = data.After(" ", 2);
    }
    else
    {
	source = OurUplink();
        msgtype = data.ExtractWord(1,": ").UpperCase();
	if (data.WordCount(" ") > 1)
	    params = data.After(" ");
    }
    if (source.empty())
	source = Parent->startup.Server_Name();
    mstring sourceL(source.LowerCase());


	if (msgtype=="INFO")
	{
	    // :source INFO :server/nick
	    { RLOCK(("IrcSvcHandler"));
	    if (Parent->ircsvchandler != NULL &&
		Parent->ircsvchandler->HTM_Level() > 3)
	    {
		mstring tmp;
		tmp.Format(Parent->getMessage(source, "MISC/HTM").c_str(),
								msgtype.c_str());
		raw(((proto.Tokens() && !proto.GetNonToken("NOTICE").empty()) ?
			proto.GetNonToken("NOTICE") : mstring("NOTICE")) +
			" " + source + " :" + tmp.c_str());
		return;
	    }
	    else
		for (unsigned int i=0; i<sizeof(contrib)/sizeof(mstring); i++)
		    sraw("371 " + source + " :" + contrib[i]);
	    }
	    sraw("374 " + source + " :End of /INFO report");
	}
	else if (msgtype=="INVITE")
	{
	    // :source INVITE target :channel
	    //:PreZ INVITE ChanServ :#chatzone

	    // We can ignore this, as our clients will not 'join on invite'
	    // and we dont really need an invitation to join ...
	}
	else if (msgtype=="ISON")
	{
	    // :heaven.darker.net ISON ChanServ
	    // :soul.darker.net 303 heaven.darker.net :ChanServ
	    // :heaven.darker.net ISON zBlerk
	    // :soul.darker.net 303 heaven.darker.net :

	    // repl: :our.server 303 source :local.nick
	    mstring isonstr;
	    for (unsigned int i=1; i<=params.WordCount(": "); i++)
	    {
		RLOCK(("UsingNick", params.ExtractWord(i, ": ").LowerCase()));
		if (isonstr.length() > proto.MaxLine())
		{
		    sraw("303 " + source + " :" + isonstr);
		    isonstr.erase();
		}
		if (Parent->nickserv.IsLive(params.ExtractWord(i, ": ")))
		    isonstr += params.ExtractWord(i, ": ") + " ";
	    }
	    if (!isonstr.empty())
		sraw("303 " + source + " :" + isonstr);
	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
}

void Server::parse_J(const mstring &data)
{
    FT("Server::parse_J", (data));
    mstring source, msgtype, params;

    if (data[0u] == ':' || data[0u] == '@')
    {
	if (data[0u] == '@' && proto.Numeric())
	{
	    RLOCK(("ServerLinking"));
	    source = GetServer(data.ExtractWord(1, "@ "));
	    for (unsigned int i = 0; source.empty() && i < 10; i++)
	    {
		ACE_OS::sleep(1);
		source = GetServer(data.ExtractWord(1, "@ "));
	    }
	}
	else
	    source = data.ExtractWord(1,": ");
	if (!(source.Contains(".") || Parent->nickserv.IsLive(source)))
	{
	    PushUser(source, data);
	    return;
	}
        msgtype = data.ExtractWord(2,": ").UpperCase();
	if (data.WordCount(" ") > 2)
	    params = data.After(" ", 2);
    }
    else
    {
	source = OurUplink();
        msgtype = data.ExtractWord(1,": ").UpperCase();
	if (data.WordCount(" ") > 1)
	    params = data.After(" ");
    }
    if (source.empty())
	source = Parent->startup.Server_Name();
    mstring sourceL(source.LowerCase());


	if (msgtype=="JOIN")
	{
	    if (source.Contains("."))
		return;

	    RLOCK(("UsingNick", sourceL));
	    // In burst mode, make sure NICK processing is done
	    { RLOCK2(("IrcSvcHandler"));
	      RLOCK3(("Server", "proc_NICK"));
	    if (proc_NICK && Parent->ircsvchandler != NULL &&
		Parent->ircsvchandler->Burst())
	    {
		WLOCK(("WaitEndNickAdd"));
	    }}
	    { WLOCK(("Server", "proc_CHAN")); proc_CHAN++; }
	    RLOCK2(("WaitEndChanAdd"));

	    // :source JOIN :#channel
	    for (unsigned int i=1; i<=params.WordCount(":, "); i++)
	    {
		WLOCK(("UsingChan", params.ExtractWord(i, ":, ").LowerCase()));
		mstring chan(params.ExtractWord(i, ":, "));
		// If we're IN channel, then we may be cycling ...
		if (Parent->chanserv.IsLive(chan) &&
		    Parent->chanserv.GetLive(chan).IsIn(sourceL) &&
		    SeenMessage(data) < Parent->config.MSG_Seen_Act())
		{
		    mBase::push_message(":" + source + " " + msgtype + " " + chan);
		}
		else
		{
		    Parent->nickserv.GetLive(sourceL).Join(chan);
		}
	    }
	    { WLOCK(("Server", "proc_CHAN")); proc_CHAN--; }
	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
}

void Server::parse_K(const mstring &data)
{
    FT("Server::parse_K", (data));
    mstring source, msgtype, params;

    if (data[0u] == ':' || data[0u] == '@')
    {
	if (data[0u] == '@' && proto.Numeric())
	{
	    RLOCK(("ServerLinking"));
	    source = GetServer(data.ExtractWord(1, "@ "));
	    for (unsigned int i = 0; source.empty() && i < 10; i++)
	    {
		ACE_OS::sleep(1);
		source = GetServer(data.ExtractWord(1, "@ "));
	    }
	}
	else
	    source = data.ExtractWord(1,": ");
	if (!(source.Contains(".") || Parent->nickserv.IsLive(source)))
	{
	    PushUser(source, data);
	    return;
	}
        msgtype = data.ExtractWord(2,": ").UpperCase();
	if (data.WordCount(" ") > 2)
	    params = data.After(" ", 2);
    }
    else
    {
	source = OurUplink();
        msgtype = data.ExtractWord(1,": ").UpperCase();
	if (data.WordCount(" ") > 1)
	    params = data.After(" ");
    }
    if (source.empty())
	source = Parent->startup.Server_Name();
    mstring sourceL(source.LowerCase());


	if (msgtype=="KICK")
	{
	    // :source KICK #channel target :reason
	    // KICK #channel target :reason

	    RLOCK(("UsingNick", sourceL));
	    RLOCK2(("UsingNick", params.ExtractWord(2, ": ").LowerCase()));
	    WLOCK(("UsingChan", params.ExtractWord(1, ": ").LowerCase()));
	    // KICK for UNKNOWN USER.
	    if (!Parent->nickserv.IsLive(params.ExtractWord(2, ": ")))
	    {
		KillUnknownUser(params.ExtractWord(2, ": "));
		return;
	    }

	    if (!source.Contains(".") &&
		!Parent->nickserv.GetLive(sourceL).IsInChan(params.ExtractWord(1, ": ")))
		sraw(((proto.Tokens() && !proto.GetNonToken("KICK").empty()) ?
			proto.GetNonToken("KICK") : mstring("KICK")) +
			" " + params.ExtractWord(1, ": ") + " " + source + " :You are not in this channel");

	    // NOTE: as the message has already been broadcasted,
	    // we still need to acomodate for it.
	    Parent->nickserv.GetLive(params.ExtractWord(2, ": ")).Kick(source, params.ExtractWord(1, ": "));
	}
	else if (msgtype=="KILL")
	{
	    // :source/server KILL target :reason
	    // LOCAL clients ONLY (remotes are a QUIT).
	    //:PreZ KILL kick`kill`abuse :castle.srealm.net.au!PreZ (blah)
	    WLOCK(("UsingNick", params.ExtractWord(1, ": ").LowerCase()));
	    if (Parent->nickserv.IsLive(params.ExtractWord(1, ": ")))
	    {
		// sign on services again if they're killed.
		if (Parent->nickserv.GetLive(params.ExtractWord(1, ": ")).IsServices())
		{
		    LOG((LM_WARNING, Parent->getLogMessage("OTHER/KILLED"),
			    params.ExtractWord(1, ": ").c_str(),
			    (!Parent->nickserv.IsLive(sourceL) ? sourceL.c_str() :
			    Parent->nickserv.GetLive(sourceL).Mask(Nick_Live_t::N_U_P_H).c_str())));
		    WLOCK2(("Server", "WaitIsOn"));
		    WaitIsOn.insert(params.ExtractWord(1, ": "));
		    sraw(((proto.Tokens() && !proto.GetNonToken("ISON").empty()) ?
			proto.GetNonToken("ISON") : mstring("ISON")) +
			" " + params.ExtractWord(1, ": "));
		}
		int wc = params.After(":").WordCount("!");
		Parent->nickserv.GetLive(params.ExtractWord(1, ": ")).Quit(
			"Killed (" + params.After(":").After("!", wc-1) + ")");
		WLOCK2(("NickServ", "live"));
		Parent->nickserv.RemLive(params.ExtractWord(1, ": "));
	    }
	    else
	    {
		LOG((LM_ERROR, Parent->getLogMessage("ERROR/REC_FORNONUSER"),
			"KILL", source.c_str(), params.ExtractWord(1, ": ").c_str()));
	    }

	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
}

void Server::parse_L(const mstring &data)
{
    FT("Server::parse_L", (data));
    mstring source, msgtype, params;

    if (data[0u] == ':' || data[0u] == '@')
    {
	if (data[0u] == '@' && proto.Numeric())
	{
	    RLOCK(("ServerLinking"));
	    source = GetServer(data.ExtractWord(1, "@ "));
	    for (unsigned int i = 0; source.empty() && i < 10; i++)
	    {
		ACE_OS::sleep(1);
		source = GetServer(data.ExtractWord(1, "@ "));
	    }
	}
	else
	    source = data.ExtractWord(1,": ");
	if (!(source.Contains(".") || Parent->nickserv.IsLive(source)))
	{
	    PushUser(source, data);
	    return;
	}
        msgtype = data.ExtractWord(2,": ").UpperCase();
	if (data.WordCount(" ") > 2)
	    params = data.After(" ", 2);
    }
    else
    {
	source = OurUplink();
        msgtype = data.ExtractWord(1,": ").UpperCase();
	if (data.WordCount(" ") > 1)
	    params = data.After(" ");
    }
    if (source.empty())
	source = Parent->startup.Server_Name();
    mstring sourceL(source.LowerCase());


	if (msgtype=="LINKS")
	{
	    //:ChanServ LINKS :temple.magick.tm
	    //:temple.magick.tm 364 ChanServ temple.magick.tm temple.magick.tm :0 Magick IRC Services Test Network
	    //:temple.magick.tm 365 ChanServ temple.magick.tm :End of /LINKS list.

	    { RLOCK(("IrcSvcHandler"));
	    if (Parent->ircsvchandler != NULL &&
		Parent->ircsvchandler->HTM_Level() > 3)
	    {
		mstring tmp;
		tmp.Format(Parent->getMessage(source, "MISC/HTM").c_str(),
								msgtype.c_str());
		raw(((proto.Tokens() && !proto.GetNonToken("NOTICE").empty()) ?
			proto.GetNonToken("NOTICE") : mstring("NOTICE")) +
			" " + source + " :" + tmp.c_str());
		return;
	    }
	    else
	    {
		sraw("364 " + source + " " + Parent->startup.Server_Name() + " " +
		    Parent->startup.Server_Name() + " :0 " + Parent->startup.Server_Desc());

		Server::list_t::iterator serv;
		RLOCK(("Server", "list"));
		for(serv=Parent->server.ListBegin(); serv!=Parent->server.ListEnd(); serv++)
		{
		    sraw("364 " + source + " " + serv->second.Name() + " " + serv->second.Uplink()
			+ " :" + serv->second.Hops() + " " + serv->second.Description());
		}
	    }}

	    sraw("365 " + source + " " + Parent->startup.Server_Name() + " :End of /LINKS list.");

	}
	else if (msgtype=="LIST")
	{
	    sraw("321 " + source + " Channel :Users  Name");

	    { RLOCK(("IrcSvcHandler"));
	    if (Parent->ircsvchandler != NULL &&
		Parent->ircsvchandler->HTM_Level() > 3)
	    {
		mstring tmp;
		tmp.Format(Parent->getMessage(source, "MISC/HTM").c_str(),
								msgtype.c_str());
		raw(((proto.Tokens() && !proto.GetNonToken("NOTICE").empty()) ?
			proto.GetNonToken("NOTICE") : mstring("NOTICE")) +
			" " + source + " :" + tmp.c_str());
		return;
	    }
	    else
	    {
		ChanServ::live_t::iterator chan;
		RLOCK(("ChanServ", "live"));
		for (chan=Parent->chanserv.LiveBegin(); chan!=Parent->chanserv.LiveEnd(); chan++)
		{
		    if (!(chan->second.HasMode("s") || chan->second.HasMode("p")))
			sraw("322 " + source + " " + chan->first + " " +
				mstring(chan->second.Users()) +  " :" +
				chan->second.Topic());
		}
	    }}

	    sraw("323 " + source + " :End of /LIST");
	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
}

void Server::parse_M(const mstring &data)
{
    FT("Server::parse_M", (data));
    mstring source, msgtype, params;

    if (data[0u] == ':' || data[0u] == '@')
    {
	if (data[0u] == '@' && proto.Numeric())
	{
	    RLOCK(("ServerLinking"));
	    source = GetServer(data.ExtractWord(1, "@ "));
	    for (unsigned int i = 0; source.empty() && i < 10; i++)
	    {
		ACE_OS::sleep(1);
		source = GetServer(data.ExtractWord(1, "@ "));
	    }
	}
	else
	    source = data.ExtractWord(1,": ");
	if (!(source.Contains(".") || Parent->nickserv.IsLive(source)))
	{
	    PushUser(source, data);
	    return;
	}
        msgtype = data.ExtractWord(2,": ").UpperCase();
	if (data.WordCount(" ") > 2)
	    params = data.After(" ", 2);
    }
    else
    {
	source = OurUplink();
        msgtype = data.ExtractWord(1,": ").UpperCase();
	if (data.WordCount(" ") > 1)
	    params = data.After(" ");
    }
    if (source.empty())
	source = Parent->startup.Server_Name();
    mstring sourceL(source.LowerCase());


	if (msgtype=="MODE")
	{
	    // :source MODE source :mode
	    // :source MODE #channel mode params...
	    // :server MODE #channel mode params... creationtime
	    if (IsChan(params.ExtractWord(1, ": ")))
	    {
		RLOCK(("UsingChan", sourceL));
		RLOCK2(("UsingChan", params.ExtractWord(1, ": ").LowerCase()));
		if (Parent->chanserv.IsLive(params.ExtractWord(1, ": ")))
		{
		    Parent->chanserv.GetLive(params.ExtractWord(1, ": ")).Mode(source, params.After(" "));
		}
		else
		{
		    if (source.Contains("."))
		    {
			if (SeenMessage(data) >= Parent->config.MSG_Seen_Act())
			{
			    LOG((LM_CRITICAL, Parent->getLogMessage("ERROR/REC_FORNONCHAN"),
				"MODE", source.c_str(), params.ExtractWord(1, ": ").c_str()));
			}
			else
			    mBase::push_message(data);
		    }
		    else
		    {
			PushUser(source, data, params.ExtractWord(1, ": "));
		    }
		}
	    }
	    else
	    {
		RLOCK(("UsingNick", params.ExtractWord(1, ": ").LowerCase()));
		if (Parent->nickserv.IsLive(params.ExtractWord(1, ": ")))
		{
		    Parent->nickserv.GetLive(params.ExtractWord(1, ": ")).Mode(params.After(" "));
		}
		else
		{
		    if (source.Contains("."))
		    {
			if (SeenMessage(data) >= Parent->config.MSG_Seen_Act())
			{
			    LOG((LM_CRITICAL, Parent->getLogMessage("ERROR/REC_FORNONCHAN"),
				"MODE", source.c_str(), params.ExtractWord(1, ": ").c_str()));
			}
			else
			    mBase::push_message(data);
		    }
		    else
		    {
			PushUser(source, data);
		    }
		}
	    }
	}
	else if (msgtype=="MODNICK")
	{
	    // forcably changed nicks (handle like nick)
	}
	else if (msgtype=="MOTD")
	{
	    if (mFile::Exists(Parent->files.Motdfile()))
	    {
		sraw("375 " + source + " :Message Of The Day");
		{ RLOCK(("IrcSvcHandler"));
		if (Parent->ircsvchandler != NULL &&
		    Parent->ircsvchandler->HTM_Level() > 3)
		{
		    mstring tmp;
		    tmp.Format(Parent->getMessage(source, "MISC/HTM").c_str(),
								msgtype.c_str());
		    raw(((proto.Tokens() && !proto.GetNonToken("NOTICE").empty()) ?
			proto.GetNonToken("NOTICE") : mstring("NOTICE")) +
			" " + source + " :" + tmp.c_str());
		    return;
		}
		else
		{
		    vector<mstring> tmp = mFile::UnDump(Parent->files.Motdfile());
		    for (unsigned int i=0; i<tmp.size(); i++)
			sraw("372 " + source + " :" + tmp[i].c_str());
		}}
		sraw("376 " + source + " :End of MOTD.");
	    }
	    else
	    {
		sraw("422 " + source + " :No MOTD exists.");
	    }
	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
}

void Server::parse_N(const mstring &data)
{
    FT("Server::parse_N", (data));
    mstring source, msgtype, params;

    if (data[0u] == ':' || data[0u] == '@')
    {
	if (data[0u] == '@' && proto.Numeric())
	{
	    RLOCK(("ServerLinking"));
	    source = GetServer(data.ExtractWord(1, "@ "));
	    for (unsigned int i = 0; source.empty() && i < 10; i++)
	    {
		ACE_OS::sleep(1);
		source = GetServer(data.ExtractWord(1, "@ "));
	    }
	}
	else
	    source = data.ExtractWord(1,": ");
	if (!(source.Contains(".") || Parent->nickserv.IsLive(source)))
	{
	    PushUser(source, data);
	    return;
	}
        msgtype = data.ExtractWord(2,": ").UpperCase();
	if (data.WordCount(" ") > 2)
	    params = data.After(" ", 2);
    }
    else
    {
	source = OurUplink();
        msgtype = data.ExtractWord(1,": ").UpperCase();
	if (data.WordCount(" ") > 1)
	    params = data.After(" ");
    }
    if (source.empty())
	source = Parent->startup.Server_Name();
    mstring sourceL(source.LowerCase());


	if (msgtype=="NAMES")
	{
	    // :source NAMES #channel our.server
//:soul.darker.net 353 ChanServ = #chatzone :killkickabuseme @Aimee Jupiter @Allanon Ghost_ wildrose
//:soul.darker.net 366 ChanServ #chatzone :End of /NAMES list.
	    sraw("366 " + source + " " + params.ExtractWord(1, ": ") + " :End of /NAMES list.");

	}
	else if (msgtype=="NETINFO")
	{
	    // Ignore -- unrealism
	}
	else if (msgtype=="NICK")
	{
	    if (source.Contains("."))
	    {
		// NEW USER
		sourceL = params.ExtractWord(1, ": ").LowerCase();
		WLOCK(("UsingNick", sourceL));

		// DONT kill when we do SQUIT protection.
		map<mstring,list<mstring> >::iterator i;
		{ RLOCK(("Server", "ToBeSquit"));
		for (i=ToBeSquit.begin(); i!=ToBeSquit.end(); i++)
		{
		    list<mstring>::iterator k;
		    WLOCK2(("Server", "ToBeSquit", i->first.LowerCase()));
		    for (k=i->second.begin(); k!=i->second.end(); k++)
			if (*k == sourceL)
			{
			    list<mstring>::iterator j = k;  j--;
			    i->second.erase(k);
			    k=j;
			}
		}}

		if (Parent->nickserv.IsLiveAll(sourceL))
		{
		    // IF the squit server = us, and the signon time matches
		    if (Parent->nickserv.GetLive(sourceL).Squit() == params.ExtractWord(6, ": ").LowerCase()
			&& Parent->nickserv.GetLive(sourceL).SignonTime() == mDateTime(static_cast<time_t>(atoul(params.ExtractWord(3, ": ")))))
		    {
			Parent->nickserv.GetLive(sourceL).ClearSquit();
			return;    // nice way to avoid repeating ones self :)
		    }
		    else
		    {
			WLOCK2(("NickServ", "live"));
			Parent->nickserv.GetLive(sourceL).Quit("SQUIT - " + Parent->nickserv.GetLive(sourceL).Server());
			Parent->nickserv.RemLive(sourceL);
		    }
		}

		// In burst mode, make sure SERVER processing is done
		{ RLOCK(("IrcSvcHandler"));
		  RLOCK2(("Server", "proc_SERVER"));
		if (proc_SERVER && Parent->ircsvchandler != NULL &&
			Parent->ircsvchandler->Burst())
		{
		    WLOCK2(("WaitEndServerAdd"));
		}}
		{ WLOCK2(("Server", "proc_NICK")); proc_NICK++; }
		RLOCK(("WaitEndNickAdd"));

		{ WLOCK2(("NickServ", "live"));
		switch (proto.Signon())
		{
		case 0000:
		case 0001:
		    break;
		case 1000: // NICK nick hops time user host server :realname
		    {
			Nick_Live_t tmp(
			    params.ExtractWord(2, ": "),
			    static_cast<time_t>(atoul(params.ExtractWord(3, ": "))),
			    GetServer(params.ExtractWord(6, ": ")),
			    params.ExtractWord(4, ": "),
			    params.ExtractWord(5, ": "),
			    params.After(":")
			);
			Parent->nickserv.AddLive(&tmp);
		    }
		    break;
		case 1001: // NICK nick hops time user host server 1 :realname
		    {
			Nick_Live_t tmp(
			    params.ExtractWord(1, ": "),
			    static_cast<time_t>(atoul(params.ExtractWord(3, ": "))),
			    GetServer(params.ExtractWord(6, ": ")),
			    params.ExtractWord(4, ": "),
			    params.ExtractWord(5, ": "),
			    params.After(":")
			);
			Parent->nickserv.AddLive(&tmp);
		    }
		    break;
		case 1002: // NICK nick hops time user host server 0 real-host :realname
		    {
			Nick_Live_t tmp(
			    params.ExtractWord(1, ": "),
			    static_cast<time_t>(atoul(params.ExtractWord(3, ": "))),
			    GetServer(params.ExtractWord(6, ": ")),
			    params.ExtractWord(4, ": "),
			    params.ExtractWord(8, ": "),
			    params.After(":")
			);
			tmp.AltHost(params.ExtractWord(5, ": "));
			Parent->nickserv.AddLive(&tmp);
		    }
		    break;
		case 1003: // NICK nick hops time user real-host host server 0 :realname
		    {
			Nick_Live_t tmp(
			    params.ExtractWord(1, ": "),
			    static_cast<time_t>(atoul(params.ExtractWord(4, ": "))),
			    GetServer(params.ExtractWord(7, ": ")),
			    params.ExtractWord(4, ": "),
			    params.ExtractWord(5, ": "),
			    params.After(":")
			);
			tmp.AltHost(params.ExtractWord(6, ": "));
			Parent->nickserv.AddLive(&tmp);
		    }
		    break;
		case 2000: // NICK nick hops time mode user host server :realname
		    {
			Nick_Live_t tmp(
			    params.ExtractWord(1, ": "),
			    static_cast<time_t>(atoul(params.ExtractWord(3, ": "))),
			    GetServer(params.ExtractWord(7, ": ")),
			    params.ExtractWord(5, ": "),
			    params.ExtractWord(6, ": "),
			    params.After(":")
			);
			tmp.Mode(params.ExtractWord(4, ": "));
			Parent->nickserv.AddLive(&tmp);
		    }
		    break;
		case 2001: // NICK nick hops time mode user host server 0 :realname
		    {
			Nick_Live_t tmp(
			    params.ExtractWord(1, ": "),
			    static_cast<time_t>(atoul(params.ExtractWord(3, ": "))),
			    GetServer(params.ExtractWord(7, ": ")),
			    params.ExtractWord(5, ": "),
			    params.ExtractWord(6, ": "),
			    params.After(":")
			);
			tmp.Mode(params.ExtractWord(4, ": "));
			Parent->nickserv.AddLive(&tmp);
		    }
		    break;
		case 2002: // NICK nick hops time mode user host maskhost server 0 :realname
		    {
			Nick_Live_t tmp(
			    params.ExtractWord(1, ": "),
			    static_cast<time_t>(atoul(params.ExtractWord(3, ": "))),
			    GetServer(params.ExtractWord(8, ": ")),
			    params.ExtractWord(5, ": "),
			    params.ExtractWord(6, ": "),
			    params.After(":")
			);
			tmp.Mode(params.ExtractWord(4, ": "));
			tmp.AltHost(params.ExtractWord(7, ": "));
			Parent->nickserv.AddLive(&tmp);
		    }
		    break;
		case 2003: // NICK nick hops time user host server 0 mode maskhost :realname
		    {
			Nick_Live_t tmp(
			    params.ExtractWord(1, ": "),
			    static_cast<time_t>(atoul(params.ExtractWord(3, ": "))),
			    GetServer(params.ExtractWord(6, ": ")),
			    params.ExtractWord(4, ": "),
			    params.ExtractWord(5, ": "),
			    params.After(":")
			);
			tmp.Mode(params.ExtractWord(8, ": "));
			tmp.AltHost(params.ExtractWord(9, ": "));
			Parent->nickserv.AddLive(&tmp);
		    }
		    break;
		}}
		if (Parent->nickserv.IsLive(sourceL))
		{
		    if (Parent->nickserv.GetLive(sourceL).Server().empty())
		    {
			PopUser(sourceL);
			KILL(Parent->nickserv.FirstName(), sourceL,
				Parent->nickserv.GetLive(sourceL).RealName());
			{ WLOCK2(("Server", "proc_NICK")); proc_NICK--; }
			return;
		    }

		    { WLOCK2(("Server", "i_UserMax"));
		    if (i_UserMax < Parent->nickserv.LiveSize())
		    {
			MCB(i_UserMax);
			i_UserMax = Parent->nickserv.LiveSize();
			MCE(i_UserMax);
		    }}
		    FlushUser(sourceL);

		    // HAS to be AFTER the nickname is added to map.
		    CommServ::list_t::iterator iter;
		    mstring setmode;
		    { RLOCK2(("CommServ", "list"));
		    for (iter = Parent->commserv.ListBegin();
				    iter != Parent->commserv.ListEnd();
				    iter++)
		    {
			if (iter->second.IsOn(sourceL))
			{
			    if (iter->first == Parent->commserv.ALL_Name())
				setmode += Parent->commserv.ALL_SetMode();
			    else if (iter->first == Parent->commserv.REGD_Name())
				setmode += Parent->commserv.REGD_SetMode();
			    else if (iter->first == Parent->commserv.OPER_Name())
				setmode += Parent->commserv.OPER_SetMode();
			    else if (iter->first == Parent->commserv.ADMIN_Name())
				setmode += Parent->commserv.ADMIN_SetMode();
			    else if (iter->first == Parent->commserv.SOP_Name())
				setmode += Parent->commserv.SOP_SetMode();
			    else if (iter->first == Parent->commserv.SADMIN_Name())
				setmode += Parent->commserv.SADMIN_SetMode();

			    MLOCK(("CommServ", "list", iter->first, "message"));
			    for (iter->second.message = iter->second.MSG_begin();
				iter->second.message != iter->second.MSG_end();
				iter->second.message++)
			    {
				Parent->servmsg.send(sourceL, "[" + IRC_Bold +
					    iter->first + IRC_Off + "] " +
					    iter->second.message->Entry());
			    }
			}
		    }}
		    if (!setmode.empty())
		    {
			mstring setmode2;
			for (unsigned int j=0; j<setmode.size(); j++)
			{
			    if (setmode[j] != '+' && setmode[j] != '-' &&
				setmode[j] != ' ' &&
				!Parent->nickserv.GetLive(sourceL).HasMode(setmode[j]))
			        setmode2 += setmode[j];
			}
			SVSMODE(Parent->nickserv.FirstName(), sourceL, "+" + setmode2);
		    }
		    if (Parent->nickserv.IsStored(sourceL))
		    {
			if (Parent->nickserv.GetStored(sourceL).Forbidden())
			{
			    Parent->nickserv.send(Parent->nickserv.FirstName(),
				sourceL, Parent->getMessage(sourceL, "ERR_SITUATION/FORBIDDEN"),
				ToHumanTime(Parent->nickserv.Ident(), sourceL).c_str());
			}
			else if (Parent->nickserv.GetStored(sourceL).Protect() &&
		    	     !Parent->nickserv.GetStored(sourceL).IsOnline())
			{
			    Parent->nickserv.send(Parent->nickserv.FirstName(),
				sourceL, Parent->getMessage(sourceL, "ERR_SITUATION/PROTECTED"),
				ToHumanTime(Parent->nickserv.Ident(), sourceL).c_str());
			}
		    }
		}
		{ WLOCK2(("Server", "proc_NICK")); proc_NICK--; }
	    }
	    else
	    {
		// CHANGE NICK
		if (!sourceL.IsSameAs(params.ExtractWord(1, ": ")))
		{
		    WLOCK(("UsingNick", sourceL));
		    WLOCK2(("UsingNick", params.ExtractWord(1, ": ").LowerCase()));
		    Nick_Live_t tmp(Parent->nickserv.GetLive(sourceL));
		    tmp.Name(params.ExtractWord(1, ": "));
		    Parent->nickserv.RemLive(sourceL);
		    Parent->nickserv.AddLive(&tmp);
		    // We just did a SVSNICK ...
		    if (Parent->nickserv.IsRecovered(source))
		    {
			Parent->server.NICK(source,
				(Parent->startup.Ownuser() ?
				sourceL : Parent->startup.Services_User()),
				Parent->startup.Services_Host(),
				Parent->startup.Server_Name(),
				"Nickname Enforcer");
		    }
		}
		else
		{
		    WLOCK(("UsingNick", sourceL));
		    Parent->nickserv.GetLive(sourceL).Name(params.ExtractWord(1, ": "));
		}
	    }
	}
	else if (msgtype=="NOTICE")
	{
	    // :source NOTICE target/#channel :message
	    // NOTICE target :message
	    if (!Parent->GotConnect())
		return;

	    if (!IsChan(params.ExtractWord(1, ": ")))
	    {
		LOG((LM_WARNING, Parent->getLogMessage("ERROR/REC_FORNONUSER"),
			"NOTICE", source.c_str(), params.ExtractWord(1, ": ").c_str()));
	    }
	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
}

void Server::parse_O(const mstring &data)
{
    FT("Server::parse_O", (data));
    mstring source, msgtype, params;

    if (data[0u] == ':' || data[0u] == '@')
    {
	if (data[0u] == '@' && proto.Numeric())
	{
	    RLOCK(("ServerLinking"));
	    source = GetServer(data.ExtractWord(1, "@ "));
	    for (unsigned int i = 0; source.empty() && i < 10; i++)
	    {
		ACE_OS::sleep(1);
		source = GetServer(data.ExtractWord(1, "@ "));
	    }
	}
	else
	    source = data.ExtractWord(1,": ");
	if (!(source.Contains(".") || Parent->nickserv.IsLive(source)))
	{
	    PushUser(source, data);
	    return;
	}
        msgtype = data.ExtractWord(2,": ").UpperCase();
	if (data.WordCount(" ") > 2)
	    params = data.After(" ", 2);
    }
    else
    {
	source = OurUplink();
        msgtype = data.ExtractWord(1,": ").UpperCase();
	if (data.WordCount(" ") > 1)
	    params = data.After(" ");
    }
    if (source.empty())
	source = Parent->startup.Server_Name();
    mstring sourceL(source.LowerCase());


	if (msgtype=="OPER")
	{
	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
}

void Server::parse_P(const mstring &data)
{
    FT("Server::parse_P", (data));
    mstring source, msgtype, params;

    if (data[0u] == ':' || data[0u] == '@')
    {
	if (data[0u] == '@' && proto.Numeric())
	{
	    RLOCK(("ServerLinking"));
	    source = GetServer(data.ExtractWord(1, "@ "));
	    for (unsigned int i = 0; source.empty() && i < 10; i++)
	    {
		ACE_OS::sleep(1);
		source = GetServer(data.ExtractWord(1, "@ "));
	    }
	}
	else
	    source = data.ExtractWord(1,": ");
	if (!(source.Contains(".") || Parent->nickserv.IsLive(source)))
	{
	    PushUser(source, data);
	    return;
	}
        msgtype = data.ExtractWord(2,": ").UpperCase();
	if (data.WordCount(" ") > 2)
	    params = data.After(" ", 2);
    }
    else
    {
	source = OurUplink();
        msgtype = data.ExtractWord(1,": ").UpperCase();
	if (data.WordCount(" ") > 1)
	    params = data.After(" ");
    }
    if (source.empty())
	source = Parent->startup.Server_Name();
    mstring sourceL(source.LowerCase());


	if (msgtype=="PART")
	{
	    if (source.Contains("."))
		return;

	    // :source PART #channel :reason
	    RLOCK(("UsingNick", sourceL));
	    WLOCK(("UsingChan", params.ExtractWord(1, ": ").LowerCase()));
	    Parent->nickserv.GetLive(sourceL).Part(params.ExtractWord(1, ": "));
	}
	else if (msgtype=="PASS")
	{
	    if (!source.Contains("."))
		return;

	    // PASS :password
	    if (params.ExtractWord(1, ": ") != Parent->startup.Server(Parent->CurrentServer()).second.second)
	    {
		LOG((LM_ERROR, Parent->getLogMessage("OTHER/WRONGPASS"),
			Parent->CurrentServer().c_str()));
		CP(("Server password mismatch.  Closing socket."));
		raw(((proto.Tokens() && !proto.GetNonToken("ERROR").empty()) ?
			proto.GetNonToken("ERROR") : mstring("ERROR")) +
			" :No Access (passwd mismatch) [" + Parent->CurrentServer() + "]");
		raw(((proto.Tokens() && !proto.GetNonToken("ERROR").empty()) ?
			proto.GetNonToken("ERROR") : mstring("ERROR")) +
			" :Closing Link: [" + Parent->CurrentServer() + "] (Bad Password)");
		sraw("464 " + Parent->CurrentServer() + " :" + "Password Incorrect");
		Parent->Disconnect();
	    }
	}
	else if (msgtype=="PING")
	{
	    // PING :some.server
	    // :some.server PING some.server :our.server
	    sraw(((proto.Tokens() && !proto.GetNonToken("PONG").empty()) ?
		proto.GetNonToken("PONG") : mstring("PONG")) +
		" " + Parent->startup.Server_Name() + " :" + source);
	}
	else if (msgtype=="PONG")
	{
	    // Receive info back for LAG MONITOR.
	    // we'll send out ':our.server PING our.server :remote.server'
	    // for EACH server, at the same time execute Ping() on it.

	    // :server PONG server :our.server
	    if (IsList(source))
		GetList(sourceL).Pong();

	}
	else if (msgtype=="PRIVMSG")
	{
	    // :source PRIVMSG target/#channel :message
	    if (!Parent->GotConnect())
		return;

	    if (!IsChan(params.ExtractWord(1, ": ")))
	    {
		LOG((LM_WARNING, Parent->getLogMessage("ERROR/REC_FORNONUSER"),
			"PRIVMSG", source.c_str(), params.ExtractWord(1, ": ").c_str()));
	    }
	}
	else if (msgtype=="PROTOCTL")
	{
	    if (!source.Contains("."))
		return;

	    // Turn on tokens dynamically ...
	    if ((data + " ").Contains(" TOKEN "))
		proto.Tokens(true);
	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
}

void Server::parse_Q(const mstring &data)
{
    FT("Server::parse_Q", (data));
    mstring source, msgtype, params;

    if (data[0u] == ':' || data[0u] == '@')
    {
	if (data[0u] == '@' && proto.Numeric())
	{
	    RLOCK(("ServerLinking"));
	    source = GetServer(data.ExtractWord(1, "@ "));
	    for (unsigned int i = 0; source.empty() && i < 10; i++)
	    {
		ACE_OS::sleep(1);
		source = GetServer(data.ExtractWord(1, "@ "));
	    }
	}
	else
	    source = data.ExtractWord(1,": ");
	if (!(source.Contains(".") || Parent->nickserv.IsLive(source)))
	{
	    PushUser(source, data);
	    return;
	}
        msgtype = data.ExtractWord(2,": ").UpperCase();
	if (data.WordCount(" ") > 2)
	    params = data.After(" ", 2);
    }
    else
    {
	source = OurUplink();
        msgtype = data.ExtractWord(1,": ").UpperCase();
	if (data.WordCount(" ") > 1)
	    params = data.After(" ");
    }
    if (source.empty())
	source = Parent->startup.Server_Name();
    mstring sourceL(source.LowerCase());


	if (msgtype=="QUIT")
	{
	    // :source QUIT :reason
	    // :source QUIT :server server

	    if (source.Contains("."))
		return;

	    WLOCK(("UsingNick", sourceL));
	    // OK, 4 words (always for squit), the 4nd word is a server
	    // and the 3rd word is the uplink of the 4th word (a server)
	    if (params.WordCount(": ")==2 && IsList(params.ExtractWord(2, ": ")) &&
		GetList(params.ExtractWord(2, ": ").LowerCase()).Uplink() == params.ExtractWord(1, ": ").LowerCase())
	    {
		// Suspected SQUIT
		//
		// - Add user to ToBeSquit map under servername
		// - Add data(4) to ServerSquit map with a timer to clear
		//
		// IF no SQUIT message received, user is QUIT and server
		// is removed from ServerSquit map -- ie. its FAKE!
		Parent->nickserv.GetLive(sourceL).SetSquit();
		WLOCK2(("Server", "ToBeSquit"));
		MCB(ToBeSquit.size());
		ToBeSquit[params.ExtractWord(2, ": ").LowerCase()].push_back(sourceL);
		LOG((LM_NOTICE, Parent->getLogMessage("OTHER/SQUIT_FIRST"),
			params.ExtractWord(2, ": ").c_str(),
			params.ExtractWord(1, ": ").c_str()));

		WLOCK3(("Server", "ServerSquit"));
		if (ServerSquit.find(Parent->nickserv.GetLive(sourceL).Server()) == ServerSquit.end())
		{
		    CB(1, ServerSquit.size());
		    ServerSquit[Parent->nickserv.GetLive(sourceL).Server()] =
			ACE_Reactor::instance()->schedule_timer(&tobesquit,
				new mstring(Parent->nickserv.GetLive(sourceL).Server()),
				ACE_Time_Value(10));
		    CE(1, ServerSquit.size());
		}
		MCE(ToBeSquit.size());
	    }
	    else
	    {
		// Normal quit ...

		// Kind of illegal to do, but accomodate anyway, re-signon
		// services if someone quits them (how?!?)
		if (Parent->nickserv.GetLive(sourceL).IsServices())
		    sraw(((proto.Tokens() && !proto.GetNonToken("ISON").empty()) ?
			proto.GetNonToken("ISON") : mstring("ISON")) +
			" " + sourceL);
		Parent->nickserv.GetLive(sourceL).Quit(params.After(":"));
		WLOCK2(("NickServ", "live"));
		Parent->nickserv.RemLive(sourceL);
	    }
	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
}

void Server::parse_R(const mstring &data)
{
    FT("Server::parse_R", (data));
    mstring source, msgtype, params;

    if (data[0u] == ':' || data[0u] == '@')
    {
	if (data[0u] == '@' && proto.Numeric())
	{
	    RLOCK(("ServerLinking"));
	    source = GetServer(data.ExtractWord(1, "@ "));
	    for (unsigned int i = 0; source.empty() && i < 10; i++)
	    {
		ACE_OS::sleep(1);
		source = GetServer(data.ExtractWord(1, "@ "));
	    }
	}
	else
	    source = data.ExtractWord(1,": ");
	if (!(source.Contains(".") || Parent->nickserv.IsLive(source)))
	{
	    PushUser(source, data);
	    return;
	}
        msgtype = data.ExtractWord(2,": ").UpperCase();
	if (data.WordCount(" ") > 2)
	    params = data.After(" ", 2);
    }
    else
    {
	source = OurUplink();
        msgtype = data.ExtractWord(1,": ").UpperCase();
	if (data.WordCount(" ") > 1)
	    params = data.After(" ");
    }
    if (source.empty())
	source = Parent->startup.Server_Name();
    mstring sourceL(source.LowerCase());


	if (msgtype=="RAKILL")
	{
	    // We will ignore AKILLS because they're not relivant to us.
	    // we will not be akilling our own clients ;P
	}
	else if (msgtype=="REHASH")
	{
	    // Will we ever get this via. net??  ignore.
	}
	else if (msgtype=="RESTART")
	{
	    // Will we ever get this via. net??  ignore.
	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
}

void Server::parse_S(const mstring &data)
{
    FT("Server::parse_S", (data));
    mstring source, msgtype, params;

    if (data[0u] == ':' || data[0u] == '@')
    {
	if (data[0u] == '@' && proto.Numeric())
	{
	    RLOCK(("ServerLinking"));
	    source = GetServer(data.ExtractWord(1, "@ "));
	    for (unsigned int i = 0; source.empty() && i < 10; i++)
	    {
		ACE_OS::sleep(1);
		source = GetServer(data.ExtractWord(1, "@ "));
	    }
	}
	else
	    source = data.ExtractWord(1,": ");
	if (!(source.Contains(".") || Parent->nickserv.IsLive(source)))
	{
	    PushUser(source, data);
	    return;
	}
        msgtype = data.ExtractWord(2,": ").UpperCase();
	if (data.WordCount(" ") > 2)
	    params = data.After(" ", 2);
    }
    else
    {
	source = OurUplink();
        msgtype = data.ExtractWord(1,": ").UpperCase();
	if (data.WordCount(" ") > 1)
	    params = data.After(" ");
    }
    if (source.empty())
	source = Parent->startup.Server_Name();
    mstring sourceL(source.LowerCase());


	if (msgtype=="SETHOST")
	{
	    if (source.Contains("."))
		return;

	    // From UnrealIRCD
	    // :source SVSHOST newhost
	    WLOCK(("UsingNick", sourceL));
	    if (Parent->nickserv.IsLive(sourceL))
	    {
		Parent->nickserv.GetLive(sourceL).AltHost(params.ExtractWord(1, ": "));
	    }
	}
	else if (msgtype=="SETTIME")
	{
	    // RWORLDism -- ignore.
	}
	else if (msgtype=="SERVER")
	{
	    if (!source.Contains("."))
		return;

	    // SERVER downlink hops :description
	    // :uplink SERVER downlink hops :description

	    // Ensure only ONE server is added at a time ...
	    { RLOCK(("IrcSvcHandler"));
	      RLOCK2(("Server", "proc_SERVER"));
	    if (proc_SERVER && Parent->ircsvchandler != NULL &&
		Parent->ircsvchandler->Burst())
	    {
		WLOCK(("WaitEndServerAdd"));
	    }}
	    { MLOCK(("Server", "proc_SERVER")); proc_SERVER++; }
	    RLOCK(("WaitEndServerAdd"));

	    unsigned long numeric = 0;
	    if (source.IsSameAs(Parent->startup.Server_Name(), true))
	    {
		if (Parent->startup.IsAllowed(params.ExtractWord(1, ": "), source))
		{
		    WLOCK(("Server", "list"));
		    // unreal puts the numeric in the desctiption, ie.
		    // SERVER downlink hops :Uversion-servopts-numeric description
		    //        Only this way for direct links --^^^^^^^
		    if (proto.Number() >= 70 && proto.Number() <= 79)
		    {
			numeric = atoi(params.After(":").ExtractWord(3, "-"));
			Server_t tmp(params.ExtractWord(1, ": ").LowerCase(),
				atoi(params.ExtractWord(2, ": ").LowerCase().c_str()),
				params.After(":").After(" "), numeric);
			AddList(&tmp);
		    }
		    else
		    {
			Server_t tmp(params.ExtractWord(1, ": ").LowerCase(),
				atoi(params.ExtractWord(2, ": ").LowerCase().c_str()),
				params.After(":"));
			if (proto.Numeric())
			{
			    numeric = atoi(params.ExtractWord(proto.Numeric(), ": "));
			    tmp.Numeric(numeric);
			}
			AddList(&tmp);
		    }

		    LOG((LM_INFO, Parent->getLogMessage("OTHER/LINK"),
			params.ExtractWord(1, ": ").c_str(),
			Parent->startup.Server_Name().c_str()));
		    Parent->GotConnect(true);
		    SignOnAll();
		}
		else
		{
//		    announce(Parent->operserv.FirstName(),
//			Parent->getMessage("MISC/INVALIDLINK"),
//			params.ExtractWord(1, ": ").c_str(),
//			Parent->startup.Server_Name().c_str());
		    LOG((LM_ERROR, Parent->getLogMessage("OTHER/INVALIDLINK"),
			params.ExtractWord(1, ": ").c_str(),
			Parent->startup.Server_Name().c_str()));
		    raw("SQUIT " + params.ExtractWord(1, ": ") + " :" +
			Parent->getMessage("MISC/MAYNOTLINK"));
		    Parent->Disconnect();
		}
	    }
	    else
	    {
		if (Parent->startup.IsAllowed(params.ExtractWord(1, ": "), source))
		{
		    if (IsList(sourceL))
		    {
			WLOCK(("Server", "list"));
			Server_t tmp(params.ExtractWord(1, ": ").LowerCase(),
			    sourceL,
			    atoi(params.ExtractWord(2, ": ").LowerCase().c_str()),
			    params.After(":"));
			if (proto.Numeric())
			{
			    numeric = atoi(params.ExtractWord(proto.Numeric(), ": "));
			    tmp.Numeric(numeric);
			}
			AddList(&tmp);
			LOG((LM_INFO, Parent->getLogMessage("OTHER/LINK"),
			    params.ExtractWord(1, ": ").c_str(), sourceL.c_str()));
		    }
		    else
		    {
			if (SeenMessage(data) >= Parent->config.MSG_Seen_Act())
			{
			    LOG((LM_ERROR, Parent->getLogMessage("ERROR/REC_FORNONSERVER"),
				"SERVER", params.ExtractWord(1, ": ").c_str(), sourceL.c_str()));
			}
		        else
			    mBase::push_message(data);
		    }
		}
		else
		{
		    announce(Parent->operserv.FirstName(),
			Parent->getMessage("MISC/INVALIDLINK"),
			params.ExtractWord(1, ": ").c_str(), source.c_str());
		    LOG((LM_ERROR, Parent->getLogMessage("OTHER/INVALIDLINK"),
			params.ExtractWord(1, ": ").c_str(), source.c_str()));
		    sraw("SQUIT " + params.ExtractWord(1, ": ") + " :" +
			Parent->getMessage("MISC/MAYNOTLINK"));
		}
	    }
	    { MLOCK(("Server", "proc_SERVER")); proc_SERVER--; }
	}
	else if (msgtype=="SILENCE")
	{
	    // Another one to ignore ...
	}
	else if (msgtype=="SJOIN")
	{
	    //:server SJOIN chan-stamp #channel +modes extra-params :@opd_nick +voice_nick nick 
	    //:relic.devel.relic.net SJOIN 952608432 #blah + <none> :@Kwahraw +PreZ Zephyr
	    
	    //:user SJOIN chan-stamp #channel #channel etc...

	    // In burst mode, make sure NICK processing is done
	    { RLOCK(("IrcSvcHandler"));
	      RLOCK2(("Server", "proc_NICK"));
	    if (proc_NICK && Parent->ircsvchandler != NULL &&
		Parent->ircsvchandler->Burst())
	    {
		WLOCK(("WaitEndNickAdd"));
	    }}
	    { WLOCK(("Server", "proc_CHAN")); proc_CHAN++; }
	    RLOCK(("WaitEndChanAdd"));

	    if (source.Contains("."))
	    {
		mstring chan(params.ExtractWord(2, ": "));
		WLOCK(("UsingChan", chan.LowerCase()));

		unsigned int i;
		vector<mstring> users;
		mstring modes;
		if (params.Before(":").WordCount(" ") >= 3)
		    modes = params.Before(":").ExtractWord(3, " ");
		mstring mode_params, nick;
		bool oped, halfoped, voiced, owner, prot, isban, isexcept;
		if (modes.length())
		{
		    for (i=0; i < proto.ChanModeArg().length(); i++)
			if (modes.Contains(proto.ChanModeArg()[i]))
			{
			    mode_params = params.Before(":").After(" ", 3);
			    break;
			}
		}
		for (i=1; i <= params.After(":").WordCount(" "); i++)
		{
		    nick = params.After(":").ExtractWord(i, " ");
		    if (!nick.empty())
		    {
			char c = 0;
			oped = halfoped = voiced = owner = prot = false;
			isban = isexcept = false;
			while (c != nick[0u])
			{
			    c = nick[0u];
			    switch(nick[0u])
			    {
			    case '@':
				oped = true;
				nick.erase(0, 0);
			        break;
			    case '%':
				halfoped = true;
				nick.erase(0, 0);
			        break;
			    case '+':
				voiced = true;
				nick.erase(0, 0);
			        break;
			    case '*':
				owner = true;
				nick.erase(0, 0);
			        break;
			    case '~':
				prot = true;
				nick.erase(0, 0);
			        break;
			    case '&':
				isban = true;
				nick.erase(0, 0);
			        break;
			    case '"':
				isexcept = true;
				nick.erase(0, 0);
			        break;
			    }
			}
			RLOCK2(("UsingNick", nick.LowerCase()));
			if (!(isban || isexcept) &&
			    Parent->nickserv.IsLive(nick))
			{
			    if (oped)
			    {
				modes += "o";
				mode_params += " " + nick;
			    }
			    if (halfoped)
			    {
				modes += "h";
				mode_params += " " + nick;
			    }
			    if (voiced)
			    {
				modes += "v";
				mode_params += " " + nick;
			    }
			    if (owner)
			    {
				modes += "q";
				mode_params += " " + nick;
			    }
			    if (prot)
			    {
				modes += "a";
				mode_params += " " + nick;
			    }

			    users.push_back(nick);
			}
			else if (isban)
			{
			    modes += "b";
			    mode_params += " " + nick;
			    nick.erase();
			}
			else if (isexcept)
			{
			    modes += "e";
			    mode_params += " " + nick;
			    nick.erase();
			}
			else
			{
			    PushUser(nick, ":" + nick + " JOIN " + chan);
			    if (oped)
				PushUser(nick, ":" + source + " MODE " +
				    chan + " +o :" + nick, chan);
			    if (halfoped)
				PushUser(nick, ":" + source + " MODE " +
				    chan + " +h :" + nick, chan);
			    if (voiced)
				PushUser(nick, ":" + source + " MODE " +
				    chan + " +v :" + nick, chan);
			    if (owner)
				PushUser(nick, ":" + source + " MODE " +
				    chan + " +q :" + nick, chan);
			    if (prot)
				PushUser(nick, ":" + source + " MODE " +
				    chan + " +a :" + nick, chan);
			}
		    }
		}

		if (users.size())
		{
		    for (i=0; i<users.size(); i++)
		    {
			RLOCK2(("UsingNick", users[i].LowerCase()));
			Parent->nickserv.GetLive(users[i]).Join(chan);
		    }
		    CP(("MODE TO %s: %s", chan.LowerCase().c_str(),
			(modes + " " + mode_params).c_str()));
		    if (Parent->chanserv.IsLive(chan))
			Parent->chanserv.GetLive(chan).Mode(
				Parent->chanserv.FirstName(), modes + " " + mode_params);
		}
		else if (modes.length() > 1)
		{
		    PushUser(nick, ":" + source + " MODE " + chan +
			" " + modes + " :" + mode_params, chan);
		}
	    }
	    else
	    {
		unsigned int i;
		bool oped, halfoped, voiced, owner, prot;
		mstring chan;
		RLOCK2(("UsingNick", sourceL));

		if (Parent->nickserv.IsLive(source))
		{
		    for (i=2; i<=params.WordCount(": "); i++)
		    {
			char c = 0;
			chan = params.ExtractWord(i, ": ");
			oped = halfoped = voiced = owner = prot = false;
			while (c != chan[0u])
			{
			    c = chan[0u];
			    switch(chan[0u])
			    {
			    case '@':
				oped = true;
				chan.erase(0, 0);
				break;
			    case '%':
				halfoped = true;
				chan.erase(0, 0);
				break;
			    case '+':
				voiced = true;
				chan.erase(0, 0);
				break;
			    case '*':
				owner = true;
				chan.erase(0, 0);
				break;
			    case '~':
				prot = true;
				chan.erase(0, 0);
				break;
			    }
			}
			WLOCK(("UsingChan", chan.LowerCase()));
			Parent->nickserv.GetLive(sourceL).Join(chan);
			if (oped)
			    Parent->chanserv.GetLive(chan).Mode(
				Parent->chanserv.FirstName(), "+o " + source);
			if (halfoped)
			    Parent->chanserv.GetLive(chan).Mode(
				Parent->chanserv.FirstName(), "+h " + source);
			if (voiced)
			    Parent->chanserv.GetLive(chan).Mode(
				Parent->chanserv.FirstName(), "+v " + source);
			if (owner)
			    Parent->chanserv.GetLive(chan).Mode(
				Parent->chanserv.FirstName(), "+q " + source);
			if (prot)
			    Parent->chanserv.GetLive(chan).Mode(
				Parent->chanserv.FirstName(), "+a " + source);
		    }
		}
		else
		{
		    PushUser(source, data);
		}
	    }
	    { WLOCK(("Server", "proc_CHAN")); proc_CHAN--; }
	}
	else if (msgtype=="SMO")
	{
	    // Thanks to unreal for ANOTHER chatter message ..
	}
	else if (msgtype=="SNICK")
	{
	    if (!source.Contains("."))
		return;

	    // NEW USER
	    sourceL = params.ExtractWord(1, ": ").LowerCase();
	    WLOCK(("UsingNick", sourceL));

	    // DONT kill when we do SQUIT protection.
	    map<mstring,list<mstring> >::iterator i;
	    { RLOCK(("Server", "ToBeSquit"));
	    for (i=ToBeSquit.begin(); i!=ToBeSquit.end(); i++)
	    {
		list<mstring>::iterator k;
		WLOCK2(("Server", "ToBeSquit", i->first.LowerCase()));
		for (k=i->second.begin(); k!=i->second.end(); k++)
		    if (*k == sourceL)
		    {
			i->second.erase(k);
			break;
		    }
	    }}

	    if (Parent->nickserv.IsLiveAll(sourceL))
	    {
		// IF the squit server = us, and the signon time matches
		if (Parent->nickserv.GetLive(sourceL).Squit() == params.ExtractWord(6, ": ").LowerCase()
		    && Parent->nickserv.GetLive(sourceL).SignonTime() == mDateTime(static_cast<time_t>(atoul(params.ExtractWord(3, ": ")))))
		{
		    Parent->nickserv.GetLive(sourceL).ClearSquit();
		    return;    // nice way to avoid repeating ones self :)
		}
		else
		{
		    Parent->nickserv.GetLive(sourceL).Quit("SQUIT - " + Parent->nickserv.GetLive(sourceL).Server());
		    WLOCK2(("NickServ", "live"));
		    Parent->nickserv.RemLive(sourceL);
		}
	    }

	    // hops = servers from us
	    // services = 1 for service, 0 for user
	    // DAL4.4.15+ SNICK name hops time user host server services modes :real name

	    // In burst mode, make sure SERVER processing is done
	    { RLOCK(("IrcSvcHandler"));
	      RLOCK2(("Server", "proc_SERVER"));
	    if (proc_SERVER && Parent->ircsvchandler != NULL &&
		Parent->ircsvchandler->Burst())
	    {
		WLOCK2(("WaitEndServerAdd"));
	    }}
	    { WLOCK2(("Server", "proc_NICK")); proc_NICK++; }
	    RLOCK(("WaitEndNickAdd"));

	    { WLOCK2(("NickServ", "live"));
	      RLOCK2(("AddingServer"));
	    switch (proto.Signon())
	    {
	    case 0000:
	    case 0001:
		break;
	    case 1000: // SNICK nick hops time user host server modes :realname
		{
		    Nick_Live_t tmp(
			params.ExtractWord(1, ": "),
			static_cast<time_t>(atoul(params.ExtractWord(3, ": "))),
			GetServer(params.ExtractWord(6, ": ")),
			params.ExtractWord(4, ": "),
			params.ExtractWord(5, ": "),
			params.After(":")
			);
		    tmp.Mode(params.ExtractWord(7, ": "));
		    Parent->nickserv.AddLive(&tmp);
		}
		break;
	    case 1001: // SNICK nick hops time user host server 1 modes :realname
		{
		    Nick_Live_t tmp(
			params.ExtractWord(1, ": "),
			static_cast<time_t>(atoul(params.ExtractWord(3, ": "))),
			GetServer(params.ExtractWord(6, ": ")),
			params.ExtractWord(4, ": "),
			params.ExtractWord(5, ": "),
			params.After(":")
			);
		    tmp.Mode(params.ExtractWord(8, ": "));
		    Parent->nickserv.AddLive(&tmp);
		}
		break;
	    case 1002: // SNICK nick hops time user host server 0 real-host modes :realname
		{
		    Nick_Live_t tmp(
			params.ExtractWord(1, ": "),
			static_cast<time_t>(atoul(params.ExtractWord(3, ": "))),
			GetServer(params.ExtractWord(6, ": ")),
			params.ExtractWord(4, ": "),
			params.ExtractWord(8, ": "),
			params.After(":")
			);
		    tmp.AltHost(params.ExtractWord(5, ": "));
		    tmp.Mode(params.ExtractWord(9, ": "));
		    Parent->nickserv.AddLive(&tmp);
		}
		break;
	    case 1003: // SNICK nick hops time user real-host host server 0 modes :realname
		{
		    Nick_Live_t tmp(
			params.ExtractWord(1, ": "),
			static_cast<time_t>(atoul(params.ExtractWord(3, ": "))),
			GetServer(params.ExtractWord(7, ": ")),
			params.ExtractWord(4, ": "),
			params.ExtractWord(5, ": "),
			params.After(":")
			);
		    tmp.AltHost(params.ExtractWord(6, ": "));
		    tmp.Mode(params.ExtractWord(9, ": "));
		    Parent->nickserv.AddLive(&tmp);
		}
		break;
	    case 2000: // NICK nick hops time mode user host server :realname
		{
		    Nick_Live_t tmp(
			params.ExtractWord(1, ": "),
			static_cast<time_t>(atoul(params.ExtractWord(3, ": "))),
			GetServer(params.ExtractWord(7, ": ")),
			params.ExtractWord(5, ": "),
			params.ExtractWord(6, ": "),
			params.After(":")
			);
		    tmp.Mode(params.ExtractWord(6, ": "));
		    Parent->nickserv.AddLive(&tmp);
		}
		break;
	    case 2001: // NICK nick hops time mode user host server 0 :realname
		{
		    Nick_Live_t tmp(
			params.ExtractWord(1, ": "),
			static_cast<time_t>(atoul(params.ExtractWord(3, ": "))),
			GetServer(params.ExtractWord(7, ": ")),
			params.ExtractWord(5, ": "),
			params.ExtractWord(6, ": "),
			params.After(":")
			);
		    tmp.Mode(params.ExtractWord(4, ": "));
		    Parent->nickserv.AddLive(&tmp);
		}
		break;
	    case 2002: // NICK nick hops time mode user host maskhost server 0 :realname
		{
		    Nick_Live_t tmp(
			params.ExtractWord(1, ": "),
			static_cast<time_t>(atoul(params.ExtractWord(3, ": "))),
			GetServer(params.ExtractWord(8, ": ")),
			params.ExtractWord(5, ": "),
			params.ExtractWord(6, ": "),
			params.After(":")
		    );
		    tmp.Mode(params.ExtractWord(4, ": "));
		    tmp.AltHost(params.ExtractWord(7, ": "));
		    Parent->nickserv.AddLive(&tmp);
		}
		break;
	    case 2003: // NICK nick hops time user host server 0 mode maskhost :realname
		{
		    Nick_Live_t tmp(
			params.ExtractWord(1, ": "),
			static_cast<time_t>(atoul(params.ExtractWord(3, ": "))),
			GetServer(params.ExtractWord(6, ": ")),
			params.ExtractWord(4, ": "),
			params.ExtractWord(5, ": "),
			params.After(":")
		    );
		    tmp.Mode(params.ExtractWord(8, ": "));
		    tmp.AltHost(params.ExtractWord(9, ": "));
		    Parent->nickserv.AddLive(&tmp);
		}
		break;
	    }}
	    if (Parent->nickserv.IsLive(sourceL))
	    {
		if (Parent->nickserv.GetLive(sourceL).Server().empty())
		{
		    PopUser(sourceL);
		    KILL(Parent->nickserv.FirstName(), sourceL,
				Parent->nickserv.GetLive(sourceL).RealName());
		    { WLOCK2(("Server", "proc_NICK")); proc_NICK--; }

		    return;
		}

		{ WLOCK2(("Server", "i_UserMax"));
		if (i_UserMax < Parent->nickserv.LiveSize())
		{
		    MCB(i_UserMax);
		    i_UserMax = Parent->nickserv.LiveSize();
		    MCE(i_UserMax);
		}}
		FlushUser(sourceL);

		// HAS to be AFTER the nickname is added to map.
		CommServ::list_t::iterator iter;
		mstring setmode;
		{ RLOCK2(("CommServ", "list"));
		for (iter = Parent->commserv.ListBegin();
				    iter != Parent->commserv.ListEnd();
				    iter++)
		{
		    if (iter->second.IsOn(sourceL))
		    {
			if (iter->first == Parent->commserv.ALL_Name())
			    setmode += Parent->commserv.ALL_SetMode();
			else if (iter->first == Parent->commserv.REGD_Name())
			    setmode += Parent->commserv.REGD_SetMode();
			else if (iter->first == Parent->commserv.OPER_Name())
			    setmode += Parent->commserv.OPER_SetMode();
			else if (iter->first == Parent->commserv.ADMIN_Name())
			    setmode += Parent->commserv.ADMIN_SetMode();
			else if (iter->first == Parent->commserv.SOP_Name())
			    setmode += Parent->commserv.SOP_SetMode();
			else if (iter->first == Parent->commserv.SADMIN_Name())
			    setmode += Parent->commserv.SADMIN_SetMode();
			MLOCK(("CommServ", "list", iter->first, "message"));
			for (iter->second.message = iter->second.MSG_begin();
			    iter->second.message != iter->second.MSG_end();
			    iter->second.message++)
			{
			    Parent->servmsg.send(sourceL, "[" + IRC_Bold +
					    iter->first + IRC_Off + "] " +
					    iter->second.message->Entry());
			}
		    }
		}}
		if (!setmode.empty())
		{
		    mstring setmode2;
		    for (unsigned int j=0; j<setmode.size(); j++)
		    {
			if (setmode[j] != '+' && setmode[j] != '-' &&
			    setmode[j] != ' ' &&
			    !Parent->nickserv.GetLive(sourceL).HasMode(setmode[j]))
			    setmode2 += setmode[j];
		    }
		    SVSMODE(Parent->nickserv.FirstName(), sourceL, "+" + setmode2);
		}
		if (Parent->nickserv.IsStored(sourceL))
		{
		    if (Parent->nickserv.GetStored(sourceL).Forbidden())
		    {
			Parent->nickserv.send(Parent->nickserv.FirstName(),
				sourceL, Parent->getMessage(sourceL, "ERR_SITUATION/FORBIDDEN"),
				ToHumanTime(Parent->nickserv.Ident(), sourceL).c_str());
		    }
		    else if (Parent->nickserv.GetStored(sourceL).Protect() &&
			!Parent->nickserv.GetStored(sourceL).IsOnline())
		   {
			Parent->nickserv.send(Parent->nickserv.FirstName(),
				sourceL, Parent->getMessage(sourceL, "ERR_SITUATION/PROTECTED"),
				ToHumanTime(Parent->nickserv.Ident(), sourceL).c_str());
		    }
		}
	    }
	    { WLOCK2(("Server", "proc_NICK")); proc_NICK--; }
	}
	else if (msgtype=="SQLINE")
	{
	    // We will ignore SQLINES because they're not relivant to us.
	    // we will not be qlining our own clients ;P
	}
	else if (msgtype=="SQUIT")
	{
	    // SQUIT shadow.darker.net :soul.darker.net lifestone.darker.net
	    // SQUIT lifestone.darker.net :Ping timeout
	    // :PreZ SQUIT server :reason
	    mstring target = params.ExtractWord(1, ": ").LowerCase();

	    LOG((LM_NOTICE, Parent->getLogMessage("OTHER/SQUIT_SECOND"),
		    target.c_str(),
		    GetList(target.LowerCase()).Uplink().c_str()));

	    { WLOCK(("Server", "list"));
	    RemList(target);
	    }
	    { WLOCK2(("Server", "ServerSquit"));
	    MCB(ToBeSquit.size());
	    CB(1, ServerSquit.size());
	    if (ServerSquit.find(target) != ServerSquit.end())
	    {
		mstring *arg = NULL;
		if (ACE_Reactor::instance()->cancel_timer(
			ServerSquit[target], reinterpret_cast<const void **>(arg))
		    && arg != NULL)
		    delete arg;
		ServerSquit[target] =
		    ACE_Reactor::instance()->schedule_timer(&squit,
			new mstring(target),
			ACE_Time_Value(Parent->config.Squit_Protect()));
	    }
	    { WLOCK3(("Server", "ToBeSquit"));
	    ToBeSquit.erase(target);
	    CE(1, ServerSquit.size());
	    MCE(ToBeSquit.size());
	    }}
	    NickServ::live_t::iterator iter;
	    vector<mstring> chunked;
	    { RLOCK(("NickServ", "live"));
	    for (iter=Parent->nickserv.LiveBegin(); iter != Parent->nickserv.LiveEnd(); iter++)
	    {
		if (iter->second.IsServices() && ListSize() == 0)
		{
		    chunked.push_back(iter->first);
		}
		else if (iter->second.Server() == target)
		    iter->second.SetSquit();
	    }}
	    // Sign off services if we have NO uplink
	    unsigned int i;
	    for (i=0; i<chunked.size(); i++)
		QUIT(chunked[i], "SQUIT - " + target);
	}
	else if (msgtype=="STATS")
	{
	    // :source STATS type :our.server
	    //:temple.magick.tm 219 ChanServ o :End of /STATS report
	    sraw("219 " + source + " " + params.ExtractWord(1, ": ") +
		" :End of /STATS report");
	}
	else if (msgtype=="SUMMON")
	{
	    // :source SUMMON user our.server *
	    sraw("445 " + source + " :SUMMON has been disabled");
	}
	else if (msgtype=="SVINFO")
	{
	    // Bahamut compatable - ignore.
	}
	else if (msgtype=="SVSHOST")
	{
	    // :source SVSHOST user newhost
	    WLOCK(("UsingNick", params.ExtractWord(1, ": ").LowerCase()));
	    if (Parent->nickserv.IsLive(params.ExtractWord(1, ": ")))
	    {
		Parent->nickserv.GetLive(params.ExtractWord(1, ": ")).AltHost(params.ExtractWord(2, ": "));
	    }
	}
	else if (msgtype=="SVSKILL")
	{
	    // Same as KILL (but by services)
	}
	else if (msgtype=="SVSMODE")
	{
	    // Handle just as mode, WITHOUT sanity
	    if (IsChan(params.ExtractWord(1, ": ")))
	    {
		RLOCK(("UsingNick", sourceL));
		RLOCK2(("UsingChan", params.ExtractWord(1, ": ").LowerCase()));
		if (Parent->chanserv.IsLive(params.ExtractWord(1, ": ")))
		{
		    Parent->chanserv.GetLive(params.ExtractWord(1, ": ")).Mode(source, params.After(" "));
		}
		else
		{
		    LOG((LM_CRITICAL, Parent->getLogMessage("ERROR/REC_FORNONCHAN"),
			"SVSMODE", source.c_str(),  params.ExtractWord(1, ": ").c_str()));
		}
	    }
	    else
	    {
		RLOCK(("UsingNick", params.ExtractWord(1, ": ").LowerCase()));
		if (Parent->nickserv.IsLive(params.ExtractWord(1, ": ")))
		{
		    Parent->nickserv.GetLive(params.ExtractWord(1, ": ")).Mode(params.After(" "));
		}
		else
		{
		    LOG((LM_CRITICAL, Parent->getLogMessage("ERROR/REC_FORNONUSER"),
			"SVSMODE", source.c_str(),  params.ExtractWord(1, ": ").c_str()));
		}
	    }
	}
	else if (msgtype=="SVSNICK")
	{
	    // forcably changed nicks (handle like nick)
	}
	else if (msgtype=="SVSNOOP")
	{
	    // Deny all OPERS on server, ignore.
	}
	else if (msgtype=="SZLINE")
	{
	    // Services ZLINE, ignore
	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
}

void Server::parse_T(const mstring &data)
{
    FT("Server::parse_T", (data));
    mstring source, msgtype, params;

    if (data[0u] == ':' || data[0u] == '@')
    {
	if (data[0u] == '@' && proto.Numeric())
	{
	    RLOCK(("ServerLinking"));
	    source = GetServer(data.ExtractWord(1, "@ "));
	    for (unsigned int i = 0; source.empty() && i < 10; i++)
	    {
		ACE_OS::sleep(1);
		source = GetServer(data.ExtractWord(1, "@ "));
	    }
	}
	else
	    source = data.ExtractWord(1,": ");
	if (!(source.Contains(".") || Parent->nickserv.IsLive(source)))
	{
	    PushUser(source, data);
	    return;
	}
        msgtype = data.ExtractWord(2,": ").UpperCase();
	if (data.WordCount(" ") > 2)
	    params = data.After(" ", 2);
    }
    else
    {
	source = OurUplink();
        msgtype = data.ExtractWord(1,": ").UpperCase();
	if (data.WordCount(" ") > 1)
	    params = data.After(" ");
    }
    if (source.empty())
	source = Parent->startup.Server_Name();
    mstring sourceL(source.LowerCase());


	if (msgtype=="TIME")
	{
	    // :source TIME :our.server
	    sraw("391 " + source + " :" + mDateTime::CurrentDateTime().DateTimeString());
	}
	else if (msgtype=="TOPIC")
	{
	    // :server/user TOPIC #channel setter time :topic
	    // :server/user TOPIC #channel setter
	    // TIME is not standard (time is optional);

	    RLOCK(("UsingNick", params.ExtractWord(2, ": ").LowerCase()));
	    RLOCK2(("UsingNick", params.ExtractWord(1, ": ").LowerCase()));
	    if (Parent->chanserv.IsLive(params.ExtractWord(1, ": ")))
	    {
		if (!params.ExtractWord(3, ": ").empty())
		{ // Setting
		    Parent->chanserv.GetLive(params.ExtractWord(1, ": ")).Topic(
			source, params.After(":"), params.ExtractWord(2, ": "),
		        static_cast<time_t>(atoul(params.ExtractWord(3, ": ")))
		    );
		}
		else
		{ // Clearing
		    Parent->chanserv.GetLive(params.ExtractWord(1, ": ")).Topic(
		        source, "", params.ExtractWord(2, ": "));
		}
	    }
	    else
	    {
		if (source.Contains("."))
		{
		    if (SeenMessage(data) >= Parent->config.MSG_Seen_Act())
		    {
			LOG((LM_CRITICAL, Parent->getLogMessage("ERROR/REC_FORNONCHAN"),
				"MODE", source.c_str(), params.ExtractWord(1, ": ").c_str()));
		    }
		    else
			mBase::push_message(data);
		}
		else
		{
		    PushUser(source, data, params.ExtractWord(1, ": "));
		}
	    }
	}
	else if (msgtype=="TRACE")
	{
	    // :source TRACE :server/target
	    // repl:
	    //:ChanServ TRACE temple.magick.tm
	    //:temple.magick.tm 206 ChanServ Server 0 0S 0C temple.magick.tm *!*@temple.magick.tm 1313208
	    //:ChanServ TRACE PreZ
	    //:temple.magick.tm 206 ChanServ Server 0 0S 0C temple.magick.tm *!*@temple.magick.tm 1313208

//:ChanServ TRACE Aimee
//:soul.darker.net 200 ChanServ Link Aurora2.2-r Aimee lifestone.darker.net
//:lifestone.darker.net 200 ChanServ Link Aurora2.2-r Aimee :requiem.darker.net
//:requiem.darker.net 204 ChanServ Operator 10 Aimee[ABD2CEFD.ipt.aol.com] :128

//:ChanServ TRACE vampire
//:soul.darker.net 200 ChanServ Link Aurora2.2-r vampire.darker.net lifestone.darker.net
//:lifestone.darker.net 200 ChanServ Link Aurora2.2-r vampire.darker.net :vampire.darker.net
//:vampire.darker.net 206 ChanServ Server 50 6S 19C lifestone.darker.net[0.0.0.0] AutoConn.!*@vampire.darker.net :0
//:vampire.darker.net 204 ChanServ Operator 10 Alien[pc134.net19.ktv.koping.se] :64
//:vampire.darker.net 205 ChanServ User 1 Nadu[bservice.org] :35
//:vampire.darker.net 205 ChanServ User 1 tomaway[adsl1453.turboline.be] :106
//:vampire.darker.net 205 ChanServ User 1 Wau|oK[axe.net.au] :46
//:vampire.darker.net 205 ChanServ User 1 Ghost_[203.58.108.129] :15
//:vampire.darker.net 205 ChanServ User 1 wildrose[cr1003083-a.crdva1.bc.wave.home.com] :48
//:vampire.darker.net 205 ChanServ User 1 Jupiter[modem035.memnoch.comcen.com.au] :76
//:vampire.darker.net 209 ChanServ Class 50 :1
//:vampire.darker.net 209 ChanServ Class 10 :1
//:vampire.darker.net 209 ChanServ Class 1 :7
	    { RLOCK(("IrcSvcHandler"));
	    if (Parent->ircsvchandler != NULL &&
		Parent->ircsvchandler->HTM_Level() > 3)
	    {
		mstring tmp;
		tmp.Format(Parent->getMessage(source, "MISC/HTM").c_str(),
								msgtype.c_str());
		raw(((proto.Tokens() && !proto.GetNonToken("NOTICE").empty()) ?
			proto.GetNonToken("NOTICE") : mstring("NOTICE")) +
			" " + source + " :" + tmp.c_str());
		return;
	    }}
	    RLOCK(("UsingNick", params.ExtractWord(1, ": ").LowerCase()));
	    mstring out;
	    if (Parent->nickserv.IsLive(params.ExtractWord(1, ": ")) &&
		Parent->nickserv.GetLive(params.ExtractWord(1, ": ")).IsServices())
	    {
		RLOCK2(("NickServ", "live", params.ExtractWord(1, ": ").LowerCase()));
		Nick_Live_t &nlive = Parent->nickserv.GetLive(params.ExtractWord(1, ": "));
		if (nlive.HasMode("o"))
		    out << "204 " << source << " Operator 10 ";
		else
		    out << "205 " << source << " User 1 ";
		out << nlive.Name() << " [" << nlive.User() << "@"
			<< nlive.Host() << "] :" << nlive.LastAction().SecondsSince();
		sraw(out);
	    }
	    else
	    {
		unsigned long opers=0, users=0;
		out << "206 " << source << "Server 50 " << ListSize()+1 <<
			"S " << Parent->nickserv.LiveSize() << "C " <<
			OurUplink() << "[0.0.0.0] AutoConn.!*@" <<
			Parent->startup.Server_Name() << " :0";
		sraw(out);

		RLOCK2(("NickServ", "live"));
		NickServ::live_t::iterator iter;
		for (iter=Parent->nickserv.LiveBegin();
			iter!=Parent->nickserv.LiveEnd(); iter++)
		{
		    if (iter->second.IsServices())
		    {
			out.erase();
			if (iter->second.HasMode("o"))
			{
			    out << "204 " << source << " Operator 10 ";
			    opers++;
			}
			else
			{
			    out << "205 " << source << " User 1 ";
			    users++;
			}
			out << iter->second.Name() << " [" <<
				iter->second.User() << "@" <<
				iter->second.Host() << "] :" <<
				iter->second.LastAction().SecondsSince();
			sraw(out);
			
		    }
		}
		out.erase();
		out << "209 " << source << " Class 50 :1";
		sraw(out);
		if (opers)
		{
		    out.erase();
		    out << "209 " << source << " Class 10 :" << opers;
		    sraw(out);
		}
		if (users)
		{
		    out.erase();
		    out << "209 " << source << " Class 1 :" << users;
		    sraw(out);
		}
	    }
	    out.erase();
	    out << "262 " << source << " " << Parent->startup.Server_Name() <<
		" :End of TRACE";
	    sraw(out);
	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
}

void Server::parse_U(const mstring &data)
{
    FT("Server::parse_U", (data));
    mstring source, msgtype, params;

    if (data[0u] == ':' || data[0u] == '@')
    {
	if (data[0u] == '@' && proto.Numeric())
	{
	    RLOCK(("ServerLinking"));
	    source = GetServer(data.ExtractWord(1, "@ "));
	    for (unsigned int i = 0; source.empty() && i < 10; i++)
	    {
		ACE_OS::sleep(1);
		source = GetServer(data.ExtractWord(1, "@ "));
	    }
	}
	else
	    source = data.ExtractWord(1,": ");
	if (!(source.Contains(".") || Parent->nickserv.IsLive(source)))
	{
	    PushUser(source, data);
	    return;
	}
        msgtype = data.ExtractWord(2,": ").UpperCase();
	if (data.WordCount(" ") > 2)
	    params = data.After(" ", 2);
    }
    else
    {
	source = OurUplink();
        msgtype = data.ExtractWord(1,": ").UpperCase();
	if (data.WordCount(" ") > 1)
	    params = data.After(" ");
    }
    if (source.empty())
	source = Parent->startup.Server_Name();
    mstring sourceL(source.LowerCase());


	if (msgtype=="UMODE2")
	{
	    if (source.Contains("."))
		return;

	    RLOCK(("UsingNick", sourceL));
	    Parent->nickserv.GetLive(sourceL).Mode(params.ExtractWord(1, ": "));
	}
	else if (msgtype=="UNGLINE")
	{
	    // We will ignore GLINES because they're not relivant to us.
	    // we will not be glining our own clients ;P
	}
	else if (msgtype=="UNSQLINE")
	{
	    // We will ignore SQLINES because they're not relivant to us.
	    // we will not be qlining our own clients ;P
	}
	else if (msgtype=="UNZLINE")
	{
	    // We will ignore ZLINES because they're not relivant to us.
	    // we will not be zlining our own clients ;P
	}
	else if (msgtype=="USER")
	{
	    if (!source.Contains("."))
		return;

	    // NEW USER
	    sourceL = params.ExtractWord(1, ": ").LowerCase();
	    WLOCK(("UsingNick", sourceL));

	    // DONT kill when we do SQUIT protection.
	    map<mstring,list<mstring> >::iterator i;
	    { RLOCK(("Server", "ToBeSquit"));
	    for (i=ToBeSquit.begin(); i!=ToBeSquit.end(); i++)
	    {
		list<mstring>::iterator k;
		WLOCK2(("Server", "ToBeSquit", i->first.LowerCase()));
		for (k=i->second.begin(); k!=i->second.end(); k++)
		    if (*k == sourceL)
		    {
		        list<mstring>::iterator j = k;  j--;
		        i->second.erase(k);
		        k=j;
		    }
	    }}

	    if (Parent->nickserv.IsLiveAll(sourceL))
	    {
	        // IF the squit server = us, and the signon time matches
	        if (Parent->nickserv.GetLive(sourceL).Squit().IsSameAs(params.ExtractWord(6, ": "), true) &&
	    	    Parent->nickserv.GetLive(sourceL).SignonTime() == mDateTime(static_cast<time_t>(atoul(params.ExtractWord(3, ": ")))))
	        {
	    	    Parent->nickserv.GetLive(sourceL).ClearSquit();
	    	    return;    // nice way to avoid repeating ones self :)
	        }
	        else
	        {
	    	    Parent->nickserv.GetLive(sourceL).Quit("SQUIT - " + Parent->nickserv.GetLive(sourceL).Server());
		    WLOCK2(("NickServ", "live"));
	    	    Parent->nickserv.RemLive(sourceL);
	        }
	    }

	    // In burst mode, make sure SERVER processing is done
	    { RLOCK(("IrcSvcHandler"));
	      RLOCK2(("Server", "proc_SERVER"));
	    if (proc_SERVER && Parent->ircsvchandler != NULL &&
		Parent->ircsvchandler->Burst())
	    {
		WLOCK2(("WaitEndServerAdd"));
	    }}
	    { WLOCK2(("Server", "proc_NICK")); proc_NICK++; }
	    RLOCK(("WaitEndNickAdd"));

	    { WLOCK2(("NickServ", "live"));
	      RLOCK2(("AddingServer"));
	    switch (proto.Signon())
	    {
	    case 0000: // USER nick user host server :realname
		{
		    Nick_Live_t tmp(
			params.ExtractWord(1, ": "),
			time(NULL),
			GetServer(params.ExtractWord(4, ": ")),
			params.ExtractWord(2, ": "),
			params.ExtractWord(3, ": "),
			params.After(":")
		    );
		    Parent->nickserv.AddLive(&tmp);
		}
		break;
	    case 0001: // USER nick time user host server :realname
		{
		    Nick_Live_t tmp(
			params.ExtractWord(1, ": "),
			static_cast<time_t>(atoul(params.ExtractWord(2, ": "))),
			GetServer(params.ExtractWord(5, ": ")),
			params.ExtractWord(3, ": "),
			params.ExtractWord(4, ": "),
			params.After(":")
		    );
		    Parent->nickserv.AddLive(&tmp);
		}
		break;
	    case 1000:
	    case 1001:
	    case 1002:
	    case 1003:
	    case 2000:
	    case 2001:
	    case 2002:
	    case 2003:
		break;
	    }}

	    if (Parent->nickserv.IsLive(sourceL))
	    {
		if (Parent->nickserv.GetLive(sourceL).Server().empty())
		{
		    PopUser(sourceL);
		    KILL(Parent->nickserv.FirstName(), sourceL,
				Parent->nickserv.GetLive(sourceL).RealName());
		    { WLOCK2(("Server", "proc_NICK")); proc_NICK--; }
		    return;
		}

		{ WLOCK2(("Server", "i_UserMax"));
		if (i_UserMax < Parent->nickserv.LiveSize())
		{
		    MCB(i_UserMax);
		    i_UserMax = Parent->nickserv.LiveSize();
		    MCE(i_UserMax);
		}}
		FlushUser(sourceL);

		// HAS to be AFTER the nickname is added to map.
		CommServ::list_t::iterator iter;
		mstring setmode;
		{ RLOCK2(("CommServ", "list"));
		for (iter = Parent->commserv.ListBegin();
				    iter != Parent->commserv.ListEnd();
				    iter++)
		{
		    if (iter->second.IsOn(sourceL))
		    {
			if (iter->first == Parent->commserv.ALL_Name())
			    setmode += Parent->commserv.ALL_SetMode();
			else if (iter->first == Parent->commserv.REGD_Name())
			    setmode += Parent->commserv.REGD_SetMode();
			else if (iter->first == Parent->commserv.OPER_Name())
			    setmode += Parent->commserv.OPER_SetMode();
			else if (iter->first == Parent->commserv.ADMIN_Name())
			    setmode += Parent->commserv.ADMIN_SetMode();
			else if (iter->first == Parent->commserv.SOP_Name())
			    setmode += Parent->commserv.SOP_SetMode();
			else if (iter->first == Parent->commserv.SADMIN_Name())
			    setmode += Parent->commserv.SADMIN_SetMode();
			MLOCK(("CommServ", "list", iter->first, "message"));
			for (iter->second.message = iter->second.MSG_begin();
			    iter->second.message != iter->second.MSG_end();
			    iter->second.message++)
			{
			    Parent->servmsg.send(sourceL, "[" + IRC_Bold +
					    iter->first + IRC_Off + "] " +
					    iter->second.message->Entry());
			}
		    }
		}}
		if (!setmode.empty())
		{
		    mstring setmode2;
		    for (unsigned int j=0; j<setmode.size(); j++)
		    {
			if (setmode[j] != '+' && setmode[j] != '-' &&
			    setmode[j] != ' ' &&
			    !Parent->nickserv.GetLive(sourceL).HasMode(setmode[j]))
			    setmode2 += setmode[j];
		    }
		    SVSMODE(Parent->nickserv.FirstName(), sourceL, "+" + setmode2);
		}
		if (Parent->nickserv.IsStored(sourceL))
		{
		    if (Parent->nickserv.GetStored(sourceL).Forbidden())
		    {
			Parent->nickserv.send(Parent->nickserv.FirstName(),
				sourceL, Parent->getMessage(sourceL, "ERR_SITUATION/FORBIDDEN"),
				ToHumanTime(Parent->nickserv.Ident(), sourceL).c_str());
		    }
		    else if (Parent->nickserv.GetStored(sourceL).Protect() &&
			!Parent->nickserv.GetStored(sourceL).IsOnline())
		   {
			Parent->nickserv.send(Parent->nickserv.FirstName(),
				sourceL, Parent->getMessage(sourceL, "ERR_SITUATION/PROTECTED"),
				ToHumanTime(Parent->nickserv.Ident(), sourceL).c_str());
		    }
		}
	    }
	    { WLOCK2(("Server", "proc_NICK")); proc_NICK--; }
	}
	else if (msgtype=="USERHOST")
	{
	    if (!params.empty())
	    {
		sraw("461 " + source + " USERHOST :Not enough paramaters");
	    }
	    else
	    {
		RLOCK(("UsingNick", params.ExtractWord(1, ": ").LowerCase()));
		if (Parent->nickserv.IsLive(params.ExtractWord(1, ": ")))
		{
		    mstring target = Parent->getLname(params.ExtractWord(1, ": "));
		    if (!Parent->nickserv.IsStored(target) ? 1 :
			!Parent->nickserv.GetStored(target).Private())
		    {
			sraw("302 " + source + " :" +
				Parent->nickserv.GetLive(target).Name() +
				"*=-" +
				Parent->nickserv.GetLive(target).User() +
				"@" +
				Parent->nickserv.GetLive(target).AltHost());
		    }
		    else
		    {
			sraw("302 " + source + " :" +
				Parent->nickserv.GetLive(target).Name() +
				"*=-" +
				Parent->nickserv.GetLive(target).User() +
				"@" + Parent->getMessage("VALS/ONLINE"));
		    }
		}
		else
		{
		    sraw("401 " + source + " " + params.ExtractWord(1, ": ") +
			" :No such nickname/channel.");
		}
	    }
	}
	else if (msgtype=="USERS")
	{
	    // :source USERS :our.server
	    sraw("446 " + source + " :USERS has been disabled");
	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
}

void Server::parse_V(const mstring &data)
{
    FT("Server::parse_V", (data));
    mstring source, msgtype, params;

    if (data[0u] == ':' || data[0u] == '@')
    {
	if (data[0u] == '@' && proto.Numeric())
	{
	    RLOCK(("ServerLinking"));
	    source = GetServer(data.ExtractWord(1, "@ "));
	    for (unsigned int i = 0; source.empty() && i < 10; i++)
	    {
		ACE_OS::sleep(1);
		source = GetServer(data.ExtractWord(1, "@ "));
	    }
	}
	else
	    source = data.ExtractWord(1,": ");
	if (!(source.Contains(".") || Parent->nickserv.IsLive(source)))
	{
	    PushUser(source, data);
	    return;
	}
        msgtype = data.ExtractWord(2,": ").UpperCase();
	if (data.WordCount(" ") > 2)
	    params = data.After(" ", 2);
    }
    else
    {
	source = OurUplink();
        msgtype = data.ExtractWord(1,": ").UpperCase();
	if (data.WordCount(" ") > 1)
	    params = data.After(" ");
    }
    if (source.empty())
	source = Parent->startup.Server_Name();
    mstring sourceL(source.LowerCase());


	if (msgtype=="VERSION")
	{
	    // :source VERSION :our.server
	    //:temple.magick.tm 351 ChanServ dal4.4.17. temple.magick.tm :AiMnW
	    mstring tmp(VERSION);
	    if(!RELEASE.empty())
		tmp+="-"+RELEASE;
	    if(!PATCH1.empty())
		tmp+="+"+PATCH1;
	    if(!PATCH2.empty())
		tmp+="+"+PATCH2;
	    if(!PATCH3.empty())
		tmp+="+"+PATCH3;
	    if(!PATCH4.empty())
		tmp+="+"+PATCH4;
	    if(!PATCH5.empty())
		tmp+="+"+PATCH5;
	    if(!PATCH6.empty())
		tmp+="+"+PATCH6;
	    if(!PATCH7.empty())
		tmp+="+"+PATCH7;
	    if(!PATCH8.empty())
		tmp+="+"+PATCH8;
	    if(!PATCH9.empty())
		tmp+="+"+PATCH9;
	    tmp << " [";
#ifdef HASCRYPT
	    tmp << "C";
#else
	    tmp << "c";
#endif
#ifdef MAGICK_TRACE_WORKS
	    tmp << "T";
#else
	    tmp << "t";
#endif
#ifdef MAGICK_LOCKS_WORK
	    tmp << "L";
#else
	    tmp << "l";
#endif
#ifdef MAGICK_HAS_EXCEPTIONS
	    tmp << "E";
#else
	    tmp << "e";
#endif
#if defined(MAGICK_USE_MPATROL) || defined(MAGICK_USE_EFENCE)
	    tmp << "D";
#else
	    tmp << "d";
#endif
	    tmp << "] Build #" << BUILD_NUMBER << " (" << BUILD_TIME <<
		") " << sysinfo_type() << " " << sysinfo_rel() << ".";
	    sraw("351 " + source + " " + PACKAGE + " " + Parent->startup.Server_Name() + " :" + tmp);
	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
}

void Server::parse_W(const mstring &data)
{
    FT("Server::parse_W", (data));
    mstring source, msgtype, params;

    if (data[0u] == ':' || data[0u] == '@')
    {
	if (data[0u] == '@' && proto.Numeric())
	{
	    RLOCK(("ServerLinking"));
	    source = GetServer(data.ExtractWord(1, "@ "));
	    for (unsigned int i = 0; source.empty() && i < 10; i++)
	    {
		ACE_OS::sleep(1);
		source = GetServer(data.ExtractWord(1, "@ "));
	    }
	}
	else
	    source = data.ExtractWord(1,": ");
	if (!(source.Contains(".") || Parent->nickserv.IsLive(source)))
	{
	    PushUser(source, data);
	    return;
	}
        msgtype = data.ExtractWord(2,": ").UpperCase();
	if (data.WordCount(" ") > 2)
	    params = data.After(" ", 2);
    }
    else
    {
	source = OurUplink();
        msgtype = data.ExtractWord(1,": ").UpperCase();
	if (data.WordCount(" ") > 1)
	    params = data.After(" ");
    }
    if (source.empty())
	source = Parent->startup.Server_Name();
    mstring sourceL(source.LowerCase());


	if (msgtype=="WALLOPS")
	{
	    // :source WALLOPS :text
	    // useless chatter ... can be ignored.
	}
	else if (msgtype=="WHO")
	{
//:ChanServ WHO
//:soul.darker.net 352 ChanServ #chatzone ~striker syd-56K-162.tpgi.com.au requiem.darker.net killkickabuseme H :2 Pain looks good on other people...
//:soul.darker.net 352 ChanServ * blah darker.net heaven.darker.net ChanServ H* :1 <bad-realname>
//:soul.darker.net 352 ChanServ #operzone ~somebody ABD2CEFD.ipt.aol.com requiem.darker.net Aimee H+ :2 anybody
//:soul.darker.net 352 ChanServ #chatzone jupiter modem035.memnoch.comcen.com.au vampire.darker.net Jupiter G :2 Jupiter
//:soul.darker.net 352 ChanServ * jason axe.net.au shadow.darker.net WauloK G :2 Nothing really matters..
//:soul.darker.net 352 ChanServ #chatzone allanon indra.darkshadow.net shadow.darker.net Allanon H*@ :2 God..
//:soul.darker.net 352 ChanServ #chatzone roamer shadow.darker.net shadow.darker.net FastinI G% :2 *
//:soul.darker.net 352 ChanServ * Tico-Rulez adsl1453.turboline.be vampire.darker.net tomaway H% :2 Captain Spartan, Dead Man Walking
//:soul.darker.net 352 ChanServ #darkernet ~ghost 203.58.108.129 vampire.darker.net Ghost_ G :2 fuck off
//:soul.darker.net 352 ChanServ #bothouse ~Nadu bservice.org vampire.darker.net Nadu H@ :2 BService Bot - #Bothouse - Owner: nicki
//:soul.darker.net 352 ChanServ #chatzone wildinbed cr1003083-a.crdva1.bc.wave.home.com vampire.darker.net wildrose H :2 Prairie Flower
//:soul.darker.net 352 ChanServ #operzone ~striker syd-56K-162.tpgi.com.au requiem.darker.net Lord_Striker G*@ :2 Pain looks good on other people...
//:soul.darker.net 352 ChanServ #chatzone ~jason axe.net.au vampire.darker.net Wau|oK G%@ :2 Nothing really matters..
//:soul.darker.net 352 ChanServ #operzone satan680 pc134.net19.ktv.koping.se vampire.darker.net Alien G*@ :2 am I GOD ?
//:soul.darker.net 352 ChanServ #chatzone prez castle.srealm.net.au soul.darker.net PreZ G*@ :0 I am what the people fear, I need not fear them.
//:soul.darker.net 352 ChanServ * ~bo pc34.net20.ktv.koping.se requiem.darker.net DarkMagic G% :2 Dark not Black
//:soul.darker.net 352 ChanServ * reaper darker.net hell.darker.net Magick-1 H% :2 Magick Outlet
//:soul.darker.net 352 ChanServ * reaper darker.net hell.darker.net Death H* :2 Global Noticer
//:soul.darker.net 352 ChanServ * reaper darker.net hell.darker.net OperServ H% :2 Operator Server
//:soul.darker.net 352 ChanServ #complaints reaper darker.net hell.darker.net DevNull H% :2 /dev/null -- message sink
//:soul.darker.net 352 ChanServ * reaper darker.net hell.darker.net MemoServ H :2 Memo Server
//:soul.darker.net 352 ChanServ * reaper darker.net hell.darker.net IrcIIHelp H :2 ircII Help Server
//:soul.darker.net 352 ChanServ * reaper darker.net hell.darker.net ServMsg H :2 Global Noticer
//:soul.darker.net 352 ChanServ * reaper darker.net hell.darker.net NickServ H :2 Nickname Server
//:soul.darker.net 352 ChanServ #magick prez castle.srealm.net.au soul.darker.net DarkMgk H%@ :0 Magick BridgeBot
//:soul.darker.net 315 ChanServ * :End of /WHO list.

//:ChanServ WHO PreZ
//:soul.darker.net 352 ChanServ #chatzone prez castle.srealm.net.au soul.darker.net PreZ G*@ :0 I am what the people fear, I need not fear them.
//:soul.darker.net 352 ChanServ #magick prez castle.srealm.net.au soul.darker.net DarkMgk H%@ :0 Magick BridgeBot
//:soul.darker.net 315 ChanServ PreZ :End of /WHO list.

//:ChanServ WHO #chatzone
//:soul.darker.net 352 ChanServ #chatzone ~striker syd-56K-162.tpgi.com.au requiem.darker.net killkickabuseme H :2 Pain looks good on other people...
//:soul.darker.net 352 ChanServ #chatzone ~somebody ABD2CEFD.ipt.aol.com requiem.darker.net Aimee H@ :2 anybody
//:soul.darker.net 352 ChanServ #chatzone jupiter modem035.memnoch.comcen.com.au vampire.darker.net Jupiter G :2 Jupiter
//:soul.darker.net 352 ChanServ #chatzone allanon indra.darkshadow.net shadow.darker.net Allanon H*@ :2 God..
//:soul.darker.net 352 ChanServ #chatzone ~ghost 203.58.108.129 vampire.darker.net Ghost_ G :2 fuck off
//:soul.darker.net 352 ChanServ #chatzone wildinbed cr1003083-a.crdva1.bc.wave.home.com vampire.darker.net wildrose H :2 Prairie Flower
//:soul.darker.net 315 ChanServ #chatzone :End of /WHO list.

//:ChanServ WHO vampire.darker.net
//:soul.darker.net 352 ChanServ #chatzone jupiter modem035.memnoch.comcen.com.au vampire.darker.net Jupiter G :2 Jupiter
//:soul.darker.net 352 ChanServ * Tico-Rulez adsl1453.turboline.be vampire.darker.net tomaway H% :2 Captain Spartan, Dead Man Walking
//:soul.darker.net 352 ChanServ #darkernet ~ghost 203.58.108.129 vampire.darker.net Ghost_ G :2 fuck off
//:soul.darker.net 352 ChanServ #bothouse ~Nadu bservice.org vampire.darker.net Nadu H@ :2 BService Bot - #Bothouse - Owner: nicki
//:soul.darker.net 352 ChanServ #chatzone wildinbed cr1003083-a.crdva1.bc.wave.home.com vampire.darker.net wildrose H :2 Prairie Flower
//:soul.darker.net 352 ChanServ #chatzone ~jason axe.net.au vampire.darker.net Wau|oK G%@ :2 Nothing really matters..
//:soul.darker.net 352 ChanServ #operzone satan680 pc134.net19.ktv.koping.se vampire.darker.net Alien G*@ :2 am I GOD ?
//:soul.darker.net 315 ChanServ vampire.darker.net :End of /WHO list.
	    { RLOCK(("IrcSvcHandler"));
	    if (Parent->ircsvchandler != NULL &&
		Parent->ircsvchandler->HTM_Level() > 3)
	    {
		mstring tmp;
		tmp.Format(Parent->getMessage(source, "MISC/HTM").c_str(),
								msgtype.c_str());
		raw(((proto.Tokens() && !proto.GetNonToken("NOTICE").empty()) ?
			proto.GetNonToken("NOTICE") : mstring("NOTICE")) +
			" " + source + " :" + tmp.c_str());
		return;
	    }}

	}
	else if (msgtype=="WHOIS")
	{
	    // :source WHOIS target :target
		//:ChanServ WHOIS PreZ :PreZ
		//:soul.darker.net 311 ChanServ PreZ prez castle.srealm.net.au * :I am what the people fear, I need not fear them.
		//:soul.darker.net 307 ChanServ PreZ :is a registered nick
		//:soul.darker.net 319 ChanServ PreZ :#chatzone #darkernet @#mrcoffee #pagan @#wicca
		//:soul.darker.net 312 ChanServ PreZ soul.darker.net :Show me a dream .. I'll show you my nightmare
		//:soul.darker.net 301 ChanServ PreZ :Automatically set away [SZon]  Away since Sun Jul 18 20:15
		//:soul.darker.net 313 ChanServ PreZ :is an IRC Operator
		//:soul.darker.net 310 ChanServ PreZ :looks very helpful.
		//:soul.darker.net 317 ChanServ PreZ 557 932291863 :seconds idle, signon time
		//:soul.darker.net 318 ChanServ PreZ :End of /WHOIS list.

	    mstring target = params.ExtractWord(1, ": ");
	    mstring targetL = target.LowerCase();
	    { RLOCK(("IrcSvcHandler"));
	    if (Parent->ircsvchandler != NULL &&
		Parent->ircsvchandler->HTM_Level() > 3)
	    {
		mstring tmp;
		tmp.Format(Parent->getMessage(source, "MISC/HTM").c_str(),
								msgtype.c_str());
		raw(((proto.Tokens() && !proto.GetNonToken("NOTICE").empty()) ?
			proto.GetNonToken("NOTICE") : mstring("NOTICE")) +
			" " + source + " :" + tmp.c_str());
		sraw("318 " + source + " " + target + " :End of /WHOIS list.");
		return;
	    }
	    else
	    {
		RLOCK2(("UsingNick", params.ExtractWord(1, ": ").LowerCase()));
		if (Parent->nickserv.IsLive(params.ExtractWord(1, ": ")))
		{
		    sraw("311 " + source + " " + target + " " + Parent->nickserv.GetLive(targetL).User() +
			" " + Parent->nickserv.GetLive(targetL).Host() + " * :" +
			Parent->nickserv.GetLive(targetL).RealName());

		    if (Parent->nickserv.GetLive(targetL).IsRecognized())
		    {
			sraw("307 " + source + " " + target + " : is a registered nick");
		    }

		    set<mstring> chans = Parent->nickserv.GetLive(targetL).Channels();
		    set<mstring>::iterator iter;
		    mstring outline = "319 " + source + " " + target + " :";
		    for (iter=chans.begin(); iter!=chans.end(); iter++)
		    {
			if (outline.size() + iter->size() > proto.MaxLine())
			{
			    sraw(outline);
			    outline = "319 " + source + " " + target + " :";
			}

			// Channel doesnt exist
			if (!Parent->chanserv.IsLive(*iter))
			    continue;

			// Channel +p or +s, and source not in chan
			if ((Parent->chanserv.GetLive(*iter).HasMode("s") ||
				Parent->chanserv.GetLive(*iter).HasMode("p")) &&
				!Parent->chanserv.GetLive(*iter).IsIn(source))
			    continue;

			if (Parent->chanserv.GetLive(*iter).IsOp(target))
			    outline += "@" + *iter + " ";
			else if (Parent->chanserv.GetLive(*iter).IsVoice(target))
			    outline += "+" + *iter + " ";
			else
			    outline += *iter + " ";
		    }
		    if (outline.After(":").length() > 0)
			sraw(outline);

		    if (Parent->nickserv.GetLive(targetL).IsServices())
			sraw("312 " + source + " " + target + " " + Parent->startup.Server_Name() +
				" :" + Parent->startup.Server_Desc());
		    else if (IsList(Parent->nickserv.GetLive(targetL).Server()))
			sraw("312 " + source + " " + target + " " + Parent->nickserv.GetLive(targetL).Server() +
				" :" + GetList(Parent->nickserv.GetLive(targetL).Server()).Description());

		    if (!Parent->nickserv.GetLive(targetL).Away().empty())
			sraw("301 " + source + " " + target + " :" + Parent->nickserv.GetLive(targetL).Away());

		    if (Parent->nickserv.GetLive(targetL).HasMode("o"))
			sraw("313 " + source + " " + target + " :is an IRC Operator");

		    if (Parent->nickserv.GetLive(targetL).HasMode("h"))
			sraw("310 " + source + " " + target + " :looks very helpful.");

		    if (Parent->nickserv.GetLive(targetL).IsServices())
		    {
    			mstring signon_idletime;
			signon_idletime << Parent->nickserv.GetLive(targetL).LastAction().SecondsSince()
				<< " " << Parent->nickserv.GetLive(targetL).SignonTime().timetstring();
			sraw("317 " + source + " " + target + " " + signon_idletime + " :seconds idle, signon time");
		    }

		    sraw("318 " + source + " " + target + " :End of /WHOIS list.");
		}
		else
		{
		    mstring target(params.ExtractWord(1, ": "));
		    sraw("401 " + source + " " + target + " :No such nickname/channel.");
		}
	    }}

	}
	else if (msgtype=="WHOWAS")
	{
	    // Dont store 'previous user' info, so ignore.
	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
}

void Server::parse_X(const mstring &data)
{
    FT("Server::parse_X", (data));
    mstring source, msgtype, params;

    if (data[0u] == ':' || data[0u] == '@')
    {
	if (data[0u] == '@' && proto.Numeric())
	{
	    RLOCK(("ServerLinking"));
	    source = GetServer(data.ExtractWord(1, "@ "));
	    for (unsigned int i = 0; source.empty() && i < 10; i++)
	    {
		ACE_OS::sleep(1);
		source = GetServer(data.ExtractWord(1, "@ "));
	    }
	}
	else
	    source = data.ExtractWord(1,": ");
	if (!(source.Contains(".") || Parent->nickserv.IsLive(source)))
	{
	    PushUser(source, data);
	    return;
	}
        msgtype = data.ExtractWord(2,": ").UpperCase();
	if (data.WordCount(" ") > 2)
	    params = data.After(" ", 2);
    }
    else
    {
	source = OurUplink();
        msgtype = data.ExtractWord(1,": ").UpperCase();
	if (data.WordCount(" ") > 1)
	    params = data.After(" ");
    }
    if (source.empty())
	source = Parent->startup.Server_Name();
    mstring sourceL(source.LowerCase());


	LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
}

void Server::parse_Y(const mstring &data)
{
    FT("Server::parse_Y", (data));
    mstring source, msgtype, params;

    if (data[0u] == ':' || data[0u] == '@')
    {
	if (data[0u] == '@' && proto.Numeric())
	{
	    RLOCK(("ServerLinking"));
	    source = GetServer(data.ExtractWord(1, "@ "));
	    for (unsigned int i = 0; source.empty() && i < 10; i++)
	    {
		ACE_OS::sleep(1);
		source = GetServer(data.ExtractWord(1, "@ "));
	    }
	}
	else
	    source = data.ExtractWord(1,": ");
	if (!(source.Contains(".") || Parent->nickserv.IsLive(source)))
	{
	    PushUser(source, data);
	    return;
	}
        msgtype = data.ExtractWord(2,": ").UpperCase();
	if (data.WordCount(" ") > 2)
	    params = data.After(" ", 2);
    }
    else
    {
	source = OurUplink();
        msgtype = data.ExtractWord(1,": ").UpperCase();
	if (data.WordCount(" ") > 1)
	    params = data.After(" ");
    }
    if (source.empty())
	source = Parent->startup.Server_Name();
    mstring sourceL(source.LowerCase());


	LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
}

void Server::parse_Z(const mstring &data)
{
    FT("Server::parse_Z", (data));
    mstring source, msgtype, params;

    if (data[0u] == ':' || data[0u] == '@')
    {
	if (data[0u] == '@' && proto.Numeric())
	{
	    RLOCK(("ServerLinking"));
	    source = GetServer(data.ExtractWord(1, "@ "));
	    for (unsigned int i = 0; source.empty() && i < 10; i++)
	    {
		ACE_OS::sleep(1);
		source = GetServer(data.ExtractWord(1, "@ "));
	    }
	}
	else
	    source = data.ExtractWord(1,": ");
	if (!(source.Contains(".") || Parent->nickserv.IsLive(source)))
	{
	    PushUser(source, data);
	    return;
	}
        msgtype = data.ExtractWord(2,": ").UpperCase();
	if (data.WordCount(" ") > 2)
	    params = data.After(" ", 2);
    }
    else
    {
	source = OurUplink();
        msgtype = data.ExtractWord(1,": ").UpperCase();
	if (data.WordCount(" ") > 1)
	    params = data.After(" ");
    }
    if (source.empty())
	source = Parent->startup.Server_Name();
    mstring sourceL(source.LowerCase());


	if (msgtype=="ZLINE")
	{
	    // We will ignore ZLINES because they're not relivant to us.
	    // we will not be zlining our own clients ;P
	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
}

void Server::numeric_execute(const mstring & data)
{
    FT("Server::numeric_execute", (data));

    mstring source, params;
    unsigned int msgtype;
    
    if (data[0u] == ':' || data[0u] == '@')
    {
	if (data[0u] == '@' && proto.Numeric())
	{
	    RLOCK(("ServerLinking"));
	    source = GetServer(data.ExtractWord(1, "@ "));
	    for (unsigned int i = 0; source.empty() && i < 10; i++)
	    {
		ACE_OS::sleep(1);
		source = GetServer(data.ExtractWord(1, "@ "));
	    }
	}
	else
	    source = data.ExtractWord(1,": ");
	if (!(source.Contains(".") || Parent->nickserv.IsLive(source)))
	{
	    PushUser(source, data);
	    return;
	}
        msgtype = atoi(data.ExtractWord(2,": "));
	if (params.WordCount(" ") > 2)
	    params = data.After(" ", 2);
    }
    else
    {
	source = OurUplink();
        msgtype = atoi(data.ExtractWord(1,": "));
	if (params.WordCount(" ") > 1)
	    params = data.After(" ", 1);
    }
    if (source.empty())
	source = Parent->startup.Server_Name();
    mstring sourceL(source.LowerCase());

    // Numerics direct from RFC1459
    // MOST we can (and will) ignore.

    switch (msgtype)
    {
    case 303:     // RPL_ISON
	{ RLOCK(("IrcSvcHandler"));
	if (Parent->ircsvchandler != NULL)
	{
	    Parent->ircsvchandler->EndBurst();
	}}
	if (!proto.Burst().empty())
	    Parent->server.sraw(((proto.Tokens() &&
		!proto.GetNonToken(proto.Burst()).empty()) ?
		proto.GetNonToken(proto.Burst()) : mstring(proto.Burst())));
	for (unsigned int i=1; i<=params.WordCount(": "); i++)
	{
	    // Remove clients from 'signon list' who are
	    // already on the network.
	    WLOCK(("Server", "WaitIsOn"));
	    if (WaitIsOn.find(params.ExtractWord(i, ": ").LowerCase()) != WaitIsOn.end())
		WaitIsOn.erase(params.ExtractWord(i, ": "));
	}

	if (WaitIsOn.size())
	{
	    set<mstring>::iterator k;
	    RLOCK(("Server", "WaitIsOn"));
	    for (k=WaitIsOn.begin(); k!=WaitIsOn.end(); k++)
	    {
		if (Parent->operserv.IsName(*k) &&
			!Parent->nickserv.IsLive(*k))
		    Parent->operserv.signon(*k);
		else if (Parent->nickserv.IsName(*k) &&
			!Parent->nickserv.IsLive(*k))
		    Parent->nickserv.signon(*k);
		else if (Parent->chanserv.IsName(*k) &&
			!Parent->nickserv.IsLive(*k))
		{
		    Parent->chanserv.signon(*k);

		    if (Parent->chanserv.FirstName() == *k)
		    {
			if (Parent->chanserv.Hide())
			    MODE(*k, "+i");
			mstring joinline;
			ChanServ::stored_t::iterator iter;
			map<mstring,mstring> modes;
			Chan_Live_t *clive;
			{ RLOCK(("ChanServ", "stored"));
			for (iter=Parent->chanserv.StoredBegin(); iter!=Parent->chanserv.StoredEnd(); iter++)
			{
			    if (Parent->chanserv.IsLive(iter->first))
				clive = &Parent->chanserv.GetLive(iter->first);
			    else
				clive = NULL;
			    // If its live and got JOIN on || not live and mlock +k or +i
			    if ((clive != NULL && iter->second.Join()) ||
				(clive == NULL && !iter->second.Forbidden() &&
				(!iter->second.Mlock_Key().empty() ||
				iter->second.Mlock_On().Contains("i"))))
			    {
				if (joinline.length())
				    joinline << ",";
				joinline << iter->first;
				if(clive == NULL)
				{
				    modes[iter->first] = "+s";
				    if (iter->second.Mlock_On().Contains("i"))
					modes[iter->first] += "i";
				    if(!iter->second.Mlock_Key().empty())
					modes[iter->first] += "k " +
						iter->second.Mlock_Key();
				}
				if (joinline.length() > proto.MaxLine())
				{
				    JOIN(Parent->chanserv.FirstName(), joinline);
				    joinline.erase();
				}
			    }
			    if (clive != NULL)
			    {
				if (!clive->Topic().empty())
				{
				    // Defer all the topic resets till we're done
				    clive->Topic(OurUplink(),
					clive->Topic(), clive->Topic_Setter(),
					clive->Topic_Set_Time());
				}
				else
				{
				    // Now lets carry over all topics at once.
				    if (iter->second.Keeptopic() &&
					!iter->second.Last_Topic().empty())
				    {
					Parent->server.TOPIC(*k,
					    iter->second.Last_Topic_Setter(),
					    iter->second.Name(),
					    iter->second.Last_Topic(),
					    iter->second.Last_Topic_Set_Time());
				    }
				}
			    }
			}}
			if (joinline.length())
			{
			    JOIN(Parent->chanserv.FirstName(), joinline);
			    joinline.erase();
			}
			map<mstring,mstring>::iterator i;
			for (i=modes.begin(); i!=modes.end(); i++)
			{
			    if (Parent->chanserv.IsLive(i->first))
				Parent->chanserv.GetLive(i->first).SendMode(i->second);
			}
		    }
		}
		else if (Parent->memoserv.IsName(*k) &&
			!Parent->nickserv.IsLive(*k))
		    Parent->memoserv.signon(*k);
		else if (Parent->commserv.IsName(*k) &&
			!Parent->nickserv.IsLive(*k))
		    Parent->commserv.signon(*k);
		else if (Parent->servmsg.IsName(*k) &&
			!Parent->nickserv.IsLive(*k))
		{
		    Parent->servmsg.signon(*k);
		    if (Parent->servmsg.FirstName() == *k)
			Parent->server.MODE(*k, "+o");
		}

		if (!(proto.P12() || (proto.Signon() >= 2000 && proto.Signon() < 3000)) &&
		    Parent->nickserv.IsLive(*k) && !Parent->startup.Setmode().empty())
		    Parent->server.MODE(*k, "+" + Parent->startup.Setmode());
		FlushMsgs(*k);
	    }
	}
	{ WLOCK(("Server", "WaitIsOn"));
	WaitIsOn.clear();
	}
	if (!proto.EndBurst().empty())
	    Parent->server.sraw(((proto.Tokens() &&
		!proto.GetNonToken(proto.EndBurst()).empty()) ?
		proto.GetNonToken(proto.EndBurst()) : mstring(proto.EndBurst())));
	break;
    case 436:     // ERR_NICKCOLLISION
	// MUST handle.
	break;
    case 464:     // ERR_PASSWDMISMATCH
	// MUST handle (Stop connecting).
	LOG((LM_ERROR, Parent->getLogMessage("OTHER/WRONGPASS"),
		Parent->CurrentServer().c_str()));
	Parent->Disconnect();
	break;
    case 465:     // ERR_YOUREBANNEDCREEP
	// MUST handle (Stop connecting).
	LOG((LM_ERROR, Parent->getLogMessage("OTHER/WEAREBANNED"),
		Parent->CurrentServer().c_str()));
	Parent->Disconnect();
	break;
    default:
	// We dont bother with what we get back -- useless anyway.
//	LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
//			data.c_str()));
	break;
    }
}

void Server::DumpB() const
{
    MB(0, (i_UserMax, ServerSquit.size(), ToBeSquit.size(),
	i_OurUplink, ToBeSent.size(), ToBeDone.size()));
}

void Server::DumpE() const
{
    ME(0, (i_UserMax, ServerSquit.size(), ToBeSquit.size(),
	i_OurUplink, ToBeSent.size(), ToBeDone.size()));
}
