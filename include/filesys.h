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

class DccFileXfer : public ACE_Task<ACE_MT_SYNCH>
{
public:
    enum XferType { Send, Get };

    virtual int close(unsigned long in);
    int send(const mstring& data);
    virtual int open(void *);
    virtual int handle_input(ACE_HANDLE handle);

private:
    XferType type;
    ACE_INET_Addr addr;
    mstring filename;
    FileMap::FileType filetype;
};

#endif
