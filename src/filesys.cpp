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

#include "lockable.h"
#include "magick.h"

queue<unsigned long> DccMap::active;
map<unsigned long, DccXfer *> DccMap::xfers;

mFile::mFile(FILE *in)
{
    FT("mFile::mFile", ("(FILE *) in"));
    fd = in;
}

mFile::mFile(mstring name, mstring mode)
{
    FT("mFile::mFile", (name, mode));
    
    if ((fd = ACE_OS::fopen(name.c_str(), mode.c_str())) == NULL)
    {
	Log(LM_ERROR, Parent->getLogMessage("SYS_ERRORS/COULDNOTOPEN"),
				name.c_str(), mode.c_str());
    }
}

bool mFile::Open(mstring name, mstring mode)
{

    FT("mFile::Open", (name, mode));
    if ((fd = ACE_OS::fopen(name.c_str(), mode.c_str())) == NULL)
    {
	Log(LM_ERROR, Parent->getLogMessage("SYS_ERRORS/COULDNOTOPEN"),
				name.c_str(), mode.c_str());
    }
    RET(fd != NULL);
}

void mFile::Close()
{
    NFT("mFile::Close");
    if (IsOpened())
    {
	ACE_OS::fflush(fd);
	ACE_OS::fclose(fd);
    }
    fd = NULL;
}

long mFile::Seek(long offset, int whence)
{
    FT("mFile::Seek", (offset, whence));
    if (!IsOpened())
	RET(-1);
    long retpos = fseek(fd, offset, whence);
    RET(retpos);
}

size_t mFile::Write(mstring buf, bool endline)
{
    FT("mFile::Write", (buf, endline));
    if (!IsOpened())
	RET(0);
    if (endline)
	buf << "\n";
    long written = Write(buf.c_str(), buf.Len());
    RET(written);
}

size_t mFile::Write(const void *buf, size_t size)
{
    FT("mFile::Write", ("(const void *)", size));
    if (!IsOpened())
	RET(0);
    long written = ACE_OS::fwrite(buf, 1, size, fd);
    RET(written);
}

size_t mFile::Read(void *buf, size_t size)
{
    FT("mFile::Read", ("(const void *)", size));
    if (!IsOpened())
	RET(0);
    long read = ACE_OS::fread(buf, 1, size, fd);
    RET(read);
}

long mFile::Length()
{
    NFT("mFile::Length");
    if (!IsOpened())
	RET(-1);
    struct stat st;
    fstat(fileno(fd), &st);
    RET(st.st_size);
}

void mFile::Flush()
{
    NFT("mFile::Flush");
    if (IsOpened())
	ACE_OS::fflush(fd);
}

bool mFile::Exists(mstring name)
{
    FT("mFile::Exists", (name));
    FILE *fd;
    if ((fd = ACE_OS::fopen(name.c_str(), "r")) == NULL)
	RET(false);
    ACE_OS::fclose(fd);
    RET(true);
}

long mFile::Length(mstring name)
{
    FT("mFile::Length", (name));
    if (!Exists(name))
	RET(0);
    struct stat st;
    stat(name.c_str(), &st);
    RET(st.st_size);
}

FILE *mFile::Detach()
{
    NFT("mFile::Detach");
    FILE *rfd = fd;
    fd = NULL;
    NRET(FILE *, rfd);
}

long mFile::Copy(mstring sin, mstring sout, bool append)
{
    FT("mFile::Copy", (sin, sout, append));
    
    if (sin == "" || !Exists(sin) || sout == "")
	RET(0);
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
long mFile::Dump(vector<mstring> sin, mstring sout, bool append, bool endline)
{
    FT("mFile::Dump", ("(vector<mstring>) sin", sout, append, endline));
    if (sout == "")
	RET(0);
    mFile out(sout.c_str(), append ? "a" : "w");
    if (!(sin.size() && out.IsOpened()))
	RET(0);
    size_t i, total = 0;
    for (i=0; i<sin.size(); i++)
    {
	if (endline)
	    sin[i] << "\n";
	total += out.Write(sin[i].c_str(), sin[i].Len());
    }
    out.Close();
    RET(total);
}


// CANNOT trace this, it is used by TRACE code ...
long mFile::Dump(list<mstring> sin, mstring sout, bool append, bool endline)
{
    FT("mFile::Dump", ("(list<mstring>) sin", sout, append, endline));
    if (sout == "")
	RET(0);
    mFile out(sout.c_str(), append ? "a" : "w");
    if (!(sin.size() && out.IsOpened()))
	RET(0);
	
    size_t total = 0;
    list<mstring>::iterator iter;
    for (iter=sin.begin(); iter!=sin.end(); iter++)
    {
	if (endline)
	    *iter << "\n";
	total += out.Write(iter->c_str(), iter->Len());
    }
    out.Close();
    RET(total);
}

vector<mstring> mFile::UnDump( const mstring &sin)
{
    FT("mFile::UnDump", (sin));
    vector<mstring> Result;

    if(sin == "" || !Exists(sin))
        NRET(vector<mstring>,Result);
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

mstring mFile::ReadLine()
{
    NFT("mFile::ReadLine");
    mstring Result;
    if(!IsOpened())
        RET("");
    char *buffer=NULL;
#ifdef MAGICK_HAS_EXCEPTIONS
    try
    {
#endif
        buffer=new char[Length()+1];
        ACE_OS::fgets(buffer,Length()+1,fd);
        Result=buffer;
        if(Result[Result.Len()-1]=='\n')
            Result=Result.Left(Result.Len()-1);
#ifdef MAGICK_HAS_EXCEPTIONS
    }
    catch(...)
    {
        // this catches any exceptions so that we free up the buffer
        Result="";
    }
#endif
    if(buffer!=NULL)
        delete [] buffer;

    RET(Result);
}

size_t mFile::DirUsage(mstring directory)
{
    FT("mFile::DirUsage", (directory));
    size_t retval = 0;

    DIR *dir = NULL;
    struct dirent *entry = NULL;
    struct stat st;

    if (!directory.Len())
	RET(0);

    if ((dir = ACE_OS::opendir(directory.c_str())) != NULL)
    {
	while ((entry = ACE_OS::readdir(dir)) != NULL)
	{
	    if (strlen(entry->d_name))
	    {
		stat((directory + DirSlash + entry->d_name).c_str(), &st);
		retval += st.st_size;
	    }
	}
	ACE_OS::closedir(dir);
    }

    RET(retval);
}


unsigned short FindAvailPort()
{
    NFT("FindAvailPort");

    ACE_INET_Addr local;
    ACE_SOCK_Acceptor accept(ACE_Addr::sap_any);
    accept.get_local_addr(local);
    unsigned short retval = local.get_port_number();
    accept.close();
    RET(retval);
}

unsigned long FileMap::FindAvail(FileMap::FileType type)
{
    FT("FileMap::FindAvail", ((int) type));

    unsigned long filenum = 1;
    while (filenum < 0xffffffff) // Guarentee 8 digits
    {
	if (i_FileMap[type].find(filenum) == i_FileMap[type].end())
	{
	    RET(filenum);
	}
	filenum++;
    }

    Log(LM_ERROR, Parent->getLogMessage("SYS_ERROR/FILEMAPFULL"),
		(int) type);
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

    if (mFile::Exists(filename.c_str()))
    {
	if (i_FileMap.find(type) != i_FileMap.end())
	{
	    if (i_FileMap[type].find(num) != i_FileMap[type].end())
	    {
		RET(true);
	    }
	}
	remove(filename.c_str());
	Log(LM_CRITICAL, Parent->getLogMessage("SYS_ERROR/MISSING_FILE1"),
		(int) type, num);
    }
    else
    {
	if (i_FileMap.find(type) != i_FileMap.end())
	{
	    if (i_FileMap[type].find(num) != i_FileMap[type].end())
	    {
	    	i_FileMap[type].erase(num);
		Log(LM_CRITICAL, Parent->getLogMessage("SYS_ERROR/MISSING_FILE2"),
			(int) type, num);
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


mstring FileMap::GetRealName(FileMap::FileType type, unsigned long num)
{
    FT("FileMap::GetRealName", ((int) type, num));

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
    	RET(filename);
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
    mstring filename = GetRealName(type, num);
    if (filename != "")
    {
	CP(("Checking file size of %s", filename.c_str()));
	long retval = mFile::Length(filename);
	RET(retval);
    }
    RET(0);
}


unsigned long FileMap::NewFile(FileMap::FileType type, mstring filename, mstring priv)
{
    FT("FileMap::NewFile", ((int) type, filename, priv));

    unsigned long filenum = 0;
    if (!GetNum(type, filename))
    {
	filenum = FindAvail(type);
	if (filenum)
	    i_FileMap[type][filenum] = pair<mstring,mstring>(filename, priv);
    }

    RET(filenum);
}

void FileMap::EraseFile(FileType type, unsigned long num)
{
    FT("FileMap::EraseFile", (type, num));

    mstring filename = GetRealName(type, num);
    if (filename != "")
    {
	remove(filename.c_str());
	i_FileMap[type].erase(num);
    }
}


vector<unsigned long> FileMap::GetList(FileMap::FileType type, mstring source)
{
    FT("FileMap::GetList", ((int) type, source));
    vector<unsigned long> retval;
    map<unsigned long, pair<mstring, mstring> >::iterator iter;
    int i;

    if (i_FileMap.find(type) != i_FileMap.end())
    {
    	for (iter = i_FileMap[type].begin(); iter != i_FileMap[type].end(); iter++)
	{
	    if (!Exists(type, iter->first))
	    {
		// This will take care of itself.
	    }
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

size_t FileMap::FileSysSize(FileMap::FileType type)
{
    FT("FileMap::FileSysSize", ((int) type));
    size_t retval = 0;

    mstring dirname;
    if (type == MemoAttach)
	dirname = Parent->files.MemoAttach();
    else if (type == Picture)
	dirname = Parent->files.Picture();
    else if (type == Public)
	dirname = Parent->files.Public();

    retval = mFile::DirUsage(dirname);
    RET(retval);
}


SXP::Tag FileMap::tag_FileMap("FileMap");
SXP::Tag FileMap::tag_File("File");

void FileMap::BeginElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    FT("FileMap::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    mstring in;
    FileMap::FileType type;
    unsigned long number;
    mstring name;
    mstring priv;
    if(pElement->IsA(tag_File))
    {
	pElement->Retrieve(in);
	if (in.WordCount("\n") == 4)
	{
	    type = (FileMap::FileType) atoi(in.ExtractWord(1, "\n").c_str());
	    number = atoul(in.ExtractWord(2, "\n").c_str());
	    name = in.ExtractWord(3, "\n");
	    priv = in.ExtractWord(4, "\n");
	    i_FileMap[type][number] = pair<mstring,mstring>(name,priv);
	}
    }
}

void FileMap::EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    FT("FileMap::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    // load up simple elements here. (ie single pieces of data)
}

void FileMap::WriteElement(SXP::IOutStream * pOut, SXP::dict& attribs)
{
    FT("FileMap::WriteElement", ("(SXP::IOutStream *) pOut", "(SXP::dict &) attribs"));
    // not sure if this is the right place to do this
    pOut->BeginObject(tag_FileMap, attribs);

    map<FileType, map<unsigned long, pair<mstring, mstring> > >::iterator i1;
    map<unsigned long, pair<mstring, mstring> >::iterator i2;
    mstring out;

    for (i1=i_FileMap.begin(); i1!=i_FileMap.end(); i1++)
	for(i2=i1->second.begin(); i2!=i1->second.end(); i2++)
	{
	    out = "";
	    out << (int) i1->first << "\n" << i2->first << "\n" <<
		i2->second.first << "\n" << i2->second.second;
	    pOut->WriteElement(tag_File, out);
	}

    pOut->EndObject(tag_FileMap);
}


void FileMap::PostLoad()
{
    NFT("FileMap::PostLoad");
    // Linkage, etc
}

DccXfer::DccXfer(unsigned long dccid, auto_ptr<ACE_SOCK_Stream> socket,
	mstring mynick, mstring source,
	FileMap::FileType filetype, unsigned long filenum)
{
    FT("DccXfer::DccXfer", (dccid, "(ACE_SOCK_Stream *) socket",
			mynick, source, (int) filetype, filenum));

    // Setup Paramaters
    i_DccId = dccid;
    i_Transiant = NULL;
    i_Mynick = mynick;
    i_Source = source;
    i_Type = Send;
    i_Socket = socket;
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
    mstring tmp = Parent->filesys.GetRealName(filetype, filenum);
    if (tmp == "")
    {
	Parent->filesys.EraseFile(filetype, filenum);
	send(mynick, source, Parent->getMessage(source, "DCC/NOFILE"),
					"SEND");
	return;
    }
    
    mFile::Copy(tmp, i_Tempfile);
    i_File.Open(i_Tempfile.c_str());
    i_Filesize = i_File.Length();

    // Initialize Transfer
    i_Transiant = (unsigned char *) ACE_OS::malloc (sizeof(unsigned char) * (i_Blocksize + 1));
    ACE_OS::memset(i_Transiant, 0, i_Blocksize + 1);
    i_Total = 0;
    i_XferTotal = 0;
    i_LastData = Now();
    Log(LM_DEBUG, Parent->getLogMessage("OTHER/DCC_INIT"),
		i_DccId, i_Source.c_str(), "SEND");
    CP(("DCC %d initialized", i_DccId));
}


DccXfer::DccXfer(unsigned long dccid, auto_ptr<ACE_SOCK_Stream> socket,
	mstring mynick, mstring source, mstring filename,
	size_t filesize, size_t blocksize)
{
    FT("DccXfer::DccXfer", (dccid, "(ACE_SOCK_Stream *) socket",
		mynick, source, filename, filesize, blocksize));

    // Setup Paramaters
    i_DccId = dccid;
    i_Transiant = NULL;
    i_Mynick = mynick;
    i_Source = source;
    i_Type = Get;
    i_Socket = socket;
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
	send(mynick, source, Parent->getMessage(source, "DCC/NOREQUEST"),
						"GET");
	return;
    }

    // Set 'Ready to Transfer'
    if (mFile::Exists(i_Tempfile.c_str()))
	remove(i_Tempfile.c_str());
    i_File.Open(i_Tempfile.c_str(), "w");
    i_Filesize = filesize;

    // Initialize Transfer
    i_Transiant = (unsigned char *) ACE_OS::malloc (sizeof(unsigned char) * (i_Blocksize + 1));
    ACE_OS::memset(i_Transiant, 0, i_Blocksize + 1);
    i_Total = 0;
    i_XferTotal = 0;
    i_LastData = Now();
    Log(LM_DEBUG, Parent->getLogMessage("OTHER/DCC_INIT"),
		i_DccId, i_Source.c_str(), "GET");
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

    if ((i_Filesize > 0) ? i_Total == i_Filesize
			  : i_Total > 0)
	Log(LM_DEBUG, Parent->getLogMessage("OTHER/DCC_CLOSE"), i_DccId);
    else
	Log(LM_DEBUG, Parent->getLogMessage("OTHER/DCC_CANCEL"), i_DccId);

    if (i_Socket.get() == NULL)
    {
	return;
    }
    else
	i_Socket->close();

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

		if (mFile::Exists(i_Tempfile.c_str()))
		{
		    mFile::Copy(i_Tempfile, tmp);
		    Parent->nickserv.live[i_Source.LowerCase()].InFlight.File(filenum);
		    CP(("Added entry %d to FileMap", filenum));
		}
		else
		    Parent->nickserv.live[i_Source.LowerCase()].InFlight.File(0);
	    }
	    else
		Parent->nickserv.live[i_Source.LowerCase()].InFlight.File(0);
	}
	else
	    Parent->nickserv.live[i_Source.LowerCase()].InFlight.File(0);
    }

    if (mFile::Exists(i_Tempfile.c_str()))
	remove(i_Tempfile.c_str());
}

void DccXfer::operator=(const DccXfer &in)
{
    FT("DccXfer::operator=", ("(const DccXfer &) in"));
    // Ungod, please look at this, will this still pass
    // ownership of the auto_ptr -- because our main
    // problem with DCC is that by the time we get it
    // into the DccXfer struct ready for an Action call,
    // the pointer has gone awry, which is why we get
    // ENOACCESS returns (ie. errno 14)
    //i_Socket=(auto_ptr<ACE_SOCK_Stream> &) in.i_Socket;

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

    DccXfer *tmp = (DccXfer *) &in;
    i_Socket = tmp->i_Socket;
    if (tmp->i_File.IsOpened())
    {
	tmp->i_File.Close();
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
    if (in.i_Transiant != NULL && ACE_OS::strlen((const char *) in.i_Transiant))
    {
	i_Transiant=(unsigned char *) ACE_OS::malloc(sizeof(unsigned char) * i_Blocksize + 1);
	ACE_OS::memset(i_Transiant, 0, i_Blocksize + 1);
	ACE_OS::strcpy((char *) i_Transiant, (char *) in.i_Transiant);
    }
    i_LastData=in.i_LastData;
}


void DccXfer::Cancel()
{
    NFT("DccXfer::Cancel");
    if (Parent->nickserv.IsLive(i_Source))
	Parent->nickserv.live[i_Source.LowerCase()].InFlight.Cancel();
    i_Total = 0;
    i_File.Close();
}

void DccXfer::Action()
{
    NFT("DccXfer::Action");
    long XferAmt = 0, TranSz = 0;
    int merrno;
    unsigned long verify;
    ACE_Time_Value onesec(1);

    if (i_Type == Get)
    {
	COM(("Executing action for DCC %d GET", i_DccId));
	XferAmt = 0;
	merrno = 0;
	if (i_Traffic.size() && (Parent->files.Max_Speed() == 0 ||
		Average() <= Parent->files.Max_Speed()))
	{
	    XferAmt = i_Socket->recv_n((void *) &i_Transiant[i_XferTotal],
			i_Blocksize - i_XferTotal, &onesec);
	    merrno = errno;
	    COM(("%d: Bytes Transferred - %d, RECV Response %d", i_DccId, XferAmt, merrno));
	}
	// Traffic Accounting ...
	map<time_t, size_t>::iterator iter;
	time_t now = time(NULL);
	for (iter=i_Traffic.begin(); iter != i_Traffic.end() &&
		iter->first < now - (Parent->files.Sampletime()+2); iter = i_Traffic.begin())
	    i_Traffic.erase(iter->first);
	if (i_Traffic.find(now) == i_Traffic.end())
	    i_Traffic[now] = 0;
	i_Traffic[now] += XferAmt;
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
		verify = htonl(i_Total);
		XferAmt = i_Socket->send_n((void *) &verify, 4, &onesec);
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
						"GET", merrno);
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
	merrno = 0;
	if (i_Traffic.size() && (Parent->files.Max_Speed() == 0 ||
		Average() <= Parent->files.Max_Speed()))
	{
	    if (i_XferTotal == i_Blocksize)
	    {
		XferAmt = i_Socket->recv_n((void *) &verify, 4, &onesec);
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
						"SEND", merrno);
			i_File.Close();
			return;
		    }
		}
		i_Total += i_XferTotal;
		i_XferTotal = 0;
	    }
	    if (!i_XferTotal && i_Total < i_Filesize)
	    {
		ACE_OS::memset(i_Transiant, 0, i_Blocksize);
		if (i_Total + i_Blocksize > i_Filesize)
		    TranSz = i_File.Read(i_Transiant, i_Filesize - i_Total);
		else
		TranSz = i_File.Read(i_Transiant, i_Blocksize);
		i_XferTotal = 0;
	    }
	    CP(("Going to send %d bytes ...", TranSz));
	    XferAmt = i_Socket->send_n((void *) &i_Transiant[i_XferTotal],
			TranSz - i_XferTotal, &onesec);
	    merrno = errno;
	    COM(("%d: Bytes Transferred - %d, SEND Response %d", i_DccId, XferAmt, merrno));
	}
	// Traffic Accounting ...
	map<time_t, size_t>::iterator iter;
	time_t now = time(NULL);
	for (iter=i_Traffic.begin(); iter != i_Traffic.end() &&
		iter->first < now - (Parent->files.Sampletime()+2); iter = i_Traffic.begin())
	    i_Traffic.erase(iter->first);
	if (i_Traffic.find(now) == i_Traffic.end())
	    i_Traffic[now] = 0;
	i_Traffic[now] += XferAmt;
	if (XferAmt > 0)
	{
	    i_XferTotal += XferAmt;
	    i_LastData = Now();
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
						"SEND", merrno);
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
}

size_t DccXfer::Average(time_t secs)
{
    FT("DccXfer::Average", (secs));
    time_t now = time(NULL);
    size_t total = 0;
    int i = 0;
    map<time_t, size_t>::iterator iter;
    if (secs > Parent->files.Sampletime())
	secs = 0;
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
    map<unsigned long, DccXfer *>::iterator iter;
    for (iter=xfers.begin(); iter != xfers.end(); iter++)
	iter->second->Cancel();
    RET(0);
}

int DccMap::svc(void)
{
    mThread::Attach(tt_MAIN);
    NFT("DccMap::svc");

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
	if (!(*xfers[WorkId]).Ready())
	{
	    delete xfers[WorkId];
	    xfers.erase(WorkId);
	    continue;
	}

	CP(("Executing ACTION for DCC #%d", WorkId));
	// Only do an action IF they have not exceeded the max speed
	xfers[WorkId]->Action();
	// Below LOW SPEED threshold OR
	// No data in X seconds...
	if (xfers[WorkId]->LastData().SecondsSince() > Parent->files.Timeout())
	{
	    xfers[WorkId]->Cancel();
	    delete xfers[WorkId];
	    xfers.erase(WorkId);
	    continue;
	}
	active.push(WorkId);
	FLUSH(); // Force TRACE output dump
    }
    mThread::Detach();
}

vector<unsigned long> DccMap::GetList(mstring in)
{
    FT("DccMap::GetList", (in));
    vector<unsigned long> retval;

    map<unsigned long, DccXfer *>::iterator iter;
    for (iter=xfers.begin(); iter!=xfers.end(); iter++)
    {
	if (iter->second->Source().CmpNoCase(in)==0)
	    retval.push_back(iter->first);
    }
    NRET(vector<unsigned long>, retval);
}

void *DccMap::Connect2(void *in)
{
    mThread::Attach(tt_MAIN);
    FT("DccMap::Connect2", ("(void *) in"));

    NewSocket *val = (NewSocket *) in;

    auto_ptr<ACE_SOCK_Stream> DCC_SOCK(new ACE_SOCK_Stream);
    ACE_Time_Value tv(Parent->files.Timeout());
    ACE_SOCK_Connector tmp(*DCC_SOCK, (ACE_Addr &) val->address, &tv);
    CP(("Connect responded with %d", errno));

    if (errno != ETIME)
    {
	unsigned long WorkId;
	bool found = false;
	for (WorkId = 1; !found && WorkId < 0xffffffff; WorkId++)
	{
	    if (xfers.find(WorkId) == xfers.end())
	    {
		xfers[WorkId] = new DccXfer(WorkId, DCC_SOCK, val->mynick,
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

    DRET(0);
}

void *DccMap::Accept2(void *in)
{
    mThread::Attach(tt_MAIN);
    FT("DccMap::Accept2", ("(void *) in"));

    NewSocket *val = (NewSocket *) in;

    ACE_INET_Addr local(val->port, Parent->LocalHost());
    auto_ptr<ACE_SOCK_Stream> DCC_SOCK(new ACE_SOCK_Stream);
    ACE_Time_Value tv(Parent->files.Timeout());
    ACE_SOCK_Acceptor tmp(local);
    tmp.accept(*DCC_SOCK, NULL, &tv);
    CP(("Accept responded with %d", errno));
    if (errno != ETIME)
    {
	unsigned long WorkId;
	bool found = false;
	for (WorkId = 1; !found && WorkId < 0xffffffff; WorkId++)
	{
	    if (xfers.find(WorkId) == xfers.end())
	    {
		xfers[WorkId] = new DccXfer(WorkId, DCC_SOCK, val->mynick,
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

    DRET(0);
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
    {
	xfers[DccId]->Cancel();
	delete xfers[DccId];
	xfers.erase(DccId);
    }
    tm.cancel_all();
}


