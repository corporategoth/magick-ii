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
static const char *ident = "@(#)$Id$";
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
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

#include "server.h"
#include "lockable.h"
#include "magick.h"

void Protocol::SetTokens(unsigned int type)
{
    FT("Protocol::SetTokens", (type));

    tokens.clear();
    switch (type)
    {
    case 1000: /* ircu >= 2.10.x */
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
/*	tokens["ACTION"] = "CTCP ACTION";
	tokens["VERSION"] = "CTCP VERSION";
	tokens["PING"] = "CTCP PING";
	tokens["PONG"] = "CTCP PONG";
	tokens["CLIENTINFO"] = "CTCP CLIENTINFO";
	tokens["USERINFO"] = "CTCP USERINFO";
	tokens["TIME"] = "CTCP TIME";
	tokens["ERRMSG"] = "CTCP ERRMSG"; */
	break;

    case 0003:
	SetTokens(0001);
	tokens["e"] = "MODNICK";

    case 0002: /* Relic >= 2.1 */
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

    case 0001: /* DAL >= 4.4.15 */
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
    }
}

Protocol::Protocol()
{
    NFT("Protocol::Protocol");
    i_Number = 0;
    i_NickLen = 9;
    i_MaxLine = 450;
    i_Globops = false;
    i_Tokens = false;
    i_P12 = false;
    i_TSora = false;
    i_Akill = 0;
    i_Signon = 0000;
    i_Modes = 3;
    i_ChanModeArg = "ovbkl";
    i_Server = "SERVER %s %d :%s";
    i_Numeric = false;

    /* Leave these commented, since they do nothing ...
    i_Burst = "";
    i_EndBurst = "";
    i_Protoctl = "";
    i_SVSNICK = "";
    i_SVSMODE = "";
    i_SVSKILL = "";
    i_SVSNOOP = "";
    i_SQLINE = "";
    i_UNSQLINE = "";
    i_SVSHOST = "";
    */

    DumpB();
}

void Protocol::Set(unsigned int in)
{
    FT("Protocol::Set", (in));

    // WE NEVER set 'i_Tokens', thats set with the PROTCTL line.

    DumpB();
    switch (in)
    {
    case 0: /* RFC */
	i_Signon = 0000;
	i_Akill = 2000;
	SetTokens(0000);
	break;

    case 1: /* RFC with TS8 */
	i_Signon = 0001;
	i_Akill = 2000;
	SetTokens(0000);
	break;

    case 10: /* DAL < 4.4.15 */
	i_NickLen = 32;
	i_Signon = 1000;
	i_Globops = true;
	i_Akill = 1000;
	i_Modes = 4;
	SetTokens(0000);
	break;

    case 11: /* DAL >= 4.4.15 */
	i_NickLen = 32;
	i_Signon = 1001;
	i_Globops = true;
	i_Akill = 1000;
	i_Modes = 6;
	i_Protoctl = "PROTOCTL NOQUIT TOKEN WATCH=128 SAFELIST";
	i_SVSNICK = "SVSNICK";
	i_SVSMODE = "SVSMODE";
	i_SVSKILL = "SVSKILL";
	i_SVSNOOP = "SVSNOOP";
	i_SQLINE = "SQLINE";
	i_UNSQLINE = "UNSQLINE";
	SetTokens(0001);
	break;

    case 12: /* Bahamut */
	i_NickLen = 32;
	i_Signon = 2001;
	i_Globops = true;
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

    case 20: /* UnderNet < 2.10.x  */
	i_Signon = 1000;
	i_Akill = 2000;
	SetTokens(0000);
	break;

    case 21: /* UnderNet >= 2.10.x */
	i_Signon = 1000;
	i_Akill = 2000;
	i_Server = "";
	i_Server << "SERVER %s %d 0 0 P10 " << Parent->startup.Level() << " :%s";
	i_Numeric = true;
	i_Burst = "BURST";
	i_EndBurst = "END_OF_BURST";
	SetTokens(1000);
	break;

    case 30: /* Hybrid */
	i_Signon = 2000;
	SetTokens(0000);
	i_TSora = true;
	i_Protoctl = "CAPAB EX DE";
	break;

    case 40: /* Elite */
	i_NickLen = 32;
	i_Signon = 1001;
	i_Globops = true;
	i_Akill = 1000;
	i_SVSNICK = "SVSNICK";
	i_SVSMODE = "SVSMODE";
	i_SVSKILL = "SVSKILL";
	i_SVSNOOP = "SVSNOOP";
	i_SQLINE = "SQLINE";
	i_UNSQLINE = "UNSQLINE";
	SetTokens(0001);
	break;

    case 50: /* Relic 2.0 */
	i_NickLen = 32;
	i_Globops = true;
	i_Signon = 1001;
	i_Akill = 1000;
	i_Modes = 6;
	i_Server = "SERVER %s %d relic2.0 :%s";
	i_Protoctl = "PROTOCTL NOQUIT TOKEN WATCH=128 SAFELIST";
	i_SVSNICK = "SVSNICK";
	i_SVSMODE = "SVSMODE";
	i_SVSKILL = "SVSKILL";
	i_SVSNOOP = "SVSNOOP";
	i_SQLINE = "SQLINE";
	i_UNSQLINE = "UNSQLINE";
	SetTokens(0001);
	break;

    case 51: /* Relic 2.1 */
	i_NickLen = 32;
	i_Globops = true;
	i_P12 = true;
	i_Signon = 1003;
	i_Akill = 2002;
	i_Modes = 6;
	i_ChanModeArg = "ovbekl";
	i_Server = "SERVER %s %d relic2.1 :%s";
	i_Protoctl = "PROTOCTL NOQUIT TOKEN WATCH=128 SAFELIST";
	i_SVSNICK = "SVSNICK";
	i_SVSMODE = "SVSMODE";
	i_SVSKILL = "SVSKILL";
	i_SVSNOOP = "SVSNOOP";
	i_SQLINE = "SQLINE";
	i_UNSQLINE = "UNSQLINE";
	SetTokens(0002);
	break;

    case 53: /* Relic 4.0 */
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

    case 60: /* Aurora */
	i_NickLen = 32;
	i_Signon = 1002;
	i_Globops = true;
	i_Akill = 1000;
	i_Modes = 6;
	i_Protoctl = "PROTOCTL NOQUIT TOKEN WATCH=128 SAFELIST";
	i_SVSNICK = "SVSNICK";
	i_SVSMODE = "SVSMODE";
	i_SVSKILL = "SVSKILL";
	i_SVSNOOP = "SVSNOOP";
	i_SQLINE = "SQLINE";
	i_UNSQLINE = "UNSQLINE";
	i_SVSHOST = "SVSHOST";
	SetTokens(0001);
	break;

    case 70: /* Unreal */
	i_NickLen = 32;
	i_Signon = 1001;
	i_Globops = true;
	i_Akill = 1000;
	i_ChanModeArg = "ovbehkl";
	i_SVSNICK = "SVSNICK";
	i_SVSMODE = "SVSMODE";
	i_SVSKILL = "SVSKILL";
	i_SVSNOOP = "SVSNOOP";
	i_SQLINE = "SQLINE";
	i_UNSQLINE = "UNSQLINE";
	SetTokens(0001);
	break;

    case 80: /* UltimateIRCD */
	i_NickLen = 32;
	i_Signon = 1001;
	i_Globops = true;
	i_Akill = 1000;
	i_Modes = 6;
	i_Protoctl = "PROTOCTL NOQUIT TOKEN WATCH=128 SAFELIST";
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

mstring Protocol::GetToken(mstring in) const
{
    FT("Protocol::GetToken", (in));
    map<mstring,mstring>::const_iterator iter = tokens.find(in);
    if (iter == tokens.end())
	RET("");
    RET(iter->second);
}

mstring Protocol::GetNonToken(mstring in) const
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
    MB(0, (i_Number, i_NickLen, i_MaxLine, i_Globops, i_Tokens,
	i_P12, i_TSora, i_Akill, i_Signon, i_Modes, i_ChanModeArg,
	i_Server, i_Numeric, i_Burst, i_EndBurst, i_Protoctl));
    MB(16, (i_SVSNICK, i_SVSMODE, i_SVSKILL, i_SVSNOOP, i_SQLINE,
	i_UNSQLINE, i_SVSHOST, tokens.size()));
}

void Protocol::DumpE() const
{
    ME(0, (i_Number, i_NickLen, i_MaxLine, i_Globops, i_Tokens,
	i_P12, i_TSora, i_Akill, i_Signon, i_Modes, i_ChanModeArg,
	i_Server, i_Numeric, i_Burst, i_EndBurst, i_Protoctl));
    ME(16, (i_SVSNICK, i_SVSMODE, i_SVSKILL, i_SVSNOOP, i_SQLINE,
	i_UNSQLINE, i_SVSHOST, tokens.size()));
}

Server::Server(mstring name, mstring description)
{
    FT("Server::Server", (name, description));
    MLOCK(("Server", "ServerList", name.LowerCase()));
    i_Name = name.LowerCase();
    i_AltName = name.LowerCase();
    i_Uplink = Parent->startup.Server_Name().LowerCase();
    i_Hops = 0;
    i_Description = description;
    i_Ping = i_Lag = 0;
    i_Jupe = true;
    Parent->server.OurUplink(i_Name);
    DumpE();
}

Server::Server(mstring name, int hops, mstring description)
{
    FT("Server::Server", (name, hops, description));
    MLOCK(("Server", "ServerList", name.LowerCase()));
    i_Name = name.LowerCase();
    i_AltName = name.LowerCase();
    i_Uplink = Parent->startup.Server_Name().LowerCase();
    i_Hops = hops;
    i_Description = description;
    i_Ping = i_Lag = 0;
    i_Jupe = false;
    Parent->server.OurUplink(i_Name);
    DumpE();
}

Server::Server(mstring name, mstring uplink, int hops, mstring description)
{
    FT("Server::Server", (name, uplink, hops, description));
    MLOCK(("Server", "ServerList", name.LowerCase()));
    i_Name = name.LowerCase();
    i_AltName = name.LowerCase();
    i_Uplink = uplink.LowerCase();
    i_Hops = hops;
    i_Description = description;
    i_Ping = i_Lag = 0;
    i_Jupe = false;
    DumpE();
}

void Server::operator=(const Server &in)
{
    FT("Server::operator=", ("(const Server &) in"));
    MLOCK(("Server", "ServerList", in.i_Name.LowerCase()));
    i_Name = in.i_Name;
    i_AltName = in.i_AltName;
    i_Uplink = in.i_Uplink;
    i_Hops = in.i_Hops;
    i_Description = in.i_Description;
    i_Ping = in.i_Ping;
    i_Lag = in.i_Lag;
    i_Jupe = in.i_Jupe;
}

mstring Server::AltName() const
{
    NFT("Server::AltName");
    RLOCK(("Server", "ServerList", i_Name.LowerCase(), "i_AltName"));
    RET(i_AltName);
}

void Server::AltName(mstring in)
{
    FT("Server::AltName", (in));
    WLOCK(("Server", "ServerList", i_Name.LowerCase(), "i_AltName"));
    MCB(i_AltName);
    i_AltName = in;
    MCE(i_AltName);
}

unsigned int Server::Numeric() const
{
    NFT("Server::Numeric");
    RLOCK(("Server", "ServerList", i_Name.LowerCase(), "i_Numeric"));
    RET(i_Numeric);
}

void Server::Numeric(unsigned int in)
{
    FT("Server::Numeric", (in));
    WLOCK(("Server", "ServerList", i_Name.LowerCase(), "i_Numeric"));
    MCB(i_Numeric);
    i_Numeric = in;
    MCE(i_Numeric);
}

mstring Server::Uplink() const
{
    NFT("Server::Uplink");
    RLOCK(("Server", "ServerList", i_Name.LowerCase(), "i_Uplink"));
    RET(i_Uplink);
}

int Server::Hops() const
{
    NFT("Server::Hops");
    RLOCK(("Server", "ServerList", i_Name.LowerCase(), "i_Hops"));
    RET(i_Hops);
}

mstring Server::Description() const
{
    NFT("Server::Description");
    RLOCK(("Server", "ServerList", i_Name.LowerCase(), "i_Description"));
    RET(i_Description);
}

void Server::Ping()
{
    NFT("Server::Ping");

    WLOCK(("Server", "ServerList", i_Name.LowerCase(), "i_Ping"));
    if (!i_Ping)
    {
        Parent->server.sraw(((Parent->server.proto.Tokens() && Parent->server.proto.GetNonToken("PING") != "") ?
		Parent->server.proto.GetNonToken("PING") : mstring("PING")) +
		" " + Parent->startup.Server_Name() + " :" + i_Name);
	MCB(i_Ping);
	i_Ping = ACE_OS::gettimeofday().msec();
	MCE(i_Ping);
    }
}

void Server::Pong()
{
    NFT("Server::Pong");
    WLOCK(("Server", "ServerList", i_Name.LowerCase(), "i_Ping"));
    if (i_Ping)
    {
	WLOCK2(("Server", "ServerList", i_Name.LowerCase(), "i_Lag"));
	MCB(i_Lag);
	CB(1, i_Ping);
	i_Lag = ACE_OS::gettimeofday().msec() - i_Ping;
	COM(("The lag time of %s is %3.3f seconds.", i_Name.c_str(), i_Lag / 1000.0));
	i_Ping = 0;
	CE(1, i_Ping);
	MCE(i_Lag);
    }
}

float Server::Lag() const
{
    NFT("Server::Lag");
    RLOCK(("Server", "ServerList", i_Name.LowerCase(), "i_Lag"));
    float retval = i_Lag / 1000.0;
    RET(retval);
}

bool Server::Jupe() const
{
    NFT("Server::Jupe");
    RLOCK(("Server", "ServerList", i_Name.LowerCase(), "i_Jupe"));
    RET(i_Jupe);
}

unsigned int Server::Users() const
{
    NFT("Server::Users");

    unsigned int count = 0;
    map<mstring,Nick_Live_t>::iterator k;
    RLOCK(("NickServ", "live"));
    for (k=Parent->nickserv.live.begin(); k!=Parent->nickserv.live.end(); k++)
	if (k->second.Server() == i_Name)
	    count++;
    RET(count);
}

unsigned int Server::Opers() const
{
    NFT("Server::Opers");

    unsigned int count = 0;
    map<mstring,Nick_Live_t>::iterator k;
    RLOCK(("NickServ", "live"));
    for (k=Parent->nickserv.live.begin(); k!=Parent->nickserv.live.end(); k++)
	if (k->second.Server() == i_Name && k->second.HasMode("o"))
	    count++;
    RET(count);
}

vector<mstring> Server::Downlinks() const
{
    NFT("Server::Downlinks");
    vector<mstring> downlinks;
    map<mstring,Server>::iterator serv;

    RLOCK(("Server", "ServerList"));
    for(serv=Parent->server.ServerList.begin(); serv!=Parent->server.ServerList.end(); serv++)
    {
	if (serv->second.Uplink() == i_Name && i_Name != "")
	    downlinks.push_back(serv->first);
    }
    NRET(vector<mstring>, downlinks);
    
}

vector<mstring> Server::AllDownlinks() const
{
    NFT("Server::AllDownlinks");
    vector<mstring> downlinks, uplinks, uplinks2;
    map<mstring,Server>::iterator serv;
    bool found = false;

    RLOCK(("Server", "ServerList"));
    for(serv=Parent->server.ServerList.begin(); serv!=Parent->server.ServerList.end(); serv++)
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
	    for(serv=Parent->server.ServerList.begin(); serv!=Parent->server.ServerList.end(); serv++)
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

Server::~Server()
{
    NFT("Server::~Server");

    WLOCK(("Server", "ServerList", i_Name.LowerCase()));
    if (Parent->Shutdown())
	return;

    // Take my sublinks with me (who will take theirs ...)
    vector<mstring> Kill = Downlinks();
    COM(("Destroying %d more servers", Kill.size()));
    for (unsigned int i=0; i<Kill.size(); i++)
	Parent->server.ServerList.erase(Kill[i]);
    if (Kill.size() && Parent->server.OurUplink() == i_Name)
	Parent->server.OurUplink("");
}

size_t Server::Usage() const
{
    size_t retval = 0;

    WLOCK(("Server", "ServerList", i_Name.LowerCase()));
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

void Server::DumpB() const
{
    MB(0, (i_Name, i_AltName, i_Uplink, i_Hops, i_Description, i_Ping,
	i_Lag, i_Jupe));
}

void Server::DumpE() const
{
    ME(0, (i_Name, i_AltName, i_Uplink, i_Hops, i_Description, i_Ping,
	i_Lag, i_Jupe));
}

void NetworkServ::raw(mstring text) const
{
    FT("NetworkServ::raw", (text));
    Parent->send(text);
}

void NetworkServ::sraw(mstring text) const
{
    mstring out;
    out << ":";
    if (proto.Numeric())
	out << Parent->startup.Level();
    else
	out << Parent->startup.Server_Name();
    out << " " << text;
    raw(out);
}

void NetworkServ::SignOnAll()
{
    NFT("NetworkServ::SignOnAll");

    mstring doison;
    unsigned int i;
    WLOCK(("Server", "WaitIsOn"));
    for (i=1; i<=Parent->operserv.GetNames().WordCount(" "); i++)
    {
	if (!Parent->nickserv.IsLive(Parent->operserv.GetNames().ExtractWord(i, " ")))
	{
	    doison += " " + Parent->operserv.GetNames().ExtractWord(i, " ");
	    WaitIsOn.insert(Parent->operserv.GetNames().ExtractWord(i, " "));
	}
    }
    for (i=1; i<=Parent->nickserv.GetNames().WordCount(" "); i++)
    {
	if (!Parent->nickserv.IsLive(Parent->nickserv.GetNames().ExtractWord(i, " ")))
	{
	    doison += " " + Parent->nickserv.GetNames().ExtractWord(i, " ");
	    WaitIsOn.insert(Parent->nickserv.GetNames().ExtractWord(i, " "));
	}
    }
    for (i=1; i<=Parent->chanserv.GetNames().WordCount(" "); i++)
    {
	if (!Parent->nickserv.IsLive(Parent->chanserv.GetNames().ExtractWord(i, " ")))
	{
	    doison += " " + Parent->chanserv.GetNames().ExtractWord(i, " ");
	    WaitIsOn.insert(Parent->chanserv.GetNames().ExtractWord(i, " "));
	}
    }
    for (i=1; i<=Parent->memoserv.GetNames().WordCount(" "); i++)
    {
	if (!Parent->nickserv.IsLive(Parent->memoserv.GetNames().ExtractWord(i, " ")))
	{
	    doison += " " + Parent->memoserv.GetNames().ExtractWord(i, " ");
	    WaitIsOn.insert(Parent->memoserv.GetNames().ExtractWord(i, " "));
	}
    }
    for (i=1; i<=Parent->commserv.GetNames().WordCount(" "); i++)
    {
	if (!Parent->nickserv.IsLive(Parent->commserv.GetNames().ExtractWord(i, " ")))
	{
	    doison += " " + Parent->commserv.GetNames().ExtractWord(i, " ");
	    WaitIsOn.insert(Parent->commserv.GetNames().ExtractWord(i, " "));
	}
    }
    for (i=1; i<=Parent->servmsg.GetNames().WordCount(" "); i++)
    {
	if (!Parent->nickserv.IsLive(Parent->servmsg.GetNames().ExtractWord(i, " ")))
	{
	    doison += " " + Parent->servmsg.GetNames().ExtractWord(i, " ");
	    WaitIsOn.insert(Parent->servmsg.GetNames().ExtractWord(i, " "));
	}
    }

    if (doison != "")
	sraw(((proto.Tokens() && proto.GetNonToken("ISON") != "") ?
		proto.GetNonToken("ISON") : mstring("ISON")) +
		doison);
}


NetworkServ::NetworkServ()
{
    NFT("NetworkServ::NetworkServ");
    messages=true;
    WLOCK(("Server", "i_UserMax"));
    i_UserMax = 0;
    DumpE();
}

size_t NetworkServ::UserMax() const
{
    NFT("NetworkServ::UserMax");
    RLOCK(("Server", "i_UserMax"));
    RET(i_UserMax);
}

void NetworkServ::OurUplink(mstring server)
{
    FT("NetworkServ::OurUplink", (server));
    WLOCK(("Server", "i_OurUplink"));
    MCB(i_OurUplink);
    i_OurUplink = server;
    MCE(i_OurUplink);
}

mstring NetworkServ::OurUplink() const
{
    NFT("NetworkServ::OurUplink");
    RLOCK(("Server", "i_OurUplink"));
    RET(i_OurUplink);
}

void NetworkServ::FlushMsgs(mstring nick)
{
    FT("NetworkServ::FlushMsgs", (nick));

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
			(int) j->first, i->first.c_str(), j->third.first.c_str(),
			j->third.second.c_str(), j->third.third.c_str(),
			ToHumanTime(j->second.SecondsSince()).c_str()));
		break;
	    }
	}
	ToBeSent.erase(i->first);
    }
    MCE(ToBeSent.size());
}

void NetworkServ::FlushUser(mstring nick, mstring channel)
{
    FT("NetworkServ::FlushUser", (nick, channel));

    map<mstring, list<triplet<mDateTime, mstring, mstring> > >::iterator i;

    list<triplet<mDateTime, mstring, mstring> >::iterator j;
    list<triplet<mDateTime, mstring, mstring> > WorkList, WorkList2;
    vector<mstring> LastProc;

    // Dont report this, thats the point of the queue...
    if (!Parent->nickserv.IsLive(nick) ||
	Parent->nickserv.live[nick.LowerCase()].IsServices())
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
		 Parent->chanserv.live[channel.LowerCase()].IsIn(nick))))
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

void NetworkServ::PushUser(mstring nick, mstring message, mstring channel)
{
    FT("NetworkServ::PushUser", (nick, message, channel));
    // If the nick is reg'd and either a channel is not specified
    // or the channel exists and the user is in it, just do it,
    // else queue it.
    if (((Parent->nickserv.IsLive(nick))) &&
	((channel.empty()) || (Parent->chanserv.IsLive(channel) &&
	Parent->chanserv.live[channel.LowerCase()].IsIn(nick))))
    {
	execute(message);
    }
    else
    {
	WLOCK(("Server", "ToBeDone", nick.LowerCase()));
	WLOCK(("Server", "ToBeDone"));
	MCB(ToBeDone.size());
	ToBeDone[nick.LowerCase()].push_back(
		triplet<mDateTime, mstring, mstring>(
		Now(), message, channel.LowerCase()));
	MCE(ToBeDone.size());
    }
}

void NetworkServ::PopUser(mstring nick, mstring channel)
{
    FT("NetworkServ::PopUser", (nick, channel));


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


bool NetworkServ::IsServer(mstring server) const
{
    FT("NetworkServ::IsServer", (server));
    RLOCK(("Server", "ServerList"));
    bool retval = (ServerList.find(server.LowerCase()) != ServerList.end());
    RET(retval);
}


mstring NetworkServ::ServerNumeric(unsigned int num) const
{
    FT("NetworkServ::ServerNumeric", (num));
    mstring retval;
    RLOCK(("Server", "ServerList"));
    map<mstring, Server>::const_iterator iter;
    for (iter=ServerList.begin(); iter!=ServerList.end(); iter++)
	if (iter->second.Numeric() == num)
	{
	    retval = iter->second.Name();
	    break;
	}
    RET(retval);
}


bool NetworkServ::IsSquit(mstring server) const
{
    FT("NetworkServ::IsSquit", (server));
    RLOCK(("Server", "ServerSquit"));
    bool retval = (ServerSquit.find(server.LowerCase()) != ServerSquit.end());
    RET(retval);
}

void NetworkServ::Jupe(mstring server, mstring reason)
{
    FT("NetworkServ::Jupe", (server, reason));
    if (IsServer(server))
	raw(((proto.Tokens() && proto.GetNonToken("SQUIT") != "") ?
		proto.GetNonToken("SQUIT") : mstring("SQUIT")) +
		" " + server.LowerCase() + " :JUPE command used.");
	    // SERVER downlink hops :description
	    // :uplink SERVER downlink hops :description
    mstring tmp;
    tmp.Format(proto.Server().c_str(),
	    server.LowerCase().c_str(), 2,
	    ("JUPED (" + reason + ")").c_str());
    raw(tmp);
    WLOCK(("Server", "ServerList"));
    Parent->server.ServerList[server.LowerCase()] =
		    Server(server.LowerCase(), "JUPED (" + reason + ")");
}

void NetworkServ::AKILL(mstring host, mstring reason, unsigned long time, mstring killer)
{
    FT("NetworkServ::AKILL", (host, reason, time, killer));

    if (!host.Contains("@"))
	return;

    mstring line;
    switch (proto.Akill())
    {
    case 0000:
	break;
    case 1000:
	if (proto.Tokens() && proto.GetNonToken("AKILL") != "")
	    line << proto.GetNonToken("AKILL");
	else
	    line << "AKILL";
	line << " " << host.After("@") << " " << host.Before("@") <<
		" :" << reason;
	break;
    case 1001:
	if (proto.Tokens() && proto.GetNonToken("AKILL") != "")
	    line << proto.GetNonToken("AKILL");
	else
	    line << "AKILL";
	line << " " << host.After("@") << " " << host.Before("@") <<
		" " << time << " " << ((killer != "") ? killer :
		Parent->operserv.FirstName()) << " " <<
		(time_t) Now() << " :" << reason;
	break;
    case 2000:
	if (proto.Tokens() && proto.GetNonToken("GLINE") != "")
	    line << proto.GetNonToken("GLINE");
	else
	    line << "GLINE";
	line << " * +" << time << " " << host << " :" << reason;
	break;
    case 2001:
	if (proto.Tokens() && proto.GetNonToken("GLINE") != "")
	    line << proto.GetNonToken("GLINE");
	else
	    line << "GLINE";
	line << " * +" << host << " " << time << " :" << reason;
	break;
    case 2002:
	if (proto.Tokens() && proto.GetNonToken("GLINE") != "")
	    line << proto.GetNonToken("GLINE");
	else
	    line << "GLINE";
	line << " +" << host << " " << time << " :" << reason;
	break;
    }

    // GLINING clients do this for us ...
    if (proto.Akill() < 2000)
    {
	map<mstring,Nick_Live_t>::iterator nlive;
	vector<mstring> killusers;
	{ RLOCK(("NickServ", "live"));
	for (nlive = Parent->nickserv.live.begin(); nlive != Parent->nickserv.live.end(); nlive++)
	{
	    if (nlive->second.Mask(Nick_Live_t::U_P_H).After("!").Matches(host, true))
		killusers.push_back(nlive->first);
	}}

	unsigned int j;
	for (j=0; j<killusers.size(); j++)
	    Parent->server.KILL(killer, killusers[j], reason);
    }

    if (line != "")
	sraw(line);
}

void NetworkServ::ANONKILL(mstring nick, mstring dest, mstring reason)
{
    FT("NetworkServ::ANONKILL", (nick, dest, reason));

    if (!Parent->nickserv.IsLive(nick))
    {
	WLOCK(("Server", "ToBeSent", nick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[nick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_KILL, Now(), triplet<mstring, mstring, mstring>(
		dest, reason, "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Parent->nickserv.live[nick.LowerCase()].IsServices())
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
	Parent->nickserv.live[dest.LowerCase()].Quit(
		"Killed (" + reason + ")");
	WLOCK(("NickServ", "live"));
	Parent->nickserv.live.erase(dest.LowerCase());
	raw(":" + nick + " " +
		((proto.Tokens() && proto.GetNonToken("KILL") != "") ?
			proto.GetNonToken("KILL") : mstring("KILL")) +
		" " + dest + " :" + reason);
    }
}


void NetworkServ::AWAY(mstring nick, mstring reason)
{
    FT("NetworkServ::AWAY", (nick, reason));

    if (!Parent->nickserv.IsLive(nick))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONUSER"),
		"AWAY", nick.c_str()));
    }
    else if (!Parent->nickserv.live[nick.LowerCase()].IsServices())
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"AWAY", nick.c_str()));
    }
    else
    {
	mstring line;
	Parent->nickserv.live[nick.LowerCase()].Away(reason);
	if (proto.Tokens() && proto.GetNonToken("AWAY") != "")
	    line << proto.GetNonToken("AWAY");
	else
	    line << "AWAY";
	if(reason != "")
	    line=" : " + reason;
	raw(":" + nick + " " +  line);
    }
}


void NetworkServ::GLOBOPS(mstring nick, mstring message)
{
    FT("NetworkServ::GLOBOPS", (nick, message));

    if (!Parent->nickserv.IsLive(nick))
    {
	WLOCK(("Server", "ToBeSent", nick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[nick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_GLOBOPS, Now(), triplet<mstring, mstring, mstring>(
		message, "", "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Parent->nickserv.live[nick.LowerCase()].IsServices())
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"GLOBOPS", nick.c_str()));
    }
    else
    {
	Parent->nickserv.live[nick.LowerCase()].Action();
	if (proto.Globops())
	    raw(":" + nick + " " +
		((proto.Tokens() && proto.GetNonToken("GLOBOPS") != "") ?
			proto.GetNonToken("GLOBOPS") : mstring("GLOBOPS")) +
		" :" + message);
	else
	    raw(":" + nick + " " +
		((proto.Tokens() && proto.GetNonToken("WALLOPS") != "") ?
			proto.GetNonToken("WALLOPS") : mstring("WALLOPS")) +
		" :" + message);
    }
}


void NetworkServ::INVITE(mstring nick, mstring dest, mstring channel)
{
    FT("NetworkServ::INVITE", (nick, dest, channel));

    if (!Parent->nickserv.IsLive(nick))
    {
	WLOCK(("Server", "ToBeSent", nick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[nick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_INVITE, Now(), triplet<mstring, mstring, mstring>(
		dest, channel, "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Parent->nickserv.live[nick.LowerCase()].IsServices())
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
		((proto.Tokens() && proto.GetNonToken("INVITE") != "") ?
			proto.GetNonToken("INVITE") : mstring("INVITE")) +
		" " + dest + " :" + channel);
    }
}


void NetworkServ::JOIN(mstring nick, mstring channel)
{
    FT("NetworkServ::JOIN", (nick, channel));

    if (!Parent->nickserv.IsLive(nick))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONUSER"),
		"JOIN", nick.c_str()));
    }
    else if (!Parent->nickserv.live[nick.LowerCase()].IsServices())
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"JOIN", nick.c_str()));
    }
    else
    {
	for (unsigned int i=0; i<channel.WordCount(", "); i++)
	    Parent->nickserv.live[nick.LowerCase()].Join(channel.ExtractWord(i+1, ", "));
	raw(":" + nick + " " +
		((proto.Tokens() && proto.GetNonToken("JOIN") != "") ?
			proto.GetNonToken("JOIN") : mstring("JOIN")) +
		" :" + channel);
    }
}


void NetworkServ::KICK(mstring nick, mstring dest, mstring channel, mstring reason)
{
    FT("NetworkServ::KICK", (nick, dest, channel, reason));

    if (!Parent->nickserv.IsLive(nick))
    {
	WLOCK(("Server", "ToBeSent", nick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[nick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_KICK, Now(), triplet<mstring, mstring, mstring>(
		dest, channel, reason)));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Parent->nickserv.live[nick.LowerCase()].IsServices())
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
    else if (!Parent->chanserv.live[channel.LowerCase()].IsIn(dest))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_NOTINCHAN"),
		"KICK", nick.c_str(), dest.c_str(), channel.c_str()));
    }
    else
    {
	Parent->nickserv.live[dest.LowerCase()].Kick(nick, channel);
	raw(":" + nick + " " +
		((proto.Tokens() && proto.GetNonToken("KICK") != "") ?
			proto.GetNonToken("KICK") : mstring("KICK")) +
		" " + channel + " " + dest + " :" + reason);
    }
}


void NetworkServ::KILL(mstring nick, mstring dest, mstring reason)
{
    FT("NetworkServ::KILL", (nick, dest, reason));

    if (!Parent->nickserv.IsLive(nick))
    {
	WLOCK(("Server", "ToBeSent", nick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[nick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_KILL, Now(), triplet<mstring, mstring, mstring>(
		dest, reason, "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Parent->nickserv.live[nick.LowerCase()].IsServices())
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
	Parent->nickserv.live[dest.LowerCase()].Quit(
		"Killed (" + nick + " (" + reason + "))");
	Parent->nickserv.live.erase(dest.LowerCase());
	raw(":" + nick + " " +
		((proto.Tokens() && proto.GetNonToken("KILL") != "") ?
			proto.GetNonToken("KILL") : mstring("KILL")) +
		" " + dest + " :" + Parent->nickserv.live[nick.LowerCase()].Host() +
		"!" + nick + " (" + reason + ")");
    }
}


void NetworkServ::MODE(mstring nick, mstring mode)
{
    FT("NetworkServ::MODE", (nick, mode));

    if (!Parent->nickserv.IsLive(nick))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONUSER"),
		"MODE", nick.c_str()));
    }
    else if (!Parent->nickserv.live[nick.LowerCase()].IsServices())
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"MODE", nick.c_str()));
    }
    else
    {
	Parent->nickserv.live[nick.LowerCase()].Mode(mode);
	raw(":" + nick + " " +
		((proto.Tokens() && proto.GetNonToken("MODE") != "") ?
			proto.GetNonToken("MODE") : mstring("MODE")) +
		" " + nick + " :" + mode);
    }
}


void NetworkServ::MODE(mstring nick, mstring channel, mstring mode) 
{
    FT("NetworkServ::MODE", (nick, channel, mode));

    if (!Parent->nickserv.IsLive(nick))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONUSER"),
		"MODE", nick.c_str()));
    }
    else if (!Parent->nickserv.live[nick.LowerCase()].IsServices())
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
	Parent->chanserv.live[channel.LowerCase()].Mode(nick, mode);
	raw(":" + nick + " " +
		((proto.Tokens() && proto.GetNonToken("MODE") != "") ?
			proto.GetNonToken("MODE") : mstring("MODE")) +
		" " + channel + " " + mode.Before(" ") + " " +
		mode.After(" "));
    }
}


void NetworkServ::NICK(mstring nick, mstring user, mstring host,
    	mstring server, mstring realname)
{
    FT("NetworkServ::NICK", (nick, user, host, server, realname));

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
	    if (proto.Tokens() && proto.GetNonToken(token) != "")
		send << proto.GetNonToken(token);
	    else
		send << token;
	    send << " " << nick  << " " << user << " " << host <<
		" " << server << " :" << realname;
	    break;
	case 0001:
	    token = "USER";
	    if (proto.Tokens() && proto.GetNonToken(token) != "")
		send << proto.GetNonToken(token);
	    else
		send << token;
	    send << " " << nick  << Now().timetstring() <<
		" " << user << " " << host << " " << server <<
		" :" << realname;
	    break;
	case 1000:
	    token = "NICK";
	    if (proto.P12())
		token = "SNICK";
	    if (proto.Tokens() && proto.GetNonToken(token) != "")
		send << proto.GetNonToken(token);
	    else
		send << token;
	    send << " " << nick << " 1 " << Now().timetstring() <<
		" " << user << " " << host << " " << server;
	    if (proto.P12())
		send << " +" << Parent->startup.Setmode();
	    send << " :" << realname;
	    break;
	case 1001:
	    token = "NICK";
	    if (proto.P12())
		token = "SNICK";
	    if (proto.Tokens() && proto.GetNonToken(token) != "")
		send << proto.GetNonToken(token);
	    else
		send << token;
	    send << " " << nick << " 1 " << Now().timetstring() <<
		" " << user << " " << host << " " << server << " 1";
	    if (proto.P12())
		send << " +" << Parent->startup.Setmode();
	    send << " :" << realname;
	    break;
	case 1002:
	    token = "NICK";
	    if (proto.P12())
		token = "SNICK";
	    if (proto.Tokens() && proto.GetNonToken(token) != "")
		send << proto.GetNonToken(token);
	    else
		send << token;
	    send << " " << nick << " 1 " << Now().timetstring() <<
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
	    if (proto.Tokens() && proto.GetNonToken(token) != "")
		send << proto.GetNonToken(token);
	    else
		send << token;
	    send << " " << nick << " 1 " << Now().timetstring() <<
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
	    if (proto.Tokens() && proto.GetNonToken(token) != "")
		send << proto.GetNonToken(token);
	    else
		send << token;
	    send << " " << nick << " 1 " << Now().timetstring() <<
		" +" << Parent->startup.Setmode() << " " << user <<
		" " << host << " " << server << " :" << realname;
	    break;
	case 2001:
	    token = "NICK";
	    if (proto.P12())
		token = "SNICK";
	    if (proto.Tokens() && proto.GetNonToken(token) != "")
		send << proto.GetNonToken(token);
	    else
		send << token;
	    send << " " << nick << " 1 " << Now().timetstring() <<
		" +" << Parent->startup.Setmode() << " " << user <<
		" " << host << " " << server << " 1 :" << realname;
	    break;
	case 2002:
	    token = "NICK";
	    if (proto.P12())
		token = "SNICK";
	    if (proto.Tokens() && proto.GetNonToken(token) != "")
		send << proto.GetNonToken(token);
	    else
		send << token;
	    send << " " << nick << " 1 " << Now().timetstring() <<
		" +" << Parent->startup.Setmode() << " " << user <<
		" " << host << " " << host << " " << server << " 0 :" <<
		realname;
	    break;
	}
	// Sign ourselves in ...

	{ WLOCK(("NickServ", "live"));
	if (Parent->nickserv.IsLiveAll(nick.LowerCase()))
	{
	    Parent->nickserv.live[nick.LowerCase()].Quit("SQUIT - " + Parent->nickserv.live[nick.LowerCase()].Server());
	    Parent->nickserv.live.erase(nick.LowerCase());
	}
	Parent->nickserv.live[nick.LowerCase()] = Nick_Live_t(
		nick, user, host, realname);
	if (proto.P12() || (proto.Signon() >= 2000 && proto.Signon() < 3000))
		Parent->nickserv.live[nick.LowerCase()].Mode(
			Parent->startup.Setmode());
	}
	{ WLOCK2(("Server", "i_UserMax"));
	if (i_UserMax < Parent->nickserv.live.size())
	{
	    MCB(i_UserMax);
	    i_UserMax = Parent->nickserv.live.size();
	    MCE(i_UserMax);
	}}
	raw(send);
    }
}


void NetworkServ::NICK(mstring oldnick, mstring newnick)
{
    FT("NetworkServ::NICK", (oldnick, newnick));

    if (!Parent->nickserv.IsLive(oldnick))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONUSER"),
		"NICK", oldnick.c_str()));
    }
    else if (!Parent->nickserv.live[oldnick.LowerCase()].IsServices())
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"NICK", oldnick.c_str()));
    }
    else
    {
	{ WLOCK(("NickServ", "live"));
	if (Parent->nickserv.IsLiveAll(newnick.LowerCase()))
	{
	    Parent->nickserv.live[newnick.LowerCase()].Quit("SQUIT - " + Parent->nickserv.live[newnick.LowerCase()].Server());
	    Parent->nickserv.live.erase(newnick.LowerCase());
	}
	Parent->nickserv.live[newnick.LowerCase()] =
		Parent->nickserv.live[oldnick.LowerCase()];
	Parent->nickserv.live.erase(oldnick.LowerCase());
	}
	Parent->nickserv.live[newnick.LowerCase()].Name(newnick);
	raw(":" + oldnick + " " +
		((proto.Tokens() && proto.GetNonToken("NICK") != "") ?
			proto.GetNonToken("NICK") : mstring("NICK")) +
		" " + newnick + (proto.TSora() ?
			" :" + Now().timetstring() :
			mstring("")));
    }
}


void NetworkServ::NOTICE(mstring nick, mstring dest, mstring text)
{
    FT("NetworkServ::NOTICE", (nick, dest, text));

    if (!Parent->nickserv.IsLive(nick))
    {
	WLOCK(("Server", "ToBeSent", nick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[nick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_NOTICE, Now(), triplet<mstring, mstring, mstring>(
		dest, text, "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Parent->nickserv.live[nick.LowerCase()].IsServices())
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
	Parent->nickserv.live[nick.LowerCase()].Action();
	raw(":" + nick + " " +
		((proto.Tokens() && proto.GetNonToken("NOTICE") != "") ?
			proto.GetNonToken("NOTICE") : mstring("NOTICE")) +
		" " + dest + " :" + text);
    }
}


void NetworkServ::PART(mstring nick, mstring channel, mstring reason)
{
    FT("NetworkServ::PART", (nick, channel, reason));

    if (!Parent->nickserv.IsLive(nick))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONUSER"),
		"PART", nick.c_str()));
    }
    else if (!Parent->nickserv.live[nick.LowerCase()].IsServices())
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"PART", nick.c_str()));
    }
    else if (!Parent->chanserv.IsLive(channel))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONCHAN"),
		"PART", nick.c_str(), channel.c_str()));
    }
    else if (!Parent->chanserv.live[channel.LowerCase()].IsIn(nick))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_NOTINCHAN"),
		"KICK", nick.c_str(), nick.c_str(), channel.c_str()));
    }
    else
    {
	Parent->nickserv.live[nick.LowerCase()].Part(channel);
	mstring tmpResult;
	if(reason!="")
	    tmpResult=" :"+reason;
	else
	    tmpResult="";
	raw(":" + nick + " " +
		((proto.Tokens() && proto.GetNonToken("PART") != "") ?
			proto.GetNonToken("PART") : mstring("PART")) +
		" " + channel + tmpResult);
    }
}


void NetworkServ::PRIVMSG(mstring nick, mstring dest, mstring text)
{
    FT("NetworkServ::PRIVMSG", (nick, dest, text));

    if (!Parent->nickserv.IsLive(nick))
    {
	WLOCK(("Server", "ToBeSent", nick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[nick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_PRIVMSG, Now(), triplet<mstring, mstring, mstring>(
		dest, text, "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Parent->nickserv.live[nick.LowerCase()].IsServices())
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
	Parent->nickserv.live[nick.LowerCase()].Action();
	raw(":" + nick + " " +
		((proto.Tokens() && proto.GetNonToken("PRIVMSG") != "") ?
			proto.GetNonToken("PRIVMSG") : mstring("PRIVMSG")) +
		" " + dest + " :" + text);
    }
}


void NetworkServ::SQLINE(mstring nick, mstring target, mstring reason)
{
    FT("NetworkServ::SQLINE", (nick, target,reason));

    if (proto.SQLINE().empty())
	return;

    if (!Parent->nickserv.IsLive(nick))
    {
	WLOCK(("Server", "ToBeSent", nick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[nick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_SQLINE, Now(), triplet<mstring, mstring, mstring>(
		target, reason, "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Parent->nickserv.live[nick.LowerCase()].IsServices())
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		proto.SQLINE().c_str(), nick.c_str()));
    }
    else
    {
	mstring output;
	output << ":" << nick << " ";
	if (proto.Tokens() && proto.GetNonToken(proto.SQLINE()) != "")
	    output << proto.GetNonToken(proto.SQLINE());
	else
	    output << proto.SQLINE();
	output << " " << target << " :" << reason;
	raw(output);
    }
}


void NetworkServ::QUIT(mstring nick, mstring reason)
{
    FT("NetworkServ::QUIT", (nick, reason));

    if (!Parent->nickserv.IsLive(nick))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONUSER"),
		"QUIT", nick.c_str()));
    }
    else if (!Parent->nickserv.live[nick.LowerCase()].IsServices())
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"QUIT", nick.c_str()));
    }
    else
    {
	Parent->nickserv.live[nick.LowerCase()].Quit(reason);
	WLOCK(("NickServ", "live"));
	Parent->nickserv.live.erase(nick.LowerCase());
	raw(":" + nick + " " +
		((proto.Tokens() && proto.GetNonToken("QUIT") != "") ?
			proto.GetNonToken("QUIT") : mstring("QUIT")) +
		" :" + reason);
    }
}


void NetworkServ::RAKILL(mstring host)
{
    FT("NetworkServ::RAKILL", (host));

    if (!host.Contains("@"))
	return;

    mstring line;
    switch (proto.Akill())
    {
    case 0000:
	break;
    case 1000:
	if (proto.Tokens() && proto.GetNonToken("RAKILL") != "")
	    line << proto.GetNonToken("RAKILL");
	else
	    line << "RAKILL";
	line << " " << host.After("@") << " " << host.Before("@");
	break;
    case 1001:
	if (proto.Tokens() && proto.GetNonToken("RAKILL") != "")
	    line << proto.GetNonToken("RAKILL");
	else
	    line << "RAKILL";
	line << " " << host.After("@") << " " << host.Before("@");
	break;
    case 2000:
	if (proto.Tokens() && proto.GetNonToken("UNGLINE") != "")
	    line << proto.GetNonToken("UNGLINE");
	else
	    line << "UNGLINE";
	line << " * " << host;
	break;
    case 2001:
	if (proto.Tokens() && proto.GetNonToken("GLINE") != "")
	    line << proto.GetNonToken("GLINE");
	else
	    line << "GLINE";
	line << " * -" << host;
	break;
    case 2002:
	if (proto.Tokens() && proto.GetNonToken("GLINE") != "")
	    line << proto.GetNonToken("GLINE");
	else
	    line << "GLINE";
	line << " -" << host;
	break;
    }
    if (line != "")
	sraw(line);
}

void NetworkServ::SVSHOST(mstring mynick, mstring nick, mstring newhost)
{
    FT("NetworkServ::SVSHOST", (mynick, nick, newhost));

    if (proto.SVSHOST().empty())
	return;

    if (!Parent->nickserv.IsLive(mynick))
    {
	WLOCK(("Server", "ToBeSent", mynick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[mynick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_SVSHOST, Now(), triplet<mstring, mstring, mstring>(
		nick, newhost, "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Parent->nickserv.live[mynick.LowerCase()].IsServices())
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
	Parent->nickserv.live[nick.LowerCase()].AltHost(newhost);
	mstring output;
	output << ":" << mynick << " ";
	if (proto.Tokens() && proto.GetNonToken(proto.SVSHOST()) != "")
	    output << proto.GetNonToken(proto.SVSHOST());
	else
	    output << proto.SVSHOST();
	output << " " << nick << " " << newhost << " :" << Now().timetstring();
	raw(output);
    }
}


void NetworkServ::SVSKILL(mstring mynick, mstring nick, mstring reason)
{
    FT("NetworkServ::SVSKILL", (mynick, nick, reason));

    if (proto.SVSKILL().empty())
	return;

    if (!Parent->nickserv.IsLive(mynick))
    {
	WLOCK(("Server", "ToBeSent", mynick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[mynick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_SVSKILL, Now(), triplet<mstring, mstring, mstring>(
		nick, reason, "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Parent->nickserv.live[mynick.LowerCase()].IsServices())
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
	Parent->nickserv.live[nick.LowerCase()].Quit(reason);
	WLOCK(("NickServ", "live"));
	Parent->nickserv.live.erase(nick.LowerCase());
	mstring output;
	output << ":" << mynick << " ";
	if (proto.Tokens() && proto.GetNonToken(proto.SVSKILL()) != "")
	    output << proto.GetNonToken(proto.SVSKILL());
	else
	    output << proto.SVSKILL();
	output << " " << nick << " " << reason;
	raw(output);
    }
}


void NetworkServ::SVSNICK(mstring mynick, mstring nick, mstring newnick)
{
    FT("NetworkServ::SVSNICK", (mynick, nick, newnick));

    if (proto.SVSNICK().empty())
	return;

    if (!Parent->nickserv.IsLive(mynick))
    {
	WLOCK(("Server", "ToBeSent", mynick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[mynick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_SVSNICK, Now(), triplet<mstring, mstring, mstring>(
		nick, newnick, "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Parent->nickserv.live[mynick.LowerCase()].IsServices())
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
	if (proto.Tokens() && proto.GetNonToken(proto.SVSNICK()) != "")
	    output << proto.GetNonToken(proto.SVSNICK());
	else
	    output << proto.SVSNICK();
	output << " " << nick << " " <<
		    newnick << " :" << time_t(NULL);
	raw(output);
    }
}


void NetworkServ::SVSNOOP(mstring nick, mstring server, bool onoff)
{
    FT("NetworkServ::SVSNOOP", (nick, server, onoff));

    if (proto.SVSNOOP().empty())
	return;

    if (!Parent->nickserv.IsLive(nick))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONUSER"),
		proto.SVSNOOP().c_str(), nick.c_str()));
    }
    else if (!Parent->nickserv.live[nick.LowerCase()].IsServices())
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		proto.SVSNOOP().c_str(), nick.c_str()));
    }
    else if (!IsServer(server))
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_FORNONSERVER"),
		proto.SVSNOOP().c_str(), nick.c_str(), server.c_str()));
    }
    else
    {
	mstring output;
	output << ":" << nick << " ";
	if (proto.Tokens() && proto.GetNonToken(proto.SVSNOOP()) != "")
	    output << proto.GetNonToken(proto.SVSNOOP());
	else
	    output << proto.SVSNOOP();
	output << " " + server + " " + mstring(onoff ? "+" : "-");
	raw(output);
    }
}


void NetworkServ::SVSMODE(mstring mynick, mstring nick, mstring mode)
{
    FT("NetworkServ::SVSMODE", (mynick, nick, mode));

    if (proto.SVSMODE().empty())
	return;

    if (!Parent->nickserv.IsLive(mynick))
    {
	WLOCK(("Server", "ToBeSent", mynick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[mynick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_SVSMODE, Now(), triplet<mstring, mstring, mstring>(
		nick, mode, "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Parent->nickserv.live[mynick.LowerCase()].IsServices())
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
	Parent->nickserv.live[nick.LowerCase()].Mode(mode);
	mstring output;
	output << ":" << mynick << " ";
	if (proto.Tokens() && proto.GetNonToken(proto.SVSMODE()) != "")
	    output << proto.GetNonToken(proto.SVSMODE());
	else
	    output << proto.SVSMODE();
	output << " " << nick << " " << mode;
	raw(output);
    }
}


void NetworkServ::TOPIC(mstring nick, mstring setter, mstring channel, mstring topic, mDateTime time)
{
    FT("NetworkServ::TOPIC", (nick, setter, channel, topic, time));

    
    if (!Parent->nickserv.IsLive(nick))
    {
	WLOCK(("Server", "ToBeSent", nick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[nick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_TOPIC, Now(), triplet<mstring, mstring, mstring>(
		setter, channel, topic)));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Parent->nickserv.live[nick.LowerCase()].IsServices())
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
		Parent->nickserv.live[nick.LowerCase()].Mask(Nick_Live_t::N_U_P_H).c_str()));
	mstring send;
	send << ":" << nick << " ";
	if (proto.Tokens() && proto.GetNonToken("TOPIC") != "")
	    send << proto.GetNonToken("TOPIC");
	else
	    send << "TOPIC";
	if (topic.empty())
	    send << " " << channel << " " << setter;
	else
	    send << " " << channel << " " <<
		setter << " " << time.timetstring() << " :" << topic;

	Parent->chanserv.live[channel.LowerCase()].Topic(nick, topic, setter, time);
	raw(send);
    }
}


void NetworkServ::UNSQLINE(mstring nick, mstring target)
{
    FT("NetworkServ::UNSQLINE", (nick, target));

    if (!proto.UNSQLINE().empty())
	return;

    if (!Parent->nickserv.IsLive(nick))
    {
	WLOCK(("Server", "ToBeSent", nick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[nick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_UNSQLINE, Now(), triplet<mstring, mstring, mstring>(
		target, "", "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Parent->nickserv.live[nick.LowerCase()].IsServices())
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		proto.UNSQLINE().c_str(), nick.c_str()));
    }
    else
    {
	mstring output;
	output << ":" << nick << " ";
	if (proto.Tokens() && proto.GetNonToken(proto.UNSQLINE()) != "")
	    output << proto.GetNonToken(proto.UNSQLINE());
	else
	    output << proto.UNSQLINE();
	output << " " << target;
	raw(output);
    }
}


void NetworkServ::WALLOPS(mstring nick, mstring message)
{
    FT("NetworkServ::WALLOPS", (nick, message));

    if (!Parent->nickserv.IsLive(nick))
    {
	WLOCK(("Server", "ToBeSent", nick.LowerCase()));
	MCB(ToBeSent.size());
	ToBeSent[nick.LowerCase()].push_back(
		triplet<send_type, mDateTime, triplet<mstring, mstring, mstring> >(
		t_WALLOPS, Now(), triplet<mstring, mstring, mstring>(
		message, "", "")));
	MCE(ToBeSent.size());
	return;
    }
    else if (!Parent->nickserv.live[nick.LowerCase()].IsServices())
    {
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/REQ_BYNONSERVICE"),
		"WALLOPS", nick.c_str()));
    }
    else
    {
	Parent->nickserv.live[nick.LowerCase()].Action();
	raw(":" + nick + " " +
		((proto.Tokens() && proto.GetNonToken("WALLOPS") != "") ?
			proto.GetNonToken("WALLOPS") : mstring("WALLOPS")) +
		" :" + message);
    }
}


void NetworkServ::KillUnknownUser(mstring user) const
{
    FT("NetworkServ::KillUnknownUser", (user));
    raw(":" + Parent->startup.Server_Name() + " " +
	((proto.Tokens() && proto.GetNonToken("KILL") != "") ?
		proto.GetNonToken("KILL") : mstring("KILL")) +
    	" " + user + " :" + Parent->startup.Server_Name() +
    	" (" + user + "(?) <- " + Parent->Server() + ")");
    LOG((LM_ERROR, Parent->getLogMessage("OTHER/KILL_UNKNOWN"),
	user.c_str()));
}


unsigned int NetworkServ::SeenMessage(mstring data)
{
    FT("NetworkServ::SeenMessage", (data));

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

    ReDoMessages[data] = pair<unsigned int,mDateTime>(times, Now());

    RET(times);
}


void NetworkServ::execute(const mstring & data)
{
    //okay this is the main networkserv command switcher
    FT("NetworkServ::execute", (data));

    mstring source, sourceL, msgtype;
    if (data[0u]==':')
    {
        source=data.ExtractWord(1,": ");
	if (proto.Numeric() && source.IsNumber())
	    source = ServerNumeric(atoi(source.c_str()));
	sourceL=source.LowerCase();
        msgtype=data.ExtractWord(2,": ").UpperCase();
	if (!(source.Contains(".") || Parent->nickserv.IsLive(source)))
	{
		PushUser(source, data);
		return;
	}
    }
    else
    {
        msgtype=data.ExtractWord(1,": ").UpperCase();
    }

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
		raw(((proto.Tokens() && proto.GetNonToken("NOTICE") != "") ?
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
	    if (source.Contains(".") || source.empty())
		return;

	    // :source AWAY
	    // :source AWAY :This is my reason
	    if (data.ExtractWord(3, ": ").empty())
	    {
		Parent->nickserv.live[sourceL].Away("");

		// HAS to be AFTER the nickname is added to map.
		map<mstring, Committee>::iterator iter;
		RLOCK(("CommServ", "list"));
		for (iter = Parent->commserv.list.begin();
				    iter != Parent->commserv.list.end();
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
		}
	    }
	    else
		Parent->nickserv.live[sourceL].Away(data.After(":", 2));
	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
	break;
    case 'B':
	if (msgtype=="BURST")
	{
	    // Thanks to bahamut :)
	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
	break;
    case 'C':
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
	    if (IsServer(data.ExtractWord(3, ": ")) ||
	    	data.ExtractWord(3, ": ").LowerCase() == Parent->startup.Server_Name().LowerCase())
	    {
		raw(((proto.Tokens() && proto.GetNonToken("NOTICE") != "") ?
			proto.GetNonToken("NOTICE") : mstring("NOTICE")) +
			" " + source + " :Connect: Server " + data.ExtractWord(3, ": ") +
			" already exists from " + Parent->startup.Server_Name());
	    }
	    else
	    {
		raw(((proto.Tokens() && proto.GetNonToken("NOTICE") != "") ?
			proto.GetNonToken("NOTICE") : mstring("NOTICE")) +
			" " + source + " :Connect: Host " +
			data.ExtractWord(3, ": ") + " not listed in irc.conf");
	    }
	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
	break;
    case 'D':
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	break;
    case 'E':
	if (msgtype=="END_OF_BURST" || msgtype=="EOB")
	{
	    // Tis only nice, afterall ...
	    Parent->server.sraw(((proto.Tokens() &&
		proto.GetNonToken("EOB_ACK") != "") ?
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
		    msgtype.c_str(), data.After(":").c_str()));
	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
	break;
    case 'F':
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	break;
    case 'G':
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
	    if (source != "")
	    {
		LOG((LM_NOTICE, Parent->getLogMessage("OTHER/SERVER_MSG"),
		    msgtype.c_str(), data.After(":", 2).c_str()));
	    }
	    else
	    {
		LOG((LM_NOTICE, Parent->getLogMessage("OTHER/SERVER_MSG"),
		    msgtype.c_str(), data.After(":").c_str()));
	    }
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
	break;
    case 'H':
	if (msgtype=="HELP")
	{
	    // ignore ...
	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
	break;
    case 'I':
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
		raw(((proto.Tokens() && proto.GetNonToken("NOTICE") != "") ?
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
	    mstring isonstr = "";
	    for (unsigned int i=3; i<=data.WordCount(": "); i++)
	    {
		if (isonstr.length() > proto.MaxLine())
		{
		    sraw("303 " + source + " :" + isonstr);
		    isonstr = "";
		}
		if (Parent->nickserv.IsLive(data.ExtractWord(i, ": ")))
		    isonstr += data.ExtractWord(i, ": ") + " ";
	    }
	    if (isonstr != "")
		sraw("303 " + source + " :" + isonstr);
	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
	break;
    case 'J':
	if (msgtype=="JOIN")
	{
	    if (source.Contains(".") || source.empty())
		return;

	    // :source JOIN :#channel
	    for (unsigned int i=3; i<=data.WordCount(":, "); i++)
	    {
		mstring chan(data.ExtractWord(i, ":, "));
		// If we're IN channel, then we may be cycling ...
		if (Parent->chanserv.IsLive(chan) &&
		    Parent->chanserv.live[chan.LowerCase()].IsIn(sourceL) &&
		    SeenMessage(data) < Parent->config.MSG_Seen_Act())
		{
		    mBase::push_message(data.Before(" ", 2) + " " + chan);
		}
		else
		{
		    MLOCK(("BlockChannel", chan.LowerCase()));
		    Parent->nickserv.live[sourceL].Join(chan);
		}
	    }
	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
	break;
    case 'K':
	if (msgtype=="KICK")
	{
	    // :source KICK #channel target :reason

	    // KICK for UNKNOWN USER.
	    if (!Parent->nickserv.IsLive(data.ExtractWord(4, ": ")))
	    {
		KillUnknownUser(data.ExtractWord(4, ": "));
		return;
	    }

	    if (!(source.Contains(".") || source.empty()) &&
		!Parent->nickserv.live[sourceL].IsInChan(data.ExtractWord(3, ": ")))
		sraw(((proto.Tokens() && proto.GetNonToken("KICK") != "") ?
			proto.GetNonToken("KICK") : mstring("KICK")) +
			" " + data.ExtractWord(3, ": ") + " " + source + " :You are not in this channel");

	    // NOTE: as the message has already been broadcasted,
	    // we still need to acomodate for it.
	    Parent->nickserv.live[data.ExtractWord(4, ": ").LowerCase()].Kick(source, data.ExtractWord(3, ": "));
	}
	else if (msgtype=="KILL")
	{
	    // :source/server KILL target :reason
	    // LOCAL clients ONLY (remotes are a QUIT).
	    //:PreZ KILL kick`kill`abuse :castle.srealm.net.au!PreZ (blah)
	    if (Parent->nickserv.IsLive(data.ExtractWord(3, ": ")))
	    {
		// sign on services again if they're killed.
		if (Parent->nickserv.live[data.ExtractWord(3, ": ").LowerCase()].IsServices())
		{
		    LOG((LM_WARNING, Parent->getLogMessage("OTHER/KILLED"),
			    data.ExtractWord(3, ": ").c_str(),
			    (!Parent->nickserv.IsLive(sourceL) ? sourceL.c_str() :
			    Parent->nickserv.live[sourceL].Mask(Nick_Live_t::N_U_P_H).c_str())));
		    WLOCK(("Server", "WaitIsOn"));
		    WaitIsOn.insert(data.ExtractWord(3, ": "));
		    sraw(((proto.Tokens() && proto.GetNonToken("ISON") != "") ?
			proto.GetNonToken("ISON") : mstring("ISON")) +
			" " + data.ExtractWord(3, ": "));
		}
		int wc = data.After(":", 2).WordCount("!");
		Parent->nickserv.live[data.ExtractWord(3, ": ").LowerCase()].Quit(
			"Killed (" + data.After(":", 2).After("!", wc-1) + ")");
		WLOCK(("NickServ", "live"));
		Parent->nickserv.live.erase(data.ExtractWord(3, ": ").LowerCase());
	    }
	    else
	    {
		LOG((LM_ERROR, Parent->getLogMessage("ERROR/REC_FORNONUSER"),
			"KILL", source.c_str(), data.ExtractWord(3, ": ").c_str()));
	    }

	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
	break;
    case 'L':
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
		raw(((proto.Tokens() && proto.GetNonToken("NOTICE") != "") ?
			proto.GetNonToken("NOTICE") : mstring("NOTICE")) +
			" " + source + " :" + tmp.c_str());
		return;
	    }
	    else
	    {
		sraw("364 " + source + " " + Parent->startup.Server_Name() + " " +
		    Parent->startup.Server_Name() + " :0 " + Parent->startup.Server_Desc());

		map<mstring,Server>::iterator serv;
		RLOCK(("Server", "ServerList"));
		for(serv=Parent->server.ServerList.begin(); serv!=Parent->server.ServerList.end(); serv++)
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
		raw(((proto.Tokens() && proto.GetNonToken("NOTICE") != "") ?
			proto.GetNonToken("NOTICE") : mstring("NOTICE")) +
			" " + source + " :" + tmp.c_str());
		return;
	    }
	    else
	    {
		map<mstring,Chan_Live_t>::iterator chan;
		RLOCK(("ChanServ", "live"));
		for (chan=Parent->chanserv.live.begin(); chan!=Parent->chanserv.live.end(); chan++)
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
	break;
    case 'M':
	if (msgtype=="MODE")
	{
	    // :source MODE source :mode
	    // :source MODE #channel mode params...
	    // :server MODE #channel mode params... creationtime
	    if (IsChan(data.ExtractWord(3, ": ")))
	    {
		if (Parent->chanserv.IsLive(data.ExtractWord(3, ": ")))
		{
		    MLOCK(("BlockChannel", data.ExtractWord(3, ": ").LowerCase()));
		    Parent->chanserv.live[data.ExtractWord(3, ": ").LowerCase()].Mode(source, data.After(" ", 3));
		}
		else
		{
		    if (source.Contains("."))
		    {
			if (SeenMessage(data) >= Parent->config.MSG_Seen_Act())
			{
			    LOG((LM_CRITICAL, Parent->getLogMessage("ERROR/REC_FORNONCHAN"),
				"MODE", source.c_str(), data.ExtractWord(3, ": ").c_str()));
			}
			else
			    mBase::push_message(data);
		    }
		    else
		    {
			PushUser(source, data, data.ExtractWord(3, ": "));
		    }
		}
	    }
	    else
	    {
		if (Parent->nickserv.IsLive(data.ExtractWord(3, ": ")))
		{
		    Parent->nickserv.live[data.ExtractWord(3, ": ").LowerCase()].Mode(data.ExtractWord(4, ": "));
		}
		else
		{
		    if (source.Contains("."))
		    {
			if (SeenMessage(data) >= Parent->config.MSG_Seen_Act())
			{
			    LOG((LM_CRITICAL, Parent->getLogMessage("ERROR/REC_FORNONCHAN"),
				"MODE", source.c_str(), data.ExtractWord(3, ": ").c_str()));
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
		    raw(((proto.Tokens() && proto.GetNonToken("NOTICE") != "") ?
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
	break;
    case 'N':
	if (msgtype=="NAMES")
	{
	    // :source NAMES #channel our.server
//:soul.darker.net 353 ChanServ = #chatzone :killkickabuseme @Aimee Jupiter @Allanon Ghost_ wildrose
//:soul.darker.net 366 ChanServ #chatzone :End of /NAMES list.
	    sraw("366 " + source + " " + data.ExtractWord(3, ": ") + " :End of /NAMES list.");

	}
	else if (msgtype=="NETINFO")
	{
	    // Ignore -- unrealism
	}
	else if (msgtype=="NICK")
	{
	    if (source.Contains("."))
		return;

	    if (source.empty()) {
		// NEW USER
		sourceL = data.ExtractWord(2, ": ").LowerCase();

		// DONT kill when we do SQUIT protection.
		map<mstring,list<mstring> >::iterator i;
		{ RLOCK(("Server", "ToBeSquit"));
		for (i=ToBeSquit.begin(); i!=ToBeSquit.end(); i++)
		{
		    list<mstring>::iterator k;
		    WLOCK(("Server", "ToBeSquit", i->first.LowerCase()));
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
		    if (Parent->nickserv.live[sourceL].Squit() == data.ExtractWord(7, ": ").LowerCase()
			&& Parent->nickserv.live[sourceL].SignonTime() == mDateTime((time_t) atol(data.ExtractWord(4, ": "))))
		    {
			Parent->nickserv.live[sourceL].ClearSquit();
			return;    // nice way to avoid repeating ones self :)
		    }
		    else
		    {
			WLOCK(("NickServ", "live"));
			Parent->nickserv.live[sourceL].Quit("SQUIT - " + Parent->nickserv.live[sourceL].Server());
			Parent->nickserv.live.erase(sourceL);
		    }
		}

		{ WLOCK(("NickServ", "live"));
		switch (proto.Signon())
		{
		case 0000:
		case 0001:
		    break;
		case 1000: // NICK nick hops time user host server :realname
		    Parent->nickserv.live[sourceL] =
			Nick_Live_t(
			    data.ExtractWord(2, ": "),
			    (time_t) atol(data.ExtractWord(4, ": ")),
			    data.ExtractWord(7, ": "),
			    data.ExtractWord(5, ": "),
			    data.ExtractWord(6, ": "),
			    data.After(":")
			);
		    break;
		case 1001: // NICK nick hops time user host server 1 :realname
		    Parent->nickserv.live[sourceL] =
			Nick_Live_t(
			    data.ExtractWord(2, ": "),
			    (time_t) atol(data.ExtractWord(4, ": ")),
			    data.ExtractWord(7, ": "),
			    data.ExtractWord(5, ": "),
			    data.ExtractWord(6, ": "),
			    data.After(":")
			);
		    break;
		case 1002: // NICK nick hops time user host server 0 real-host :realname
		    Parent->nickserv.live[sourceL] =
			Nick_Live_t(
			    data.ExtractWord(2, ": "),
			    (time_t) atol(data.ExtractWord(4, ": ")),
			    data.ExtractWord(7, ": "),
			    data.ExtractWord(5, ": "),
			    data.ExtractWord(9, ": "),
			    data.After(":")
			);
		    Parent->nickserv.live[sourceL].AltHost(data.ExtractWord(6, ": "));
		    break;
		case 1003: // NICK nick hops time user real-host host server 0 :realname
		    Parent->nickserv.live[sourceL] =
			Nick_Live_t(
			    data.ExtractWord(2, ": "),
			    (time_t) atol(data.ExtractWord(4, ": ")),
			    data.ExtractWord(8, ": "),
			    data.ExtractWord(5, ": "),
			    data.ExtractWord(6, ": "),
			    data.After(":")
			);
		    Parent->nickserv.live[sourceL].AltHost(data.ExtractWord(7, ": "));
		    break;
		case 2000: // NICK nick hops time mode user host server :realname
		    Parent->nickserv.live[sourceL] =
			Nick_Live_t(
			    data.ExtractWord(2, ": "),
			    (time_t) atol(data.ExtractWord(4, ": ")),
			    data.ExtractWord(8, ": "),
			    data.ExtractWord(6, ": "),
			    data.ExtractWord(7, ": "),
			    data.After(":")
			);
		    Parent->nickserv.live[sourceL].Mode(data.ExtractWord(5, ": "));
		    break;
		case 2001: // NICK nick hops time mode user host server 0 :realname
		    Parent->nickserv.live[sourceL] =
			Nick_Live_t(
			    data.ExtractWord(2, ": "),
			    (time_t) atol(data.ExtractWord(4, ": ")),
			    data.ExtractWord(8, ": "),
			    data.ExtractWord(6, ": "),
			    data.ExtractWord(7, ": "),
			    data.After(":")
			);
		    Parent->nickserv.live[sourceL].Mode(data.ExtractWord(5, ": "));
		    break;
		case 2002: // NICK nick hops time mode user host maskhost server 0 :realname
		    Parent->nickserv.live[sourceL] =
			Nick_Live_t(
			    data.ExtractWord(2, ": "),
			    (time_t) atol(data.ExtractWord(4, ": ")),
			    data.ExtractWord(9, ": "),
			    data.ExtractWord(6, ": "),
			    data.ExtractWord(7, ": "),
			    data.After(":")
			);
		    Parent->nickserv.live[sourceL].Mode(data.ExtractWord(5, ": "));
		    Parent->nickserv.live[sourceL].AltHost(data.ExtractWord(8, ": "));
		    break;
		}}
		if (Parent->nickserv.IsLive(sourceL))
		{
		    if (Parent->nickserv.live[sourceL].Server().empty())
		    {
			PopUser(sourceL);
			KILL(Parent->nickserv.FirstName(), sourceL,
				Parent->nickserv.live[sourceL].RealName());
			return;
		    }

		    { WLOCK(("Server", "i_UserMax"));
		    if (i_UserMax < Parent->nickserv.live.size())
		    {
			MCB(i_UserMax);
			i_UserMax = Parent->nickserv.live.size();
			MCE(i_UserMax);
		    }}
		    FlushUser(sourceL);

		    // HAS to be AFTER the nickname is added to map.
		    map<mstring, Committee>::iterator iter;
		    mstring setmode;
		    { RLOCK(("CommServ", "list"));
		    for (iter = Parent->commserv.list.begin();
				    iter != Parent->commserv.list.end();
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
		    if (setmode != "")
		    {
			mstring setmode2;
			for (unsigned int j=0; j<setmode.size(); j++)
			{
			    if (setmode[j] != '+' && setmode[j] != '-' &&
				setmode[j] != ' ' &&
				!Parent->nickserv.live[sourceL].HasMode(setmode[j]))
			        setmode2 += setmode[j];
			}
			SVSMODE(Parent->nickserv.FirstName(), sourceL, "+" + setmode2);
		    }
		    if (Parent->nickserv.IsStored(sourceL))
		    {
			if (Parent->nickserv.stored[sourceL].Forbidden())
			{
			    Parent->nickserv.send(Parent->nickserv.FirstName(),
				sourceL, Parent->getMessage(sourceL, "ERR_SITUATION/FORBIDDEN"),
				ToHumanTime(Parent->nickserv.Ident(), sourceL).c_str());
			}
			else if (Parent->nickserv.stored[sourceL].Protect() &&
		    	     !Parent->nickserv.stored[sourceL].IsOnline())
			{
			    Parent->nickserv.send(Parent->nickserv.FirstName(),
				sourceL, Parent->getMessage(sourceL, "ERR_SITUATION/PROTECTED"),
				ToHumanTime(Parent->nickserv.Ident(), sourceL).c_str());
			}
		    }
		}
	    }
	    else
	    {
		// CHANGE NICK
		if (!Parent->nickserv.IsLive(data.ExtractWord(3, ": ")))
		{
		    Parent->nickserv.live[data.ExtractWord(3, ": ").LowerCase()] =
			Parent->nickserv.live[sourceL];
		}
		Parent->nickserv.live[data.ExtractWord(3, ": ").LowerCase()].
		    Name(data.ExtractWord(3, ": "));

		if (sourceL != data.ExtractWord(3, ": ").LowerCase())
		{
		    { WLOCK(("NickServ", "live"));
		    Parent->nickserv.live.erase(sourceL);
		    }
		    // We just did a SVSNICK ...
		    RLOCK(("Server", "recovered"));
		    if (Parent->nickserv.recovered.find(source.LowerCase()) !=
			    Parent->nickserv.recovered.end())
		    {
			Parent->server.NICK(source,
				(Parent->startup.Ownuser() ?
				sourceL : Parent->startup.Services_User()),
				Parent->startup.Services_Host(),
				Parent->startup.Server_Name(),
				"Nickname Enforcer");
		    }
		}
	    }
	}
	else if (msgtype=="NOTICE")
	{
	    // :source NOTICE target/#channel :message
	    // NOTICE target :message
	    if (!Parent->GotConnect())
		return;

	    if (source.empty())
		if (!IsChan(data.ExtractWord(2, ": ")))
		{
		    LOG((LM_WARNING, Parent->getLogMessage("ERROR/REC_FORNONUSER"),
			"NOTICE", Parent->startup.Server_Name().c_str(), data.ExtractWord(2, ": ").c_str()));
		}
	    else
		if (!IsChan(data.ExtractWord(3, ": ")))
		{
		    LOG((LM_WARNING, Parent->getLogMessage("ERROR/REC_FORNONUSER"),
			"NOTICE", source.c_str(), data.ExtractWord(3, ": ").c_str()));
		}
	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
	break;
    case 'O':
	if (msgtype=="OPER")
	{
	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
	break;
    case 'P':
	if (msgtype=="PART")
	{
	    if (source.Contains(".") || source.empty())
		return;

	    // :source PART #channel :reason
	    MLOCK(("BlockChannel", data.ExtractWord(3, ": ").LowerCase()));
	    Parent->nickserv.live[sourceL].Part(data.ExtractWord(3, ": "));
	}
	else if (msgtype=="PASS")
	{
	    // PASS :password
	    if (data.ExtractWord(2, ": ") != Parent->startup.Server(Parent->Server()).second)
	    {
		LOG((LM_ERROR, Parent->getLogMessage("OTHER/WRONGPASS"),
			Parent->Server().c_str()));
		CP(("Server password mismatch.  Closing socket."));
		raw(((proto.Tokens() && proto.GetNonToken("ERROR") != "") ?
			proto.GetNonToken("ERROR") : mstring("ERROR")) +
			" :No Access (passwd mismatch) [" + Parent->Server() + "]");
		raw(((proto.Tokens() && proto.GetNonToken("ERROR") != "") ?
			proto.GetNonToken("ERROR") : mstring("ERROR")) +
			" :Closing Link: [" + Parent->Server() + "] (Bad Password)");

		Parent->Disconnect();
	    }
            else
	    {
		Parent->GotConnect(true);
		SignOnAll();
            }
	}
	else if (msgtype=="PING")
	{
	    // PING :some.server
	    // :some.server PING some.server :our.server
	    if (source != "")
		sraw(((proto.Tokens() && proto.GetNonToken("PONG") != "") ?
			proto.GetNonToken("PONG") : mstring("PONG")) +
			" " + Parent->startup.Server_Name() + " :" + source);
	    else
		sraw(((proto.Tokens() && proto.GetNonToken("PONG") != "") ?
			proto.GetNonToken("PONG") : mstring("PONG")) +
			" " + Parent->startup.Server_Name() + " :" +
			data.ExtractWord(2, ": "));
	}
	else if (msgtype=="PONG")
	{
	    // Receive info back for LAG MONITOR.
	    // we'll send out ':our.server PING our.server :remote.server'
	    // for EACH server, at the same time execute Ping() on it.

	    // :server PONG server :our.server
	    if (IsServer(source))
		ServerList[sourceL].Pong();

	}
	else if (msgtype=="PRIVMSG")
	{
	    // :source PRIVMSG target/#channel :message
	    if (!Parent->GotConnect())
		return;

	    if (source.empty())
		if (!IsChan(data.ExtractWord(2, ": ")))
		{
		    LOG((LM_WARNING, Parent->getLogMessage("ERROR/REC_FORNONUSER"),
			"PRIVMSG", Parent->startup.Server_Name().c_str(), data.ExtractWord(2, ": ").c_str()));
		}
	    else
		if (!IsChan(data.ExtractWord(3, ": ")))
		{
		    LOG((LM_WARNING, Parent->getLogMessage("ERROR/REC_FORNONUSER"),
			"PRIVMSG", source.c_str(), data.ExtractWord(3, ": ").c_str()));
		}
	}
	else if (msgtype=="PROTOCTL")
	{
	    // Turn on tokens dynamically ...
	    if ((data + " ").Contains(" TOKEN "))
		proto.Tokens(true);
	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
	break;
    case 'Q':
	if (msgtype=="QUIT")
	{
	    // :source QUIT :reason
	    // :source QUIT :server server

	    if (source.Contains(".") || source.empty())
		return;

	    // OK, 4 words (always for squit), the 4nd word is a server
	    // and the 3rd word is the uplink of the 4th word (a server)
	    if (data.WordCount(": ")==4 && IsServer(data.ExtractWord(4, ": ")) &&
		ServerList[data.ExtractWord(4, ": ").LowerCase()].Uplink() == data.ExtractWord(3, ": ").LowerCase())
	    {
		/* Suspected SQUIT
		 *
		 * - Add user to ToBeSquit map under servername
		 * - Add data(4) to ServerSquit map with a timer to clear
		 *
		 * IF no SQUIT message received, user is QUIT and server
		 * is removed from ServerSquit map -- ie. its FAKE!
		 */
		Parent->nickserv.live[sourceL].SetSquit();
		WLOCK(("Server", "ToBeSquit"));
		MCB(ToBeSquit.size());
		ToBeSquit[data.ExtractWord(4, ": ").LowerCase()].push_back(sourceL);
		LOG((LM_NOTICE, Parent->getLogMessage("OTHER/SQUIT_FIRST"),
			data.ExtractWord(4, ": ").c_str(),
			data.ExtractWord(3, ": ").c_str()));

		WLOCK2(("Server", "ServerSquit"));
		if (ServerSquit.find(Parent->nickserv.live[sourceL].Server().LowerCase()) == ServerSquit.end())
		{
		    CB(1, ServerSquit.size());
		    ServerSquit[Parent->nickserv.live[sourceL].Server().LowerCase()] =
			ACE_Reactor::instance()->schedule_timer(&tobesquit,
				new mstring(Parent->nickserv.live[sourceL].Server().LowerCase()),
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
		if (Parent->nickserv.live[sourceL].IsServices())
		    sraw(((proto.Tokens() && proto.GetNonToken("ISON") != "") ?
			proto.GetNonToken("ISON") : mstring("ISON")) +
			" " + sourceL);
		Parent->nickserv.live[sourceL].Quit(data.After(":", 2));
		WLOCK(("NickServ", "live"));
		Parent->nickserv.live.erase(sourceL);
	    }
	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
	break;
    case 'R':
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
	break;
    case 'S':
	if (msgtype=="SETHOST")
	{
	    // From UnrealIRCD
	    // :source SVSHOST newhost
	    if (Parent->nickserv.IsLive(sourceL))
	    {
		Parent->nickserv.live[sourceL].AltHost(data.ExtractWord(3, ": "));
	    }
	}
	else if (msgtype=="SETTIME")
	{
	    // RWORLDism -- ignore.
	}
	else if (msgtype=="SERVER")
	{
	    // SERVER downlink hops :description
	    // :uplink SERVER downlink hops :description
	    if (source.empty())
	    {
		WLOCK(("Server", "ServerList"));
		ServerList[data.ExtractWord(2, ": ").LowerCase()] = Server(
			data.ExtractWord(2, ": ").LowerCase(),
			atoi(data.ExtractWord(3, ": ").LowerCase().c_str()),
			data.After(":"));
		if (proto.Numeric())
		    ServerList[data.ExtractWord(2, ": ").LowerCase()].Numeric(
			atoi(data.ExtractWord(7, ": ").c_str()));
		LOG((LM_INFO, Parent->getLogMessage("OTHER/LINK"),
			data.ExtractWord(2, ": ").c_str(),
			Parent->startup.Server_Name().c_str()));
	    }
	    else
	    {
		if (IsServer(sourceL))
		{
		    WLOCK(("Server", "ServerList"));
		    ServerList[data.ExtractWord(3, ": ").LowerCase()] = Server(
			data.ExtractWord(3, ": ").LowerCase(),
			sourceL,
			atoi(data.ExtractWord(4, ": ").LowerCase().c_str()),
			data.After(":", 2));
		    if (proto.Numeric())
			ServerList[data.ExtractWord(2, ": ").LowerCase()].Numeric(
			   atoi(data.ExtractWord(8, ": ").c_str()));
		    LOG((LM_INFO, Parent->getLogMessage("OTHER/LINK"),
			data.ExtractWord(3, ": ").c_str(), sourceL.c_str()));
		}
		else
		{
		    if (SeenMessage(data) >= Parent->config.MSG_Seen_Act())
		    {
			LOG((LM_ERROR, Parent->getLogMessage("ERROR/REC_FORNONSERVER"),
				"SERVER", data.ExtractWord(3, ": ").c_str(), sourceL.c_str()));
		    }
		    else
			mBase::push_message(data);
		}
	    }
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

	    if (source.Contains(".") || source.empty())
	    {
		unsigned int i;

		vector<mstring> users;
		mstring modes = data.ExtractWord(5, ": ");
		mstring mode_params = "", nick;
		bool oped, voiced;
		if (modes.Contains("l") || modes.Contains("k"))
		    mode_params = data.Before(":", 2).After(" ", 5);
		for (i=0; i < data.After(":", 2).WordCount(" "); i++)
		{
		    nick = data.After(":", 2).ExtractWord(i+1, " ");
		    if (nick != "")
		    {
			oped = voiced = false;
			if (nick[0u] == '@')
			{
			    oped = true;
			    nick.replace("@", "", false);
			}
			if (nick[0u] == '+')
			{
			    voiced = true;
			    nick.replace("+", "", false);
			}
			if (Parent->nickserv.IsLive(nick))
			{
			    if (oped)
			    {
				modes += "o";
				mode_params += " " + nick;
			    }
			    if (voiced)
			    {
				modes += "v";
				mode_params += " " + nick;
			    }
			    users.push_back(nick);
			}
			else
			{
			    PushUser(nick, ":" + nick + " JOIN " +
				data.ExtractWord(4, ": "));
			    if (oped)
				PushUser(nick, ":" + source + " MODE " +
				    data.ExtractWord(4, ": ") + " +o :" +
				    nick, data.ExtractWord(4, ": "));
			    if (voiced)
				PushUser(nick, ":" + source + " MODE " +
				    data.ExtractWord(4, ": ") + " +v :" +
				    nick, data.ExtractWord(4, ": "));
			}
		    }
		}

		if (users.size())
		{
		    for (i=0; i<users.size(); i++)
		    {
			Parent->nickserv.live[users[i].LowerCase()].Join(data.ExtractWord(4, ": "));
		    }
		    CP(("MODE TO %s: %s", data.ExtractWord(4, ": ").LowerCase().c_str(),
			(modes + " " + mode_params).c_str()));
		    if (Parent->chanserv.IsLive(data.ExtractWord(4, ": ")))
			Parent->chanserv.live[data.ExtractWord(4, ": ").LowerCase()].Mode(
				Parent->chanserv.FirstName(), modes + " " + mode_params);
		}
		else if (modes.length() > 1)
		{
		    PushUser(nick, ":" + source + " MODE " +
			data.ExtractWord(4, ": ") + " " + modes +
			" :" + mode_params, data.ExtractWord(4, ": "));
		}
	    }
	    else
	    {
		unsigned int i;
		bool oped, voiced;
		mstring chan;

		for (i=4; i<=data.WordCount(": "); i++)
		{
		    chan = data.ExtractWord(i, ": ");
		    voiced = oped = false;
		    if (chan[0u] == '@')
		    {
			oped = true;
			chan.replace("@", "", false);
		    }
		    if (chan[0u] == '+')
		    {
			voiced = true;
			chan.replace("+", "", false);
		    }
		    Parent->nickserv.live[sourceL].Join(chan);
		    if (oped)
			Parent->chanserv.live[chan.LowerCase()].Mode(
				Parent->chanserv.FirstName(), "+o " + source);
		    if (voiced)
			Parent->chanserv.live[chan.LowerCase()].Mode(
				Parent->chanserv.FirstName(), "+v " + source);
		}
	    }
	}
	else if (msgtype=="SMO")
	{
	    // Thanks to unreal for ANOTHER chatter message ..
	}
	else if (msgtype=="SNICK")
	{
	    if (source.Contains("."))
		return;

	    // NEW USER
	    sourceL = data.ExtractWord(2, ": ").LowerCase();

	    // DONT kill when we do SQUIT protection.
	    map<mstring,list<mstring> >::iterator i;
	    { RLOCK(("Server", "ToBeSquit"));
	    for (i=ToBeSquit.begin(); i!=ToBeSquit.end(); i++)
	    {
		list<mstring>::iterator k;
		WLOCK(("Server", "ToBeSquit", i->first.LowerCase()));
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
		if (Parent->nickserv.live[sourceL].Squit() == data.ExtractWord(7, ": ").LowerCase()
		    && Parent->nickserv.live[sourceL].SignonTime() == mDateTime((time_t) atol(data.ExtractWord(4, ": "))))
		{
		    Parent->nickserv.live[sourceL].ClearSquit();
		    return;    // nice way to avoid repeating ones self :)
		}
		else
		{
		    Parent->nickserv.live[sourceL].Quit("SQUIT - " + Parent->nickserv.live[sourceL].Server());
		    WLOCK(("NickServ", "live"));
		    Parent->nickserv.live.erase(sourceL);
		}
	    }

	    // hops = servers from us
	    // services = 1 for service, 0 for user
	    // DAL4.4.15+ SNICK name hops time user host server services modes :real name

	    {WLOCK(("NickServ", "live"));
	    switch (proto.Signon())
	    {
	    case 0000:
	    case 0001:
		break;
	    case 1000: // SNICK nick hops time user host server modes :realname
		Parent->nickserv.live[sourceL] =
			Nick_Live_t(
			    data.ExtractWord(2, ": "),
			    (time_t) atol(data.ExtractWord(4, ": ")),
			    data.ExtractWord(7, ": "),
			    data.ExtractWord(5, ": "),
			    data.ExtractWord(6, ": "),
			    data.After(":")
			);
		Parent->nickserv.live[sourceL].Mode(data.ExtractWord(8, ": "));
		break;
	    case 1001: // SNICK nick hops time user host server 1 modes :realname
		Parent->nickserv.live[sourceL] =
			Nick_Live_t(
			    data.ExtractWord(2, ": "),
			    (time_t) atol(data.ExtractWord(4, ": ")),
			    data.ExtractWord(7, ": "),
			    data.ExtractWord(5, ": "),
			    data.ExtractWord(6, ": "),
			    data.After(":")
			);
		Parent->nickserv.live[sourceL].Mode(data.ExtractWord(9, ": "));
		break;
	    case 1002: // SNICK nick hops time user host server 0 real-host modes :realname
		Parent->nickserv.live[sourceL] =
			Nick_Live_t(
			    data.ExtractWord(2, ": "),
			    (time_t) atol(data.ExtractWord(4, ": ")),
			    data.ExtractWord(7, ": "),
			    data.ExtractWord(5, ": "),
			    data.ExtractWord(9, ": "),
			    data.After(":")
			);
		Parent->nickserv.live[sourceL].AltHost(data.ExtractWord(6, ": "));
		Parent->nickserv.live[sourceL].Mode(data.ExtractWord(10, ": "));
		break;
	    case 1003: // SNICK nick hops time user real-host host server 0 modes :realname
		Parent->nickserv.live[sourceL] =
			Nick_Live_t(
			    data.ExtractWord(2, ": "),
			    (time_t) atol(data.ExtractWord(4, ": ")),
			    data.ExtractWord(8, ": "),
			    data.ExtractWord(5, ": "),
			    data.ExtractWord(6, ": "),
			    data.After(":")
			);
		Parent->nickserv.live[sourceL].AltHost(data.ExtractWord(7, ": "));
		Parent->nickserv.live[sourceL].Mode(data.ExtractWord(10, ": "));
		break;
	    case 2000: // NICK nick hops time mode user host server :realname
		Parent->nickserv.live[sourceL] =
			Nick_Live_t(
			    data.ExtractWord(2, ": "),
			    (time_t) atol(data.ExtractWord(4, ": ")),
			    data.ExtractWord(8, ": "),
			    data.ExtractWord(6, ": "),
			    data.ExtractWord(7, ": "),
			    data.After(":")
			);
		Parent->nickserv.live[sourceL].Mode(data.ExtractWord(5, ": "));
		break;
	    case 2001: // NICK nick hops time mode user host server 0 :realname
		Parent->nickserv.live[sourceL] =
			Nick_Live_t(
			    data.ExtractWord(2, ": "),
			    (time_t) atol(data.ExtractWord(4, ": ")),
			    data.ExtractWord(8, ": "),
			    data.ExtractWord(6, ": "),
			    data.ExtractWord(7, ": "),
			    data.After(":")
			);
		Parent->nickserv.live[sourceL].Mode(data.ExtractWord(5, ": "));
		break;
	    case 2002: // NICK nick hops time mode user host maskhost server 0 :realname
		Parent->nickserv.live[sourceL] =
			Nick_Live_t(
			    data.ExtractWord(2, ": "),
			    (time_t) atol(data.ExtractWord(4, ": ")),
			    data.ExtractWord(9, ": "),
			    data.ExtractWord(6, ": "),
			    data.ExtractWord(7, ": "),
			    data.After(":")
			);
		Parent->nickserv.live[sourceL].Mode(data.ExtractWord(5, ": "));
		Parent->nickserv.live[sourceL].AltHost(data.ExtractWord(8, ": "));
		break;
	    }}
	    if (Parent->nickserv.IsLive(sourceL))
	    {
		if (Parent->nickserv.live[sourceL].Server().empty())
		{
		    PopUser(sourceL);
		    KILL(Parent->nickserv.FirstName(), sourceL,
				Parent->nickserv.live[sourceL].RealName());
		    return;
		}

		{ WLOCK(("Server", "i_UserMax"));
		if (i_UserMax < Parent->nickserv.live.size())
		{
		    MCB(i_UserMax);
		    i_UserMax = Parent->nickserv.live.size();
		    MCE(i_UserMax);
		}}
		FlushUser(sourceL);

		// HAS to be AFTER the nickname is added to map.
		map<mstring, Committee>::iterator iter;
		mstring setmode;
		{ RLOCK(("CommServ", "list"));
		for (iter = Parent->commserv.list.begin();
				    iter != Parent->commserv.list.end();
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
		if (setmode != "")
		{
		    mstring setmode2;
		    for (unsigned int j=0; j<setmode.size(); j++)
		    {
			if (setmode[j] != '+' && setmode[j] != '-' &&
			    setmode[j] != ' ' &&
			    !Parent->nickserv.live[sourceL].HasMode(setmode[j]))
			    setmode2 += setmode[j];
		    }
		    SVSMODE(Parent->nickserv.FirstName(), sourceL, "+" + setmode2);
		}
		if (Parent->nickserv.IsStored(sourceL))
		{
		    if (Parent->nickserv.stored[sourceL].Forbidden())
		    {
			Parent->nickserv.send(Parent->nickserv.FirstName(),
				sourceL, Parent->getMessage(sourceL, "ERR_SITUATION/FORBIDDEN"),
				ToHumanTime(Parent->nickserv.Ident(), sourceL).c_str());
		    }
		    else if (Parent->nickserv.stored[sourceL].Protect() &&
			!Parent->nickserv.stored[sourceL].IsOnline())
		   {
			Parent->nickserv.send(Parent->nickserv.FirstName(),
				sourceL, Parent->getMessage(sourceL, "ERR_SITUATION/PROTECTED"),
				ToHumanTime(Parent->nickserv.Ident(), sourceL).c_str());
		    }
		}
	    }
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
	    mstring target;
	    if (source.empty())
		target = data.ExtractWord(2, ": ").LowerCase();
	    else
		target = data.ExtractWord(3, ": ").LowerCase();

	    LOG((LM_NOTICE, Parent->getLogMessage("OTHER/SQUIT_SECOND"),
		    target.c_str(),
		    ServerList[target.LowerCase()].Uplink().c_str()));

	    { WLOCK(("Server", "ServerList"));
	    ServerList.erase(target);
	    }
	    { WLOCK2(("Server", "ServerSquit"));
	    MCB(ToBeSquit.size());
	    CB(1, ServerSquit.size());
	    if (ServerSquit.find(target) != ServerSquit.end())
	    {
		mstring *arg = NULL;
		if (ACE_Reactor::instance()->cancel_timer(
			ServerSquit[target], (const void **) arg)
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
	    map<mstring,Nick_Live_t>::iterator iter;
	    vector<mstring> chunked;
	    { RLOCK(("NickServ", "live"));
	    for (iter=Parent->nickserv.live.begin(); iter != Parent->nickserv.live.end(); iter++)
	    {
		if (iter->second.IsServices() && ServerList.size() == 0)
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
	    sraw("219 " + source + " " + data.ExtractWord(3, ": ") +
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
	    if (Parent->nickserv.IsLive(data.ExtractWord(3, ": ")))
	    {
		Parent->nickserv.live[data.ExtractWord(3, ": ").LowerCase()].AltHost(data.ExtractWord(4, ": "));
	    }
	}
	else if (msgtype=="SVSKILL")
	{
	    // Same as KILL (but by services)
	}
	else if (msgtype=="SVSMODE")
	{
	    // Handle just as mode, WITHOUT sanity
	    if (IsChan(data.ExtractWord(3, ": ")))
	    {
		if (Parent->chanserv.IsLive(data.ExtractWord(3, ": ")))
		{
		    Parent->chanserv.live[data.ExtractWord(3, ": ").LowerCase()].Mode(source, data.After(" ", 3));
		}
		else
		{
		    LOG((LM_CRITICAL, Parent->getLogMessage("ERROR/REC_FORNONCHAN"),
			"SVSMODE", source.c_str(),  data.ExtractWord(3, ": ").c_str()));
		}
	    }
	    else
	    {
		if (Parent->nickserv.IsLive(data.ExtractWord(3, ": ")))
		{
		    Parent->nickserv.live[data.ExtractWord(3, ": ").LowerCase()].Mode(data.ExtractWord(4, ": "));
		}
		else
		{
		    LOG((LM_CRITICAL, Parent->getLogMessage("ERROR/REC_FORNONUSER"),
			"SVSMODE", source.c_str(),  data.ExtractWord(3, ": ").c_str()));
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
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
	break;
    case 'T':
	if (msgtype=="TIME")
	{
	    // :source TIME :our.server
	    sraw("391 " + source + " :" + Now().DateTimeString());
	}
	else if (msgtype=="TOPIC")
	{
	    // :server/user TOPIC #channel setter time :topic
	    // :server/user TOPIC #channel setter
	    // TIME is not standard (time is optional);
	    if (Parent->chanserv.IsLive(data.ExtractWord(3, ": ")))
	    {
		MLOCK(("BlockChannel", data.ExtractWord(3, ": ").LowerCase()));
		if (data.ExtractWord(5, ": ") != "")
		{ // Setting
		    Parent->chanserv.live[data.ExtractWord(3, ": ").LowerCase()].Topic(
			source,
		        data.After(":", 2),
		        data.ExtractWord(4, ": "),
		        (time_t) atol(data.ExtractWord(5, ": "))
		    );
		}
		else
		{ // Clearing
		    Parent->chanserv.live[data.ExtractWord(3, ": ").LowerCase()].Topic(
		        source, "", data.ExtractWord(4, ": "));
		}
	    }
	    else
	    {
		if (source.Contains("."))
		{
		    if (SeenMessage(data) >= Parent->config.MSG_Seen_Act())
		    {
			LOG((LM_CRITICAL, Parent->getLogMessage("ERROR/REC_FORNONCHAN"),
				"MODE", source.c_str(), data.ExtractWord(3, ": ").c_str()));
		    }
		    else
			mBase::push_message(data);
		}
		else
		{
		    PushUser(source, data, data.ExtractWord(3, ": "));
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
		raw(((proto.Tokens() && proto.GetNonToken("NOTICE") != "") ?
			proto.GetNonToken("NOTICE") : mstring("NOTICE")) +
			" " + source + " :" + tmp.c_str());
		return;
	    }}
	    mstring out;
	    if (Parent->nickserv.IsLive(data.ExtractWord(3, ": ")) &&
		Parent->nickserv.live[data.ExtractWord(3, ": ").LowerCase()].IsServices())
	    {
		RLOCK(("NickServ", "live", data.ExtractWord(3, ": ").LowerCase()));
		Nick_Live_t *nlive = &Parent->nickserv.live[data.ExtractWord(3, ": ").LowerCase()];
		if (nlive->HasMode("o"))
		    out << "204 " << source << " Operator 10 ";
		else
		    out << "205 " << source << " User 1 ";
		out << nlive->Name() << " [" << nlive->User() << "@"
			<< nlive->Host() << "] :" << nlive->IdleTime().timetstring();
		sraw(out);
	    }
	    else
	    {
		unsigned long opers=0, users=0;
		out << "206 " << source << "Server 50 " << ServerList.size()+1 <<
			"S " << Parent->nickserv.live.size() << "C " <<
			OurUplink() << "[0.0.0.0] AutoConn.!*@" <<
			Parent->startup.Server_Name() << " :0";
		sraw(out);

		RLOCK(("NickServ", "live"));
		map<mstring, Nick_Live_t>::iterator iter;
		for (iter=Parent->nickserv.live.begin();
			iter!=Parent->nickserv.live.end(); iter++)
		{
		    if (iter->second.IsServices())
		    {
			out = "";
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
				iter->second.IdleTime().timetstring();
			sraw(out);
			
		    }
		}
		out = "";
		out << "209 " << source << " Class 50 :1";
		sraw(out);
		if (opers)
		{
		    out = "";
		    out << "209 " << source << " Class 10 :" << opers;
		    sraw(out);
		}
		if (users)
		{
		    out = "";
		    out << "209 " << source << " Class 1 :" << users;
		    sraw(out);
		}
	    }
	    out = "";
	    out << "262 " << source << " " << Parent->startup.Server_Name() <<
		" :End of TRACE";
	    sraw(out);
	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
	break;
    case 'U':
	if (msgtype=="UNGLINE")
	{
	    // We will ignore GLINES because they're not relivant to us.
	    // we will not be glining our own clients ;P
	}
	else if (msgtype=="UNSQLINE")
	{
	    // We will ignore SQLINES because they're not relivant to us.
	    // we will not be qlining our own clients ;P
	}
	if (msgtype=="UNZLINE")
	{
	    // We will ignore ZLINES because they're not relivant to us.
	    // we will not be zlining our own clients ;P
	}
	else if (msgtype=="USER")
	{
	    if (source.Contains("."))
		return;

	    // NEW USER
	    sourceL = data.ExtractWord(2, ": ").LowerCase();

	    // DONT kill when we do SQUIT protection.
	    map<mstring,list<mstring> >::iterator i;
	    { RLOCK(("Server", "ToBeSquit"));
	    for (i=ToBeSquit.begin(); i!=ToBeSquit.end(); i++)
	    {
		list<mstring>::iterator k;
		WLOCK(("Server", "ToBeSquit", i->first.LowerCase()));
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
	        if (Parent->nickserv.live[sourceL].Squit() == data.ExtractWord(7, ": ").LowerCase()
	    	    && Parent->nickserv.live[sourceL].SignonTime() == mDateTime((time_t) atol(data.ExtractWord(4, ": "))))
	        {
	    	    Parent->nickserv.live[sourceL].ClearSquit();
	    	    return;    // nice way to avoid repeating ones self :)
	        }
	        else
	        {
	    	    Parent->nickserv.live[sourceL].Quit("SQUIT - " + Parent->nickserv.live[sourceL].Server());
		    WLOCK(("NickServ", "live"));
	    	    Parent->nickserv.live.erase(sourceL);
	        }
	    }

	    { WLOCK(("NickServ", "live"));
	    switch (proto.Signon())
	    {
	    case 0000: // USER nick user host server :realname
	        Parent->nickserv.live[sourceL] =
		    Nick_Live_t(
			data.ExtractWord(2, ": "),
			time(NULL),
			data.ExtractWord(5, ": "),
			data.ExtractWord(3, ": "),
			data.ExtractWord(4, ": "),
			data.After(":")
		    );
		break;
	    case 0001: // USER nick time user host server :realname
	        Parent->nickserv.live[sourceL] =
		    Nick_Live_t(
			data.ExtractWord(2, ": "),
			(time_t) atol(data.ExtractWord(3, ": ")),
			data.ExtractWord(6, ": "),
			data.ExtractWord(4, ": "),
			data.ExtractWord(5, ": "),
			data.After(":")
		    );
		break;
	    case 1000:
	    case 1001:
	    case 1002:
	    case 1003:
	    case 2000:
	    case 2001:
	    case 2002:
		break;
	    }}

	    if (Parent->nickserv.IsLive(sourceL))
	    {
		if (Parent->nickserv.live[sourceL].Server().empty())
		{
		    PopUser(sourceL);
		    KILL(Parent->nickserv.FirstName(), sourceL,
				Parent->nickserv.live[sourceL].RealName());
		    return;
		}

		{ WLOCK(("Server", "i_UserMax"));
		if (i_UserMax < Parent->nickserv.live.size())
		{
		    MCB(i_UserMax);
		    i_UserMax = Parent->nickserv.live.size();
		    MCE(i_UserMax);
		}}
		FlushUser(sourceL);

		// HAS to be AFTER the nickname is added to map.
		map<mstring, Committee>::iterator iter;
		mstring setmode;
		{ RLOCK(("CommServ", "list"));
		for (iter = Parent->commserv.list.begin();
				    iter != Parent->commserv.list.end();
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
		if (setmode != "")
		{
		    mstring setmode2;
		    for (unsigned int j=0; j<setmode.size(); j++)
		    {
			if (setmode[j] != '+' && setmode[j] != '-' &&
			    setmode[j] != ' ' &&
			    !Parent->nickserv.live[sourceL].HasMode(setmode[j]))
			    setmode2 += setmode[j];
		    }
		    SVSMODE(Parent->nickserv.FirstName(), sourceL, "+" + setmode2);
		}
		if (Parent->nickserv.IsStored(sourceL))
		{
		    if (Parent->nickserv.stored[sourceL].Forbidden())
		    {
			Parent->nickserv.send(Parent->nickserv.FirstName(),
				sourceL, Parent->getMessage(sourceL, "ERR_SITUATION/FORBIDDEN"),
				ToHumanTime(Parent->nickserv.Ident(), sourceL).c_str());
		    }
		    else if (Parent->nickserv.stored[sourceL].Protect() &&
			!Parent->nickserv.stored[sourceL].IsOnline())
		   {
			Parent->nickserv.send(Parent->nickserv.FirstName(),
				sourceL, Parent->getMessage(sourceL, "ERR_SITUATION/PROTECTED"),
				ToHumanTime(Parent->nickserv.Ident(), sourceL).c_str());
		    }
		}
	    }
	}
	else if (msgtype=="USERHOST")
	{
	    if (data.WordCount(": ") > 2)
	    {
		sraw("461 " + source + " USERHOST :Not enough paramaters");
	    }
	    else if (Parent->nickserv.IsLive(data.ExtractWord(3, ": ")))
	    {
		mstring target = Parent->getLname(data.ExtractWord(3, ": "));
		if (!Parent->nickserv.IsStored(target) ? 1 :
			!Parent->nickserv.stored[target.LowerCase()].Private())
		{
		    sraw("302 " + source + " :" +
			Parent->nickserv.live[target.LowerCase()].Name() +
			"*=-" +
			Parent->nickserv.live[target.LowerCase()].User() +
			"@" +
			Parent->nickserv.live[target.LowerCase()].AltHost());
		}
		else
		{
		    sraw("302 " + source + " :" +
			Parent->nickserv.live[target.LowerCase()].Name() +
			"*=-" +
			Parent->nickserv.live[target.LowerCase()].User() +
			"@" + Parent->getMessage("VALS/ONLINE"));
		}

	    }
	    else
	    {
		sraw("401 " + source + " " + data.ExtractWord(3, ": ") +
			" :No such nickname/channel.");
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
	break;
    case 'V':
	if (msgtype=="VERSION")
	{
	    // :source VERSION :our.server
	    //:temple.magick.tm 351 ChanServ dal4.4.17. temple.magick.tm :AiMnW
	    mstring tmp(VERSION);
	    if(RELEASE!="")
		tmp+="-"+RELEASE;
	    if(PATCH1!="")
		tmp+="+"+PATCH1;
	    if(PATCH2!="")
		tmp+="+"+PATCH2;
	    if(PATCH3!="")
		tmp+="+"+PATCH3;
	    if(PATCH4!="")
		tmp+="+"+PATCH4;
	    if(PATCH5!="")
		tmp+="+"+PATCH5;
	    if(PATCH6!="")
		tmp+="+"+PATCH6;
	    if(PATCH7!="")
		tmp+="+"+PATCH7;
	    if(PATCH8!="")
		tmp+="+"+PATCH8;
	    if(PATCH9!="")
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
		") " << BUILD_SYS << "/" << BUILD_TYPE << ".";
	    sraw("351 " + source + " " + PACKAGE + " " + Parent->startup.Server_Name() + " :" + tmp);
	}
	else
	{
	    LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	}
	break;
    case 'W':
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
		raw(((proto.Tokens() && proto.GetNonToken("NOTICE") != "") ?
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

	    mstring target = data.ExtractWord(3, ": ");
	    mstring targetL = target.LowerCase();
	    { RLOCK(("IrcSvcHandler"));
	    if (Parent->ircsvchandler != NULL &&
		Parent->ircsvchandler->HTM_Level() > 3)
	    {
		mstring tmp;
		tmp.Format(Parent->getMessage(source, "MISC/HTM").c_str(),
								msgtype.c_str());
		raw(((proto.Tokens() && proto.GetNonToken("NOTICE") != "") ?
			proto.GetNonToken("NOTICE") : mstring("NOTICE")) +
			" " + source + " :" + tmp.c_str());
		sraw("318 " + source + " " + target + " :End of /WHOIS list.");
		return;
	    }
	    else if (Parent->nickserv.IsLive(data.ExtractWord(3, ": ")))
	    {
		sraw("311 " + source + " " + target + " " + Parent->nickserv.live[targetL].User() +
			" " + Parent->nickserv.live[targetL].Host() + " * :" +
			Parent->nickserv.live[targetL].RealName());

		if (Parent->nickserv.live[targetL].IsRecognized())
		{
		    sraw("307 " + source + " " + target + " : is a registered nick");
		}

		set<mstring> chans = Parent->nickserv.live[targetL].Channels();
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
		    if ((Parent->chanserv.live[*iter].HasMode("s") ||
				Parent->chanserv.live[*iter].HasMode("p")) &&
				!Parent->chanserv.live[*iter].IsIn(source))
			continue;

		    if (Parent->chanserv.live[*iter].IsOp(target))
			outline += "@" + *iter + " ";
		    else if (Parent->chanserv.live[*iter].IsVoice(target))
			outline += "+" + *iter + " ";
		    else
			outline += *iter + " ";
		}
		if (outline.After(":").length() > 0)
		    sraw(outline);

		if (Parent->nickserv.live[targetL].IsServices())
		    sraw("312 " + source + " " + target + " " + Parent->startup.Server_Name() +
			" :" + Parent->startup.Server_Desc());
		else if (IsServer(Parent->nickserv.live[targetL].Server()))
		    sraw("312 " + source + " " + target + " " + Parent->nickserv.live[targetL].Server() +
			" :" + ServerList[Parent->nickserv.live[targetL].Server()].Description());


		if (Parent->nickserv.live[targetL].Away() != "")
		    sraw("301 " + source + " " + target + " :" + Parent->nickserv.live[targetL].Away());

		if (Parent->nickserv.live[targetL].HasMode("o"))
		    sraw("313 " + source + " " + target + " :is an IRC Operator");

		if (Parent->nickserv.live[targetL].HasMode("h"))
		    sraw("310 " + source + " " + target + " :looks very helpful.");

		if (Parent->nickserv.live[targetL].IsServices())
		{
    		    mstring signon_idletime;
		    signon_idletime << Parent->nickserv.live[targetL].IdleTime().timetstring()
			<< " " << Parent->nickserv.live[targetL].SignonTime().timetstring();
		    sraw("317 " + source + " " + target + " " + signon_idletime + " :seconds idle, signon time");
		}

		sraw("318 " + source + " " + target + " :End of /WHOIS list.");

	    }
	    else
	    {
		mstring target = data.ExtractWord(3, ": ");
		sraw("401 " + source + " " + target + " :No such nickname/channel.");
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
	break;
    case 'X':
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	break;
    case 'Y':
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	break;
    case 'Z':
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
	break;
    default:
	LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
	break;
    }

}

void NetworkServ::numeric_execute(const mstring & data)
{
    FT("NetworkServ::numeric_execute", (data));
    unsigned int i;

    mstring source, sourceL;
    unsigned int msgtype;
    if (data[0u]==':' || data.ExtractWord(1, ": ").Contains("."))
    {
        source=data.ExtractWord(1,": ");
	sourceL=source.LowerCase();
        msgtype=atoi(data.ExtractWord(2,": "));
	if (!(source.Contains(".") || Parent->nickserv.IsLive(source)))
	{
	    PushUser(source, data);
	}
    }
    else
    {
        msgtype=atoi(data.ExtractWord(1,": "));
    }

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
	if (proto.Burst() != "")
	    Parent->server.sraw(((proto.Tokens() &&
		proto.GetNonToken(proto.Burst()) != "") ?
		proto.GetNonToken(proto.Burst()) : mstring(proto.Burst())));
	for (i=4; i<=data.WordCount(": "); i++)
	{
	    // Remove clients from 'signon list' who are
	    // already on the network.
	    WLOCK(("Server", "WaitIsOn"));
	    if (WaitIsOn.find(data.ExtractWord(i, ": ").LowerCase()) != WaitIsOn.end())
		WaitIsOn.erase(data.ExtractWord(i, ": "));
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
			map<mstring,Chan_Stored_t>::iterator iter;
			map<mstring,mstring> modes;
			Chan_Live_t *clive;
			{ RLOCK(("ChanServ", "stored"));
			for (iter=Parent->chanserv.stored.begin(); iter!=Parent->chanserv.stored.end(); iter++)
			{
			    if (Parent->chanserv.IsLive(iter->first))
				clive = &Parent->chanserv.live[iter->first];
			    else
				clive = NULL;
			    // If its live and got JOIN on || not live and mlock +k or +i
			    if ((clive != NULL && iter->second.Join()) ||
				(clive == NULL && !iter->second.Forbidden() &&
				(iter->second.Mlock_Key() != "" ||
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
				    if(iter->second.Mlock_Key() != "")
					modes[iter->first] += "k " +
						iter->second.Mlock_Key();
				}
				if (joinline.length() > proto.MaxLine())
				{
				    JOIN(Parent->chanserv.FirstName(), joinline);
				    joinline = "";
				}
			    }
			    if (clive != NULL)
			    {
				if (clive->Topic() != "")
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
					iter->second.Last_Topic() != "")
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
			    joinline = "";
			}
			map<mstring,mstring>::iterator i;
			for (i=modes.begin(); i!=modes.end(); i++)
			{
			    if (Parent->chanserv.IsLive(i->first))
				Parent->chanserv.live[i->first].SendMode(i->second);
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
		    Parent->nickserv.IsLive(*k) && Parent->startup.Setmode() != "")
		    Parent->server.MODE(*k, "+" + Parent->startup.Setmode());
		FlushMsgs(*k);
	    }
	}
	{ WLOCK(("Server", "WaitIsOn"));
	WaitIsOn.clear();
	}
	if (proto.EndBurst() != "")
	    Parent->server.sraw(((proto.Tokens() &&
		proto.GetNonToken(proto.EndBurst()) != "") ?
		proto.GetNonToken(proto.EndBurst()) : mstring(proto.EndBurst())));
	break;
    case 436:     // ERR_NICKCOLLISION
	// MUST handle.
	break;
    case 464:     // ERR_PASSWDMISMATCH
	// MUST handle (Stop connecting).
	LOG((LM_ERROR, Parent->getLogMessage("OTHER/WRONGPASS"),
		Parent->Server().c_str()));
	Parent->Disconnect();
	break;
    case 465:     // ERR_YOUREBANNEDCREEP
	// MUST handle (Stop connecting).
	LOG((LM_ERROR, Parent->getLogMessage("OTHER/WEAREBANNED"),
		Parent->Server().c_str()));
	Parent->Disconnect();
	break;
    default:
	// We dont bother with what we get back -- useless anyway.
/*	LOG((LM_WARNING, Parent->getLogMessage("ERROR/UNKNOWN_MSG"),
			data.c_str()));
*/
	break;
    }
}

void NetworkServ::DumpB() const
{
    MB(0, (i_UserMax, ServerSquit.size(), ToBeSquit.size(),
	i_OurUplink, ToBeSent.size(), ToBeDone.size()));
}

void NetworkServ::DumpE() const
{
    ME(0, (i_UserMax, ServerSquit.size(), ToBeSquit.size(),
	i_OurUplink, ToBeSent.size(), ToBeDone.size()));
}

