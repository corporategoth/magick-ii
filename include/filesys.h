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
#ifndef _FILESYS_H
#define _FILESYS_H
#include "pch.h"
static const char *ident_filesys_h = "@(#) $Id$";
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
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

#include "datetime.h"
#include "xml/sxp.h"

unsigned short FindAvailPort();

class mFile
{
    FILE *fd;
    mstring i_name;
public:
    mFile() { fd = NULL; }
    mFile(FILE *in);
    mFile(mstring name, mstring mode = "r");
    ~mFile() { Close(); }
    mstring Name()		{ return i_name; }
    bool Open(mstring name, mstring mode = "r");
    void Close();
    bool IsOpened();
    long Seek(long offset, int whence = SEEK_SET);
    size_t Write(mstring buf, bool endline = true);
    size_t Write(const void *buf, size_t size);
    size_t Read(void *buf, size_t size);
    mstring ReadLine();
    long Length();
    bool Eof();
    void Attach(mstring name, FILE *in);
    FILE *Detach();
    void Flush();
    
    static bool Exists(mstring name);
    static bool Erase(mstring name);
    static long Length(mstring name);
    static long Copy(mstring sin, mstring sout, bool append = false);
    static long Dump(vector<mstring> sin, mstring sout, bool append = false, bool endline = true);
    static long Dump(list<mstring> sin, mstring sout, bool append = false, bool endline = true);
    static vector<mstring> UnDump( const mstring &sin);
    static size_t DirUsage(mstring directory);
};

class FileMap : public SXP::IPersistObj
{
public:
    enum FileType { MemoAttach, Picture, Public };

    unsigned long FindAvail(FileType type);
    bool Exists(FileType type, unsigned long num);
    mstring GetName(FileType type, unsigned long num);
    mstring GetRealName(FileType type, unsigned long num);
    mstring GetPriv(FileType type, unsigned long num);
    bool SetPriv(FileType type, unsigned long num, mstring priv);
    bool Rename(FileType type, unsigned long num, mstring newname);
    bool GetFile(FileType type, unsigned long num, mstring recipiant);
    size_t GetSize(FileType type, unsigned long num);
    unsigned long NewFile(FileType type, mstring filename, mstring priv = "");
    void EraseFile(FileType type, unsigned long num);
    vector<unsigned long> GetList(FileType type, mstring source);
    unsigned long GetNum(FileType type, mstring name);
    size_t FileSysSize(FileType type);

    virtual SXP::Tag& GetClassTag() const { return tag_FileMap; }
    virtual void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement);
    virtual void EndElement(SXP::IParser * pIn, SXP::IElement * pElement);
    virtual void WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs);
    void PostLoad();
private:
    map<FileType, map<unsigned long, pair<mstring, mstring> > > i_FileMap;
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
    auto_ptr<ACE_SOCK_Stream> i_Socket;
    mFile i_File;
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
    DccXfer(unsigned long dccid, auto_ptr<ACE_SOCK_Stream> socket,
	mstring mynick, mstring source,
	FileMap::FileType filetype, unsigned long filenum);
    DccXfer(unsigned long dccid, auto_ptr<ACE_SOCK_Stream> socket,
	mstring mynick, mstring source, mstring filename,
	size_t filesize, size_t blocksize);
    DccXfer(const DccXfer &in)
	{ *this = in; }
    void operator=(const DccXfer &in);

    ~DccXfer();

    unsigned long DccId()	{ return i_DccId; }
    bool Ready();
    XF_Type Type();
    mstring Mynick();
    mstring Source();
    mstring Filename();
    size_t Filesize();
    size_t Total();
    mDateTime LastData();

    void ChgNick(mstring in);
    void Cancel();
    void Action();	// Do what we want!
    size_t Average(time_t secs = 0);
    size_t Traffic()		{ return i_Traffic.size(); }
    size_t Usage();
};

class DccMap : public ACE_Task<ACE_MT_SYNCH>
{
    static queue<unsigned long> active;

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
    virtual int open(void *in=0);
    virtual int close(unsigned long in);
    virtual int svc(void);

    static map<unsigned long, DccXfer *> xfers;
    vector<unsigned long> GetList(mstring in);

    // These start in their own threads.
    void Connect(ACE_INET_Addr address,
	mstring mynick, mstring source, mstring filename,
	size_t filesize = 0, size_t blocksize = 0);
    void Accept(unsigned short port, mstring mynick,
	mstring source, FileMap::FileType filetype,
	unsigned long filenum);
    void Cancel(unsigned long DccId, bool silent = false);
};

#endif
