#ifndef WIN32
#pragma interface
#endif
/*  Magick IRC Services
**
** (c) 1997-2000 Preston Elder <prez@magick.tm>
** (c) 1998-2000 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any
** circumstances, however it may be added to if any
** modifications are made to this file.  All modified
** code must be clearly documented and labelled.
**
** ========================================================== */
#ifndef _LOGFILE_H
#define _LOGFILE_H
static const char *ident_logfile_h = "@(#) $Id$";
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.5  2000/03/19 08:50:53  prez
** More Borlandization -- Added WHAT project, and fixed a bunch
** of minor warnings that appear in borland.
**
** Revision 1.4  2000/02/23 12:21:01  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.3  2000/02/15 10:37:47  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */


/* Automatically generated hard-coded log output file.
 * Based upon lang/english.lfo.
 * Created on Tuesday February 15 21:35:58 EST 2000
 */

unsigned int def_logent =      281;
char *def_log[] = {
"; Magick IRC Services",
"; (c) 1996-1999 Preston A. Elder, W. King",
";",
"; English Log Output Language File",
"; Translated By: Preston A. Elder <prez@magick.tm>",
"; Please also edit the COMMANDLINE/TRANSLATED token",
"; to reflect the above (this is displayed on startup).",
";",
"; --------------------------------------------------------------",
";",
"; This file is split up into sections.  These sections are so",
"; that we can logically group responses together.  IF you change",
"; the section names, or the response 'tags' within them, then",
"; you will likely break Magick's resopnses.  IF you are a",
"; developer and adding new sections, please note that they may",
"; NOT have a '/' (forward-slash) or space in their name.",
";",
"; Throughout this file there are verious codes that are replaced",
"; with a value direct from Magick.  See below for a table that",
"; outlines the codes and their meanings.  All lines in this .��file",
"; starting with ';' are cots.  If your response string has",
"; leading spaces, you MUST put a '\\' in front of the first space,",
"; else leading spaces are stripped.",
";",
"; The C replacement codes are used to insert a value from Magick",
"; into the string.  These should NEVER be removed, they must also",
"; stay in the same ORDER as they are.  If they're removed or the",
"; order in which they appear is changed, the results are undefined.",
"; These are also known as 'live' or 'active' codes.",
";",
"; --------------------------------------------------------------",
";",
"; C replacement codes:",
";   %s = String         %c = Single Character",
";   %d = Integer        %f = Numeric (with DP's)",
";   %o = Octal          %x = Hexidecimal",
";   %% = Literal %      %g = Like %f",
"; Please see the manual page on C formatting characters",
"; (included with Magick, or available with the command",
"; 'man format' or 'man formats' depending on the system).",
"; Briefly tho, if a number is specified in between the %",
"; and the letter, then it indicates the LENGTH of the",
"; field in question (usually its not recommended you change",
"; these).  If the number is negative, it forces it to be",
"; LEFT justified.  If the number has a decimal point, then",
"; it specifies how many digits appear AFTER the DP.",
";",
"",
";",
"; These are responses to command line options or config options.",
";",
"[COMMANDLINE]",
"TRANSLATED       =Preston A. Elder <prez@magick.tm>",
"UNKNOWN_OPTION   =Unknown option %s, ignored.",
"NEEDPARAM        =Option %s requires a paramater.",
"MUSTBENUMBER     =Paramater for %s must be a positive number.",
"NO_CFG_FILE      =Could not read config file %s, aborting.",
"STOP             =CONFIG: [Startup] STOP code received.",
"RECONNECT        =Reconnecting to server due to configuration change.",
"CANNOT_CONVERT   =Database type %s invalid (aborting).",
"NO_KEYFILE       =Keyfile %s does not exist.",
"NONOPTION        =Non-option arguments are not allowed.",
"ONEOPTION        =You may only specify one paramater after a group of options.",
"TRACE_SYNTAX     =Paramater for --trace must be in the format of TYPE:LEVEL",
"ZERO_LEVEL       =Zero or error in parsing trace level, ignoring.",
"CFG_SYNTAX       =Configuration token \"%s\" contains a syntax error or invalid data.",
"DBASE_ID         =Identification tag for database not valid.",
"DBASE_VER        =Version of database is not recognized.",
"",
";",
"; These are used when a command is executed from NickServ.",
";",
"[NICKSERV]",
"REGISTER         =%s registered nickname %s.",
"DROP             =%s dropped nickname %s (%d slaves).",
"LINK             =%s linked nickname %s to %s.",
"UNLINK           =%s unlinked nickname %s from %s.",
"HOST             =%s changed their host nickname from %s to %s.",
"IDENTIFY         =%s identified for nickname %s.",
"GHOST            =%s killed nickname %s with the ghost command.",
"RECOVER          =%s requested services to recover nickname %s.",
"SUSPEND          =%s suspended nickname %s for %s.",
"UNSUSPEND        =%s unsuspended nickname %s.",
"FORBID           =%s forbade nickname %s from being registered.",
"GETPASS          =%s retrieved the password for nickname %s.",
"ACCESS_ADD       =%s added hostmask %s to nickname %s.",
"ACCESS_DEL       =%s removed hostmask %s from nickname %s.",
"IGNORE_ADD       =%s added nickname %s to the ignore list for nickname %s.",
"IGNORE_DEL       =%s removed nickname %s from the ignore list for nickname %s.",
"SET_PASSWORD     =%s changed the password for nickname %s.",
"SET              =%s set the value of %s for nickname %s to %s.",
"LOCK             =%s locked the value of %s for nickname %s to %s.",
"UNLOCK           =%s unlocked the value of %s for nickname %s.",
"",
";",
"; These are used when a command is executed from ChanServ.",
";",
"[CHANSERV]",
"REGISTER         =%s registered channel %s.",
"DROP             =%s dropped channel %s.",
"SUSPEND          =%s suspended channel %s for %s.",
"UNSUSPEND        =%s unsuspended channel %s.",
"FORBID           =%s forbade channel %s from being registered.",
"GETPASS          =%s retrieved the password for channel %s.",
"MODE             =%s changed mode \"%s\" on channel %s with the MODE command.",
"OP               =%s opped %s in channel %s with the OP command.",
"DEOP             =%s deopped %s in channel %s with the DEOP command.",
"VOICE            =%s voiced %s in channel %s with the VOICE command.",
"DEVOICE          =%s devoiced %s in channel %s with the DEVOICE command.",
"TOPIC            =%s set the topic on %s with the TOPIC command.",
"KICK             =%s kicked %s from channel %s with the KICK command.",
"ANONKICK         =%s anonymously kicked %s from channel %s with the ANONKICK command.",
"INVITE           =%s invited %s to channel %s with the INVITE command.",
"UNBAN            =%s unbanned %s from channel %s with the UNBAN command.",
"COMMAND          =%s executed the %s command for channel %s.",
"LEVEL            =%s set the level of %s to %s.",
"ACCESS_ADD       =%s added %s to the access list for channel %s at level %s.",
"ACCESS_CHANGE    =%s changed %s on the access list of channel %s to level %s.",
"ACCESS_DEL       =%s removed %s from the access list for channel %s.",
"AKICK_ADD        =%s added %s to the autokick list for channel %s.",
"AKICK_DEL        =%s removed %s from the autokick list for channel %s.",
"GREET_ADD        =%s added a greeting for %s on channel %s.",
"GREET_DEL        =%s removed a greeting for %s on channel %s.",
"MESSAGE_ADD      =%s added a on-join message for channel %s.",
"MESSAGE_DEL      =%s removed a on-join message from channel %s.",
"SET_PASSWORD     =%s changed the password for channel %s.",
"SET              =%s set the value of %s for channel %s to %s.",
"LOCK             =%s locked the value of %s for channel %s to %s.",
"UNLOCK           =%s unlocked the value of %s for channel %s.",
"",
";",
"; These are used when a command is executed from MemoServ.",
";",
"[MEMOSERV]",
"SEND             =%s sent a memo to %s.",
"FILE             =%s sent a file attachment (%s / %d) to %s.",
"DEL              =%s removed a memo from the %s list.",
"PICTURE_ADD      =%s set a picture for nickname %s (%d).",
"PICTURE_DEL      =%s removed a picture from nickname %s.",
"",
";",
"; These are used when a command is executed from CommServ.",
";",
"[COMMSERV]",
"NEW              =%s created a new committee called %s with %s as its head.",
"KILL             =%s deleted committee %s.",
"ADD              =%s added nickname %s to committee %s.",
"DEL              =%s removed nickname %s from committee %s.",
"MEMO             =%s sent a memo to all members of committee %s.",
"LOGON_ADD        =%s added a new logon message to committee %s.",
"LOGON_DEL        =%s removed a logon message from committee %s.",
"SET              =%s set the value of %s for committee %s to %s.",
"",
";",
"; These are used when a command is executed from ServMsg.",
";",
"[SERVMSG]",
"GLOBAL           =%s sent message \"%s\" to all users.",
"",
";",
"; These are used when a command is executed from OperServ.",
";",
"[OPERSERV]",
"TRACE            =%s set the trace level of %s to %s.",
"MODE             =%s set mode \"%s\" for %s with the MODE command.",
"QLINE            =%s quarentined nickname %s.",
"UNQLINE          =%s unquarentined nickname %s.",
"NOOP_ON          =%s quarentined all ircops on server %s.",
"NOOP_OFF         =%s unquarentined all ircops on server %s.",
"KILL             =%s silently killed nickname %s with \"%s\".",
"PING             =%s triggered server pings manually.",
"UPDATE           =%s triggered database update manually.",
"SHUTDOWN         =%s REQUESTED A SHUTDOWN OF SERVICES.",
"RELOAD           =%s reloaded the services configuration file.",
"UNLOAD           =%s unloaded language %s.",
"JUPE             =%s juped server %s (%s).",
"ONOFF            =%s TURNED SERVICES %s.",
"ONOFF_SERVICE    =%s TURNED SERVICE %s %s.",
"CLONE_ADD        =%s added host %s to the clone override list with %d clones.",
"CLONE_DEL        =%s removed host %s from the clone override list.",
"AKILL_ADD        =%s added mask %s to the autokill list for %s for \"%s\".",
"AKILL_DEL        =%s removed mask %s from the autokill list.",
"OPERDENY_ADD     =%s added mask %s to the operdeny list (%s).",
"OPERDENY_DEL     =%s removed mask %s fro mthe operdeny list (%s).",
"IGNORE_ADD       =%s added mask %s to the services permanent ignore list.",
"IGNORE_DEL       =%s removed mask %s from the services permanent ignore list.",
"IGNORED          =Ignored message from %s: %s",
"",
";",
"; These are used when an automated event kicks off.",
";",
"[EVENT]",
"STARTUP          =MAGICK II HAS STARTED UP SUCCESSFULLY.",
"EXPIRE_AKILL     =Expiring akill for %s (%s) set by %s for %s.",
"EXPIRE_NICK      =Expiring nickname %s.",
"EXPIRE_CHANNEL   =Expiring channel %s.",
"EXPIRE_NEWS      =Expiring news article for channel %s.",
"LOAD             =Database version %d.%d has been loaded.",
"SAVE             =Database saved.",
"UNBAN            =Removed ban %s from channel %s due to timeout (%s).",
"KILLPROTECT      =Killed/renamed nickname %s for failure to identify.",
"PING             =Pinged all servers.",
"LEVEL_UP         =LIVE LEVEL HAS INCREASED BY ONE.",
"LEVEL_DOWN       =LIVE LEVEL HAS DECREASED BY ONE.",
"",
";",
"; These are errors that come from wx routines.",
";",
"[WX_ERRORS]",
"ENVIRONMENT      =Environment variables expansion failed - missing '%c' at position %d in '%s'.",
"EXTRA            ='%s' has extra '..', ignored.",
"GLOBAL_CFG       =Can't open global configuration file '%s'.",
"USER_CFG         =Can't open user configuration file '%s'.",
"USER_CFG_WRITE   =Can't write to user configuration file '%s'.",
"USER_CFG_DEL     =Can't delete user configuration file '%s'.",
"UNEXPECTED_CHAR  =File '%s': unexpected character %c at line %d.",
"UNEXPECTED_CHAR2 =Unexpected '%c' at position %d in '%s'.",
"LINE_IGNORED     =File '%s', line %d: '%s' ignored after group header.",
"EQUALS_EXPECTED  =File '%s', line %d: '=' expected but not received, ignored.",
"IMMUT_IGNORE     =File '%s', line %d: value for immutable key '%s' ignored.",
"IMMUT_IGNORE2    =Attempt to change immutable key '%s' ignored.",
"DUPLICATE_KEY    =File '%s', line %d: key '%s' was first found at line %d.",
"DUPLICATE_KEY2   =Entry '%s' appears more than once in group '%s'",
"MAYNOTSTART      =Entry name can't start with '%c'.",
"MAYNOTCONTAIN    =Character '%c' is invalid in a config entry name.",
"NOHOMEDIR        =Can't find user's HOME, using current directory.",
"BINFILE          ='%s' is probably a binary file.",
"CANTWRITE        =Can't write file '%s' to disk.",
"OUTOFTXNIDS      =Cannot create any more Transaction ID's",
"",
";",
"; This is a section for miscellaneous other occurances.",
";",
"[OTHER]",
"SQUIT_FIRST      =Suspected SQUIT detected, servers %s and %s, waiting ...",
"SQUIT_CANCEL     =Suspected SQUIT of servers %s and %s was fake.",
"SQUIT_SECOND     =SQUIT of servers %s and %s confirmed, taking appropriate action.",
"KILL_CLONE       =Clone protection triggered for %s, killing user.",
"KILL_AKILL       =Killing user %s on autokill list (%s).",
"CONNECTING       =Connecting to server %s:%d ...",
"CONNECTED        =Connection established and authenticated to server %s.",
"WRONGPASS        =Password mismatch connecting to server %s, skipping.",
"WEAREBANNED      =We are k-lined from server %s, skipping.",
"REFUSED          =Connection to server %s:%d refused.",
"SERVER_MSG       =SERVER MESSAGE (%s): %s",
"LINK             =Server %s has linked to the network via. %s.",
"KILLED           =Service %s has been killed by %s, attempting to respawn.",
"TEMP_IGNORE      =%s triggered services temporary ignore.",
"PERM_IGNORE      =%s triggered services permanent ignore.",
"KILL_NICK_PASS   =Killing user %s for password failures on nickname %s.",
"KILL_CHAN_PASS   =Killing user %s for password failures on channel %s.",
"LOAD_LANGUAGE    =Loaded %s language file.",
"",
";",
"; These are errors (of varying severity) that we may encounter.",
";",
"[ERROR]",
"SIGNAL           =RECEIVED SIGNAL %d, EXITING.",
"REQ_BYNONUSER    =%s command requested by non-existant user %s.",
"REQ_BYNONSERVICE =%s command requested by non-service %s.",
"REQ_FORNONUSER   =%s command requested by %s for non-existant user %s.",
"REQ_TONONUSER    =%s command requested by %s to non-existant user %s.",
"REQ_FORNONCHAN   =%s command requested by %s for non-existant channel %s.",
"REQ_FORNONSERVER =%s command requested by %s for non-existant server %s.",
"REQ_NOTINCHAN    =%s command requested by %s for %s who is not in channel %s.",
"UNKNOWN_MSG      =Unknown message from server: %s",
"REC_FORNONUSER   =Received %s from %s for non-existant user %s.",
"REC_FORNONCHAN   =Received %s from %s for non-existant channel %s.",
"REC_NOTINCHAN    =Received %s from %s for %s who is not in channel %s.",
"REC_FORNOTINCHAN =Received %s for %s who is not in channel %s.",
"DUP_CHAN         =Duplicate %s received from %s for channel %s.",
"MODE_INEFFECT    =MODE change %c%c received from %s for %s that is already in effect.",
"MODE_NOTINCHAN   =MODE change %c%c received from %s for %s who is not in channel %s.",
"HOST_NOTREGD     =Nickname %s was listed as the host of %s, but does not exist (rectified)!",
"SLAVE_NOTREGD    =Nickname %s was listed as a slave of %s, but does not exist (rectified)!",
"KEYMISMATCH      =Channel key mismatch (%s | %s) for channel %s from %s.",
"NOLIMIT          =No limit specified when required for channel %s from %s.",
"NOLANGTOKEN      =Invalid token %s for language %s used, error returned.",
"FAILED_FORK      =Failed to fork new process with %d, terminating.",
"FAILED_SETPGID   =Failed to set permissions on process with %d, terminating.",
"EXCEPTION        =Exception thrown on line %d, column %d of %s.",
"" };
#endif
