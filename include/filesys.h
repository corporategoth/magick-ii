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

class FileMap
{
public:
    enum FileType { MemoAttach, Picture };

    unsigned long FindAvail(FileType type);
    bool Exists(FileType type, unsigned long num);
    mstring GetName(FileType type, unsigned long num);
    bool GetFile(FileType type, unsigned long num, mstring recipiant);
    unsigned long NewFile(FileType type, mstring filename);

    void load_database(wxInputStream& in);
    void save_database(wxOutputStream& in);
private:
    map<FileType, map<unsigned long, mstring> > i_FileMap;
};

class DccXfer : public ACE_Svc_Handler<ACE_SOCK_STREAM,ACE_MT_SYNCH>
{
    typedef ACE_Svc_Handler<ACE_SOCK_STREAM,ACE_MT_SYNCH> inherited;
public:
    enum XferType { Send, Get };
    DccXfer(XferType type, ACE_INET_Addr addr, mstring source,
	    mstring filename, FileMap::FileType filetype);
    ~DccXfer();

    virtual int close(unsigned long in);
    virtual int open(void *);
    virtual int handle_input(ACE_HANDLE handle);
    bool Established();

private:
    XferType i_type;
    ACE_INET_Addr i_addr;
    mstring i_filename;
    FileMap::FileType i_filetype;
    mstring tmpfile;
    mstring buffer;
    unsigned long blocksize;

    // Timers to wait for confirmation and/or data,
    // and the connection to be established.
    unsigned long T_Confirmation, T_Established;

    // Did we get correct checksum packet ...
    bool confirmation;
};

#endif
