#ifndef WIN32
#pragma interface
#endif

/* Magick IRC Services
**
** (c) 1997-2002 Preston Elder <prez@magick.tm>
** (c) 1998-2002 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any circumstances,
** however it may be added to if any modifications are made to this
** file.  All modified code must be clearly documented and labelled.
**
** This code is released under the GNU General Public License, which
** means (in short), it may be distributed freely, and may not be sold
** or used as part of any closed-source product.  Please check the
** COPYING file for full rights and restrictions of this software.
**
** ======================================================================= */
#ifndef _FILESYS_H
#define _FILESYS_H
#include "pch.h"
RCSID(filesys_h, "@(#) $Id$");

/* ======================================================================= **
**
** For official changes (by the Magick Development Team),please
** check the ChangeLog* files that come with this distribution.
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** ======================================================================= */

#include "variant.h"

class mFile
{
private:
    mutable FILE * fd;
    mstring i_name;
    mstring i_mode;

public:
      mFile()
    {
	fd = NULL;
    }
    mFile(const mFile & in)
    {
	*this = in;
    }
    mFile(const mstring & name, FILE * in, const mstring & mode = "r");
    mFile(const mstring & name, const mstring & mode = "r");

    ~mFile()
    {
	Close();
    }
    mFile &operator=(const mFile & in);
    mstring Name() const
    {
	return i_name;
    }
    mstring Mode() const
    {
	return i_mode;
    }
    bool Open(const mstring & name, const mstring & mode = "r");
    void Attach(const mstring & name, FILE * in, const mstring & mode = "r");
    FILE *Detach();
    void Close();
    bool IsOpened() const;
    bool IsReadable() const;
    bool IsWritable() const;
    bool IsBoth() const;
    long Seek(const long offset, const int whence = SEEK_SET);
    size_t Write(const mstring & buf, const bool endline = true);
    size_t Write(const void *buf, const size_t size);
    size_t Read(void *buf, const size_t size);
    mstring ReadLine();
    long Length() const;
    mDateTime LastMod() const;
    bool Eof() const;
    void Flush();

    static bool Exists(const mstring & name);
    static bool Erase(const mstring & name);
    static long Length(const mstring & name);
    static mDateTime LastMod(const mstring & name);
    static long Copy(const mstring & sin, const mstring & sout, const bool append = false);
    static long Dump(const vector < mstring > & sin, const mstring & sout, const bool append = false, const bool endline =
		     true);
    static long Dump(const list < mstring > & sin, const mstring & sout, const bool append = false, const bool endline = true);
    static vector < mstring > UnDump(const mstring & sin);
    static size_t DirUsage(const mstring & directory);
    static set < mstring > DirList(const mstring & directory, const mstring & filemask);
};

#ifndef JUST_MFILE
#include "xml/sxp.h"
#include "lockable.h"

unsigned short FindAvailPort();

class FileMap : public SXP::IPersistObj
{
public:
    FileMap()
    {
    }
    virtual ~FileMap()
    {
    }
    enum FileType
    { MemoAttach, Picture, Public, Unknown };
    typedef map < FileType, map < unsigned long, pair < mstring, mstring > > > filemap_t;

    unsigned long FindAvail(const FileType type);
    bool Exists(const FileType type, const unsigned long num);
    mstring GetName(const FileType type, const unsigned long num);
    mstring GetRealName(const FileType type, const unsigned long num);
    mstring GetPriv(const FileType type, const unsigned long num);
    bool SetPriv(const FileType type, const unsigned long num, const mstring & priv);
    bool Rename(const FileType type, const unsigned long num, const mstring & newname);
    size_t GetSize(const FileType type, const unsigned long num);
    unsigned long NewFile(const FileType type, const mstring & filename, const mstring & priv = "");
    void EraseFile(const FileType type, const unsigned long num);
    vector < unsigned long > GetList(const FileType type, const mstring & source);
    unsigned long GetNum(const FileType type, const mstring & name);
    size_t FileSysSize(const FileType type) const;

    SXP::Tag & GetClassTag() const
    {
	return tag_FileMap;
    }
    void BeginElement(SXP::IParser * pIn, SXP::IElement * pElement);
    void EndElement(SXP::IParser * pIn, SXP::IElement * pElement);
    void WriteElement(SXP::IOutStream * pOut, SXP::dict & attribs = SXP::blank_dict);
    void PostLoad();

  private:
    filemap_t i_FileMap;
    vector < mstring * > fm_array;
    static SXP::Tag tag_FileMap, tag_File;
};

class DccXfer
{
public:
    enum XF_Type
    { Get, Send };

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
    map < time_t, size_t > i_Traffic;

public:
    DccXfer()
    {
	i_Transiant = NULL;
    }
    DccXfer(const unsigned long dccid, const mSocket & socket, const mstring & mynick, const mstring & source,
	    const FileMap::FileType filetype, const unsigned long filenum);
    DccXfer(const unsigned long dccid, const mSocket & socket, const mstring & mynick, const mstring & source,
	    const mstring & filename, const size_t filesize, const size_t blocksize);
    DccXfer(const DccXfer & in)
    {
	*this = in;
    }
    DccXfer &operator=(const DccXfer & in);

    ~DccXfer();

    unsigned long DccId() const
    {
	return i_DccId;
    }
    bool Ready() const;
    XF_Type Type() const;
    mstring Mynick() const;
    mstring Source() const;
    mstring Filename() const;
    size_t Filesize() const;
    size_t Total() const;
    mDateTime LastData() const;

    void ChgNick(const mstring & in);
    void Cancel();
    void Action();		// Do what we want!
    size_t Average(time_t secs = 0) const;
    size_t Traffic() const
    {
	return i_Traffic.size();
    }
    size_t Usage() const;
    void DumpB() const;
    void DumpE() const;
};

class DccMap : public ACE_Task < ACE_MT_SYNCH >
{
    typedef ACE_Task < ACE_MT_SYNCH > internal;

    // Damn solaris already HAS a 'queue'
    static std::queue < unsigned long > active;

    Magick *magick_instance;
    ACE_Thread_Manager tm;
    struct NewSocket
    {
	mstring mynick;
	mstring source;

	// Connector
	Magick *magick_instance;
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
    typedef map < unsigned long, DccXfer * > xfers_t;

private:
    static xfers_t xfers;

public:
    DccMap(ACE_Thread_Manager * tm = 0) : internal(tm)
    {
    }

    int open(void *in = 0);
    int close(u_long flags = 0);
    int svc(void);
    int fini()
    {
	return 0;
    }

#ifdef MAGICK_HAS_EXCEPTIONS
    static void AddXfers(DccXfer * in) throw (E_DccMap_Xfers);
    static DccXfer &GetXfers(const unsigned long in) throw (E_DccMap_Xfers);
    static void RemXfers(const unsigned long in) throw (E_DccMap_Xfers);
    static bool IsXfers(const unsigned long in) throw (E_DccMap_Xfers);
    static vector < unsigned long > GetList(const mstring & in) throw (E_DccMap_Xfers);
#else
    static void AddXfers(DccXfer * in);
    static DccXfer &GetXfers(const unsigned long in);
    static void RemXfers(const unsigned long in);
    static bool IsXfers(const unsigned long in);
    static vector < unsigned long > GetList(const mstring & in);
#endif
    static xfers_t::iterator XfersBegin()
    {
	return xfers.begin();
    }
    static xfers_t::iterator XfersEnd()
    {
	return xfers.begin();
    }
    static size_t XfersSize()
    {
	return xfers.size();
    }

    // These start in their own threads.
    void Connect(const ACE_INET_Addr & address, const mstring & mynick, const mstring & source, const mstring & filename,
		 const size_t filesize = 0, const size_t blocksize = 0);
    void Accept(const unsigned short port, const mstring & mynick, const mstring & source, const FileMap::FileType filetype,
		const unsigned long filenum);
    void Cancel(const unsigned long DccId, const bool silent = false);
};

#endif /* JUST_MFILE */
#endif
