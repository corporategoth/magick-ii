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

unsigned short FindAvailPort();

class FileMap
{
public:
    enum FileType { MemoAttach, Picture };

    unsigned long FindAvail(FileType type);
    bool Exists(FileType type, unsigned long num);
    mstring GetName(FileType type, unsigned long num);
    bool GetFile(FileType type, unsigned long num, mstring recipiant);
    unsigned long NewFile(FileType type, mstring filename);
    void EraseFile(FileType type, unsigned long num);

    void load_database(wxInputStream& in);
    void save_database(wxOutputStream& in);
private:
    map<FileType, map<unsigned long, mstring> > i_FileMap;
};

class DccXfer
{
    enum XF_Type { Get, Send };

private:

    // We dont care HOW we got the socket, just
    // that we now have it.  This way we can just
    // concentrate on getting the job done.
    auto_ptr<ACE_SOCK_Stream> i_Socket;
    wxFile i_File;
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

public:
    DccXfer() { i_Transiant = NULL; }
    DccXfer(unsigned long dccid, ACE_SOCK_Stream *socket,
	mstring mynick, mstring source,
	FileMap::FileType filetype, unsigned long filenum);
    DccXfer(unsigned long dccid, ACE_SOCK_Stream *socket,
	mstring mynick, mstring source, mstring filename,
	size_t filesize, size_t blocksize);
    DccXfer(const DccXfer &in)
	{ *this = in; }
    void operator=(const DccXfer &in);

    ~DccXfer();

    bool Ready()		{ return i_File.IsOpened(); }
    unsigned long DccId()	{ return i_DccId; }
    XF_Type Type()		{ return i_Type; }
    mstring Source()		{ return i_Source; }
    mstring Filename()		{ return i_Filename; }
    size_t Filesize()		{ return i_Filesize; }
    size_t Total()		{ return i_Total; }
    mDateTime LastData()	{ return i_LastData; }

    void ChgNick(mstring in)	{ i_Source = in; }
    void Cancel()		{ i_Total = 0;
				  i_File.Close(); }
    void Action();	// Do what we want!

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

    static map<unsigned long, DccXfer> xfers;
    vector<unsigned long> GetList(mstring in);

    // These start in their own threads.
    void Connect(ACE_INET_Addr address,
	mstring mynick, mstring source, mstring filename,
	size_t filesize = 0, size_t blocksize = 0);
    void Accept(unsigned short port, mstring mynick,
	mstring source, FileMap::FileType filetype,
	unsigned long filenum);
    void Close(unsigned long DccId);
};

#endif
