#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
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
#define RCSID(x,y) const char *rcsid_filesys_cpp_ ## x () { return y; }
RCSID(filesys_cpp, "@(#)$Id$");
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.69  2001/05/01 14:00:23  prez
** Re-vamped locking system, and entire dependancy system.
** Will work again (and actually block across threads), however still does not
** work on larger networks (coredumps).  LOTS OF PRINTF's still int he code, so
** DO NOT RUN THIS WITHOUT REDIRECTING STDOUT!  Will remove when debugged.
**
** Revision 1.68  2001/04/02 02:11:23  prez
** Fixed up some inlining, and added better excption handling
**
** Revision 1.67  2001/03/27 07:04:31  prez
** All maps have been hidden, and are now only accessable via. access functions.
**
** Revision 1.66  2001/03/20 14:22:14  prez
** Finished phase 1 of efficiancy updates, we now pass mstring/mDateTime's
** by reference all over the place.  Next step is to stop using operator=
** to initialise (ie. use mstring blah(mstring) not mstring blah = mstring).
**
** Revision 1.65  2001/03/08 08:07:41  ungod
** fixes for bcc 5.5
**
** Revision 1.64  2001/03/04 02:04:14  prez
** Made mstring a little more succinct ... and added vector/list operations
**
** Revision 1.63  2001/03/02 05:24:41  prez
** HEAPS of modifications, including synching up my own archive.
**
** Revision 1.62  2001/02/11 07:41:27  prez
** Enhansed support for server numerics, specifically for Unreal.
**
** Revision 1.61  2001/02/03 02:21:33  prez
** Loads of changes, including adding ALLOW to ini file, cleaning up
** the includes, RCSID, and much more.  Also cleaned up most warnings.
**
** Revision 1.60  2001/01/16 15:47:40  prez
** Fixed filesys not generating first entry in maps, fixed chanserv level
** changes (could confuse set) and fixed idle times on whois user user
**
** Revision 1.59  2001/01/15 23:31:38  prez
** Added LogChan, HelpOp from helpserv, and changed all string != ""'s to
** !string.empty() to save processing.
**
** Revision 1.58  2001/01/01 05:32:44  prez
** Updated copywrights.  Added 'reversed help' syntax (so ACCESS HELP ==
** HELP ACCESS).
**
** Revision 1.57  2000/12/23 22:22:24  prez
** 'constified' all classes (ie. made all functions that did not need to
** touch another non-const function const themselves, good for data integrity).
**
** Revision 1.56  2000/12/21 14:18:17  prez
** Fixed AKILL expiry, added limit for chanserv on-join messages and commserv
** logon messages.  Also added ability to clear stats and showing of time
** stats are effective for (ie. time since clear).  Also fixed ordering of
** commands, anything with 2 commands (ie. a space in it) should go before
** anything with 1.
**
** Revision 1.55  2000/12/19 07:24:53  prez
** Massive updates.  Linux works again, added akill reject threshold, and
** lots of other stuff -- almost ready for b6 -- first beta after the
** re-written strings class.  Also now using log adapter!
**
** Revision 1.54  2000/10/10 11:47:51  prez
** mstring is re-written totally ... find or occurances
** or something has a problem, but we can debug that :)
**
** Revision 1.53  2000/09/30 10:48:07  prez
** Some general code cleanups ... got rid of warnings, etc.
**
** Revision 1.52  2000/09/22 12:26:11  prez
** Fixed that pesky bug with chanserv not seeing modes *sigh*
**
** Revision 1.51  2000/09/18 08:17:57  prez
** Intergrated mpatrol into the xml/des sublibs, and did
** some minor fixes as a result of mpatrol.
**
** Revision 1.50  2000/09/13 12:45:33  prez
** Added intergration of mpatrol (memory leak finder).  Default is set OFF,
** must enable with --enable-mpatrol in configure (and have mpatrol in system).
**
** Revision 1.49  2000/09/12 21:17:02  prez
** Added IsLiveAll (IsLive now checks to see if user is SQUIT).
**
** Revision 1.48  2000/09/05 10:53:06  prez
** Only have operserv.cpp and server.cpp to go with T_Changing / T_Modify
** tracing -- also modified keygen to allow for cmdline generation (ie.
** specify 1 option and enter keys, or 2 options and the key is read from
** a file).  This allows for paragraphs with \n's in them, and helps so you
** do not have to type out 1024 bytes :)
**
** Revision 1.47  2000/09/02 07:20:45  prez
** Added the DumpB/DumpE functions to all major objects, and put in
** some example T_Modify/T_Changing code in NickServ (set email).
**
** Revision 1.46  2000/08/31 06:25:09  prez
** Added our own socket class (wrapper around ACE_SOCK_Stream,
** ACE_SOCK_Connector and ACE_SOCK_Acceptor, with tracing).
**
** Revision 1.45  2000/08/28 10:51:37  prez
** Changes: Locking mechanism only allows one lock to be set at a time.
** Activation_Queue removed, and use pure message queue now, mBase::init()
** now resets us back to the stage where we havnt started threads, and is
** called each time we re-connect.  handle_close added to ircsvchandler.
** Also added in locking for all accesses of ircsvchandler, and checking
** to ensure it is not null.
**
** Revision 1.44  2000/08/08 03:46:21  prez
** Fixed problem with dcc not connecting (eg. connection refused) crashing
** services.
**
** Revision 1.43  2000/08/07 12:20:27  prez
** Fixed akill and news expiry (flaw in logic), added transferral of
** memo list when set new nick as host, and fixed problems with commserv
** caused by becoming a new host (also made sadmin check all linked nicks).
**
** Revision 1.42  2000/08/06 21:56:14  prez
** Fixed some small problems in akill/clone protection
**
** Revision 1.41  2000/07/30 09:04:05  prez
** All bugs fixed, however I've disabled COM(()) and CP(()) tracing
** on linux, as it seems to corrupt the databases.
**
** Revision 1.40  2000/07/29 21:58:53  prez
** Fixed XML loading of weird characters ...
** 2 known bugs now, 1) last_seen dates are loaded incorrectly on alot
** of nicknames, which means we expire lots of nicknames.  2) services
** wont rejoin a +i/+k channel when last user exits.
**
** Revision 1.39  2000/07/21 00:18:49  prez
** Fixed database loading, we can now load AND save databases...
**
** Almost ready to release now :)
**
** Revision 1.38  2000/07/11 13:22:18  prez
** Fixed loading/saving -- they now work with encryption and compression.
** Tested, it works too!  Now all we need to do is fix the loading, and
** we're set ... :))
**
** Revision 1.37  2000/06/25 07:58:49  prez
** Added Bahamut support, listing of languages, and fixed some minor bugs.
**
** Revision 1.36  2000/06/21 09:00:05  prez
** Fixed bug in mFile
**
** Revision 1.35  2000/06/18 12:49:27  prez
** Finished locking, need to do some cleanup, still some small parts
** of magick.cpp/h not locked properly, and need to ensure the case
** is the same every time something is locked/unlocked, but for the
** most part, locks are done, we lock pretty much everything :)
**
** Revision 1.34  2000/06/15 13:29:58  ungod
** #ifdef out of DirUsage for win32 until a viable alternative can be written
**
** Revision 1.33  2000/06/10 07:01:03  prez
** Fixed a bunch of little bugs ...
**
** Revision 1.32  2000/06/08 13:07:34  prez
** Added Secure Oper and flow control to DCC's.
** Also added DCC list and cancel ability
**
** Revision 1.31  2000/06/06 08:57:56  prez
** Finished off logging in backend processes except conver (which I will
** leave for now).  Also fixed some minor bugs along the way.
**
** Revision 1.30  2000/05/28 05:05:14  prez
** More makefile stuff ... Now we should work on all platforms.
** Added alot of checking for different .h files, functions, etc.
** So now all #define's are config.h based (also added a default
** windows config.h, which will need to be copied on these systems).
**
** Revision 1.29  2000/05/27 15:10:12  prez
** Misc changes, mainly re-did the makefile system, makes more sense.
** Also added a config.h file.
**
** Revision 1.28  2000/05/21 04:49:40  prez
** Removed all wxLog tags, now totally using our own logging.
**
** Revision 1.27  2000/05/20 03:28:11  prez
** Implemented transaction based tracing (now tracing wont dump its output
** until logical 'transactions' are done, which are ended by the thread
** being re-attached to another type, ending, or an explicit FLUSH() call).
**
** Revision 1.26  2000/05/20 01:17:07  ungod
** added UnDump static function and ReadLine function
**
** Revision 1.25  2000/05/19 10:48:14  prez
** Finalized the DCC Sending (now uses the Action map properly)
**
** Revision 1.24  2000/05/18 11:42:44  prez
** fixed the default assignment
**
** Revision 1.23  2000/05/18 11:41:46  prez
** Fixed minor front-end issues with the filesystem...
**
** Revision 1.22  2000/05/18 10:13:15  prez
** Finished off the mFile structure, and the DCC system, it all works.
**
** Revision 1.21  2000/05/17 14:08:11  prez
** More tweaking with DCC, and getting iostream mods working ...
**
** Revision 1.20  2000/05/17 12:39:55  prez
** Fixed DCC Sending and file lookups (bypassed the DccMap for now).
** Still to fix DCC Receiving.  Looks like a wxFile::Length() issue.
**
** Revision 1.19  2000/05/17 09:10:35  ungod
** changed most wxOutputStream to ofstream and wxInputStream
** to ifstream
**
** Revision 1.18  2000/05/17 07:47:58  prez
** Removed all save_databases calls from classes, and now using XML only.
** To be worked on: DCC Xfer pointer transferal and XML Loading
**
** Revision 1.17  2000/05/14 06:30:14  prez
** Trying to get XML loading working -- debug code (printf's) in code.
**
** Revision 1.16  2000/05/14 04:02:53  prez
** Finished off per-service XML stuff, and we should be ready to go.
**
** Revision 1.15  2000/05/03 14:12:22  prez
** Added 'public' filesystem, ie. the ability to add
** arbitary files for download via. servmsg (sops may
** upload/download, and set the committees who can
** grab the file).
**
** Revision 1.14  2000/04/30 03:48:29  prez
** Replaced all system calls with ACE_OS equivilants,
** also removed any dependancy on ACE from sxp (xml)
**
** Revision 1.13  2000/04/02 07:25:05  prez
** Fixed low watermarks with threads, it all works now!
**
** Revision 1.12  2000/03/30 11:24:53  prez
** Added threads to the filesys establishment.
**
** Revision 1.11  2000/03/28 16:20:58  prez
** LOTS of RET() fixes, they should now be safe and not do double
** calculations.  Also a few bug fixes from testing.
**
** Revision 1.10  2000/03/25 04:26:48  prez
** Added tracing into filesys for easier detection as to why it will not
** receive data.  Also put version number to a2 now, ready for release soon.
**
** Revision 1.9  2000/03/24 15:35:18  prez
** Fixed establishment of DCC transfers, and some other misc stuff
** (eg. small bug in trace, etc).  Still will not send or receive
** any data through DCC tho (will time out, but not receive data,
** error 14 - "Bad Access" -- to be investigated).
**
** Revision 1.8  2000/03/24 12:53:04  prez
** FileSystem Logging
**
** Revision 1.7  2000/03/23 10:22:25  prez
** Fully implemented the FileSys and DCC system, untested,
**
** Revision 1.6  2000/03/19 08:50:55  prez
** More Borlandization -- Added WHAT project, and fixed a bunch
** of minor warnings that appear in borland.
**
** Revision 1.5  2000/02/27 03:58:39  prez
** Fixed the WHAT program, also removed RegEx from Magick.
**
** Revision 1.4  2000/02/27 02:43:50  prez
** More FileSystem additions, plus created 'what' tool
**
** Revision 1.3  2000/02/23 21:01:25  prez
** Fixing CVS tags again ...
**
** Revision 1.2  2000/02/23 20:56:38  prez
** Fixed compilation of FileSystem (including ident tags)
**
** Revision 1.1  2000/02/23 14:29:05  prez
** Added beginnings of a File Map for stored files.
** Also updated Help files (finished nickserv).
**
**
** ========================================================== */

#include "magick.h"

queue<unsigned long> DccMap::active;
DccMap::xfers_t DccMap::xfers;
static DccXfer GLOB_DccXfer;

mFile::mFile(const mstring& name, FILE *in)
{
    FT("mFile::mFile", (name, "(FILE *) in"));
    MLOCK(("mFile", name));
    fd = in;
    if (fd != NULL)
	i_name = name;
}

mFile::mFile(const mstring& name, const mstring& mode)
{
    FT("mFile::mFile", (name, mode));
    
    MLOCK(("mFile", name));
    if ((fd = ACE_OS::fopen(name.c_str(), mode.c_str())) == NULL)
    {
	LOG((LM_ERROR, Parent->getLogMessage("SYS_ERRORS/COULDNOTOPEN"),
		name.c_str(), mode.c_str(), errno, strerror(errno)));
    }
    else
	i_name = name;
}

bool mFile::Open(const mstring& name, const mstring& mode)
{
    FT("mFile::Open", (name, mode));

    int opres;
    if (fd != NULL)
    {
	MLOCK(("mFile", i_name));
	opres = ACE_OS::fclose(fd);
	if (opres != 0 && errno)
	{
	    LOG((LM_ERROR, Parent->getLogMessage("SYS_ERRORS/FILEOPERROR"),
			"fclose", i_name.c_str(), errno, strerror(errno)));
	}
	fd = NULL;
    }
    i_name.erase();
    MLOCK(("mFile", name));
    if ((fd = ACE_OS::fopen(name.c_str(), mode.c_str())) == NULL)
    {
	LOG((LM_ERROR, Parent->getLogMessage("SYS_ERRORS/COULDNOTOPEN"),
		name.c_str(), mode.c_str(), errno, strerror(errno)));
    }
    else
	i_name = name;
    RET(fd != NULL);
}

void mFile::Close()
{
    NFT("mFile::Close");

    int opres;
    MLOCK(("mFile", i_name));
    if (IsOpened())
    {
	opres = ACE_OS::fflush(fd);
	if (opres != 0 && errno)
	{
	    LOG((LM_ERROR, Parent->getLogMessage("SYS_ERRORS/FILEOPERROR"),
			"fflush", i_name.c_str(), errno, strerror(errno)));
	}
	opres = ACE_OS::fclose(fd);
	if (opres != 0 && errno)
	{
	    LOG((LM_ERROR, Parent->getLogMessage("SYS_ERRORS/FILEOPERROR"),
			"fclose", i_name.c_str(), errno, strerror(errno)));
	}
	i_name.erase();
    }
    fd = NULL;
}

bool mFile::IsOpened() const
{
    NFT("mFile::IsOpened");
    MLOCK(("mFile", i_name));
    bool retval = (fd != NULL);
    RET(retval);
}

long mFile::Seek(const long offset, const int whence)
{
    FT("mFile::Seek", (offset, whence));
    MLOCK(("mFile", i_name));
    if (!IsOpened())
	RET(-1);
    long retpos = ACE_OS::fseek(fd, offset, whence);
    if (retpos < 0 && errno)
    {
	LOG((LM_ERROR, Parent->getLogMessage("SYS_ERRORS/FILEOPERROR"),
			"fseek", i_name.c_str(), errno, strerror(errno)));
    }
    RET(retpos);
}

size_t mFile::Write(const mstring& buf, const bool endline)
{
    FT("mFile::Write", (buf, endline));
    mstring data(buf);
    if (endline)
	data << "\n";
    long written = Write(data.c_str(), data.length());
    RET(written);
}

size_t mFile::Write(const void *buf, const size_t size)
{
    FT("mFile::Write", ("(const void *)", size));
    MLOCK(("mFile", i_name));
    if (!IsOpened())
	RET(0);
    long written = ACE_OS::fwrite(buf, 1, size, fd);
    if (ferror(fd) && errno)
    {
	LOG((LM_ERROR, Parent->getLogMessage("SYS_ERRORS/FILEOPERROR"),
			"fwrite", i_name.c_str(), errno, strerror(errno)));
    }
    RET(written);
}

size_t mFile::Read(void *buf, const size_t size)
{
    FT("mFile::Read", ("(const void *)", size));
    MLOCK(("mFile", i_name));
    if (!IsOpened())
	RET(0);
    long read = ACE_OS::fread(buf, 1, size, fd);
    if (ferror(fd) && errno)
    {
	LOG((LM_ERROR, Parent->getLogMessage("SYS_ERRORS/FILEOPERROR"),
			"fread", i_name.c_str(), errno, strerror(errno)));
    }
    RET(read);
}

mstring mFile::ReadLine()
{
    NFT("mFile::ReadLine");
    mstring Result;
    MLOCK(("mFile", i_name));
    if(!IsOpened())
        RET("");

    char buffer[1025];
    memset(buffer, 0, 1025);
#ifdef MAGICK_HAS_EXCEPTIONS
    try
    {
#endif
        ACE_OS::fgets(buffer,1024,fd);
	if (ferror(fd) && errno)
	{
	    LOG((LM_ERROR, Parent->getLogMessage("SYS_ERRORS/FILEOPERROR"),
			"fgets", i_name.c_str(), errno, strerror(errno)));
	    clearerr(fd);
	}
	else
	{
	    Result=buffer;
	    Result.Remove("\n");
	    Result.Remove("\r");
	}
#ifdef MAGICK_HAS_EXCEPTIONS
    }
    catch(...)
    {
        // this catches any exceptions so that we free up the buffer
        Result.erase();
    }
#endif

    RET(Result);
}

long mFile::Length() const
{
    NFT("mFile::Length");

    int opres;
    MLOCK(("mFile", i_name));
    if (!IsOpened())
	RET(-1);
    long retval = 0;
    struct stat st;
    opres = ACE_OS::stat(i_name, &st);
    if (opres < 0 && errno)
    {
	LOG((LM_ERROR, Parent->getLogMessage("SYS_ERRORS/FILEOPERROR"),
			"fstat", i_name.c_str(), errno, strerror(errno)));
    }
    else
    {
	retval = static_cast<long>(st.st_size);
    }
    RET(retval);
}

mDateTime mFile::LastMod() const
{
    NFT("mFile::LastMod");

    int opres;
    MLOCK(("mFile", i_name));
    if (!IsOpened())
	RET(0.0);
    mDateTime retval;
    struct stat st;
    opres = ACE_OS::stat(i_name, &st);
    if (opres < 0 && errno)
    {
	LOG((LM_ERROR, Parent->getLogMessage("SYS_ERRORS/FILEOPERROR"),
			"fstat", i_name.c_str(), errno, strerror(errno)));
    }
    else
    {
	retval = static_cast<time_t>(ACE_Time_Value(st.st_mtime).sec());
    }
    RET(retval);
}

bool mFile::Eof() const
{
    NFT("mFile::Eof");
    MLOCK(("mFile", i_name));
    if (!IsOpened())
	RET(true);
    bool retval = feof(fd);
    if (ferror(fd) && errno)
    {
	LOG((LM_ERROR, Parent->getLogMessage("SYS_ERRORS/FILEOPERROR"),
			"feof", i_name.c_str(), errno, strerror(errno)));
    }
    RET(retval);
}

void mFile::Attach(const mstring& name, FILE *in)
{
    FT("mFile::Attach", (name, "(FILE *) in"));
    MLOCK(("mFile", name));
    if (in == NULL)
	i_name.erase();
    else
	i_name = name;
    fd = in;
}

FILE *mFile::Detach()
{
    NFT("mFile::Detach");
    MLOCK(("mFile", i_name));
    FILE *rfd = fd;
    fd = NULL;
    NRET(FILE *, rfd);
}

void mFile::Flush()
{
    NFT("mFile::Flush");

    int opres;
    MLOCK(("mFile", i_name));
    if (IsOpened())
    {
	opres = ACE_OS::fflush(fd);
	if (opres < 0 && errno)
	{
	    LOG((LM_ERROR, Parent->getLogMessage("SYS_ERRORS/FILEOPERROR"),
			"fflush", i_name.c_str(), errno, strerror(errno)));
	}
    }
}

bool mFile::Exists(const mstring& name)
{
    FT("mFile::Exists", (name));
    MLOCK(("mFile", name));
    FILE *fd;
    if ((fd = ACE_OS::fopen(name.c_str(), "r")) == NULL)
	RET(false);
    ACE_OS::fclose(fd);
    RET(true);
}

bool mFile::Erase(const mstring& name)
{
    FT("mFile::Erase", (name));

    int opres;
    MLOCK(("mFile", name));
    if (!Exists(name))
	RET(false);
    opres = remove(name);
    if (opres < 0 && errno)
    {
	LOG((LM_ERROR, Parent->getLogMessage("SYS_ERRORS/FILEOPERROR"),
			"remove", name.c_str(), errno, strerror(errno)));
    }
    RET(true);
}

long mFile::Length(const mstring& name)
{
    FT("mFile::Length", (name));

    int opres;
    MLOCK(("mFile", name));
    if (!Exists(name))
	RET(0);
    long retval = 0;
    struct stat st;
    opres = ACE_OS::stat(name.c_str(), &st);
    if (opres < 0 && errno)
    {
	LOG((LM_ERROR, Parent->getLogMessage("SYS_ERRORS/FILEOPERROR"),
			"stat", name.c_str(), errno, strerror(errno)));
    }
    else
    {
	retval = static_cast<long>(st.st_size);
    }
    RET(retval);
}

mDateTime mFile::LastMod(const mstring& name)
{
    FT("mFile::LastMod", (name));

    int opres;
    MLOCK(("mFile", name));
    if (!Exists(name))
	RET(0.0);
    mDateTime retval;
    struct stat st;
    opres = ACE_OS::stat(name.c_str(), &st);
    if (opres < 0 && errno)
    {
	LOG((LM_ERROR, Parent->getLogMessage("SYS_ERRORS/FILEOPERROR"),
			"stat", name.c_str(), errno, strerror(errno)));
    }
    else
    {
	retval = static_cast<time_t>(ACE_Time_Value(st.st_mtime).sec());
    }
    RET(retval);
}

long mFile::Copy(const mstring& sin, const mstring& sout, const bool append)
{
    FT("mFile::Copy", (sin, sout, append));
    
    if (sin.empty() || !Exists(sin) || sout.empty())
	RET(0);
    MLOCK(("mFile", sin));
    MLOCK2(("mFile", sout));
    mFile in(sin.c_str());
    mFile out(sout.c_str(), append ? "a" : "w");
    if (!(in.IsOpened() && out.IsOpened()))
	RET(0);
    
    unsigned char c[1024];
    size_t read, total = 0;
    do {
        read = in.Read(c, 1024);
	total += out.Write(c, read);
    } while (read == 1024);
    in.Close();
    out.Close();
    RET(total);
}

// CANNOT trace this, it is used by TRACE code ...
long mFile::Dump(const vector<mstring>& sin, const mstring& sout,
	const bool append, const bool endline)
{
    FT("mFile::Dump", ("(vector<mstring>) sin", sout, append, endline));
    if (!sin.size() || sout.empty())
	RET(0);
    MLOCK(("mFile", sout));
    mFile out(sout.c_str(), append ? "a" : "w");
    if (!out.IsOpened())
	RET(0);

    size_t i, total = 0;
    for (i=0; i<sin.size(); i++)
    {
	if (endline)
	{
	    total += out.Write((sin[i]+"\n").c_str(), sin[i].length()+1);
	}
	else
	{
	    total += out.Write(sin[i].c_str(), sin[i].length());
	}
    }
    out.Close();
    RET(total);
}


// CANNOT trace this, it is used by TRACE code ...
long mFile::Dump(const list<mstring>& sin, const mstring& sout,
	const bool append, const bool endline)
{
    FT("mFile::Dump", ("(list<mstring>) sin", sout, append, endline));
    if (!sin.size() || sout.empty())
	RET(0);
    MLOCK(("mFile", sout));
    mFile out(sout.c_str(), append ? "a" : "w");
    if (!out.IsOpened())
	RET(0);
	
    size_t total = 0;
    list<mstring>::const_iterator iter;
    for (iter=sin.begin(); iter!=sin.end(); iter++)
    {
	if (endline)
	{
	    total += out.Write((*iter+"\n").c_str(), iter->length()+1);
	}
	else
	{
	    total += out.Write(iter->c_str(), iter->length());
	}
    }
    out.Close();
    RET(total);
}

vector<mstring> mFile::UnDump(const mstring &sin)
{
    FT("mFile::UnDump", (sin));
    vector<mstring> Result;

    if(sin.empty() || !Exists(sin))
        NRET(vector<mstring>,Result);
    MLOCK(("mFile", sin));
    mFile in(sin.c_str(), "r");
    if(!in.IsOpened())
        NRET(vector<mstring>,Result);

    mstring Line;
    Line=in.ReadLine();
    while(!in.Eof())
    {
        Result.push_back(Line);
        Line=in.ReadLine();
    }

    NRET(vector<mstring>,Result);
}

size_t mFile::DirUsage(const mstring& directory)
{
    FT("mFile::DirUsage", (directory));
    int opres;
    size_t retval = 0;
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    struct stat st;

#ifdef WIN32
    retval = -1;
    //todo: change over to findfirst/findnext (in io.h)
#else

    if (!directory.length())
	RET(0);

    if ((dir = ACE_OS::opendir(directory.c_str())) != NULL)
    {
	while ((entry = ACE_OS::readdir(dir)) != NULL)
	{
	    if (strlen(entry->d_name))
	    {
		opres = ACE_OS::stat((directory + DirSlash + entry->d_name).c_str(), &st);
		if (opres < 0 && errno)
		{
		    LOG((LM_ERROR, Parent->getLogMessage("SYS_ERRORS/FILEOPERROR"),
			"stat", (directory + DirSlash + entry->d_name).c_str(), errno, strerror(errno)));
		}
		else
		{
		    retval += st.st_size;
		}
	    }
	}
	ACE_OS::closedir(dir);
    }
    else
    {
	LOG((LM_ERROR, Parent->getLogMessage("SYS_ERRORS/DIROPERROR"),
		"opendir", directory.c_str(), errno, strerror(errno)));
    }
#endif
    RET(retval);
}


set<mstring> mFile::DirList(const mstring& directory, const mstring& filemask)
{
    FT("mFile::DirList", (directory, filemask));
    set<mstring> retval;
    DIR *dir = NULL;
    struct dirent *entry = NULL;

#ifdef WIN32
    //todo: change over to findfirst/findnext (in io.h)
#else

    if (!directory.length())
	NRET(set<mstring>, retval);

    if ((dir = ACE_OS::opendir(directory.c_str())) != NULL)
    {
	while ((entry = ACE_OS::readdir(dir)) != NULL)
	{
	    if (strlen(entry->d_name) &&
		mstring(entry->d_name).Matches(filemask))
	    {
		retval.insert(entry->d_name);
	    }
	}
	ACE_OS::closedir(dir);
    }
    else
    {
	LOG((LM_ERROR, Parent->getLogMessage("SYS_ERRORS/DIROPERROR"),
		"opendir", directory.c_str(), errno, strerror(errno)));
    }
#endif
    NRET(set<mstring>, retval);
}

unsigned long FileMap::FindAvail(const FileMap::FileType type)
{
    FT("FileMap::FindAvail", (static_cast<int>(type)));

    unsigned long filenum = 1;
    { RLOCK(("FileMap", static_cast<int>(type)));
    while (filenum > 0) // Guarentee 8 digits
    {
	if (i_FileMap[type].find(filenum) == i_FileMap[type].end())
	{
	    RET(filenum);
	}
	filenum++;
    }}

    LOG((LM_ERROR, Parent->getLogMessage("SYS_ERRORS/FILEMAPFULL"),
		static_cast<int>(type)));
    RET(0);
}


bool FileMap::Exists(const FileMap::FileType type, const unsigned long num)
{
    FT("FileMap::Exists", (static_cast<int>(type), num));

    mstring filename;
    filename.Format("%08x", num);

    if (type == MemoAttach)
	filename.prepend(Parent->files.MemoAttach() + DirSlash);
    else if (type == Picture)
	filename.prepend(Parent->files.Picture() + DirSlash);
    else if (type == Public)
    	filename.prepend(Parent->files.Public() + DirSlash);

    if (mFile::Exists(filename))
    {
	filemap_t::const_iterator fmi = i_FileMap.find(type);
	if (fmi != i_FileMap.end())
	{
	    RLOCK(("FileMap", static_cast<int>(type)));
	    if (fmi->second.find(num) != fmi->second.end())
	    {
		RET(true);
	    }
	}
	mFile::Erase(filename);
	LOG((LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/MISSING_FILE1"),
		static_cast<int>(type), num));
    }
    else
    {
	filemap_t::iterator fmi = i_FileMap.find(type);
	if (fmi != i_FileMap.end())
	{
	    WLOCK(("FileMap", static_cast<int>(type)));
	    if (fmi->second.find(num) != fmi->second.end())
	    {
	    	fmi->second.erase(num);
		LOG((LM_CRITICAL, Parent->getLogMessage("SYS_ERRORS/MISSING_FILE2"),
			static_cast<int>(type), num));
	    }
	}
    }
    RET(false);
}


mstring FileMap::GetName(const FileMap::FileType type, const unsigned long num)
{
    FT("FileMap::GetName", (static_cast<int>(type), num));

    if (Exists(type, num))
    {
	RLOCK(("FileMap", static_cast<int>(type), num));
	RET(i_FileMap[type][num].first);
    }
    RET("");
}


mstring FileMap::GetRealName(const FileMap::FileType type, const unsigned long num)
{
    FT("FileMap::GetRealName", (static_cast<int>(type), num));

    if (Exists(type, num))
    {
	mstring filename;
	filename.Format("%08x", num);

	if (type == MemoAttach)
	    filename.prepend(Parent->files.MemoAttach() + DirSlash);
	else if (type == Picture)
	    filename.prepend(Parent->files.Picture() + DirSlash);
	else if (type == Public)
    	    filename.prepend(Parent->files.Public() + DirSlash);
    	RET(filename);
    }
    RET("");
}


mstring FileMap::GetPriv(const FileMap::FileType type, const unsigned long num)
{
    FT("FileMap::GetPriv", (static_cast<int>(type), num));

    if (Exists(type, num))
    {
	RLOCK(("FileMap", static_cast<int>(type), num));
	RET(i_FileMap[type][num].second);
    }
    RET("");
}


bool FileMap::SetPriv(const FileMap::FileType type, const unsigned long num,
	const mstring& priv)
{
    FT("FileMap::SetPriv", (static_cast<int>(type), num, priv));

    if (Exists(type, num))
    {
	WLOCK(("FileMap", static_cast<int>(type), num));
	i_FileMap[type][num] = pair<mstring,mstring>(
	    			i_FileMap[type][num].first, priv);
	RET(true);
    }
    RET(false);
}


bool FileMap::Rename(const FileMap::FileType type, const unsigned long num,
	const mstring& newname)
{
    FT("FileMap::SetPriv", (static_cast<int>(type), num, newname));

    if (type != Picture && Exists(type, num))
    {
	WLOCK(("FileMap", static_cast<int>(type), num));
	i_FileMap[type][num] = pair<mstring,mstring>(newname,
	    			i_FileMap[type][num].second);
	RET(true);
    }
    RET(false);
}


size_t FileMap::GetSize(const FileMap::FileType type, const unsigned long num)
{
    FT("FileMap::GetSize", (static_cast<int>(type), num));
    mstring filename = GetRealName(type, num);
    if (!filename.empty())
    {
	CP(("Checking file size of %s", filename.c_str()));
	long retval = mFile::Length(filename);
	if (retval < 0)
	    retval = 0;
	RET(retval);
    }
    RET(0);
}


unsigned long FileMap::NewFile(const FileMap::FileType type, const mstring& filename,
	const mstring& priv)
{
    FT("FileMap::NewFile", (static_cast<int>(type), filename, priv));

    unsigned long filenum = 0;
    if (!GetNum(type, filename))
    {
	filenum = FindAvail(type);
	if (filenum)
	{
	    WLOCK(("FileMap", static_cast<int>(type), filenum));
	    i_FileMap[type][filenum] = pair<mstring,mstring>(filename, priv);
	}
    }

    RET(filenum);
}

void FileMap::EraseFile(const FileType type, const unsigned long num)
{
    FT("FileMap::EraseFile", (type, num));

    mstring filename = GetRealName(type, num);
    if (!filename.empty())
    {
	mFile::Erase(filename);
	WLOCK(("FileMap", static_cast<int>(type)));
	i_FileMap[type].erase(num);
    }
}

vector<unsigned long> FileMap::GetList(const FileMap::FileType type, const mstring& source)
{
    FT("FileMap::GetList", (static_cast<int>(type), source));
    vector<unsigned long> retval;
    map<unsigned long, pair<mstring, mstring> >::iterator iter;
    unsigned int i;

    if (i_FileMap.find(type) != i_FileMap.end())
    {
	RLOCK(("FileMap", static_cast<int>(type)));
    	for (iter = i_FileMap[type].begin(); iter != i_FileMap[type].end(); iter++)
	{
	    if (!Exists(type, iter->first))
	    {
		// This will take care of itself.
	    }
	    else
	    {
		RLOCK(("FileMap", static_cast<int>(type), iter->first));
		if (iter->second.second.empty())
		    retval.push_back(iter->first);
		else
		    for (i=1; i<=iter->second.second.WordCount(" "); i++)
		    {
			if (Parent->commserv.IsList(iter->second.second.ExtractWord(i, " ")) &&
			    Parent->commserv.GetList(iter->second.second.ExtractWord(i, " ").UpperCase()).IsOn(source))
			{
			    retval.push_back(iter->first);
			    break;
			}
		    }
	    }
	}
    }
    NRET(vector<unsigned long>, retval);
}


unsigned long FileMap::GetNum(const FileMap::FileType type, const mstring& name)
{
    FT("FileMap::GetNum", (static_cast<int>(type), name));
    map<unsigned long, pair<mstring, mstring> >::iterator iter;

    filemap_t::iterator fmi = i_FileMap.find(type);
    if (fmi != i_FileMap.end())
    {
	RLOCK(("FileMap", static_cast<int>(type)));
    	for (iter = fmi->second.begin(); iter != fmi->second.end(); iter++)
	{
	    RLOCK(("FileMap", static_cast<int>(type), iter->first));
	    if (iter->second.first == name)
	    {
	    	if (Exists(type, iter->first))
	    	{
	    	    RET(iter->first);
	    	}
	    }
	}
    }
    RET(0);
}

size_t FileMap::FileSysSize(const FileMap::FileType type) const
{
    FT("FileMap::FileSysSize", (static_cast<int>(type)));
    size_t retval = 0;

    if (type == MemoAttach)
	retval = mFile::DirUsage(Parent->files.MemoAttach());
    else if (type == Picture)
	retval = mFile::DirUsage(Parent->files.Picture());
    else if (type == Public)
	retval = mFile::DirUsage(Parent->files.Public());

    RET(retval);
}


SXP::Tag FileMap::tag_FileMap("FileMap");
SXP::Tag FileMap::tag_File("File");

void FileMap::BeginElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    FT("FileMap::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
}

void FileMap::EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    FT("FileMap::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    // load up simple elements here. (ie single pieces of data)

    if(pElement->IsA(tag_File))
    {
	mstring *tmp = new mstring;
	fm_array.push_back(tmp);
	pElement->Retrieve(*tmp);
    }
}

void FileMap::WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs)
{
    FT("FileMap::WriteElement", ("(SXP::IOutStream *) pOut", "(SXP::dict &) attribs"));
    // not sure if this is the right place to do this
    pOut->BeginObject(tag_FileMap, attribs);

    map<FileType, map<unsigned long, pair<mstring, mstring> > >::iterator i1;
    map<unsigned long, pair<mstring, mstring> >::iterator i2;
    mstring out;

    WLOCK(("FileMap"));
    for (i1=i_FileMap.begin(); i1!=i_FileMap.end(); i1++)
    {
	for(i2=i1->second.begin(); i2!=i1->second.end(); i2++)
	{
	    out.erase();
	    out << static_cast<int>(i1->first) << "\n" << i2->first <<
		"\n" << i2->second.first << "\n" << i2->second.second;
	    pOut->WriteElement(tag_File, out);
	}
    }

    pOut->EndObject(tag_FileMap);
}


void FileMap::PostLoad()
{
    NFT("FileMap::PostLoad");
    // Linkage, etc

    FileMap::FileType type;
    unsigned long number;
    mstring name;
    mstring priv;
    unsigned int i;
    for (i=0; i<fm_array.size(); i++)
    {
	if (fm_array[i] != NULL)
	{
	    if (fm_array[i]->WordCount("\n") >= 3)
	    {
		type = static_cast<FileMap::FileType>(atoi(fm_array[i]->ExtractWord(1, "\n").c_str()));
		number = atoul(fm_array[i]->ExtractWord(2, "\n").c_str());
		name = fm_array[i]->ExtractWord(3, "\n");
		if (fm_array[i]->WordCount("\n") > 3)
		    priv = fm_array[i]->ExtractWord(4, "\n");
		else
		    priv.erase();
		i_FileMap[type][number] = pair<mstring,mstring>(name,priv);
	    }
	    delete fm_array[i];
	}
    }
    fm_array.clear();
}

DccXfer::DccXfer(const unsigned long dccid, const mSocket& socket,
	const mstring& mynick, const mstring& source,
	const FileMap::FileType filetype, const unsigned long filenum)
	: i_Socket(socket), i_Source(source), i_Mynick(mynick),
	  i_Filename(Parent->filesys.GetName(filetype, filenum)),
	  i_Blocksize(Parent->files.Blocksize()), i_Type(Send), i_DccId(dccid)
{
    FT("DccXfer::DccXfer", (dccid, "(mSocket *) socket",
			mynick, source, static_cast<int>(filetype), filenum));

    // Setup Paramaters
    i_Transiant = NULL;
//    i_Blocksize = Parent->files.Blocksize();
    i_Tempfile.Format("%s%s%08x", Parent->files.TempDir().c_str(),
		 DirSlash.c_str(), i_DccId);
//    i_Filename = Parent->filesys.GetName(filetype, filenum);

    // Verify Paramaters
    if (!Parent->nickserv.IsLive(i_Source))
	return;
    if (i_Filename.empty())
    {
	send(mynick, source, Parent->getMessage(source, "DCC/NOFILE"),
					"SEND");
	return;
    }

    // Set 'Ready to Transfer'
    mstring tmp = Parent->filesys.GetRealName(filetype, filenum);
    if (tmp.empty())
    {
	Parent->filesys.EraseFile(filetype, filenum);
	send(mynick, source, Parent->getMessage(source, "DCC/NOFILE"),
					"SEND");
	return;
    }
    
    mFile::Copy(tmp, i_Tempfile);
    i_File.Open(i_Tempfile.c_str());
    i_Filesize = i_File.Length();
    if (i_Filesize <= 0)
    {
	i_File.Close();
	Parent->filesys.EraseFile(filetype, filenum);
	send(mynick, source, Parent->getMessage(source, "DCC/NOFILE"),
					"SEND");
	return;
    }

    // Initialize Transfer
    i_Transiant = new unsigned char[i_Blocksize + 1];
    memset(i_Transiant, 0, i_Blocksize + 1);
    i_Total = 0;
    i_XferTotal = 0;
    i_LastData = mDateTime::CurrentDateTime();
    DumpE();
    LOG((LM_DEBUG, Parent->getLogMessage("OTHER/DCC_INIT"),
		i_DccId, i_Source.c_str(), "SEND"));
    CP(("DCC %d initialized", i_DccId));
}


DccXfer::DccXfer(const unsigned long dccid, const mSocket& socket,
	const mstring& mynick, const mstring& source, const mstring& filename,
	const size_t filesize, const size_t blocksize)
	: i_Socket(socket), i_Source(source), i_Mynick(mynick),
	  i_Filename(filename), i_Blocksize(Parent->files.Blocksize()),
	  i_Type(Get), i_DccId(dccid)	  
{
    FT("DccXfer::DccXfer", (dccid, "(mSocket *) socket",
		mynick, source, filename, filesize, blocksize));

    // Setup Paramaters
    i_Transiant = NULL;
//    i_Blocksize = Parent->files.Blocksize();
    if (blocksize > 0)
	i_Blocksize = blocksize;
    i_Tempfile.Format("%s%s%08x", Parent->files.TempDir().c_str(),
		 DirSlash.c_str(), i_DccId);
//    i_Filename = filename;


    // Verify Paramaters
    if (!Parent->nickserv.IsLive(i_Source))
	return;

    if (Parent->nickserv.GetLive(i_Source).InFlight.File() &&
	!Parent->nickserv.GetLive(i_Source).InFlight.InProg())
    {
	Parent->nickserv.GetLive(i_Source).InFlight.SetInProg();
    }
    else
    {
	send(mynick, source, Parent->getMessage(source, "DCC/NOREQUEST"),
						"GET");
	return;
    }

    // Set 'Ready to Transfer'
    if (mFile::Exists(i_Tempfile))
	mFile::Erase(i_Tempfile);
    i_File.Open(i_Tempfile.c_str(), "w");
    i_Filesize = filesize;

    // Initialize Transfer
    i_Transiant = new unsigned char[i_Blocksize + 1];
    memset(i_Transiant, 0, i_Blocksize + 1);
    i_Total = 0;
    i_XferTotal = 0;
    i_LastData = mDateTime::CurrentDateTime();
    DumpE();
    LOG((LM_DEBUG, Parent->getLogMessage("OTHER/DCC_INIT"),
		i_DccId, i_Source.c_str(), "GET"));
    CP(("DCC %d initialized", i_DccId));
}

DccXfer::~DccXfer()
{
    NFT("DccXfer::~DccXfer");

    if (i_Transiant != NULL)
	delete [] i_Transiant;
    i_Transiant = NULL;

    if (i_File.IsOpened())
	i_File.Close();

    if ((i_Filesize > 0) ? i_Total == i_Filesize
			  : i_Total > 0)
    {
	LOG((LM_DEBUG, Parent->getLogMessage("OTHER/DCC_CLOSE"), i_DccId));
    }
    else
    {
	LOG((LM_DEBUG, Parent->getLogMessage("OTHER/DCC_CANCEL"), i_DccId));
    }

    if (!i_Socket.IsConnected())
    {
	return;
    }
    else
	i_Socket.close();

    CP(("DCC Xfer #%d Completed", i_DccId));
    // If we know the size, verify it, else we take
    // what we get!
    if (i_Type == Get &&
	Parent->nickserv.IsLiveAll(i_Source) &&
	Parent->nickserv.GetLive(i_Source).InFlight.File() &&
	Parent->nickserv.GetLive(i_Source).InFlight.InProg())
    {
	if ((i_Filesize > 0) ? i_Total == i_Filesize
			  : i_Total > 0)
	{
	    mstring tmp;
	    FileMap::FileType filetype = FileMap::Unknown;
	    if (Parent->nickserv.GetLive(i_Source).InFlight.Memo())
		filetype = FileMap::MemoAttach;
	    else if (Parent->nickserv.GetLive(i_Source).InFlight.Picture())
		filetype = FileMap::Picture;
	    else if (Parent->nickserv.GetLive(i_Source).InFlight.Public())
		filetype = FileMap::Public;
	    unsigned long filenum = Parent->filesys.NewFile(filetype, i_Filename);
	    if (filenum)
	    {
		if (filetype == FileMap::MemoAttach)
		    tmp.Format("%s%s%08x", Parent->files.MemoAttach().c_str(),
			DirSlash.c_str(), filenum);
		else if (filetype == FileMap::Picture)
		    tmp.Format("%s%s%08x", Parent->files.Picture().c_str(),
			DirSlash.c_str(), filenum);
		else if (filetype == FileMap::Public)
		    tmp.Format("%s%s%08x", Parent->files.Public().c_str(),
			DirSlash.c_str(), filenum);

		if (mFile::Exists(i_Tempfile))
		{
		    mFile::Copy(i_Tempfile, tmp);
		    Parent->nickserv.GetLive(i_Source).InFlight.File(filenum);
		    CP(("Added entry %d to FileMap", filenum));
		}
		else
		    Parent->nickserv.GetLive(i_Source).InFlight.File(0);
	    }
	    else
		Parent->nickserv.GetLive(i_Source).InFlight.File(0);
	}
	else
	    Parent->nickserv.GetLive(i_Source).InFlight.File(0);
    }

    if (mFile::Exists(i_Tempfile))
	mFile::Erase(i_Tempfile);
}

void DccXfer::operator=(const DccXfer &in)
{
    FT("DccXfer::operator=", ("(const DccXfer &) in"));

    // i_File=in.i_File;
    i_Source=in.i_Source;
    i_Mynick=in.i_Mynick;
    i_Tempfile=in.i_Tempfile;
    i_Filename=in.i_Filename;
    i_Blocksize=in.i_Blocksize;
    i_XferTotal=in.i_XferTotal;
    i_Total=in.i_Total;
    i_Filesize=in.i_Filesize;
    i_Type=in.i_Type;
    i_DccId=in.i_DccId;
    i_Socket = in.i_Socket;

    if (in.i_File.IsOpened())
    {
	in.i_File.Close();
	if (i_Type == Get)
	{
	    i_File.Open(i_Tempfile.c_str(), "a");
	}
	else if (i_Type == Send)
	{
	    i_File.Open(i_Tempfile.c_str());
	    if (i_Total + i_Blocksize > i_Filesize)
		i_File.Seek(0, SEEK_END);
	    else
		i_File.Seek(i_Total + i_Blocksize);
	}
    }
    i_Transiant = NULL;
    if (in.i_Transiant != NULL && in.i_XferTotal)
    {
	i_Transiant=new unsigned char[i_Blocksize + 1];
	memset(i_Transiant, 0, i_Blocksize + 1);
	memcpy(i_Transiant, in.i_Transiant, i_XferTotal);
    }
    i_LastData=in.i_LastData;
}

bool DccXfer::Ready() const
{
    NFT("DccXfer::Ready");
    RLOCK(("DccMap", "xfers", i_DccId, "i_File"));
    bool retval = i_File.IsOpened();
    RET(retval);
}

DccXfer::XF_Type DccXfer::Type() const
{
    NFT("DccXfer::Type");
    RLOCK(("DccMap", "xfers", i_DccId, "i_Type"));
    RET(i_Type);
}

mstring DccXfer::Mynick() const
{
    NFT("DccXfer::Mynick");
    RLOCK(("DccMap", "xfers", i_DccId, "i_Mynick"));
    RET(i_Mynick);
}

mstring DccXfer::Source() const
{
    NFT("DccXfer::Source");
    RLOCK(("DccMap", "xfers", i_DccId, "i_Source"));
    RET(i_Source);
}

mstring DccXfer::Filename() const
{
    NFT("DccXfer::Filename");
    RLOCK(("DccMap", "xfers", i_DccId, "i_Filename"));
    RET(i_Filename);
}

size_t DccXfer::Filesize() const
{
    NFT("DccXfer::Filesize");
    RLOCK(("DccMap", "xfers", i_DccId, "i_Filesize"));
    RET(i_Filesize);
}

size_t DccXfer::Total() const
{
    NFT("DccXfer::Total");
    RLOCK(("DccMap", "xfers", i_DccId, "i_Total"));
    RET(i_Total);
}

mDateTime DccXfer::LastData() const
{
    NFT("DccXfer::LastData");
    RLOCK(("DccMap", "xfers", i_DccId, "i_LastData"));
    RET(i_LastData);
}

void DccXfer::ChgNick(const mstring& in)
{
    FT("DccXfer::ChgNick", (in));
    WLOCK(("DccMap", "xfers", i_DccId, "i_Source"));
    MCB(i_Source);
    i_Source = in;
    MCE(i_Source);
}

void DccXfer::Cancel()
{
    NFT("DccXfer::Cancel");
    RLOCK(("DccMap", "xfers", i_DccId, "i_Source"));
    WLOCK(("DccMap", "xfers", i_DccId, "i_Total"));
    WLOCK2(("DccMap", "xfers", i_DccId, "i_File"));
    if (Parent->nickserv.IsLiveAll(i_Source))
	Parent->nickserv.GetLive(i_Source).InFlight.Cancel();
    MCB(i_Total);
    CB(1, i_File.Length());
    i_Total = 0;
    i_File.Close();
    CE(1, i_File.Length());
    MCE(i_Total);
}

void DccXfer::Action()
{
    NFT("DccXfer::Action");
    long XferAmt = 0, TranSz = 0;
    unsigned long verify;

    WLOCK(("DccMap", "xfers", i_DccId));
    DumpB();
    if (i_Type == Get)
    {
	COM(("Executing action for DCC %d GET", i_DccId));
	XferAmt = 0;
	if (i_Traffic.size() && (Parent->files.Max_Speed() == 0 ||
		Average() <= Parent->files.Max_Speed()))
	{
	    XferAmt = i_Socket.recv(reinterpret_cast<void *>(&i_Transiant[i_XferTotal]),
			i_Blocksize - i_XferTotal, 1);
	    COM(("%d: Bytes Transferred - %d, RECV Response %d (%s)",
		i_DccId, XferAmt, i_Socket.Last_Error(),
		i_Socket.Last_Error_String().c_str()));
	}
	// Traffic Accounting ...
	map<time_t, size_t>::iterator iter;
	time_t now = time(NULL);
	for (iter=i_Traffic.begin(); iter != i_Traffic.end() &&
		iter->first < now - static_cast<time_t>(Parent->files.Sampletime()+2);
		iter = i_Traffic.begin())
	    i_Traffic.erase(iter->first);
	if (i_Traffic.find(now) == i_Traffic.end())
	    i_Traffic[now] = 0;
	i_Traffic[now] += XferAmt;
	if (XferAmt > 0)
	{
	    i_XferTotal += XferAmt;
	    i_LastData = mDateTime::CurrentDateTime();
	    if ((i_Filesize > 0 &&
		i_Filesize == i_Total + i_XferTotal) ||
		i_XferTotal == i_Blocksize)
	    {
		i_Total += i_XferTotal;
		if (!i_File.IsOpened())
		    return;
		if (i_File.Write(i_Transiant, i_XferTotal) < 1)
		{
		    send(i_Mynick, i_Source, Parent->getMessage(i_Source, "DCC/FAILED"), "GET");
		    i_File.Close();
		}
		else
		{
		    i_XferTotal = 0;
		    memset(i_Transiant, 0, i_Blocksize);
		    verify = htonl(i_Total);
		    XferAmt = i_Socket.send(reinterpret_cast<void *>(&verify), 4, 1);
		    COM(("%d: Bytes Transferred - %d, SEND Response %d (%s)",
			i_DccId, XferAmt, i_Socket.Last_Error(),
			i_Socket.Last_Error_String().c_str()));
		    if (i_Filesize == i_Total)
		    {
			send(i_Mynick, i_Source, Parent->getMessage(i_Source, "DCC/COMPLETED"),
					"GET", i_Total);
			i_File.Close();
		    }
		}
	    }
	}
	else
	{
	    switch (i_Socket.Last_Error())
	    {
	    case 0:
	    case EINTR:		// Interrupted System Call
	    case EAGAIN:	// Temporarily Unavailable
	    case ENOMEM:	// Not Enough Memory
	    case ETIME:		// Request Timed Out
	    case EINPROGRESS:	// Operation In Progress
		break;
	    default:
		send(i_Mynick, i_Source, Parent->getMessage(i_Source, "DCC/SOCKERR"),
			"GET", i_Socket.Last_Error(), i_Socket.Last_Error_String().c_str());
		i_File.Close();
	    }
	}
	if (i_File.IsOpened() &&
		i_Traffic.size() > Parent->files.Sampletime() &&
		Average() < Parent->files.Min_Speed())
	{
	    send(i_Mynick, i_Source, Parent->getMessage(i_Source, "DCC/TOOSLOW"),
						"GET");
	    i_File.Close();
	}
    }
    else if (i_Type == Send)
    {
	COM(("Executing action for DCC %d SEND", i_DccId));
	XferAmt = 0;
	if (i_Traffic.size() && (Parent->files.Max_Speed() == 0 ||
		Average() <= Parent->files.Max_Speed()))
	{
	    if (i_XferTotal == i_Blocksize)
	    {
		XferAmt = i_Socket.recv(reinterpret_cast<void *>(&verify), 4, 1);
		COM(("%d: Bytes Transferred - %d, RECV Response %d (%s)",
			i_DccId, XferAmt, i_Socket.Last_Error(),
			i_Socket.Last_Error_String().c_str()));
		if (XferAmt <= 0 || ntohl(verify) != i_Total)
		{
		    switch (i_Socket.Last_Error())
		    {
		    case 0:
		    case EINTR:		// Interrupted System Call
		    case EAGAIN:		// Temporarily Unavailable
		    case ENOMEM:		// Not Enough Memory
		    case ETIME:		// Request Timed Out
		    case EINPROGRESS:	// Operation In Progress
			break;
		    default:
			send(i_Mynick, i_Source, Parent->getMessage(i_Source, "DCC/SOCKERR"),
				"SEND", i_Socket.Last_Error(), i_Socket.Last_Error_String().c_str());
			i_File.Close();
			return;
		    }
		}
		i_Total += i_XferTotal;
		i_XferTotal = 0;
	    }
	    if (!i_XferTotal && i_Total < i_Filesize)
	    {
		memset(i_Transiant, 0, i_Blocksize);
		if (!i_File.IsOpened())
		    return;
		if (i_Total + i_Blocksize > i_Filesize)
		    TranSz = i_File.Read(i_Transiant, i_Filesize - i_Total);
		else
		    TranSz = i_File.Read(i_Transiant, i_Blocksize);
		if (TranSz < 0)
		{
		    send(i_Mynick, i_Source, Parent->getMessage(i_Source, "DCC/FAILED"), "SEND");
		    i_File.Close();
		    return;
		}
		i_XferTotal = 0;
	    }
	    CP(("Going to send %d bytes ...", TranSz));
	    XferAmt = i_Socket.send(reinterpret_cast<void *>(&i_Transiant[i_XferTotal]),
			TranSz - i_XferTotal, 1);
	    COM(("%d: Bytes Transferred - %d, SEND Response %d (%s)",
		i_DccId, XferAmt, i_Socket.Last_Error(),
		i_Socket.Last_Error_String().c_str()));
	}
	// Traffic Accounting ...
	map<time_t, size_t>::iterator iter;
	time_t now = time(NULL);
	for (iter=i_Traffic.begin(); iter != i_Traffic.end() &&
		iter->first < now - static_cast<time_t>(Parent->files.Sampletime()+2);
		iter = i_Traffic.begin())
	    i_Traffic.erase(iter->first);
	if (i_Traffic.find(now) == i_Traffic.end())
	    i_Traffic[now] = 0;
	i_Traffic[now] += XferAmt;
	if (XferAmt > 0)
	{
	    i_XferTotal += XferAmt;
	    i_LastData = mDateTime::CurrentDateTime();
	    if (i_Filesize == i_Total + i_XferTotal)
	    {
		i_Total += i_XferTotal;
		send(i_Mynick, i_Source, Parent->getMessage(i_Source, "DCC/COMPLETED"),
					"SEND", i_Total);
		i_File.Close();
	    }
	}
	else
	{
	    switch (i_Socket.Last_Error())
	    {
	    case 0:
	    case EINTR:		// Interrupted System Call
	    case EAGAIN:	// Temporarily Unavailable
	    case ENOMEM:	// Not Enough Memory
	    case ETIME:		// Request Timed Out
	    case EINPROGRESS:	// Operation In Progress
		break;
	    default:
		send(i_Mynick, i_Source, Parent->getMessage(i_Source, "DCC/SOCKERR"),
			"SEND", i_Socket.Last_Error(), i_Socket.Last_Error_String().c_str());
		i_File.Close();
	    }
	}
	if (i_File.IsOpened() &&
		i_Traffic.size() > Parent->files.Sampletime() &&
		Average() < Parent->files.Min_Speed())
	{
	    send(i_Mynick, i_Source, Parent->getMessage(i_Source, "DCC/TOOSLOW"),
						"GET");
	    i_File.Close();
	}
    }
    DumpE();
}

size_t DccXfer::Average(time_t secs) const
{
    FT("DccXfer::Average", (secs));
    time_t now = time(NULL);
    size_t total = 0;
    int i = 0;
    map<time_t, size_t>::const_iterator iter;
    if (secs > static_cast<time_t>(Parent->files.Sampletime()))
	secs = 0;

    RLOCK(("DccMap", "xfers", i_DccId, "i_Traffic"));
    for (iter=i_Traffic.begin(); iter != i_Traffic.end() &&
			iter->first < now; iter++)
    {
	if (secs ? iter->first >= (now-1) - secs : 1)
	{
	    total += iter->second;
	    i++;
	}
    }
    RET(total / (i ? i : 1));
}

size_t DccXfer::Usage() const
{
    size_t retval = 0;

    WLOCK(("DccMap", "xfers", i_DccId));
    retval += i_Source.capacity();
    retval += i_Mynick.capacity();
    retval += i_Tempfile.capacity();
    retval += i_Filename.capacity();
    retval += sizeof(i_Blocksize);
    retval += sizeof(i_XferTotal);
    retval += sizeof(i_Total);
    retval += sizeof(i_Filesize);
    retval += sizeof(i_Type);
    retval += sizeof(i_DccId);
    retval += sizeof(i_Transiant);
    retval += sizeof(i_LastData.Internal());

    map<time_t, size_t>::const_iterator iter;
    for (iter=i_Traffic.begin(); iter!=i_Traffic.end(); iter++)
    {
	retval += sizeof(iter->first);
	retval += sizeof(iter->second);
    }

    return retval;
}

void DccXfer::DumpB() const
{
    MB(0, (i_Socket.Last_Error(), i_File.Length(), i_Source, i_Mynick,
	i_Tempfile, i_Filename, i_Blocksize, i_XferTotal, i_Total, i_Filesize,
	i_Type, i_DccId, i_Transiant, i_LastData, i_Traffic.size()));
}

void DccXfer::DumpE() const
{
    ME(0, (i_Socket.Last_Error(), i_File.Length(), i_Source, i_Mynick,
	i_Tempfile, i_Filename, i_Blocksize, i_XferTotal, i_Total, i_Filesize,
	i_Type, i_DccId, i_Transiant, i_LastData, i_Traffic.size()));
}

int DccMap::open(void *in)
{
    FT("DccMap::open", ("(void *) in"));
    int retval = activate();
    RET(retval);
}

int DccMap::close(const unsigned long in)
{
    FT("DccMap::close", (in));
    // dump all and close open file handles.
    DccMap::xfers_t::iterator iter;
    RLOCK(("DccMap", "xfers"));
    for (iter=XfersBegin(); iter != XfersEnd(); iter++)
    {
	RLOCK2(("DccMap", "xfers", iter->first));
	if (iter->second != NULL)
	{
	    send(iter->second->Mynick(), iter->second->Source(),
		Parent->getMessage(iter->second->Source(), "DCC/FAILED"),
		((iter->second->Type() == DccXfer::Get) ? "GET" : "SEND"));
	    iter->second->Cancel();
	}
    }
    WLOCK(("DccMap", "xfers"));
    for (iter=XfersBegin(); XfersSize(); iter=XfersBegin())
	RemXfers(iter->first);

    RET(0);
}

int DccMap::svc(void)
{
    mThread::Attach(tt_MAIN);
    NFT("DccMap::svc");

    unsigned long WorkId;
    FLUSH();
    while (!Parent->Shutdown())
    {
	/*COM(("Active Size is %d", active.size()));*/

	if (!active.size())
	{
	    ACE_OS::sleep(1);
	    continue;
	}

	{ WLOCK(("DccMap", "active"));
	WorkId = active.front();
	active.pop();
	}

#ifdef MAGICK_HAS_EXCEPTIONS
	try
	{
#endif
	    if (!IsXfers(WorkId))
		continue;
#ifdef MAGICK_HAS_EXCEPTIONS
	}
	catch(E_DccMap_Xfers &e)
	{
	    switch (e.type())
	    {
	    case E_DccMap_Xfers::T_Blank:
	    case E_DccMap_Xfers::T_Invalid:
		RemXfers(WorkId);
		break;
	    default:
		throw;
	    }
	    continue;
	}
#endif
	RLOCK(("DccMap", "xfers", WorkId));
	DccXfer &dcc = GetXfers(WorkId);
	if (!dcc.Ready())
	{
	    RemXfers(WorkId);
	    continue;
	}

	CP(("Executing ACTION for DCC #%d", WorkId));
	// Only do an action IF they have not exceeded the max speed
	dcc.Action();
	// Below LOW SPEED threshold OR
	// No data in X seconds...
	if (dcc.LastData().SecondsSince() > Parent->files.Timeout())
	{
	    send(dcc.Mynick(), dcc.Source(),
		Parent->getMessage(dcc.Source(), "DCC/TIMEOUT"),
		((dcc.Type() == DccXfer::Get) ? "GET" : "SEND"));
	    dcc.Cancel();
	    RemXfers(WorkId);
	    continue;
	}
	{ WLOCK(("DccMap", "active"));
	active.push(WorkId);
	}
	FLUSH(); // Force TRACE output dump
    }
    DRET(0);
}

#ifdef MAGICK_HAS_EXCEPTIONS
void DccMap::AddXfers(DccXfer *in) throw(E_DccMap_Xfers)
#else
void DccMap::AddXfers(DccXfer *in)
#endif
{
    FT("DccMap::AddXfers", ("(DccXfer *) in"));

    if (in == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_DccMap_Xfers(E_DccMap_Xfers::W_Add, E_DccMap_Xfers::T_Invalid));
#else
	LOG((LM_CRITICAL, "Exception - Nick:Xfers:Add:Invalid"));
	return;
#endif
    }
    if (!in->DccId())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_DccMap_Xfers(E_DccMap_Xfers::W_Add, E_DccMap_Xfers::T_Blank));
#else
	LOG((LM_CRITICAL, "Exception - Nick:Xfers:Add:Blank"));
	return;
#endif
    }

    WLOCK(("DccMap", "xfers"));
    xfers[in->DccId()] = in;
}


#ifdef MAGICK_HAS_EXCEPTIONS
DccXfer &DccMap::GetXfers(const unsigned long in) throw(E_DccMap_Xfers)
#else
DccXfer &DccMap::GetXfers(const unsigned long in)
#endif
{
    FT("DccMap::GetXfers", (in));

    RLOCK(("DccMap", "xfers", in));
    DccMap::xfers_t::iterator iter = xfers.find(in);
    if (iter == xfers.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_DccMap_Xfers(E_DccMap_Xfers::W_Get, E_DccMap_Xfers::T_NotFound));
#else
	LOG((LM_EMERGENCY, "Exception - DccMap:Xfers:Get:NotFound"));
	NRET(DccXfer &, GLOB_DccXfer);
#endif
    }
    if (iter->second == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_DccMap_Xfers(E_DccMap_Xfers::W_Get, E_DccMap_Xfers::T_Invalid));
#else
	LOG((LM_EMERGENCY, "Exception - DccMap:Xfers:Get:Invalid"));
	NRET(DccXfer &, GLOB_DccXfer);
#endif
    }
    if (!iter->second->DccId())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_DccMap_Xfers(E_DccMap_Xfers::W_Get, E_DccMap_Xfers::T_Blank));
#else
	LOG((LM_EMERGENCY, "Exception - DccMap:Xfers:Get:Blank"));
	NRET(DccXfer &, GLOB_DccXfer);
#endif
    }

    NRET(DccXfer &, const_cast<DccXfer &>(*iter->second));
}

#ifdef MAGICK_HAS_EXCEPTIONS
void DccMap::RemXfers(const unsigned long in) throw(E_DccMap_Xfers)
#else
void DccMap::RemXfers(const unsigned long in)
#endif
{
    FT("DccMap::RemXfers", (in));

    WLOCK(("DccMap", "xfers"));
    DccMap::xfers_t::iterator iter = xfers.find(in);
    if (iter == xfers.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw(E_DccMap_Xfers(E_DccMap_Xfers::W_Rem, E_DccMap_Xfers::T_NotFound));
#else
	LOG((LM_CRITICAL, "Exception - Nick:Xfers:Rem:NotFound"));
	return;
#endif
    }
    WLOCK2(("DccMap", "xfers", iter->first));
    if (iter->second != NULL)
    {
	delete iter->second;
    }
    xfers.erase(iter);
}

#ifdef MAGICK_HAS_EXCEPTIONS
bool DccMap::IsXfers(const unsigned long in) throw(E_DccMap_Xfers)
#else
bool DccMap::IsXfers(const unsigned long in)
#endif
{
    FT("DccMap::IsXfers", (in));

    RLOCK(("DccMap", "xfers", in));
    DccMap::xfers_t::const_iterator iter = xfers.find(in);
    if (iter != xfers.end())
    {
	if (iter->second == NULL)
	{
#ifdef MAGICK_HAS_EXCEPTIONS
	    throw(E_DccMap_Xfers(E_DccMap_Xfers::W_Get, E_DccMap_Xfers::T_Invalid));
#else
	    LOG((LM_CRITICAL, "Exception - DccMap:Xfers:Get:Invalid"));
	    RET(false);
#endif
	}
	if (!iter->second->DccId())
	{
#ifdef MAGICK_HAS_EXCEPTIONS
	    throw(E_DccMap_Xfers(E_DccMap_Xfers::W_Get, E_DccMap_Xfers::T_Blank));
#else
	    LOG((LM_CRITICAL, "Exception - DccMap:Xfers:Get:Blank"));
	    RET(false);
#endif
	}
	RET(true);
    }
    RET(false);
}

#ifdef MAGICK_HAS_EXCEPTIONS
vector<unsigned long> DccMap::GetList(const mstring& in) throw(E_DccMap_Xfers)
#else
vector<unsigned long> DccMap::GetList(const mstring& in)
#endif
{
    FT("DccMap::GetList", (in));
    vector<unsigned long> retval;

    DccMap::xfers_t::const_iterator iter;
    RLOCK(("DccMap", "xfers"));
    for (iter=xfers.begin(); iter!=xfers.end(); iter++)
    {
	RLOCK(("DccMap", "xfers", iter->first));
	if (iter->second == NULL)
	{
#ifdef MAGICK_HAS_EXCEPTIONS
	    throw(E_DccMap_Xfers(E_DccMap_Xfers::W_Get, E_DccMap_Xfers::T_Invalid));
#else
	    LOG((LM_CRITICAL, "Exception - DccMap:Xfers:Get:Invalid"));
	    continue;
#endif
	}
	if (!iter->second->DccId())
	{
#ifdef MAGICK_HAS_EXCEPTIONS
	    throw(E_DccMap_Xfers(E_DccMap_Xfers::W_Get, E_DccMap_Xfers::T_Blank));
#else
	    LOG((LM_CRITICAL, "Exception - DccMap:Xfers:Get:Blank"));
	    continue;
#endif
	}
	if (iter->second->Source().IsSameAs(in, true))
	    retval.push_back(iter->first);
    }
    NRET(vector<unsigned long>, retval);
}

void *DccMap::Connect2(void *in)
{
    mThread::Attach(tt_MAIN);
    FT("DccMap::Connect2", ("(void *) in"));

    NewSocket *val = reinterpret_cast<NewSocket *>(in);

    mSocket DCC_SOCK(val->address, Parent->files.Timeout());
    if (DCC_SOCK.IsConnected())
    {
	unsigned long WorkId;
	xfers_t::iterator iter;
	DCC_SOCK.Resolve(S_DCCFile, val->source);
	{ RLOCK(("DccMap", "xfers"));
	for (iter=XfersBegin(), WorkId=1; iter!=XfersEnd(); iter++, WorkId++)
	{
	    if (WorkId < iter->first)
		break;
	}}
	if (WorkId)
	{
	    AddXfers(new DccXfer(WorkId, DCC_SOCK, val->mynick, val->source,
		val->filename, val->filesize, val->blocksize));
	    { WLOCK(("DccMap", "active"));
	      active.push(WorkId);
	    }
	    CP(("Created DCC entry #%d", WorkId));
	}
	else
	    send(val->mynick, val->source, Parent->getMessage("DCC/FAILED"),
						"GET");
    }
    else
	send(val->mynick, val->source, Parent->getMessage("DCC/NOCONNECT"),
						"GET");

    if (val != NULL)
	delete val;

    DRET(0);
}

void *DccMap::Accept2(void *in)
{
    mThread::Attach(tt_MAIN);
    FT("DccMap::Accept2", ("(void *) in"));

    NewSocket *val = reinterpret_cast<NewSocket *>(in);

    mSocket DCC_SOCK(val->port, Parent->files.Timeout());
    if (DCC_SOCK.IsConnected())
    {
	unsigned long WorkId;
	xfers_t::iterator iter;
	DCC_SOCK.Resolve(S_DCCFile, val->source);
	{ RLOCK(("DccMap", "xfers"));
	for (iter=XfersBegin(), WorkId=1; iter!=XfersEnd(); iter++, WorkId++)
	{
	    if (WorkId < iter->first)
		break;
	}}
	if (WorkId)
	{
	    AddXfers(new DccXfer(WorkId, DCC_SOCK, val->mynick,
		val->source, val->filetype, val->filenum));
	    { WLOCK(("DccMap", "active"));
	      active.push(WorkId);
	    }
	    CP(("Created DCC entry #%d", WorkId));
	}
	else
	    send(val->mynick, val->source, Parent->getMessage("DCC/FAILED"),
						"SEND");
    }
    else
	send(val->mynick, val->source, Parent->getMessage("DCC/NOCONNECT"),
						"SEND");

    if (val != NULL)
	delete val;

    DRET(0);
}

void DccMap::Connect(const ACE_INET_Addr& address,
	const mstring& mynick, const mstring& source, const mstring& filename,
	const size_t filesize, const size_t blocksize)
{
    FT("DccMap::Connect", ("(ACE_INET_Addr) address", mynick,
			source, filename, filesize, blocksize));
    NewSocket *tmp = new NewSocket;
    tmp->address = address;
    tmp->source = source;
    tmp->mynick = mynick;
    tmp->filename = filename;
    tmp->filesize = filesize;
    tmp->blocksize = blocksize;

    tm.spawn(Connect2, reinterpret_cast<void *>(tmp));
}

void DccMap::Accept(const unsigned short port, const mstring& mynick,
	const mstring& source, const FileMap::FileType filetype,
	const unsigned long filenum)
{
    FT("DccMap::Accept", (port, mynick, source, static_cast<int>(filetype), filenum));
    NewSocket *tmp = new NewSocket;
    tmp->port = port;
    tmp->source = source;
    tmp->mynick = mynick;
    tmp->filetype = filetype;
    tmp->filenum = filenum;

    tm.spawn(Accept2, reinterpret_cast<void *>(tmp));
}

void DccMap::Cancel(const unsigned long DccId, const bool silent)
{
    FT("DccMap::Cancel", (DccId, silent));

#ifdef MAGICK_HAS_EXCEPTIONS
    try
    {
#endif
	if (IsXfers(DccId))
	{
	    RLOCK(("DccMap", "xfers", DccId));
	    DccXfer &dcc = GetXfers(DccId);
	    if (!silent)
		send(dcc.Mynick(), dcc.Source(),
		    Parent->getMessage(dcc.Source(), "DCC/FAILED"),
		    ((dcc.Type() == DccXfer::Get) ? "GET" : "SEND"));
	    dcc.Cancel();
	    RemXfers(DccId);
	}
#ifdef MAGICK_HAS_EXCEPTIONS
    }
    catch(E_DccMap_Xfers &e)
    {
	switch (e.type())
	{
	case E_DccMap_Xfers::T_Blank:
	case E_DccMap_Xfers::T_Invalid:
	    RemXfers(DccId);
	    break;
	default:
	    throw;
	}
    }
#endif
    tm.cancel_all();
}
