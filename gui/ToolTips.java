// Magick IRC Services
//
// (c) 1997-2002 Preston Elder <prez@magick.tm>
// (c) 1998-2002 William King <ungod@magick.tm>
//
// The above copywright may not be removed under any circumstances,
// however it may be added to if any modifications are made to this
// file.  All modified code must be clearly documented and labelled.
//
// This code is released under the GNU General Public License, which
// means (in short), it may be distributed freely, and may not be sold
// or used as part of any closed-source product.  Please check the
// COPYING file for full rights and restrictions of this software.
//
// =======================================================================
// rcsid = @(#)$Id$
// =======================================================================
//
// For official changes (by the Magick Development Team),please
// check the ChangeLog* files that come with this distribution.
//
// Third Party Changes (please include e-mail address):
//
// N/A
//
// =======================================================================

public class ToolTips
{
    private static final String[][] tips = {
	{ "Startup/SERVER_NAME",	"The server name we will sign onto the IRC network as." },
	{ "Startup/SERVER_DESC",	"The description of our 'server'.  This will show up when\n" +
					"a user does a /list (to obtain a list of servers)." },
	{ "Startup/SERVICES_USER",	"The username (ident) that all services users will sign on\n" +
					"with.  This is disabled if Own User is active." },
	{ "Startup/SERVICES_HOST",	"The host that all services users will appear to be coming\n" +
					"from." },
	{ "Startup/OWNUSER",		"If this is on, all services will sign on with thier own\n" +
					"nickname (lowercased) as their username (ident), making\n" +
					"the Services User field irrelivant." },
	{ "Startup/SETMODE",		"The user modes that all services users will set on signon." },
	{ "Startup/BIND",		"IP Address to bind to.  If you have more than one IP address\n" +
					"attached to your machine, and you want to bind to a specific\n" +
					"one, use this parameter.  Setting this to 0.0.0.0 will mean\n" +
					"'bind to anything', usually being IP address of the first\n" +
					"network interface on the machine." },
	{ "Startup/REMOTES",		"Each entry in this table is an IRC server that Magick will\n" +
					"attempt to connect to.  Magick will go through each entry in\n" +
					"first priority order, then in a numerically sorted order.  You\n" +
					"may not have two or more entries in this list with the same\n" +
					"IP address." },
	{ "Startup/ALLOWS",		"If any entries are listed in this table, ONLY servers listed\n" +
					"in this table will be able to connect to the IRC network.  If\n" +
					"a server connects with a name not in this table, Magick will\n" +
					"disconnect it from the network.  The servers in this list are\n" +
					"case sensative.  If any entries are listed, you must add an\n" +
					"entry for each entry in the REMOTE table, however you do not\n" +
					"need to list the services server name.\n\n" +
					"If the uplink field is used, the server specified may only\n" +
					"connect to the server(s) listed.  You may specify multipal\n" +
					"uplink servers by separating them with a comma (,).  You may\n" +
					"also use wildcards in this field." },
	{ "Startup/LEVEL",		"As lag to the IRC network increases and decreases, the level\n" +
					"services operate at will increase and decrease respectively,\n" +
					"with this value as the minimum.  If a set of services with a\n" +
					"lower services level is detected on the network, we will hand\n" +
					"over all primary services duties to that set of services.  We\n" +
					"will only regain these duties if our current services level\n" +
					"falls below the current primary set of services.  This value\n" +
					"should be used to weight this calculation to ensure that some\n" +
					"services will always become primary services again." },
	{ "Startup/LAGTIME",		"This determines how much lag (PING response time) is required\n" +
					"to increase the current level services operate at.  The lag\n" +
					"time is determined by the formula of:\n" +
					"((all server pings)-(highest)-(lowest))/(number of servers)-2" },
	{ "Startup/STOP",		"This will stop Magick from starting up.  This is here to force\n" +
					"people to edit their configuration instead of just running with\n" +
					"the defaults."  },

	{ "Services/NickServ",		"Nicknames that will respond as a NickServ equivalent." },
	{ "Services/NickServ_Name",	"Real name (aka. GCOS) of all NickServ clients." },
	{ "Services/Enforcer_Name",	"Real name (aka. GCOS) of any nickname signed on to hold a nick\n" +
					"requested with the NickServ RECOVER command, or that has been\n" +
					"renamed or killed for failure to identify." },
	{ "Services/ChanServ",		"Nicknames that will respond as a ChanServ equivalent." },
	{ "Services/ChanServ_Name",	"Real name (aka. GCOS) of all ChanServ clients." },
	{ "Services/MemoServ",		"Nicknames that will respond as a MemoServ equivalent." },
	{ "Services/MemoServ_Name",	"Real name (aka. GCOS) of all MemoServ clients." },
	{ "Services/OperServ",		"Nicknames that will respond as a OperServ equivalent." },
	{ "Services/OperServ_Name",	"Real name (aka. GCOS) of all OperServ clients." },
	{ "Services/CommServ",		"Nicknames that will respond as a CommServ equivalent." },
	{ "Services/CommServ_Name",	"Real name (aka. GCOS) of all CommServ clients." },
	{ "Services/ServMsg",		"Nicknames that will respond as a ServMsg equivalent." },
	{ "Services/ServMsg_Name",	"Real name (aka. GCOS) of all ServMsg clients." },
	{ "Services/SHOWSYNC",		"This will show the \"Databases sync in ...\" message at the\n" +
					"bottom of several INFO displays throughout Magick.  Some\n" +
					"networks do not want users to know how long until databases\n" +
					"are saved." },
	{ "Services/QUIT_MESSAGE",	"Whenever services signs off one of its client for any reason,\n" +
					"this will be the quit message sent." },

	{ "Files/UMASK",		"This is permissions mask used when creating a file or directory.\n" +
					"This number is on octal (ie. only numbers 0-7 count), and should\n" +
					"be 3 digits (with an optional leading 0), representing the access\n" +
					"permissions of the user creating the file or directory, the group\n" +
					"the file or directory is set to and everyone else respectively.\n\n" +
					"The value for each digit should be:\n" +
					"0 = FULL access                 1 = Read and write access\n" +
					"2 = Read and exeute access      3 = Read only access\n" +
					"4 = Write and execute access    5 = Write only access\n" +
					"6 = Execute only access         7 = No access\n\n" +
					"Read access gives the ability to either view the file if it is\n" +
					"a file being created, or list files in the directory if it is a\n" +
					"directory being created.\n" +
					"Write access gives the ability to either edit the contents of\n" +
					"the file if it is a file being created, or create or remove\n" +
					"files in the directory if it is a directory being created.\n" +
					"Execute access only relates to the creation of directories, and\n" +
					"gives access to enter the particular directory." },
	{ "Files/PROTOCOL",		"The file that defines details about the IRC protocol (language)\n" +
					"that Magick will use to talk to the IRC server.\n" +
					"Each IRC server software implements the IRC protocol in a\n" +
					"slightly different way, so if this value is incorrectly set,\n" +
					"Magick will not function properly (and may not even sign on)." },
	{ "Files/PIDFILE",		"The file that the process ID of Magick will be put into.\n" +
					"If the path is not absolute, it will be put in a place relative\n" +
					"to the services directory (ie. the value of the --dir commandline\n" +
					"argument, or the current directory at magick's invocation)." },
	{ "Files/LOGFILE",		"The file that Magick will output its log information to.\n" +
					"If the path is not absolute, it will be put in a place relative\n" +
					"to the services directory (ie. the value of the --dir commandline\n" +
					"argument, or the current directory at magick's invocation)." },
	{ "Files/VERBOSE",		"This will cause ALL commands sucessfully executed by services to\n" +
					"be logged (as opposed to only commands of importance)." },
	{ "Files/LOGCHAN",		"A channel (or channels separated by a comma (,)) that all output\n" +
					"that goes into the log file, will also be output to online." },
	{ "Files/MOTDFILE",		"The file where the Message of the Day (that is sent on a /motd\n" +
					"request from a user) is read from.\n" +
					"If the path is not absolute, it will be read from a place relative\n" +
					"to the services directory (ie. the value of the --dir commandline\n" +
					"argument, or the current directory at magick's invocation)." },
	{ "Files/LANGDIR",		"The directory where all language files (*.lng, *.lfo and *.hlp)\n" +
					"are kept.  Each language file should be the name of the language\n" +
					"all in lower case, with an extension of lng for general command\n" +
					"response text, lfo for messages that output to the log file, and\n" +
					"hlp for the response to all HELP commands.\n" +
					"If the path is not absolute, a path relative to the services\n" +
					"directory (ie. the value of the --dir commandline argument or the\n" +
					"current directory at magick's invocation) will be assumed." },
	{ "Files/DATABASE",		"The file the Magick New Database (mnd) is stored in.\n" +
					"If the path is not absolute, it will be put in a place relative\n" +
					"to the services directory (ie. the value of the --dir commandline\n" +
					"argument, or the current directory at magick's invocation)." },
	{ "Files/COMPRESSION",		"The level of compression applied to the database when stored." },
	{ "Files/KEYFILE",		"The file that contains the encrypted key that is used to encrypt\n" +
					"and decrypt the database file.\n" +
					"If the path is not absolute, a path relative to the services\n" +
					"directory (ie. the value of the --dir commandline argument or the\n" +
					"current directory at magick's invocation) will be assumed." },
	{ "Files/ENCRYPTION",		"This will activate database encryption on databases (assuming\n" +
					"a valid key is found)." },
	{ "Files/PICTURE",		"The directory pictures (attached to nickname records) are stored.\n" +
					"If the path is not absolute, a path relative to the services\n" +
					"directory (ie. the value of the --dir commandline argument or the\n" +
					"current directory at magick's invocation) will be assumed." },
	{ "Files/MEMOATTACH",		"The directory memo attachments are stored.\n" +
					"If the path is not absolute, a path relative to the services\n" +
					"directory (ie. the value of the --dir commandline argument or the\n" +
					"current directory at magick's invocation) will be assumed." },
	{ "Files/PUBLIC",		"The directory files in the public file system are stored.\n" +
					"If the path is not absolute, a path relative to the services\n" +
					"directory (ie. the value of the --dir commandline argument or the\n" +
					"current directory at magick's invocation) will be assumed." },
	{ "Files/TEMPDIR",		"The directory files in transit (temporary files) are stored.\n" +
					"If the path is not absolute, a path relative to the services\n" +
					"directory (ie. the value of the --dir commandline argument or the\n" +
					"current directory at magick's invocation) will be assumed." },
	{ "Files/PICTURE_SIZE",		"The amount of space we do not wish to exceed for storage of all\n" +
					"files in the picture directory.  If we exceed this size, no more\n" +
					"pictures may be stored.  Setting this to 0b means no limit." },
	{ "Files/MEMOATTACH_SIZE",	"The amount of space we do not wish to exceed for storage of all\n" +
					"files in the memo attachment directory.  If we exceed this size, no\n" +
					"more memos may have attachments may be stored.  Setting this to 0b\n" +
					"means no limit." },
	{ "Files/PUBLIC_SIZE",		"The amount of space we do not wish to exceed for storage of all\n" +
					"files in the public file system directory.  If we exceed this size,\n" +
					"no more files may be uploaded into the public file system.  Setting\n" +
					"this to 0b means no limit." },
	{ "Files/TEMPDIR_SIZE",		"The amount of space we do not wish to exceed for storage of all\n" +
					"files in temporary files directory.  If we exceed this size, no\n" +
					"more DCC's may initiate.  Setting this to 0b means no limit." },
	{ "Files/BLOCKSIZE",		"The default block size (amount of data sent before a response is\n" +
					"required to continue the DCC transfer)." },
	{ "Files/TIMEOUT",		"The amount of time that may elapse with no data sent or recieved\n" +
					"in the DCC session before it times out the DCC transfer.  This is\n" +
					"also the amount of time we will wait for a connection to establish." },
	{ "Files/MIN_SPEED",		"The minimum speed (in bytes per second) that must be maintained\n" +
					"for a DCC transfer.  If the average speed over any one sample time\n" +
					"frame falls below this, the DCC will be terminated." },
	{ "Files/MAX_SPEED",		"The maximum speed (in bytes per second) that may be achieved for\n" +
					"a DCC transfer.  If the average speed over any one sample time\n" +
					"exceeds this, the DCC will be throttled to reduce its speed." },
	{ "Files/SAMPLETIME",		"The amount of time to take into consideration when working out the\n" +
					"current average speed of a DCC transfer." },

	{ "Config/SERVER_RELINK",	"Delay beteen attempts to reconnect to a server if disconnected.\n" +
					"Setting this to 0 will disable reconnecting to a server." },
	{ "Config/SQUIT_PROTECT",	"The amount of time to remember details about a user after they\n" +
					"have quit because of a network split.  After this time, if the\n" +
					"network is rejoined, the user will be considered a new sign-on." },
	{ "Config/SQUIT_CANCEL",	"The amount of time to wait for a SQUIT message after seeing a\n" +
					"user quit with 2 server names as their quit message.  This is\n" +
					"to protect against people trying to trick the SQUIT protection." },
	{ "Config/CYCLETIME",		"Delay between 'active' cycles.  The active cycle includes such\n" +
					"things as nickname/channel/news/akill expiry." },
	{ "Config/SAVETIME",		"Delay between database saves." },
	{ "Config/CHECKTIME",		"Delay between 'hypercheck' cycles.  The hyperative cycle includes\n" +
					"such things as checking to see if bans should be removed, checks\n" +
					"to see who needs to be renamed/killed for failure to identify,\n" +
					"etc.  Some of these checks have to go through the entire list\n" +
					"of channels or nicknames to complete their task, so if you are\n" +
					"seeing magick continually use too much CPU, try raising this." },
	{ "Config/PING_FREQUENCY",	"Delay between server pings (used to check the lag)." },
	{ "Config/STARTHRESH",		"Non-wildcard characters needed for most lists that store hosts or\n" +
					"hostmasks of any kind.\n\n" +
					"Setting this to 3 means *.com will be allows, but *.au wont.\n" +
					"Setting this to 4 means *.com will not work." },
	{ "Config/LISTSIZE",		"The default amount of entries to return doing any kind of LIST." },
	{ "Config/MAXLIST",		"The maximum amount of entries you may request from any kind of LIST." },
	{ "Config/MIN_THREADS",		"The minimum amount of 'worker' threads to keep active at any time.\n" +
					"Usually this should be more than one, otherwise if a request is\n" +
					"recieved that requies something intensive to happen (eg. loadinf of\n" +
					"a help file), nothing else can be done until it is finished." },
	{ "Config/MAX_THREADS",		"The maximum amount of 'worker' threads allows to be active at any\n" +
					"one time.  Creating too many threads can cause a number of problems\n" +
					"including running out of process space, and increasing the amount of\n" +
					"time it takes to process a message.  Threads are designed to allow\n" +
					"multipal things to happen at once, but too many can bring a system\n" +
					"to its knees." },
	{ "Config/LOW_WATER_MARK",	"This number is used in an equasion to figure out whether to release\n" +
					"a thread.  The equasion to work this out is:\n\n" +
					"(((number of threads) - 2) * (high water mark)) + (low water mark)\n" +
					"    is greater than (number of unprocessed messages)" },
	{ "Config/HIGH_WATER_MARK",	"This number is used in an equasion to figure out whether to spawn a\n" +
					"new thread.  The equasion to work this out is:\n\n" +
					"(number of threads) * (high water mark)\n" +
					"    is less than (number of unprocessed messages)" },
	{ "Config/HEARTBEAT_TIME",	"Delay between checks to ensure all 'worker' threads are still alive.\n" +
					"If a thread stops responding, it will be terminated and re-started.\n" +
					"If more than half of all threads are dead, Magick will terminate." },
	{ "Config/MSG_SEEN_TIME",	"How long to wait for a message's dependancies to be filled, before\n" +
					"marking it as processable (even without its dependancies)." },
	{ "Config/MSG_CHECK_TIME",	"Delay between going through all outstanding messages, looking for\n" +
					"messages that have been marked processable (without dependancies),\n" +
					"and scheduling them for rush processing." },

	{ "NickServ/APPEND_RENAME",	"This determines the renaming scheme to use when renaming a user for\n" +
					"failure to identify.  Selecting append will cause services to go\n" +
					"through the suffixes list, and attempt to rename by appending one (or\n" +
					"more) suffix character to the nickname.  Selecting rename will cause\n" +
					"services to rename the user to a defined prefix with 5 random numbers." },
	{ "NickServ/SUFFIXES",		"When renaming a user, services will go through each character in this\n" +
					"list, and try to attach 1, then 2, and so on occurances of it to the\n" +
					"end of the nickname until either a nickname is found that is not in\n" +
					"use and not registered, or the maximum nickname length is reached.\n" +
					"If no suitable nickname can be found by the time all suffix characters\n" +
					"have been exhausted, the user will be killed instead." },
	{ "NickServ/PREFIX",		"When renaming a user, services will attach 5 random numbers to this\n" +
					"prefix, and attempt to use that as the nickname.  Services will try\n" +
					"up to 64 different random numbers.  If one combination cannot be\n" +
					"found that is both not in use, and not registered, the user will\n" +
					"will be killed instead." },
	{ "NickServ/EXPIRE",		"Time to keep a nick registered without seeing the user who owns the\n" +
					"nick online (either recognized or identified)." },
	{ "NickServ/DELAY",		"Delay between allowing nicknames to be registered by one user." },
	{ "NickServ/IDENT",		"Time to wait for user to identify when secure or not recognized." },
	{ "NickServ/RELEASE",		"Time to keep nickname after taking it because of a RECOVER command\n" +
					"or failure to identifiy." },
	{ "NickServ/PASSFAIL",		"Amount of times the user is allowd to fail identifying to their nick\n" +
					"before being killed for too many failed attempts." },
	{ "NickServ/OPTIONS",		"Default setting for all options settable on a nickname record, and\n" +
					"whether this setting should be changable by the user." },
	{ "NickServ/PICSIZE",		"Maximum size of the picture a user may store in their nickname record.\n" +
					"Setting this to 0b means there is no size limit." },
	{ "NickServ/PICEXT",		"Valid extensions for the picture a user mat store in their nickname\n" +
					"record.  Setting this to nothing will disable picture storage." },
	{ "NickServ/DEF_LANGUAGE",	"Default language a user will have set (also the language that log\n" +
					"files are output in)." },
	{ "NickServ/LCK_LANGUAGE",	"Whether the user should be able to choose the language services uses." },

	{ "ChanServ/HIDE",		"This will cause the primary ChanServ nickname to set mode +h (hidden).\n" +
					"This is used to prevent people using ChanServ as some kind of advert\n" +
					"for their channel by setting JOIN on, and hoping people do a /whois\n" +
					"on ChanServ and join their channel because of it.  It also helps cut\n" +
					"down the size of the channel list when doing a /whois on ChanServ." },
	{ "ChanServ/EXPIRE",		"Time to keep a channel registered without anyone on the access list\n" +
					"or the founder joining." },
	{ "ChanServ/DELAY",		"Delay between allowing channels to be registered by one user." },
	{ "ChanServ/MAX_PER_NICK",	"Maximum amount of channels that may have a single user listed as\n" +
					"their founder or co-founder." },
	{ "ChanServ/MAX_MESSAGES",	"Maximum number of on-join messages a channel may have." },
	{ "ChanServ/DEF_AKICK",		"Default reason for AutoKicking someone if none is supplied." },
	{ "ChanServ/PASSFAIL",		"Amount of times a user is allowed to fail identifying to a single\n" +
					"channel before being killed for too many failed attempts." },
	{ "ChanServ/CHANKEEP",		"Delay before ChanServ exits a channel after auto kicking the last user\n" +
					"in it (to maintain the ban, and stop any auto-rejoin)." },
	{ "ChanServ/DEF_MLOCK",		"Default Mode Lock to set for a channel." },
	{ "ChanServ/LCK_MLOCK",		"Modes that cannot be removed from a channel's Mode Lock." },
	{ "ChanServ/DEF_BANTIME",	"Default time that has to elapse before a ban is automatically removed." },
	{ "ChanServ/LCK_BANTIME",	"Whether the default ban removal time may be changed by channel staff." },
	{ "ChanServ/DEF_PARTTIME",	"Default amount of time that a user has to have been outside of a\n" +
					"channel before their greeting will be displayed again when they join." },
	{ "ChanServ/LCK_PARTTIME",	"Whether the default part time may be changed by channel staff." },
	{ "ChanServ/OPTIONS",		"Default setting for all options settable on a channel record, and\n" +
					"whether this setting should be changable by channel staff." },
	{ "ChanServ/DEF_REVENGE",	"Default revenge mode.  Revenge is triggered by a user deoping, kicking\n" +
					"or banning another user who is higher on the access list." },
	{ "ChanServ/LCK_REVENGE",	"Whether the default revenge mode may be changed by channel staff." },
	{ "ChanServ/LEVEL_MIN",		"Minimum level that someone may be added to the access list at." },
	{ "ChanServ/LEVEL_MAX",		"Maximum level that someone may be added to the access list at." },
	{ "ChanServ/LEVELS",		"Default levels for all access categories that are used to grant\n" +
					"users privilages on a channel.  Each entry may not have a value\n" +
					"less than the minimum access level, or more than the maximum\n" +
					"access level + 2.  A setting of the maximum access level + 1 means\n" +
					"only the channel founder may recieve the privilages, a setting\n" +
					"of maximum access level + 2 means noone recieves these privilages.\n" +
					"These values may always be changed per-channel by the channel founder." },

	{ "MemoServ/NEWS_EXPIRE",	"Time to keep a news entry active before expiring it." },
	{ "MemoServ/INFLIGHT",		"Delay between last memo sending action (SEND, FORWARD, REPLY, FILE\n" +
					"CONTINUE) and actually sending it to its final destination.  This\n" +
					"delay gives the user a chance to continue the memo, or cancel it.  A\n" +
					"memo will automatically flushed (sent) upon any new memo sending\n" +
					"action (except CONTINUE)." },
	{ "MemoServ/DELAY",		"Delay between allowing a new memo to be sent by one user." },
	{ "MemoServ/FILES",		"Amount of files a user may have waiting for them (in the form of\n" +
					"memo attachments) before any new memo attachments to this user are\n" +
					"rejected.  This limit is not a 'hard' limit, if the user has room\n" +
					"to accept one more memo attachment, and two people start sending him\n" +
					"one at once, both will get through.  Setting this to 0 will disable\n" +
					"sending memo attachments." },
	{ "MemoServ/FILESIZE",		"Maximum size of each file a user may have waitinf for them as a memo\n" +
					"attachment.  Setting this to 0b means there is no limit." },

	{ "OperServ/SERVICES_ADMIN",	"A list of nicknames (separated by a space) that will automatically be\n" +
					"added to the Services Administrator committee." },
	{ "OperServ/SECURE",		"Only people who are IRC Operators (have mode +o) may access OperServ.\n" +
					"If off, anyone may access OperServ as long as they're on one of the\n" +
					"privilaged committees." },
	{ "OperServ/SECUREOPER",	"Only people on the IRC Operator committee may set mode +o (become an\n" +
					"IRC Operator).  If the IRC Operator committee is secure, then this\n" +
					"will force a user to identify before becoming an IRC Operator." },
	{ "OperServ/DEF_EXPIRE",	"Default time for AutoKills to expire if no expiry time is supplied." },
	{ "OperServ/EXPIRE_OPER",	"Maximum expiry time a member of the IRC Operator committee may set on\n" +
					"an AutoKill." },
	{ "OperServ/EXPIRE_ADMIN",	"Maximum expiry time a member of the IRC Administrator committee may set\n" +
					"on an AutoKill." },
	{ "OperServ/EXPIRE_SOP",	"Maximum expiry time a member of the Services Operator committee may set\n" +
					"on an AutoKill." },
	{ "OperServ/EXPIRE_SADMIN",	"Maximum expiry time a member of the Services Administrator committee may\n" +
					"set on an AutoKill." },
	{ "OperServ/AKILL_REJECT",	"Reject an AutoKill if it would kill more than this percentage of the\n" +
					"network." },
	{ "OperServ/MAX_CLONE",		"Maximum amount that a clone override may be added to grant." },
	{ "OperServ/CLONE_LIMIT",	"Default clone limit.  Any connections from the same host above this\n" +
					"number will automatically be killed off the network, unless there is\n" +
					"a clone override in place - in which case the limit specified in the\n" +
					"clone override will be used instead." },
	{ "OperServ/DEF_CLONE",		"Reason to use for killing a user for exceeding the clone limit." },
	{ "OperServ/CLONE_TRIGGER",	"Amount of times a host may be killed from the network for triggering\n" +
					"clone protection before an automatic AutoKill will be place on that host." },
	{ "OperServ/CLONE_TIME",	"Amount of time to remember old triggers of clone protection." },
	{ "OperServ/CLONE_AKILL",	"Reason to use for the automatic AutoKill set for exceeding the clone\n" +
					"trigger limit." },
	{ "OperServ/CLONE_AKILLTIME",	"Expiry time to use for the automatic AutoKill set for exceeding the\n" +
					"clone trigger limit." },
	{ "OperServ/FLOOD_TIME",	"Time in which the amount of messages in Flood Messages must be recieved\n" +
					"to trigger flood protection." },
	{ "OperServ/FLOOD_MSGS",	"Amount of messages that must be recieved in the time specified in Flood\n" +
					"Time to trigger flood protection." },
	{ "OperServ/IGNORE_TIME",	"Time to ignore a user when they trigger flood protection." },
	{ "OperServ/IGNORE_LIMIT",	"Amount of times a user may trigger flood protection before they are added\n" +
					"to the permanent ignore list." },
	{ "OperServ/IGNORE_REMOVE",	"Amount of time to remember old triggers of flood protection." },
	{ "OperServ/IGNORE_METHOD",	"Mask that should be used when setting an ignore on a user." },
	{ "OperServ/LOG_IGNORE",	"Log all messages recieved from a user who is being ignored." },
	{ "Files/SAMPLETIME",		"The amount of time to take into consideration when working out the\n" +
					"current average speed of a DCC transfer." },
	{ "OperServ/INIT_HTM_GAP",	"Initial amount of time to take into consideration when working out the\n" +
					"current average speed of the IRC connection for determining whether to\n" +
					"switch to High Traffic Mode." },
	{ "OperServ/INIT_HTM_THRESH",	"Average amount of traffic that must be achieved over the HTM gap to\n" +
					"switch to or stay in High Traffic Mode." },
	{ "OperServ/MAX_HTM_GAP",	"Maximum size of the HTM gap before automatically dropping connection to\n" +
					"the IRC server." },
	{ "OperServ/HTM_ON_GAP",	"Amount of time to set the HTM gap to when it is manually switched on." },

	{ "CommServ/MAX_LOGON",		"Maximum number of logon messages a committee may add." },
	{ "CommServ/OPTIONS",		"Default setting for all options settable on a committee, and\n" +
					"whether this setting should be changable by the committee head." },
	{ "CommServ/COMMITTEES",	"Names, default settings, and modes to set on members for all\n" +
					"default committees." },
	{ "CommServ/OVERRIDES",		"Committee membership required to perform various overrides\n" +
					"throughout services." },

    };

    public String getTip(String key)
    {
	for (int i=0; i<tips.length; i++)
	    if (key.equals(tips[i][0]))
		return tips[i][1];
	return "";
    }
}
