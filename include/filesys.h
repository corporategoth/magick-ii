#ifndef WIN32
#pragma interface
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
#ifndef _FILESYS_H
#define _FILESYS_H
#include "pch.h"
RCSID(filesys_h, "@(#) $Id$");
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.43  2001/06/20 06:07:01  prez
** ome GCC 3.0 and solaris fixes
**
** Revision 1.42  2001/06/15 07:20:39  prez
** Fixed windows compiling -- now works with MS Visual Studio 6.0
**
** Revision 1.41  2001/06/02 16:27:04  prez
** Intergrated the staging system for dbase loading/saving.
**
** Revision 1.40  2001/05/17 19:18:53  prez
** Added ability to chose GETPASS or SETPASS.
**
** Revision 1.39  2001/04/05 05:59:50  prez
** Turned off -fno-default-inline, and split up server.cpp, it should
** compile again with no special options, and have default inlines :)
**
** Revision 1.38  2001/04/02 02:13:27  prez
** Added inlines, fixed more of the exception code.
**
** Revision 1.37  2001/03/20 14:22:14  prez
** Finished phase 1 of efficiancy updates, we now pass mstring/mDateTime's
** by reference all over the place.  Next step is to stop using operator=
** to initialise (ie. use mstring blah(mstring) not mstring blah = mstring).
**
** Revision 1.36  2001/02/11 07:41:27  prez
** Enhansed support for server numerics, specifically for Unreal.
**
** Revision 1.35  2001/02/03 03:20:33  prez
** Fixed up some differences in previous committed versions ...
**
** Revision 1.31  2000/12/23 22:22:23  prez
** 'constified' all classes (ie. made all functions that did not need to
** touch another non-const function const themselves, good for data integrity).
**
** Revision 1.30  2000/09/30 10:48:06  prez
** Some general code cleanups ... got rid of warnings, etc.
**
** Revision 1.29  2000/09/22 12:26:10  prez
** Fixed that pesky bug with chanserv not seeing modes *sigh*
**
** Revision 1.28  2000/09/02 07:20:44  prez
** Added the DumpB/DumpE functions to all major objects, and put in
** some example T_Modify/T_Changing code in NickServ (set email).
**
** Revision 1.27  2000/08/31 06:25:08  prez
** Added our own socket class (wrapper around ACE_SOCK_Stream,
** ACE_SOCK_Connector and ACE_SOCK_Acceptor, with tracing).
**
** Revision 1.26  2000/07/21 00:18:46  prez
** Fixed database loading, we can now load AND save databases...
**
** Almost ready to release now :)
**
** Revision 1.25  2000/07/11 13:22:18  prez
** Fixed loading/saving -- they now work with encryption and compression.
** Tested, it works too!  Now all we need to do is fix the loading, and
** we're set ... :))
**
** Revision 1.24  2000/06/25 07:58:48  prez
** Added Bahamut support, listing of languages, and fixed some minor bugs.
**
** Revision 1.23  2000/06/21 09:00:05  prez
** Fixed bug in mFile
**
** Revision 1.22  2000/06/18 12:49:26  prez
** Finished locking, need to do some cleanup, still some small parts
** of magick.cpp/h not locked properly, and need to ensure the case
** is the same every time something is locked/unlocked, but for the
** most part, locks are done, we lock pretty much everything :)
**
** Revision 1.21  2000/06/10 07:01:02  prez
** Fixed a bunch of little bugs ...
**
** Revision 1.20  2000/06/08 13:07:33  prez
** Added Secure Oper and flow control to DCC's.
** Also added DCC list and cancel ability
**
** Revision 1.19  2000/06/06 08:57:54  prez
** Finished off logging in backend processes except conver (which I will
** leave for now).  Also fixed some minor bugs along the way.
**
** Revision 1.18  2000/05/28 05:05:13  prez
** More makefile stuff ... Now we should work on all platforms.
** Added alot of checking for different .h files, functions, etc.
** So now all #define's are config.h based (also added a default
** windows config.h, which will need to be copied on these systems).
**
** Revision 1.17  2000/05/20 03:28:10  prez
** Implemented transaction based tracing (now tracing wont dump its output
** until logical 'transactions' are done, which are ended by the thread
** being re-attached to another type, ending, or an explicit FLUSH() call).
**
** Revision 1.16  2000/05/20 01:17:43  ungod
** added UnDump static function and ReadLine function
**
** Revision 1.15  2000/05/20 00:08:01  ungod
** getting ConfigEngine compiling and cleaning up SXP to stop circular includes of "datetime.h"
**
** Revision 1.14  2000/05/19 10:48:14  prez
** Finalized the DCC Sending (now uses the Action map properly)
**
** Revision 1.13  2000/05/18 10:13:15  prez
** Finished off the mFile structure, and the DCC system, it all works.
**
** Revision 1.12  2000/05/17 07:47:57  prez
** Removed all save_databases calls from classes, and now using XML only.
** To be worked on: DCC Xfer pointer transferal and XML Loading
**
** Revision 1.11  2000/05/14 04:02:52  prez
** Finished off per-service XML stuff, and we should be ready to go.
**
** Revision 1.10  2000/05/03 14:12:22  prez
** Added 'public' filesystem, ie. the ability to add
** arbitary files for download via. servmsg (sops may
** upload/download, and set the committees who can
** grab the file).
**
** Revision 1.9  2000/03/30 11:24:53  prez
** Added threads to the filesys establishment.
**
** Revision 1.8  2000/03/24 15:35:17  prez
** Fixed establishment of DCC transfers, and some other misc stuff
** (eg. small bug in trace, etc).  Still will not send or receive
** any data through DCC tho (will time out, but not receive data,
** error 14 - "Bad Access" -- to be investigated).
**
** Revision 1.7  2000/03/23 10:22:24  prez
** Fully implemented the FileSys and DCC system, untested,
**
** Revision 1.6  2000/03/19 08:50:52  prez
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

#include "variant.h"

class mFile
{
private:
    mutable FILE *fd;
    mstring i_name;
    mstring i_mode;

public:
    mFile() { fd = NULL; }
    mFile(const mFile &in) { *this = in; }
    mFile(const mstring& name, FILE *in, const mstring& mode = "r");
    mFile(const mstring& name, const mstring& mode = "r");
    ~mFile() { Close(); }
    void operator=(const mFile &in);
    mstring Name() const	{ return i_name; }
    mstring Mode() const	{ return i_mode; }
    bool Open(const mstring& name, const mstring& mode = "r");
    void Attach(const mstring& name, FILE *in, const mstring& mode = "r");
    FILE *Detach();
    void Close();
    bool IsOpened() const;
    bool IsReadable() const;
    bool IsWritable() const;
    bool IsBoth() const;
    long Seek(const long offset, const int whence = SEEK_SET);
    size_t Write(const mstring& buf, const bool endline = true);
    size_t Write(const void *buf, const size_t size);
    size_t Read(void *buf, const size_t size);
    mstring ReadLine();
    long Length() const;
    mDateTime LastMod() const;
    bool Eof() const;
    void Flush();
    
    static bool Exists(const mstring& name);
    static bool Erase(const mstring& name);
    static long Length(const mstring& name);
    static mDateTime LastMod(const mstring& name);
    static long Copy(const mstring& sin, const mstring& sout,
		const bool append = false);
    static long Dump(const vector<mstring>& sin, const mstring& sout,
		const bool append = false, const bool endline = true);
    static long Dump(const list<mstring>& sin, const mstring& sout,
		const bool append = false, const bool endline = true);
    static vector<mstring> UnDump(const mstring &sin);
    static size_t DirUsage(const mstring& directory);
    static set<mstring> DirList(const mstring& directory, const mstring& filemask);
};

#ifndef JUST_MFILE
#include "xml/sxp.h"
#include "lockable.h"

unsigned short FindAvailPort();

class FileMap : public SXP::IPersistObj
{
public:
    FileMap() {}
    ~FileMap() {}
    enum FileType { MemoAttach, Picture, Public, Unknown };
    typedef map<FileType, map<unsigned long, pair<mstring, mstring> > > filemap_t;

    unsigned long FindAvail(const FileType type);
    bool Exists(const FileType type, const unsigned long num);
    mstring GetName(const FileType type, const unsigned long num);
    mstring GetRealName(const FileType type, const unsigned long num);
    mstring GetPriv(const FileType type, const unsigned long num);
    bool SetPriv(const FileType type, const unsigned long num, const mstring& priv);
    bool Rename(const FileType type, const unsigned long num, const mstring& newname);
    size_t GetSize(const FileType type, const unsigned long num);
    unsigned long NewFile(const FileType type, const mstring& filename,
	const mstring& priv = "");
    void EraseFile(const FileType type, const unsigned long num);
    vector<unsigned long> GetList(const FileType type, const mstring& source);
    unsigned long GetNum(const FileType type, const mstring& name);
    size_t FileSysSize(const FileType type) const;

    SXP::Tag& GetClassTag() const { return tag_FileMap; }
    void BeginElement(const SXP::IParser * pIn, const SXP::IElement * pElement);
    void EndElement(const SXP::IParser * pIn, const SXP::IElement * pElement);
    void WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs = SXP::blank_dict);
    void PostLoad();
private:
    filemap_t i_FileMap;
    vector<mstring *> fm_array;
    static SXP::Tag tag_FileMap, tag_File;
};

class DccXfer
{
public:
    enum XF_Type { Get, Send };

private:

    // We dont care HOW we got the socket, just
    // that we now have it.  This way we can just
    // concentrate on getting the job done.
    mSocket i_Socket;
    mutable mFile i_File;
    mstring i_Source;
    mstring i_Mynick;
    mstring i_Tempfile;
    mstring i_Filename;
    size_t i_Blocksize;
    size_t i_XferTotal;
    size_t i_Total;
    size_t i_Filesize;
    XF_Type i_Type;
    unsigned long i_DccId;
    unsigned char *i_Transiant;
    mDateTime i_LastData;
    map<time_t, size_t> i_Traffic;

public:
    DccXfer() { i_Transiant = NULL; }
    DccXfer(const unsigned long dccid, const mSocket& socket,
	const mstring& mynick, const mstring& source,
	const FileMap::FileType filetype, const unsigned long filenum);
    DccXfer(const unsigned long dccid, const mSocket& socket,
	const mstring& mynick, const mstring& source, const mstring& filename,
	const size_t filesize, const size_t blocksize);
    DccXfer(const DccXfer &in)
	{ *this = in; }
    void operator=(const DccXfer &in);

    ~DccXfer();

    unsigned long DccId() const	{ return i_DccId; }
    bool Ready() const;
    XF_Type Type() const;
    mstring Mynick() const;
    mstring Source() const;
    mstring Filename() const;
    size_t Filesize() const;
    size_t Total() const;
    mDateTime LastData() const;

    void ChgNick(const mstring& in);
    void Cancel();
    void Action();	// Do what we want!
    size_t Average(time_t secs = 0) const;
    size_t Traffic() const	{ return i_Traffic.size(); }
    size_t Usage() const;
    void DumpB() const;
    void DumpE() const;
};

class DccMap : public ACE_Task<ACE_MT_SYNCH>
{
    // Damn solaris already HAS a 'queue'
    static std::queue<unsigned long> active;

    ACE_Thread_Manager tm;
    struct NewSocket
    {
	mstring mynick;
	mstring source;

	// Connector
	ACE_INET_Addr address;
	mstring filename;
	size_t filesize;
	size_t blocksize;

	// Acceptor
	unsigned short port;
	FileMap::FileType filetype;
	unsigned int filenum;
    };

    static void *Connect2(void *);
    static void *Accept2(void *);

public:
    typedef map<unsigned long, DccXfer * > xfers_t;

private:
    static xfers_t xfers;

public:
    int open(void *in=0);
    int close(const unsigned long in);
    int svc(void);

#ifdef MAGICK_HAS_EXCEPTIONS
    static void AddXfers(DccXfer *in) throw(E_DccMap_Xfers);
    static DccXfer &GetXfers(const unsigned long in) throw(E_DccMap_Xfers);
    static void RemXfers(const unsigned long in) throw(E_DccMap_Xfers);
    static bool IsXfers(const unsigned long in) throw(E_DccMap_Xfers);
    static vector<unsigned long> GetList(const mstring& in) throw(E_DccMap_Xfers);
#else
    static void AddXfers(DccXfer *in);
    static DccXfer &GetXfers(const unsigned long in);
    static void RemXfers(const unsigned long in);
    static bool IsXfers(const unsigned long in);
    static vector<unsigned long> GetList(const mstring& in);
#endif
    static xfers_t::iterator XfersBegin() { return xfers.begin(); }
    static xfers_t::iterator XfersEnd() { return xfers.begin(); }
    static size_t XfersSize() { return xfers.size(); }

    // These start in their own threads.
    void Connect(const ACE_INET_Addr& address, const mstring& mynick,
	const mstring& source, const mstring& filename,
	const size_t filesize = 0, const size_t blocksize = 0);
    void Accept(const unsigned short port, const mstring& mynick,
	const mstring& source, const FileMap::FileType filetype,
	const unsigned long filenum);
    void Cancel(const unsigned long DccId, const bool silent = false);
};

#endif /* JUST_MFILE */
#endif
