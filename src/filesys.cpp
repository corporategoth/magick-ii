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

#include "lockable.h"
#include "magick.h"

unsigned short FindAvailPort()
{
    NFT("FindAvailPort");

    ACE_INET_Addr local;
    ACE_SOCK_Acceptor accept(ACE_Addr::sap_any);
    accept.get_local_addr(local);
    RET(local.get_port_number());
}

unsigned long FileMap::FindAvail(FileMap::FileType type)
{
    FT("FileMap::FindAvail", ((int) type));

    unsigned long filenum = 1;
    if (i_FileMap.find(type) != i_FileMap.end())
    {
	while (filenum <= 0xffffffff) // Guarentee 8 digits
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
    filename.Format("%08x", num);

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


unsigned long FileMap::NewFile(FileMap::FileType type, mstring filename)
{
    FT("FileMap::NewFile", ((int) type, filename));

    unsigned long filenum = 0;
    if (i_FileMap.find(type) != i_FileMap.end())
    {
	filenum = FindAvail(type);
	i_FileMap[type][filenum] = filename;
    }

    RET(filenum);
}

void FileMap::EraseFile(FileType type, unsigned long num)
{
    FT("FileMap::EraseFile", (type, num));

    if (i_FileMap.find(type) != i_FileMap.end())
	if (i_FileMap[type].find(num) != i_FileMap[type].end())
	{
	    mstring sourcefile;
	    if (type == MemoAttach)
		sourcefile.Format("%s%s%08x",
			Parent->files.MemoAttach().c_str(),
			DirSlash.c_str(), num);
	    else if (type == Picture)
		sourcefile.Format("%s%s%08x",
			Parent->files.Picture().c_str(),
			DirSlash.c_str(), num);
	    if (sourcefile != "" && wxFile::Exists(sourcefile.c_str()))
		remove(sourcefile.c_str());
	    i_FileMap[type].erase(num);
	}
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
	    COM(("Entry FILE %08x (%s) saved ...",
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
	    COM(("Entry FILE %08x (%s) loaded ...", val2, val3.c_str()));
	}
	COM(("Entry FILE MAP %d loaded ...", (int) val1));
    }
}

DccXfer::DccXfer(unsigned long dccid, ACE_SOCK_Stream *socket,
	mstring mynick, mstring source,
	FileMap::FileType filetype, unsigned long filenum)
	: i_Socket(socket), i_File(new wxFile)
{
    FT("DccXfer::DccXfer", (dccid, "(ACE_SOCK_Stream *) socket",
			mynick, source, (int) filetype, filenum));

    // Setup Paramaters
    i_DccId = dccid;
    i_Transiant = NULL;
    i_Source = source;
    i_Type = Send;
    i_Blocksize = Parent->files.Blocksize();
    i_Tempfile.Format("%s%s%08x", Parent->files.TempDir().c_str(),
		 DirSlash.c_str(), i_DccId);
    i_Filename = Parent->filesys.GetName(filetype, filenum);

    // Verify Paramaters
    if (!Parent->nickserv.IsLive(i_Source))
	return;
    if (i_Filename == "")
	return;

    // Set 'Ready to Transfer'
    mstring tmp;
    if (filetype == FileMap::MemoAttach)
	tmp.Format("%s%s%08x", Parent->files.MemoAttach().c_str(),
			DirSlash.c_str(), filenum);
    else if (filetype == FileMap::Picture)
	tmp.Format("%s%s%08x", Parent->files.Picture().c_str(),
			DirSlash.c_str(), filenum);
    if (!wxFile::Exists(tmp.c_str()))
	return;
    if (wxFile::Exists(i_Tempfile.c_str()))
	remove(i_Tempfile.c_str());
    wxFileInputStream fin(tmp.c_str());
    wxFileOutputStream fout(i_Tempfile.c_str());
    fout << fin;
    i_File->Open(i_Tempfile.c_str(), wxFile::read);
    i_Filesize = i_File->Length();

    // Initialize Transfer
    i_Transiant = (unsigned char *) malloc (sizeof(unsigned char) * (i_Blocksize + 1));
    memset(i_Transiant, 0, i_Blocksize + 1);
    i_Total = 0;
    i_XferTotal = 0;
    i_LastData = Now();
}


DccXfer::DccXfer(unsigned long dccid, ACE_SOCK_Stream *socket,
	mstring mynick, mstring source, mstring filename,
	size_t filesize, size_t blocksize)
	: i_Socket(socket), i_File(new wxFile)
{
    FT("DccXfer::DccXfer", (dccid, "(ACE_SOCK_Stream *) socket",
		mynick, source, filename, filesize, blocksize));

    // Setup Paramaters
    i_DccId = dccid;
    i_Transiant = NULL;
    i_Source = source;
    i_Type = Get;
    i_Blocksize = Parent->files.Blocksize();
    if (blocksize > 0)
	i_Blocksize = blocksize;
    i_Tempfile.Format("%s%s%08x", Parent->files.TempDir().c_str(),
		 DirSlash.c_str(), i_DccId);
    i_Filename = filename;

    // Verify Paramaters
    if (Parent->nickserv.IsLive(i_Source) &&
	Parent->nickserv.live[i_Source.LowerCase()].InFlight.File() &&
	!Parent->nickserv.live[i_Source.LowerCase()].InFlight.InProg())
	Parent->nickserv.live[i_Source.LowerCase()].InFlight.SetInProg();
    else
	return;

    // Set 'Ready to Transfer'
    if (wxFile::Exists(i_Tempfile.c_str()))
	remove(i_Tempfile.c_str());
    i_File->Open(i_Tempfile.c_str(), wxFile::write);
    i_Filesize = filesize;

    // Initialize Transfer
    i_Transiant = (unsigned char *) malloc (sizeof(unsigned char) * (i_Blocksize + 1));
    memset(i_Transiant, 0, i_Blocksize + 1);
    i_Total = 0;
    i_XferTotal = 0;
    i_LastData = Now();
}

DccXfer::~DccXfer()
{
    NFT("DccXfer::~DccXfer");

    // If we know the size, verify it, else we take
    // what we get!
    if (i_Type == Get &&
	Parent->nickserv.IsLive(i_Source) &&
	Parent->nickserv.live[i_Source.LowerCase()].InFlight.File() &&
	Parent->nickserv.live[i_Source.LowerCase()].InFlight.InProg())
    {
	if ((i_Filesize > 0) ? i_Total == i_Filesize
			  : i_Total > 0)
	{
	    mstring tmp;
	    FileMap::FileType filetype;
	    if (Parent->nickserv.live[i_Source.LowerCase()].InFlight.IsMemo())
		filetype = FileMap::MemoAttach;
	    else
		filetype = FileMap::Picture;
	    unsigned long filenum = Parent->filesys.NewFile(filetype, i_Filename);
	    if (filetype == FileMap::MemoAttach)
		tmp.Format("%s%s%08x", Parent->files.MemoAttach().c_str(),
			DirSlash.c_str(), filenum);
	    else if (filetype == FileMap::Picture)
		tmp.Format("%s%s%08x", Parent->files.Picture().c_str(),
			DirSlash.c_str(), filenum);
	    if (!wxFile::Exists(i_Tempfile.c_str()))
		return;
	    if (wxFile::Exists(tmp.c_str()))
		remove(tmp.c_str());
	    wxFileInputStream fin(i_Tempfile.c_str());
	    wxFileOutputStream fout(tmp.c_str());
	    fout << fin;
	    Parent->nickserv.live[i_Source.LowerCase()].InFlight.File(filenum);
	}
	else
	    Parent->nickserv.live[i_Source.LowerCase()].InFlight.File(0);
    }
    if (wxFile::Exists(i_Tempfile.c_str()))
	remove(i_Tempfile.c_str());
    if (i_Transiant != NULL)
	free(i_Transiant);
}

void DccXfer::operator=(const DccXfer &in)
{
    FT("DccXfer::operator=", ("(const DccXfer &) in"));
    i_Socket=(auto_ptr<ACE_SOCK_Stream> &) in.i_Socket;
    i_File=(auto_ptr<wxFile> &) in.i_File;
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
    i_Transiant=(unsigned char *) malloc(i_Blocksize);
    memset(i_Transiant, 0, i_Blocksize);
    strcpy(i_Transiant, in.i_Transiant);
    i_LastData=in.i_LastData;
}


void DccXfer::Action()
{
    NFT("DccXfer::Action");
    size_t XferAmt = 0;
    int merrno;

    if (i_Type == Get)
    {
	XferAmt = i_Socket->recv_n((void *) &i_Transiant[i_XferTotal],
			i_Blocksize - i_XferTotal);
	merrno = errno;
	if (XferAmt > 0)
	{
	    i_XferTotal += XferAmt;
	    i_LastData = Now();
	    if ((i_Filesize > 0 &&
		i_Filesize == i_Total + i_XferTotal) ||
		i_XferTotal == i_Blocksize)
	    {
		i_Total += i_XferTotal;
		i_File->Write(i_Transiant, i_XferTotal);
		i_XferTotal = 0;
		memset(i_Transiant, 0, i_Blocksize);
		i_Socket->send_n((void *) htonl(i_Total), 4);
		merrno = errno;
	    }
	}
	else if (XferAmt < 0)
	{
	    switch (merrno)
	    {
	    case EINTR:		// Interrupted System Call
	    case EAGAIN:	// Temporarily Unavailable
	    case EWOULDBLOCK:	// Blocking Request Would Block
	    case ENOMEM:	// Not Enough Memory
	    case ETIME:		// Request Timed Out
		break;
	    default:
		i_File->Close();
	    }
	}
    }
    else if (i_Type == Send)
    {
	if (i_Filesize == i_Total + i_XferTotal ||
	    i_XferTotal == i_Blocksize)
	{
	    unsigned long verify;
	    XferAmt = i_Socket->recv_n((void *) verify, 4);
	    merrno = errno;
	    if (XferAmt < 0 || ntohl(verify) != i_Total)
	    {
		switch (merrno)
		{
		case EINTR:		// Interrupted System Call
		case EAGAIN:		// Temporarily Unavailable
		case EWOULDBLOCK:	// Blocking Request Would Block
		case ENOMEM:		// Not Enough Memory
		case ETIME:		// Request Timed Out
		    break;
		default:
		    i_File->Close();
		}
		return;
	    }
	    else if (XferAmt < 1)
	    {
		return;
	    }
	    memset(i_Transiant, 0, i_Blocksize);
	    i_Total += i_XferTotal;
	    i_XferTotal = 0;
	    if (i_Total == i_Filesize)
	    {
		i_File->Close();
		return;
	    }
	}
	if (i_Transiant[0] == 0 && i_Total < i_Filesize)
	{
	    if (i_Total + i_Blocksize > i_Filesize)
		i_File->Read(i_Transiant, i_Filesize - i_Total);
	    else
		i_File->Read(i_Transiant, i_Blocksize);
	}
	XferAmt = i_Socket->send_n((void *) &i_Transiant[i_XferTotal],
			strlen(i_Transiant) - i_XferTotal);
	merrno = errno;
	if (XferAmt > 0)
	{
	    i_XferTotal += XferAmt;
	    i_LastData = Now();
	}
	else if (XferAmt < 0)
	{
	    switch (merrno)
	    {
	    case EINTR:		// Interrupted System Call
	    case EAGAIN:	// Temporarily Unavailable
	    case EWOULDBLOCK:	// Blocking Request Would Block
	    case ENOMEM:	// Not Enough Memory
	    case ETIME:		// Request Timed Out
		break;
	    default:
		i_File->Close();
	    }
	}
    }
}
int DccMap::open(void *in)
{
    FT("DccMap::open", ("(void *) in"));
    RET(activate());
}

int DccMap::close(unsigned long in)
{
    FT("DccMap::close", (in));
    // dump all and close open file handles.
    map<unsigned long, DccXfer>::iterator iter;
    for (iter=xfers.begin(); iter != xfers.end(); iter++)
	iter->second.Cancel();
    RET(0);
}

int DccMap::svc(void)
{
    NFT("DccMap::svc");

    unsigned long WorkId;
    while (!Parent->Shutdown())
    {
	if (!active.size())
	{
#ifdef WIN32
	    Sleep(1000);
#else
	    sleep(1);
#endif
	    continue;
	}

	WorkId = active.front();
	active.pop();

	if (xfers.find(WorkId) == xfers.end())
	    continue;
	if (!xfers[WorkId].Ready())
	{
	    xfers.erase(WorkId);
	    continue;
	}

	xfers[WorkId].Action();
	if (xfers[WorkId].LastData().SecondsSince() > Parent->files.Timeout())
	{
	    xfers[WorkId].Cancel();
	    xfers.erase(WorkId);
	    continue;
	}
	active.push(WorkId);
    }
}

vector<unsigned long> DccMap::GetList(mstring in)
{
    FT("DccMap::GetList", (in));
    vector<unsigned long> retval;

    map<unsigned long, DccXfer>::iterator iter;
    for (iter=xfers.begin(); iter!=xfers.end(); iter++)
    {
	if (iter->second.Source().CmpNoCase(in)==0)
	    retval.push_back(iter->first);
    }
    NRET(vector<unsigned long>, retval);
}

unsigned long DccMap::Connect(ACE_INET_Addr address,
	mstring mynick, mstring source, mstring filename,
	size_t filesize, size_t blocksize)
{
    FT("DccMap::Connect", ("(ACE_INET_Addr) address", mynick,
			source, filename, filesize, blocksize));
    ACE_SOCK_Stream *DCC_SOCK = new ACE_SOCK_Stream;
    ACE_SOCK_Connector tmp((ACE_SOCK_Stream &) *DCC_SOCK, (ACE_Addr &) address,
			&ACE_Time_Value(Parent->files.Timeout()));
    if (errno != ETIME)
    {
	unsigned long WorkId;
	for (WorkId = 1; WorkId > 0; WorkId++)
	{
	    if (xfers.find(WorkId) == xfers.end())
	    {
		xfers[WorkId] = DccXfer(WorkId, DCC_SOCK, source,
			mynick, filename, filesize, blocksize);
		active.push(WorkId);
		RET(WorkId);
	    }
	}
	send(mynick, source, "Unable to accept DCC");
    }
    else
	send(mynick, source, "DCC tiemd out");
    RET(0);
}

unsigned long DccMap::Accept(unsigned short port, mstring mynick,
	mstring source, FileMap::FileType filetype, unsigned long filenum)
{
    FT("DccMap::Accept", (port, mynick, source, (int) filetype, filenum));
    ACE_INET_Addr local(port, Parent->LocalHost());
    ACE_SOCK_Stream *DCC_SOCK = new ACE_SOCK_Stream;
    ACE_SOCK_Acceptor tmp(local);
    tmp.accept((ACE_SOCK_Stream &) *DCC_SOCK,
    	NULL, &ACE_Time_Value(Parent->files.Timeout()));
    if (errno != ETIME)
    {
	unsigned long WorkId;
	for (WorkId = 1; WorkId > 0; WorkId++)
	{
	    if (xfers.find(WorkId) == xfers.end())
	    {
		xfers[WorkId] = DccXfer(WorkId, DCC_SOCK, source,
			mynick, filetype, filenum);
		active.push(WorkId);
		RET(WorkId);
	    }
	}
	send(mynick, source, "Unable to initiate DCC");
    }
    else
	send(mynick, source, "DCC tiemd out");
    RET(0);
}

void DccMap::Close(unsigned long DccId)
{
    FT("DccMap::Close", (DccId));
    if (xfers.find(DccId) != xfers.end())
	xfers[DccId].Cancel();
}


