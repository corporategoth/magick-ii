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

#include "lockable.h"
#include "magick.h"

queue<unsigned long> DccMap::active;
map<unsigned long, DccXfer> DccMap::xfers;

unsigned short FindAvailPort()
{
    NFT("FindAvailPort");

    ACE_INET_Addr local;
    ACE_SOCK_Acceptor accept(ACE_Addr::sap_any);
    accept.get_local_addr(local);
    unsigned short retval = local.get_port_number();
    RET(retval);
}

unsigned long FileMap::FindAvail(FileMap::FileType type)
{
    FT("FileMap::FindAvail", ((int) type));

    unsigned long filenum = 1;
    if (i_FileMap.find(type) != i_FileMap.end())
    {
	while (filenum < 0xffffffff) // Guarentee 8 digits
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
    else if (type == Public)
    	filename.Prepend(Parent->files.Public() + DirSlash);

    if (wxFile::Exists(filename.c_str()))
    {
	if (i_FileMap.find(type) != i_FileMap.end())
	{
	    if (i_FileMap[type].find(num) != i_FileMap[type].end())
	    {
		RET(true);
	    }
	}
	remove(filename.c_str());
    }
    else
    {
	if (i_FileMap.find(type) != i_FileMap.end())
	{
	    if (i_FileMap[type].find(num) != i_FileMap[type].end())
	    {
	    	i_FileMap[type].erase(num);
	    }
	}
    }
    RET(false);
}


mstring FileMap::GetName(FileMap::FileType type, unsigned long num)
{
    FT("FileMap::GetName", ((int) type, num));

    if (Exists(type, num))
    {
	RET(i_FileMap[type][num].first);
    }
    RET("");
}


mstring FileMap::GetPriv(FileMap::FileType type, unsigned long num)
{
    FT("FileMap::GetPriv", ((int) type, num));

    if (Exists(type, num))
    {
	RET(i_FileMap[type][num].second);
    }
    RET("");
}


bool FileMap::SetPriv(FileMap::FileType type, unsigned long num, mstring priv)
{
    FT("FileMap::SetPriv", ((int) type, num, priv));

    if (Exists(type, num))
    {
	i_FileMap[type][num] = pair<mstring,mstring>(
	    			i_FileMap[type][num].first, priv);
	RET(true);
    }
    RET(false);
}


bool FileMap::Rename(FileMap::FileType type, unsigned long num, mstring newname)
{
    FT("FileMap::SetPriv", ((int) type, num, newname));

    if (type != Picture && Exists(type, num))
    {
	i_FileMap[type][num] = pair<mstring,mstring>(newname,
	    			i_FileMap[type][num].second);
	RET(true);
    }
    RET(false);
}


size_t FileMap::GetSize(FileMap::FileType type, unsigned long num)
{
    FT("FileMap::GetSize", ((int) type, num));
    if (Exists(type, num))
    {
	mstring filename;
	filename.Format("%08x", num);

	if (type == MemoAttach)
	    filename.Prepend(Parent->files.MemoAttach() + DirSlash);
	else if (type == Picture)
	    filename.Prepend(Parent->files.Picture() + DirSlash);
	else if (type == Public)
	    filename.Prepend(Parent->files.Public() + DirSlash);

	wxFile tmp(filename);
	size_t retval = tmp.Length();
	RET(retval);
    }
    RET(0);
}


unsigned long FileMap::NewFile(FileMap::FileType type, mstring filename, mstring priv)
{
    FT("FileMap::NewFile", ((int) type, filename, priv));

    unsigned long filenum = 0;
    if (i_FileMap.find(type) != i_FileMap.end())
    {
    	if (!GetNum(type, filename))
    	{
	    filenum = FindAvail(type);
	    i_FileMap[type][filenum] = pair<mstring,mstring>(filename, priv);
	}
    }

    RET(filenum);
}

void FileMap::EraseFile(FileType type, unsigned long num)
{
    FT("FileMap::EraseFile", (type, num));

    if (Exists(type, num))
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
	else if (type == Public)
	    sourcefile.Format("%s%s%08x",
			Parent->files.Public().c_str(),
			DirSlash.c_str(), num);
	if (sourcefile != "")
	    remove(sourcefile.c_str());
	i_FileMap[type].erase(num);
    }
}


vector<unsigned long> FileMap::GetList(FileMap::FileType type, mstring source)
{
    FT("FileMap::GetList", ((int) type, source));
    vector<unsigned long> retval, chunked;
    map<unsigned long, pair<mstring, mstring> >::iterator iter;
    int i;

    if (i_FileMap.find(type) != i_FileMap.end())
    {
    	for (iter = i_FileMap[type].begin(); iter != i_FileMap[type].end(); iter++)
	{
	    if (!Exists(type, iter->first))
	    	chunked.push_back(iter->first);
	    else if (iter->second.second == "")
	    	retval.push_back(iter->first);
	    else
	    	for (i=1; i<=iter->second.second.WordCount(" "); i++)
	    	{
	    	    if (Parent->commserv.IsList(iter->second.second.ExtractWord(i, " ")) &&
	    	    	Parent->commserv.list[iter->second.second.ExtractWord(i, " ").UpperCase()].IsOn(source))
	    	    {
	    	    	retval.push_back(iter->first);
	    	    	break;
	    	    }
	    	}
	}
	for (i=0; i<chunked.size(); i++)
	    i_FileMap[type].erase(chunked[i]);
    }
    NRET(vector<unsigned long>, retval);
}


unsigned long FileMap::GetNum(FileMap::FileType type, mstring name)
{
    FT("FileMap::GetNum", ((int) type, name));
    map<unsigned long, pair<mstring, mstring> >::iterator iter;
    int i;

    if (i_FileMap.find(type) != i_FileMap.end())
    {
    	for (iter = i_FileMap[type].begin(); iter != i_FileMap[type].end(); iter++)
	{
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


void FileMap::save_database(wxOutputStream& out)
{
    FT("FileMap::save_database", ("(wxOutputStream &) out"));
    map<FileType, map <unsigned long, pair<mstring, mstring> > >::iterator i;
    map <unsigned long, pair<mstring, mstring> >::iterator j;

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
				j->first, j->second.first.c_str()));
	}
    }
}


void FileMap::load_database(wxInputStream& in)
{
    FT("FileMap::load_database", ("(wxInputStream &) in"));

    map<FileType, map <unsigned long, pair<mstring, mstring> > >::size_type cnt1, i;
    map <unsigned long, pair<mstring, mstring> >::size_type cnt2, j;

    int val1;
    unsigned long val2;
    pair<mstring, mstring> val3;

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
	    COM(("Entry FILE %08x (%s) loaded ...", val2, val3.first.c_str()));
	}
	COM(("Entry FILE MAP %d loaded ...", (int) val1));
    }
}

DccXfer::DccXfer(unsigned long dccid, ACE_SOCK_Stream *socket,
	mstring mynick, mstring source,
	FileMap::FileType filetype, unsigned long filenum)
	: i_Socket(socket)
{
    FT("DccXfer::DccXfer", (dccid, "(ACE_SOCK_Stream *) socket",
			mynick, source, (int) filetype, filenum));

    // Setup Paramaters
    i_DccId = dccid;
    i_Transiant = NULL;
    i_Mynick = mynick;
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
    {
	send(mynick, source, Parent->getMessage(source, "DCC/NOFILE"),
					"SEND");
	return;
    }

    // Set 'Ready to Transfer'
    mstring tmp;
    if (filetype == FileMap::MemoAttach)
	tmp.Format("%s%s%08x", Parent->files.MemoAttach().c_str(),
			DirSlash.c_str(), filenum);
    else if (filetype == FileMap::Picture)
	tmp.Format("%s%s%08x", Parent->files.Picture().c_str(),
			DirSlash.c_str(), filenum);
    else if (filetype == FileMap::Public)
	tmp.Format("%s%s%08x", Parent->files.Public().c_str(),
			DirSlash.c_str(), filenum);
    if (!wxFile::Exists(tmp.c_str()))
    {
	Parent->filesys.EraseFile(filetype, filenum);
	send(mynick, source, Parent->getMessage(source, "DCC/NOFILE"),
					"SEND");
	return;
    }
    if (wxFile::Exists(i_Tempfile.c_str()))
	remove(i_Tempfile.c_str());
    wxFileInputStream fin(tmp.c_str());
    wxFileOutputStream fout(i_Tempfile.c_str());
    fout << fin;
    i_File.Open(i_Tempfile.c_str(), wxFile::read);
    i_Filesize = i_File.Length();

    // Initialize Transfer
    i_Transiant = (unsigned char *) ACE_OS::malloc (sizeof(unsigned char) * (i_Blocksize + 1));
    ACE_OS::memset(i_Transiant, 0, i_Blocksize + 1);
    i_Total = 0;
    i_XferTotal = 0;
    i_LastData = Now();
    CP(("DCC %d initialized", i_DccId));
}


DccXfer::DccXfer(unsigned long dccid, ACE_SOCK_Stream *socket,
	mstring mynick, mstring source, mstring filename,
	size_t filesize, size_t blocksize)
	: i_Socket(socket)
{
    FT("DccXfer::DccXfer", (dccid, "(ACE_SOCK_Stream *) socket",
		mynick, source, filename, filesize, blocksize));

    // Setup Paramaters
    i_DccId = dccid;
    i_Transiant = NULL;
    i_Mynick = mynick;
    i_Source = source;
    i_Type = Get;
    i_Blocksize = Parent->files.Blocksize();
    if (blocksize > 0)
	i_Blocksize = blocksize;
    i_Tempfile.Format("%s%s%08x", Parent->files.TempDir().c_str(),
		 DirSlash.c_str(), i_DccId);
    i_Filename = filename;


    // Verify Paramaters
    if (!Parent->nickserv.IsLive(i_Source))
	return;

    if (Parent->nickserv.live[i_Source.LowerCase()].InFlight.File() &&
	!Parent->nickserv.live[i_Source.LowerCase()].InFlight.InProg())
    {
	Parent->nickserv.live[i_Source.LowerCase()].InFlight.SetInProg();
    }
    else
    {
	send(mynick, source, Parent->getMessage(source, "DCC/NOREQ0UEST"),
						"GET");
	return;
    }

    // Set 'Ready to Transfer'
    if (wxFile::Exists(i_Tempfile.c_str()))
	remove(i_Tempfile.c_str());
    i_File.Open(i_Tempfile.c_str(), wxFile::write);
    i_Filesize = filesize;

    // Initialize Transfer
    i_Transiant = (unsigned char *) ACE_OS::malloc (sizeof(unsigned char) * (i_Blocksize + 1));
    ACE_OS::memset(i_Transiant, 0, i_Blocksize + 1);
    i_Total = 0;
    i_XferTotal = 0;
    i_LastData = Now();
    CP(("DCC %d initialized", i_DccId));
}

DccXfer::~DccXfer()
{
    NFT("DccXfer::~DccXfer");

    if (i_Transiant != NULL)
	ACE_OS::free(i_Transiant);
    i_Transiant = NULL;

    if (i_File.IsOpened())
	i_File.Close();

    if (i_Socket.get() == NULL)
	return;

    CP(("DCC Xfer #%d Completed", i_DccId));
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
	    if (Parent->nickserv.live[i_Source.LowerCase()].InFlight.Memo())
		filetype = FileMap::MemoAttach;
	    else if (Parent->nickserv.live[i_Source.LowerCase()].InFlight.Picture())
		filetype = FileMap::Picture;
	    else if (Parent->nickserv.live[i_Source.LowerCase()].InFlight.Public())
		filetype = FileMap::Public;
	    unsigned long filenum = Parent->filesys.NewFile(filetype, i_Filename);
	    if (filetype == FileMap::MemoAttach)
		tmp.Format("%s%s%08x", Parent->files.MemoAttach().c_str(),
			DirSlash.c_str(), filenum);
	    else if (filetype == FileMap::Picture)
		tmp.Format("%s%s%08x", Parent->files.Picture().c_str(),
			DirSlash.c_str(), filenum);
	    else if (filetype == FileMap::Public)
		tmp.Format("%s%s%08x", Parent->files.Public().c_str(),
			DirSlash.c_str(), filenum);
	    if (wxFile::Exists(i_Tempfile.c_str()))
	    {
		if (wxFile::Exists(tmp.c_str()))
		    remove(tmp.c_str());
		wxFileInputStream fin(i_Tempfile.c_str());
		wxFileOutputStream fout(tmp.c_str());
		fout << fin;
		Parent->nickserv.live[i_Source.LowerCase()].InFlight.File(filenum);
		CP(("Added entry %d to FileMap", filenum));
	    }
	    else
		Parent->nickserv.live[i_Source.LowerCase()].InFlight.File(0);
	}
	else
	    Parent->nickserv.live[i_Source.LowerCase()].InFlight.File(0);
    }

    if (wxFile::Exists(i_Tempfile.c_str()))
	remove(i_Tempfile.c_str());
}

void DccXfer::operator=(const DccXfer &in)
{
    FT("DccXfer::operator=", ("(const DccXfer &) in"));
    i_Socket=(auto_ptr<ACE_SOCK_Stream> &) in.i_Socket;
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

    if (in.i_File.IsOpened())
    {
	DccXfer *tmp = (DccXfer *) &in;
	tmp->i_File.Close();
	if (i_Type == Get)
	{
	    i_File.Open(i_Tempfile.c_str(), wxFile::write_append);
	}
	else if (i_Type == Send)
	{
	    i_File.Open(i_Tempfile.c_str(), wxFile::read);
	    if (i_Total + i_Blocksize > i_Filesize)
		i_File.SeekEnd();
	    else
		i_File.Seek(i_Total + i_Blocksize);
	}
    }
    i_Transiant = NULL;
    if (in.i_Transiant != NULL && ACE_OS::strlen((const char *) in.i_Transiant))
    {
	i_Transiant=(unsigned char *) ACE_OS::malloc(sizeof(unsigned char) * i_Blocksize + 1);
	ACE_OS::memset(i_Transiant, 0, i_Blocksize + 1);
	ACE_OS::strcpy((char *) i_Transiant, (char *) in.i_Transiant);
    }
    i_LastData=in.i_LastData;
}


void DccXfer::Action()
{
    NFT("DccXfer::Action");
    size_t XferAmt = 0;
    int merrno;
    ACE_Time_Value onesec(1);

    if (i_Type == Get)
    {
	XferAmt = i_Socket->recv_n((void *) &i_Transiant[i_XferTotal],
			i_Blocksize - i_XferTotal, &onesec);
	merrno = errno;
	COM(("%d: Bytes Transferred - %d, RECV Response %d", i_DccId, XferAmt, merrno));
	if (XferAmt > 0)
	{
	    i_XferTotal += XferAmt;
	    i_LastData = Now();
	    if ((i_Filesize > 0 &&
		i_Filesize == i_Total + i_XferTotal) ||
		i_XferTotal == i_Blocksize)
	    {
		i_Total += i_XferTotal;
		i_File.Write(i_Transiant, i_XferTotal);
		i_XferTotal = 0;
		ACE_OS::memset(i_Transiant, 0, i_Blocksize);
		XferAmt = i_Socket->send_n((void *) htonl(i_Total), 4, &onesec);
		merrno = errno;
		COM(("%d: Bytes Transferred - %d, SEND Response %d", i_DccId, XferAmt, merrno));
		if (i_Filesize == i_Total)
		{
		    send(i_Mynick, i_Source, Parent->getMessage(i_Source, "DCC/COMPLETED"),
					"GET", i_Total);
		    i_File.Close();
		}
	    }
	}
	else
	{
	    switch (merrno)
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
						"GET");
		i_File.Close();
	    }
	}
    }
    else if (i_Type == Send)
    {
	if (i_Filesize == i_Total + i_XferTotal ||
	    i_XferTotal == i_Blocksize)
	{
	    unsigned long verify;
	    XferAmt = i_Socket->recv_n((void *) verify, 4, &onesec);
	    merrno = errno;
	    COM(("%d: Bytes Transferred - %d, RECV Response %d", i_DccId, XferAmt, merrno));
	    if (XferAmt <= 0 || ntohl(verify) != i_Total)
	    {
		switch (merrno)
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
						"SEND");
		    i_File.Close();
		}
		return;
	    }
	    ACE_OS::memset(i_Transiant, 0, i_Blocksize);
	    i_Total += i_XferTotal;
	    i_XferTotal = 0;
	    if (i_Total == i_Filesize)
	    {
		send(i_Mynick, i_Source, Parent->getMessage(i_Source, "DCC/COMPLETED"),
					"SEND", i_Total);
		i_File.Close();
		return;
	    }
	}
	if (i_Transiant[0] == 0 && i_Total < i_Filesize)
	{
	    if (i_Total + i_Blocksize > i_Filesize)
		i_File.Read(i_Transiant, i_Filesize - i_Total);
	    else
		i_File.Read(i_Transiant, i_Blocksize);
	}
	XferAmt = i_Socket->send_n((void *) &i_Transiant[i_XferTotal],
			ACE_OS::strlen((char *) i_Transiant) - i_XferTotal,
			&onesec);
	merrno = errno;
	COM(("%d: Bytes Transferred - %d, SEND Response %d", i_DccId, XferAmt, merrno));
	if (XferAmt > 0)
	{
	    i_XferTotal += XferAmt;
	    i_LastData = Now();
	}
	else
	{
	    switch (merrno)
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
						"SEND");
		i_File.Close();
	    }
	}
    }
}
int DccMap::open(void *in)
{
    FT("DccMap::open", ("(void *) in"));
    int retval = activate();
    RET(retval);
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
    mThread::Attach(tt_MAIN);

    unsigned long WorkId;
    while (!Parent->Shutdown())
    {
	COM(("Active Size is %d", active.size()));
	if (!active.size())
	{
	    ACE_OS::sleep(1);
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

	CP(("Executing ACTION for DCC #%d", WorkId));
	xfers[WorkId].Action();
	if (xfers[WorkId].LastData().SecondsSince() > Parent->files.Timeout())
	{
	    xfers[WorkId].Cancel();
	    xfers.erase(WorkId);
	    continue;
	}
	active.push(WorkId);
    }
    mThread::Detach(tt_MAIN);
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

void *DccMap::Connect2(void *in)
{
    mThread::Attach(tt_MAIN);
    FT("DccMap::Connect2", ("(void *) in"));

    NewSocket *val = (NewSocket *) in;

    ACE_SOCK_Stream *DCC_SOCK = new ACE_SOCK_Stream;
    ACE_Time_Value tv(Parent->files.Timeout());
    ACE_SOCK_Connector tmp((ACE_SOCK_Stream &) *DCC_SOCK, (ACE_Addr &) val->address, &tv);
    CP(("Connect responded with %d", errno));
    if (errno != ETIME)
    {
	unsigned long WorkId;
	bool found = false;
	for (WorkId = 1; !found && WorkId < 0xffffffff; WorkId++)
	{
	    if (xfers.find(WorkId) == xfers.end())
	    {
		xfers[WorkId] = DccXfer(WorkId, DCC_SOCK, val->mynick,
			val->source, val->filename, val->filesize, val->blocksize);
		active.push(WorkId);
		found = true;
		CP(("Created DCC entry #%d", WorkId));
	    }
	}
	if (!found)
	    send(val->mynick, val->source, Parent->getMessage("DCC/FAILED"),
						"GET");
    }
    else
	send(val->mynick, val->source, Parent->getMessage("DCC/NOCONNECT"),
						"GET");
    if (val != NULL)
	delete val;

    mThread::Detach(tt_MAIN);
    RET(0);
}

void *DccMap::Accept2(void *in)
{
    mThread::Attach(tt_MAIN);
    FT("DccMap::Accept2", ("(void *) in"));

    NewSocket *val = (NewSocket *) in;

    ACE_INET_Addr local(val->port, Parent->LocalHost());
    ACE_SOCK_Stream *DCC_SOCK = new ACE_SOCK_Stream;
    ACE_Time_Value tv(Parent->files.Timeout());
    ACE_SOCK_Acceptor tmp(local);
    tmp.accept((ACE_SOCK_Stream &) *DCC_SOCK, NULL, &tv);
    CP(("Accept responded with %d", errno));
    if (errno != ETIME)
    {
	unsigned long WorkId;
	bool found = false;
	for (WorkId = 1; !found && WorkId < 0xffffffff; WorkId++)
	{
	    if (xfers.find(WorkId) == xfers.end())
	    {
		xfers[WorkId] = DccXfer(WorkId, DCC_SOCK, val->mynick,
			val->source, val->filetype, val->filenum);
		active.push(WorkId);
		found = true;
		CP(("Created DCC entry #%d", WorkId));
	    }
	}
	if (!found)
	    send(val->mynick, val->source, Parent->getMessage("DCC/FAILED"),
						"SEND");
    }
    else
	send(val->mynick, val->source, Parent->getMessage("DCC/NOCONNECT"),
						"SEND");
    if (val != NULL)
	delete val;

    mThread::Detach(tt_MAIN);
    RET(0);
}

void DccMap::Connect(ACE_INET_Addr address,
	mstring mynick, mstring source, mstring filename,
	size_t filesize, size_t blocksize)
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

    tm.spawn(Connect2, (void *) tmp);
}

void DccMap::Accept(unsigned short port, mstring mynick,
	mstring source, FileMap::FileType filetype, unsigned long filenum)
{
    FT("DccMap::Accept", (port, mynick, source, (int) filetype, filenum));
    NewSocket *tmp = new NewSocket;
    tmp->port = port;
    tmp->source = source;
    tmp->mynick = mynick;
    tmp->filetype = filetype;
    tmp->filenum = filenum;

    tm.spawn(Accept2, (void *) tmp);
}

void DccMap::Close(unsigned long DccId)
{
    FT("DccMap::Close", (DccId));
    if (xfers.find(DccId) != xfers.end())
	xfers[DccId].Cancel();
    tm.cancel_all();
}


