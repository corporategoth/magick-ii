#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
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
static const char *ident = "@(#)$Id$";
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
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

#include "lockable.h"
#include "magick.h"

unsigned long FileMap::FindAvail(FileMap::FileType type)
{
    FT("FileMap::FindAvail", ((int) type));

    unsigned long filenum = 1;
    if (i_FileMap.find(type) != i_FileMap.end())
    {
	while (filenum < 100000000) // Guarentee 8 digits
	{
	    if (i_FileMap[type].find(filenum) == i_FileMap[type].end())
	    {
		RET(filenum);
	    }
	    filenum++;
	}
    }

    RET(0);
}


bool FileMap::Exists(FileMap::FileType type, unsigned long num)
{
    FT("FileMap::Exists", ((int) type, num));

    mstring filename;
    filename.Format("%08d", num);

    if (type == MemoAttach)
	filename.Prepend(Parent->files.MemoAttach() + DirSlash);
    else if (type == Picture)
	filename.Prepend(Parent->files.Picture() + DirSlash);

    if (wxFile::Exists(filename.c_str()))
    {
	RET(true);
    }

    RET(false);
}


mstring FileMap::GetName(FileMap::FileType type, unsigned long num)
{
    FT("FileMap::GetName", ((int) type, num));

    if (i_FileMap.find(type) != i_FileMap.end())
    {
	if (i_FileMap[type].find(num) != i_FileMap[type].end())
	{
	    RET(i_FileMap[type][num]);
	}
    }
    RET("");
}


bool FileMap::GetFile(FileMap::FileType type, unsigned long num, mstring recipiant)
{
    FT("FileMap::GetFile", ((int) type, num, recipiant));

    if (!Exists(type, num))
    {
	RET(false);
    }
    mstring filename = GetName(type, num);

    RET(true);
}


unsigned long FileMap::NewFile(FileMap::FileType type, mstring sender)
{
    FT("FileMap::NewFile", ((int) type, sender));

    unsigned long filenum = FindAvail(type);

    RET(filenum);
}

void FileMap::save_database(wxOutputStream& out)
{
    FT("FileMap::save_database", ("(wxOutputStream &) out"));
    map<FileType, map <unsigned long, mstring> >::iterator i;
    map <unsigned long, mstring>::iterator j;

    CP(("Saving FILE TYPE entries (%d) ...", i_FileMap.size()));
    out<<i_FileMap.size();
    for (i=i_FileMap.begin(); i!=i_FileMap.end(); i++)
    {
	CP(("Entry FILE TYPE (%d) saved ...", (int) i->first));
	out<<(int) i->first;

	CP(("Saving FILE entries (%d) ...", i->second.size()));
	out<<i->second.size();
	for(j=i->second.begin();j!=i->second.end();j++)
	{
	    out<<j->first<<j->second;
	    COM(("Entry FILE %l (%s) saved ...",
				j->first, j->second.c_str()));
	}
    }
}


void FileMap::load_database(wxInputStream& in)
{
    FT("FileMap::load_database", ("(wxInputStream &) in"));

    map<FileType, map <unsigned long, mstring> >::size_type cnt1, i;
    map <unsigned long, mstring>::size_type cnt2, j;

    int val1;
    unsigned long val2;
    mstring val3;

    in>>cnt1;
    CP(("Loading FILE TYPE entries (%d) ...", cnt1));
    i_FileMap.clear();
    for(i=0; i<cnt1; i++)
    {
	COM(("Loading FILE TYPE entry %d ...", i));
	in>>val1;

	in>>cnt2;
	CP(("Loading FILE entries (%d) ...", cnt2));
	for (j=0; j<cnt2; j++)
	{
	    COM(("Loading FILE entry %d ...", j));
	    in>>val2>>val3;
	    i_FileMap[(FileType) val1][val2] = val3;
	    COM(("Entry FILE %d (%s) loaded ...", val2, val3.c_str()));
	}
	COM(("Entry FILE MAP %d loaded ...", (int) val1));
    }
}

DccXfer::DccXfer(DccXfer::XferType type, ACE_INET_Addr addr, mstring source,
		 mstring filename, FileMap::FileType filetype)
{
    FT("DccXfer::DccXfer", ((int) type, "(ACE_INET_Addr) addr", source,
					filename, (int) filetype));
    i_type = type;
    i_addr = addr;
    i_filename = filename;
    i_filetype = filetype;
    tmpfile = Parent->files.TempDir() + DirSlash + source;
    blocksize = Parent->files.Blocksize();
    total = 0;
    fout = NULL;
    fin = NULL;
    open();
}

DccXfer::~DccXfer()
{
    NFT("DccXfer::~DccXfer");
    if (fout != NULL)
	delete fout;
    if (fin != NULL)
	delete fin;
    remove(tmpfile.c_str());
    close(0);
}

int DccXfer::close(unsigned long in)
{
    FT("DccXfer::close", (in));
    RET(0);
}

int DccXfer::open(void *in)
{
    FT("DccXfer::open", (in));
    if (i_type == Get)
    {
	// We let handle_input handle most of this...
	fout = new wxFileOutputStream(tmpfile, true);
    }
    else if (i_type == Send)
    {
    }
    else
    {
	wxLogWarning("Unknown DCC Transfer Type!  Aborting DCC");
	close(0);
    }

    RET(0);
}

int DccXfer::handle_input(ACE_HANDLE handle)
{
    FT("DccXfer::handle_input", ("ACE_HANDLE handle"));

    if (i_type == Send)
    {
	int recvResult;
	char data[5];
	recvResult=peer().recv(data,4);
	if(recvResult<=0 || Parent->Shutdown())
	{
	    // Check if we got nuff yet?
	    close(0);
	    return -1;
	}

	// Kill timer to wait for more data or confirmation
	confirmation = true;
    }
    else if (i_type == Get)
    {
	int recvResult, remaining = blocksize;
	char data[1025];

	while (remaining > 0)
	{
	    memset(data,0,1025);
	    if (remaining > 1024)
	    {
		recvResult=peer().recv(data,1024);
		remaining -= 1024;
	    }
	    else
	    {
		recvResult=peer().recv(data,remaining);
		remaining = 0;
	    }
	    if(recvResult<=0 || Parent->Shutdown())
	    {
		// Check if we got nuff yet?
		close(0);
		return -1;
	    }
	    *fout << data;
	}
	// Kill timer to wait for more data or confirmation
	confirmation = true;

	total += blocksize;
	recvResult = peer().send((char *) htonl(total), 4);
	if(recvResult<=0 || Parent->Shutdown())
	{
	    // Check if we got nuff yet?
	    close(0);
	    return -1;
	}

	// Add timer to wait for more data or close
	confirmation = false;
    }

    RET(0);
}

