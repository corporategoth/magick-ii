#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
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
** This code is released under the Artistic License v2.0 or better.
** The full text of this license should be contained in a file called
** COPYING distributed with this code.  If this file does not exist,
** it may be viewed here: http://www.magick.tm/m2/license.html
**
** ======================================================================= */
#define RCSID(x,y) const char *rcsid_filesys_cpp_ ## x () { return y; }
RCSID(filesys_cpp, "@(#)$Id$");

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

#include "magick.h"

std::queue < unsigned long > DccMap::active;

DccMap::xfers_t DccMap::xfers;
#ifndef MAGICK_HAS_EXCEPTIONS
static DccXfer GLOB_DccXfer;
#endif

mFile::mFile(const mstring & name, FILE * in, const mstring & mode)
{
    BTCB();
    FT("mFile::mFile", (name, "(FILE *) in"));
    MLOCK((lck_mFile, name));
    fd = in;
    if (fd != NULL)
    {
	i_name = name;
	i_mode = mode;
    }
    ETCB();
}

mFile::mFile(const mstring & name, const mstring & mode)
{
    BTCB();
    FT("mFile::mFile", (name, mode));

    MLOCK((lck_mFile, name));
    if ((fd = ACE_OS::fopen(name.c_str(), mode.c_str())) == NULL)
    {
	LOG(LM_ERROR, "SYS_ERRORS/COULDNOTOPEN", (name, mode, errno, strerror(errno)));
    }
    else
    {
	i_name = name;
	i_mode = mode;
    }
    ETCB();
}

mFile &mFile::operator=(const mFile & in)
{
    BTCB();
    FT("mFile::operator=", ("(const mFile &) in"));
    i_name = in.i_name;
    i_mode = in.i_mode;
    fd = in.fd;
    in.fd = NULL;
    NRET(mFile &, *this);
    ETCB();
}

bool mFile::Open(const mstring & name, const mstring & mode)
{
    BTCB();
    FT("mFile::Open", (name, mode));

    int opres;

    if (fd != NULL)
    {
	MLOCK((lck_mFile, i_name));
	opres = ACE_OS::fclose(fd);
	if (opres != 0 && errno)
	{
	    LOG(LM_ERROR, "SYS_ERRORS/FILEOPERROR", ("fclose", i_name, errno, strerror(errno)));
	}
	fd = NULL;
    }
    i_name.erase();
    i_mode.erase();
    MLOCK((lck_mFile, name));
    if ((fd = ACE_OS::fopen(name.c_str(), mode.c_str())) == NULL)
    {
	LOG(LM_ERROR, "SYS_ERRORS/COULDNOTOPEN", (name, mode, errno, strerror(errno)));
    }
    else
    {
	i_name = name;
	i_mode = mode;
    }
    RET(fd != NULL);
    ETCB();
}

void mFile::Close()
{
    BTCB();
    NFT("mFile::Close");

    int opres;

    MLOCK((lck_mFile, i_name));
    if (IsOpened())
    {
	opres = ACE_OS::fclose(fd);
	if (opres != 0 && errno)
	{
	    LOG(LM_ERROR, "SYS_ERRORS/FILEOPERROR", ("fclose", i_name, errno, strerror(errno)));
	}
	i_name.erase();
	i_mode.erase();
	fd = NULL;
    }
    ETCB();
}

bool mFile::IsOpened() const
{
    BTCB();
    NFT("mFile::IsOpened");
    MLOCK((lck_mFile, i_name));
    bool retval = (fd != NULL);

    RET(retval);
    ETCB();
}

bool mFile::IsReadable() const
{
    BTCB();
    NFT("mFile::IsReadable");
    MLOCK((lck_mFile, i_name));
    bool retval = (i_mode.Contains("r") || i_mode.Contains("+"));

    RET(retval);
    ETCB();
}

bool mFile::IsWritable() const
{
    BTCB();
    NFT("mFile::IsWritable");
    MLOCK((lck_mFile, i_name));
    bool retval = (i_mode.Contains("w") || i_mode.Contains("a") || i_mode.Contains("+"));

    RET(retval);
    ETCB();
}

bool mFile::IsBoth() const
{
    BTCB();
    NFT("mFile::IsBoth");
    MLOCK((lck_mFile, i_name));
    bool retval = i_mode.Contains("+");

    RET(retval);
    ETCB();
}

long mFile::Seek(const long offset, const int whence)
{
    BTCB();
    FT("mFile::Seek", (offset, whence));
    MLOCK((lck_mFile, i_name));
    if (!IsOpened())
	RET(-1);
    long retpos = ACE_OS::fseek(fd, offset, whence);

    if (retpos < 0 && errno)
    {
	LOG(LM_ERROR, "SYS_ERRORS/FILEOPERROR", ("fseek", i_name, errno, strerror(errno)));
    }
    RET(retpos);
    ETCB();
}

size_t mFile::Write(const mstring & buf, const bool endline)
{
    BTCB();
    FT("mFile::Write", (buf, endline));
    mstring data(buf);

    if (endline)
	data << "\n";
    long written = Write(data.c_str(), data.length());

    RET(written);
    ETCB();
}

size_t mFile::Write(const void *buf, const size_t size)
{
    BTCB();
    FT("mFile::Write", ("(const void *)", size));
    MLOCK((lck_mFile, i_name));
    if (!IsOpened())
	RET(0);
    long written = ACE_OS::fwrite(buf, 1, size, fd);

    if (ferror(fd) && errno)
    {
	LOG(LM_ERROR, "SYS_ERRORS/FILEOPERROR", ("fwrite", i_name, errno, strerror(errno)));
    }
    RET(written);
    ETCB();
}

size_t mFile::Read(void *buf, const size_t size)
{
    BTCB();
    FT("mFile::Read", ("(const void *)", size));
    MLOCK((lck_mFile, i_name));
    if (!IsOpened())
	RET(0);
    long bytesread = ACE_OS::fread(buf, 1, size, fd);

    if (ferror(fd) && errno)
    {
	LOG(LM_ERROR, "SYS_ERRORS/FILEOPERROR", ("fread", i_name, errno, strerror(errno)));
    }
    RET(bytesread);
    ETCB();
}

mstring mFile::ReadLine()
{
    BTCB();
    NFT("mFile::ReadLine");
    mstring Result;

    MLOCK((lck_mFile, i_name));
    if (!IsOpened())
	RET("");

    char buffer[1025];

    memset(buffer, 0, 1025);
#ifdef MAGICK_HAS_EXCEPTIONS
    try
    {
#endif
	ACE_OS::fgets(buffer, 1024, fd);
	if (ferror(fd) && errno)
	{
	    LOG(LM_ERROR, "SYS_ERRORS/FILEOPERROR", ("fgets", i_name, errno, strerror(errno)));
	    clearerr(fd);
	}
	else
	{
	    Result = buffer;
	    Result.Remove("\n");
	    Result.Remove("\r");
	}
#ifdef MAGICK_HAS_EXCEPTIONS
    }
    catch (...)
    {
	// this catches any exceptions so that we free up the buffer
	Result.erase();
    }
#endif

    RET(Result);
    ETCB();
}

long mFile::Length() const
{
    BTCB();
    NFT("mFile::Length");

    int opres;

    MLOCK((lck_mFile, i_name));
    if (!IsOpened())
	RET(-1);
    long retval = 0;
    ACE_stat st;

    opres = ACE_OS::stat(i_name, &st);
    if (opres < 0 && errno)
    {
	LOG(LM_ERROR, "SYS_ERRORS/FILEOPERROR", ("fstat", i_name, errno, strerror(errno)));
    }
    else
    {
	retval = static_cast < long > (st.st_size);
    }
    RET(retval);
    ETCB();
}

mDateTime mFile::LastMod() const
{
    BTCB();
    NFT("mFile::LastMod");

    int opres;

    MLOCK((lck_mFile, i_name));
    if (!IsOpened())
	RET(0.0);
    mDateTime retval;
    ACE_stat st;

    opres = ACE_OS::stat(i_name, &st);
    if (opres < 0 && errno)
    {
	LOG(LM_ERROR, "SYS_ERRORS/FILEOPERROR", ("fstat", i_name, errno, strerror(errno)));
    }
    else
    {
	retval = static_cast < time_t > (ACE_Time_Value(st.st_mtime).sec());
    }
    RET(retval);
    ETCB();
}

bool mFile::Eof() const
{
    BTCB();
    NFT("mFile::Eof");
    MLOCK((lck_mFile, i_name));
    if (!IsOpened())
	RET(true);
    bool retval = (feof(fd) != 0);

    if (ferror(fd) && errno)
    {
	LOG(LM_ERROR, "SYS_ERRORS/FILEOPERROR", ("feof", i_name, errno, strerror(errno)));
    }
    RET(retval);
    ETCB();
}

void mFile::Attach(const mstring & name, FILE * in, const mstring & mode)
{
    BTCB();
    FT("mFile::Attach", (name, "(FILE *) in"));
    MLOCK((lck_mFile, name));
    if (in == NULL)
    {
	i_name.erase();
	i_mode.erase();
    }
    else
    {
	i_name = name;
	i_mode = mode;
    }
    fd = in;
    ETCB();
}

FILE *mFile::Detach()
{
    BTCB();
    NFT("mFile::Detach");
    MLOCK((lck_mFile, i_name));
    FILE *rfd = fd;

    fd = NULL;
    i_name.erase();
    i_mode.erase();
    NRET(FILE *, rfd);
    ETCB();
}

void mFile::Flush()
{
    BTCB();
    NFT("mFile::Flush");

    int opres;

    MLOCK((lck_mFile, i_name));
    if (IsOpened())
    {
	opres = ACE_OS::fflush(fd);
	if (opres < 0 && errno)
	{
	    LOG(LM_ERROR, "SYS_ERRORS/FILEOPERROR", ("fflush", i_name, errno, strerror(errno)));
	}
    }
    ETCB();
}

bool mFile::Exists(const mstring & name)
{
    BTCB();
    FT("mFile::Exists", (name));
    MLOCK((lck_mFile, name));
    FILE *file;

    if ((file = ACE_OS::fopen(name.c_str(), "r")) == NULL)
	RET(false);
    ACE_OS::fclose(file);
    RET(true);
    ETCB();
}

bool mFile::Erase(const mstring & name)
{
    BTCB();
    FT("mFile::Erase", (name));

    int opres;

    MLOCK((lck_mFile, name));
    if (!Exists(name))
	RET(false);
    opres = remove(name);
    if (opres < 0 && errno)
    {
	LOG(LM_ERROR, "SYS_ERRORS/FILEOPERROR", ("remove", name, errno, strerror(errno)));
    }
    RET(true);
    ETCB();
}

long mFile::Length(const mstring & name)
{
    BTCB();
    FT("mFile::Length", (name));

    int opres;

    MLOCK((lck_mFile, name));
    if (!Exists(name))
	RET(0);
    long retval = 0;
    ACE_stat st;

    opres = ACE_OS::stat(name.c_str(), &st);
    if (opres < 0 && errno)
    {
	LOG(LM_ERROR, "SYS_ERRORS/FILEOPERROR", ("stat", name, errno, strerror(errno)));
    }
    else
    {
	retval = static_cast < long > (st.st_size);
    }
    RET(retval);
    ETCB();
}

mDateTime mFile::LastMod(const mstring & name)
{
    BTCB();
    FT("mFile::LastMod", (name));

    int opres;

    MLOCK((lck_mFile, name));
    if (!Exists(name))
	RET(0.0);
    mDateTime retval;
    ACE_stat st;

    opres = ACE_OS::stat(name.c_str(), &st);
    if (opres < 0 && errno)
    {
	LOG(LM_ERROR, "SYS_ERRORS/FILEOPERROR", ("stat", name, errno, strerror(errno)));
    }
    else
    {
	retval = static_cast < time_t > (ACE_Time_Value(st.st_mtime).sec());
    }
    RET(retval);
    ETCB();
}

long mFile::Copy(const mstring & infile, const mstring & outfile, const bool append)
{
    BTCB();
    FT("mFile::Copy", (infile, outfile, append));

    if (infile.empty() || !Exists(infile) || outfile.empty())
	RET(0);
    MLOCK((lck_mFile, infile));
    MLOCK2((lck_mFile, outfile));
    mFile in(infile.c_str());
    mFile out(outfile.c_str(), append ? "ab" : "wb");

    if (!(in.IsOpened() && out.IsOpened()))
	RET(0);

    unsigned char c[65535];
    size_t total = 0, length = in.Length();

    do
    {
	size_t bytesread = in.Read(c, 65535);

	total += out.Write(c, bytesread);
    } while (total < length - 1);
    in.Close();
    out.Close();
    RET(total);
    ETCB();
}

// CANNOT trace this, it is used by TRACE code ...
long mFile::Dump(const vector < mstring > & invector, const mstring & outfile, const bool append, const bool endline)
{
    BTCB();
    FT("mFile::Dump", ("(vector<mstring>) invector", outfile, append, endline));
    if (!invector.size() || outfile.empty())
	RET(0);
    MLOCK((lck_mFile, outfile));
    mFile out(outfile.c_str(), append ? "ab" : "wb");

    if (!out.IsOpened())
	RET(0);

    size_t i, total = 0;

    for (i = 0; i < invector.size(); i++)
    {
	if (endline)
	{
	    total += out.Write((invector[i] + "\n").c_str(), invector[i].length() + 1);
	}
	else
	{
	    total += out.Write(invector[i].c_str(), invector[i].length());
	}
    }
    out.Close();
    RET(total);
    ETCB();
}

// CANNOT trace this, it is used by TRACE code ...
long mFile::Dump(const list < mstring > & inlist, const mstring & outfile, const bool append, const bool endline)
{
    BTCB();
    FT("mFile::Dump", ("(list<mstring>) inlist", outfile, append, endline));
    if (!inlist.size() || outfile.empty())
	RET(0);
    MLOCK((lck_mFile, outfile));
    mFile out(outfile.c_str(), append ? "ab" : "wb");

    if (!out.IsOpened())
	RET(0);

    size_t total = 0;

    list < mstring >::const_iterator iter;
    for (iter = inlist.begin(); iter != inlist.end(); iter++)
    {
	if (endline)
	{
	    total += out.Write((*iter + "\n").c_str(), iter->length() + 1);
	}
	else
	{
	    total += out.Write(iter->c_str(), iter->length());
	}
    }
    out.Close();
    RET(total);
    ETCB();
}

vector < mstring > mFile::UnDump(const mstring & infile)
{
    BTCB();
    FT("mFile::UnDump", (infile));
    vector < mstring > Result;

    if (infile.empty() || !Exists(infile))
	NRET(vector < mstring >, Result);
    MLOCK((lck_mFile, infile));
    mFile in(infile.c_str(), "r");

    if (!in.IsOpened())
	NRET(vector < mstring >, Result);

    mstring Line;

    Line = in.ReadLine();
    while (!in.Eof())
    {
	Result.push_back(Line);
	Line = in.ReadLine();
    }

    NRET(vector < mstring >, Result);
    ETCB();
}

size_t mFile::DirUsage(const mstring & directory)
{
    BTCB();
    FT("mFile::DirUsage", (directory));
    int opres;
    size_t retval = 0;

    if (!directory.length())
	RET(0);

    // Not supported on WIN32 until ACE 5.1.13 ...
#if (ACE_MAJOR_VERSION > 5 || (ACE_MAJOR_VERSION == 5 && \
	(ACE_MINOR_VERSION > 1 || (ACE_MINOR_VERSION == 1 && \
	ACE_BETA_VERSION >= 13)))) || !defined(WIN32)

    struct dirent *entry = NULL;
    ACE_DIR *dir = NULL;
    ACE_stat st;

    if ((dir = ACE_OS_Dirent::opendir(directory.c_str())) != NULL)
    {
	while ((entry = ACE_OS_Dirent::readdir(dir)) != NULL)
	{
	    if (strlen(entry->d_name))
	    {
		opres = ACE_OS::stat((directory + DirSlash + entry->d_name).c_str(), &st);
		if (opres < 0 && errno)
		{
		    LOG(LM_ERROR, "SYS_ERRORS/FILEOPERROR",
			("stat", directory + DirSlash + entry->d_name, errno, strerror(errno)));
		}
		else
		{
		    retval += st.st_size;
		}
	    }
	}
	ACE_OS_Dirent::closedir(dir);
    }
    else
    {
	LOG(LM_ERROR, "SYS_ERRORS/DIROPERROR", ("opendir", directory, errno, strerror(errno)));
    }
    RET(retval);
#endif
    RET(0);
    ETCB();
}

set < mstring > mFile::DirList(const mstring & directory, const mstring & filemask)
{
    BTCB();
    FT("mFile::DirList", (directory, filemask));
    set < mstring > retval;

    if (!directory.length())
	NRET(set < mstring >, retval);

    // Not supported on WIN32 until ACE 5.1.13 ...
#if (ACE_MAJOR_VERSION > 5 || (ACE_MAJOR_VERSION == 5 && \
	(ACE_MINOR_VERSION > 1 || (ACE_MINOR_VERSION == 1 && \
	ACE_BETA_VERSION >= 13)))) || !defined(WIN32)

    struct dirent *entry = NULL;
    ACE_DIR *dir = NULL;

    if ((dir = ACE_OS_Dirent::opendir(directory.c_str())) != NULL)
    {
	while ((entry = ACE_OS_Dirent::readdir(dir)) != NULL)
	{
	    if (strlen(entry->d_name) && mstring(entry->d_name).Matches(filemask))
	    {
		retval.insert(entry->d_name);
	    }
	}
	ACE_OS_Dirent::closedir(dir);
    }
    else
    {
	LOG(LM_ERROR, "SYS_ERRORS/DIROPERROR", ("opendir", directory, errno, strerror(errno)));
    }
#endif
    NRET(set < mstring >, retval);
    ETCB();
}

unsigned long FileMap::FindAvail(const FileMap::FileType type)
{
    BTCB();
    FT("FileMap::FindAvail", (static_cast < int > (type)));

    unsigned long filenum = 1;

    {
	RLOCK((lck_FileMap, static_cast < int > (type)));
	while (filenum > 0)	// Guarentee 8 digits
	{
	    if (i_FileMap[type].find(filenum) == i_FileMap[type].end())
	    {
		RET(filenum);
	    }
	    filenum++;
	}
    }

    LOG(LM_ERROR, "SYS_ERRORS/FILEMAPFULL", (static_cast < int > (type)));
    RET(0);
    ETCB();
}

bool FileMap::Exists(const FileMap::FileType type, const unsigned long num)
{
    BTCB();
    FT("FileMap::Exists", (static_cast < int > (type), num));

    mstring filename;

    filename.Format("%08x", num);

    if (type == MemoAttach)
	filename.prepend(Magick::instance().files.MemoAttach() + DirSlash);
    else if (type == Picture)
	filename.prepend(Magick::instance().files.Picture() + DirSlash);
    else if (type == Public)
	filename.prepend(Magick::instance().files.Public() + DirSlash);

    if (mFile::Exists(filename))
    {
	filemap_t::const_iterator fmi = i_FileMap.find(type);
	if (fmi != i_FileMap.end())
	{
	    RLOCK((lck_FileMap, static_cast < int > (type)));
	    if (fmi->second.find(num) != fmi->second.end())
	    {
		RET(true);
	    }
	}
	mFile::Erase(filename);
	LOG(LM_CRITICAL, "SYS_ERRORS/MISSING_FILE1", (static_cast < int > (type), fmstring("%08x", num)));
    }
    else
    {
	filemap_t::iterator fmi = i_FileMap.find(type);
	if (fmi != i_FileMap.end())
	{
	    WLOCK((lck_FileMap, static_cast < int > (type)));
	    if (fmi->second.find(num) != fmi->second.end())
	    {
		fmi->second.erase(num);
		LOG(LM_CRITICAL, "SYS_ERRORS/MISSING_FILE2", (static_cast < int > (type), fmstring("%08x", num)));
	    }
	}
    }
    RET(false);
    ETCB();
}

mstring FileMap::GetName(const FileMap::FileType type, const unsigned long num)
{
    BTCB();
    FT("FileMap::GetName", (static_cast < int > (type), num));

    if (Exists(type, num))
    {
	RLOCK((lck_FileMap, static_cast < int > (type), num));
	RET(i_FileMap[type] [num].first);
    }
    RET("");
    ETCB();
}

mstring FileMap::GetRealName(const FileMap::FileType type, const unsigned long num)
{
    BTCB();
    FT("FileMap::GetRealName", (static_cast < int > (type), num));

    if (Exists(type, num))
    {
	mstring filename;

	filename.Format("%08x", num);

	if (type == MemoAttach)
	    filename.prepend(Magick::instance().files.MemoAttach() + DirSlash);
	else if (type == Picture)
	    filename.prepend(Magick::instance().files.Picture() + DirSlash);
	else if (type == Public)
	    filename.prepend(Magick::instance().files.Public() + DirSlash);
	RET(filename);
    }
    RET("");
    ETCB();
}

mstring FileMap::GetPriv(const FileMap::FileType type, const unsigned long num)
{
    BTCB();
    FT("FileMap::GetPriv", (static_cast < int > (type), num));

    if (Exists(type, num))
    {
	RLOCK((lck_FileMap, static_cast < int > (type), num));
	RET(i_FileMap[type] [num].second);
    }
    RET("");
    ETCB();
}

bool FileMap::SetPriv(const FileMap::FileType type, const unsigned long num, const mstring & priv)
{
    BTCB();
    FT("FileMap::SetPriv", (static_cast < int > (type), num, priv));

    if (Exists(type, num))
    {
	WLOCK((lck_FileMap, static_cast < int > (type), num));
	i_FileMap[type] [num] = pair < mstring, mstring > (i_FileMap[type] [num].first, priv);
	RET(true);
    }
    RET(false);
    ETCB();
}

bool FileMap::Rename(const FileMap::FileType type, const unsigned long num, const mstring & newname)
{
    BTCB();
    FT("FileMap::SetPriv", (static_cast < int > (type), num, newname));

    if (type != Picture && Exists(type, num))
    {
	WLOCK((lck_FileMap, static_cast < int > (type), num));
	i_FileMap[type] [num] = pair < mstring, mstring > (newname, i_FileMap[type] [num].second);
	RET(true);
    }
    RET(false);
    ETCB();
}

size_t FileMap::GetSize(const FileMap::FileType type, const unsigned long num)
{
    BTCB();
    FT("FileMap::GetSize", (static_cast < int > (type), num));
    mstring filename = GetRealName(type, num);

    if (!filename.empty())
    {
	CP(("Checking file size of %s", filename.c_str()));
	long retval = mFile::Length(filename);

	if (retval < 0)
	    retval = 0;
	RET(retval);
    }
    RET(0);
    ETCB();
}

unsigned long FileMap::NewFile(const FileMap::FileType type, const mstring & filename, const mstring & priv)
{
    BTCB();
    FT("FileMap::NewFile", (static_cast < int > (type), filename, priv));

    unsigned long filenum = 0;

    if (!GetNum(type, filename))
    {
	filenum = FindAvail(type);
	if (filenum)
	{
	    WLOCK((lck_FileMap, static_cast < int > (type), filenum));
	    i_FileMap[type] [filenum] = pair < mstring, mstring > (filename, priv);
	}
    }

    RET(filenum);
    ETCB();
}

void FileMap::EraseFile(const FileType type, const unsigned long num)
{
    BTCB();
    FT("FileMap::EraseFile", (type, num));

    mstring filename = GetRealName(type, num);

    if (!filename.empty())
    {
	mFile::Erase(filename);
	WLOCK((lck_FileMap, static_cast < int > (type)));
	i_FileMap[type].erase(num);
    }
    ETCB();
}

vector < unsigned long > FileMap::GetList(const FileMap::FileType type, const mstring & source)
{
    BTCB();
    FT("FileMap::GetList", (static_cast < int > (type), source));
    vector < unsigned long > retval;
    map < unsigned long, pair < mstring, mstring > >::iterator iter;
    unsigned int i;

    if (i_FileMap.find(type) != i_FileMap.end())
    {
	RLOCK((lck_FileMap, static_cast < int > (type)));
	for (iter = i_FileMap[type].begin(); iter != i_FileMap[type].end(); iter++)
	{
	    if (!Exists(type, iter->first))
	    {
		// This will take care of itself.
	    }
	    else
	    {
		RLOCK2((lck_FileMap, static_cast < int > (type), iter->first));
		if (iter->second.second.empty())
		    retval.push_back(iter->first);
		else
		    for (i = 1; i <= iter->second.second.WordCount(" "); i++)
		    {
			if (Magick::instance().commserv.IsList(iter->second.second.ExtractWord(i, " ")) &&
			    Magick::instance().commserv.GetList(iter->second.second.ExtractWord(i, " ").UpperCase())->IsOn(source))
			{
			    retval.push_back(iter->first);
			    break;
			}
		    }
	    }
	}
    }
    NRET(vector < unsigned long >, retval);

    ETCB();
}

unsigned long FileMap::GetNum(const FileMap::FileType type, const mstring & name)
{
    BTCB();
    FT("FileMap::GetNum", (static_cast < int > (type), name));
    map < unsigned long, pair < mstring, mstring > >::iterator iter;

    filemap_t::iterator fmi = i_FileMap.find(type);
    if (fmi != i_FileMap.end())
    {
	RLOCK((lck_FileMap, static_cast < int > (type)));
	for (iter = fmi->second.begin(); iter != fmi->second.end(); iter++)
	{
	    RLOCK2((lck_FileMap, static_cast < int > (type), iter->first));
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
    ETCB();
}

size_t FileMap::FileSysSize(const FileMap::FileType type) const
{
    BTCB();
    FT("FileMap::FileSysSize", (static_cast < int > (type)));
    size_t retval = 0;

    if (type == MemoAttach)
	retval = mFile::DirUsage(Magick::instance().files.MemoAttach());
    else if (type == Picture)
	retval = mFile::DirUsage(Magick::instance().files.Picture());
    else if (type == Public)
	retval = mFile::DirUsage(Magick::instance().files.Public());

    RET(retval);
    ETCB();
}

SXP::Tag FileMap::tag_FileMap("FileMap");
SXP::Tag FileMap::tag_File("File");

void FileMap::BeginElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    BTCB();
    static_cast < void > (pIn);
    static_cast < void > (pElement);

    FT("FileMap::BeginElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    ETCB();
}

void FileMap::EndElement(SXP::IParser * pIn, SXP::IElement * pElement)
{
    BTCB();
    static_cast < void > (pIn);

    FT("FileMap::EndElement", ("(SXP::IParser *) pIn", "(SXP::IElement *) pElement"));
    // load up simple elements here. (ie single pieces of data)

    if (pElement->IsA(tag_File))
    {
	mstring *tmp = new mstring;

	fm_array.push_back(tmp);
	pElement->Retrieve(*tmp);
    }
    ETCB();
}

void FileMap::WriteElement(SXP::IOutStream * pOut, SXP::dict & attribs)
{
    BTCB();
    static_cast < void > (attribs);

    FT("FileMap::WriteElement", ("(SXP::IOutStream *) pOut", "(SXP::dict &) attribs"));
    // not sure if this is the right place to do this
    pOut->BeginObject(tag_FileMap);

    map < FileType, map < unsigned long, pair < mstring, mstring > > >::iterator i1;
    map < unsigned long, pair < mstring, mstring > >::iterator i2;
    mstring out;

    WLOCK((lck_FileMap));
    for (i1 = i_FileMap.begin(); i1 != i_FileMap.end(); i1++)
    {
	for (i2 = i1->second.begin(); i2 != i1->second.end(); i2++)
	{
	    out.erase();
	    out << static_cast < int >
		(i1->first) << "\n" << i2->first << "\n" << i2->second.first << "\n" << i2->second.second;

	    pOut->WriteElement(tag_File, out);
	}
    }

    pOut->EndObject(tag_FileMap);
    ETCB();
}

void FileMap::PostLoad()
{
    BTCB();
    NFT("FileMap::PostLoad");
    // Linkage, etc

    FileMap::FileType type;
    unsigned long number;
    mstring name;
    mstring priv;
    unsigned int i;

    for (i = 0; i < fm_array.size(); i++)
    {
	if (fm_array[i] != NULL)
	{
	    if (fm_array[i]->WordCount("\n") >= 3)
	    {
		type = static_cast < FileMap::FileType > (atoi(fm_array[i]->ExtractWord(1, "\n").c_str()));
		number = atoul(fm_array[i]->ExtractWord(2, "\n").c_str());
		name = fm_array[i]->ExtractWord(3, "\n");
		if (fm_array[i]->WordCount("\n") > 3)
		    priv = fm_array[i]->ExtractWord(4, "\n");
		else
		    priv.erase();
		i_FileMap[type] [number] = pair < mstring, mstring > (name, priv);
	    }
	    delete fm_array[i];
	}
    }
    fm_array.clear();
    ETCB();
}

DccXfer::DccXfer(const unsigned long dccid, const mSocket & sock, const mstring & mynick, const mstring & source,
		 const FileMap::FileType filetype, const unsigned long filenum) : i_Socket(sock), i_Source(source),
i_Mynick(mynick), i_Filename(Magick::instance().filesys.GetName(filetype, filenum)),
i_Blocksize(Magick::instance().files.Blocksize()), i_Type(Send), i_DccId(dccid)
{
    BTCB();
    FT("DccXfer::DccXfer", (dccid, "(mSocket *) sock", mynick, source, static_cast < int > (filetype), filenum));

    // Setup Paramaters
    i_Transiant = NULL;
//    i_Blocksize = Magick::instance().files.Blocksize();
    i_Tempfile.Format("%s%s%08x", Magick::instance().files.TempDir().c_str(), DirSlash.c_str(), i_DccId);
//    i_Filename = Magick::instance().filesys.GetName(filetype, filenum);

    // Verify Paramaters
    if (!Magick::instance().nickserv.IsLive(i_Source))
	return;
    if (i_Filename.empty())
    {
	SEND(mynick, source, "DCC/NOFILE", ("SEND"));
	return;
    }

    // Set 'Ready to Transfer'
    mstring tmp = Magick::instance().filesys.GetRealName(filetype, filenum);

    if (tmp.empty())
    {
	Magick::instance().filesys.EraseFile(filetype, filenum);
	SEND(mynick, source, "DCC/NOFILE", ("SEND"));
	return;
    }

    mFile::Copy(tmp, i_Tempfile);
    i_File.Open(i_Tempfile.c_str());
    i_Filesize = i_File.Length();
    if (i_Filesize <= 0)
    {
	i_File.Close();
	Magick::instance().filesys.EraseFile(filetype, filenum);
	SEND(mynick, source, "DCC/NOFILE", ("SEND"));
	return;
    }

    // Initialize Transfer
    i_Transiant = new unsigned char [i_Blocksize + 1];

    memset(i_Transiant, 0, i_Blocksize + 1);
    i_Total = 0;
    i_XferTotal = 0;
    i_LastData = mDateTime::CurrentDateTime();
    DumpE();
    LOG(LM_DEBUG, "OTHER/DCC_INIT", (fmstring("%08x", i_DccId), i_Source, "SEND"));
    CP(("DCC %d initialized", i_DccId));
    ETCB();
}

DccXfer::DccXfer(const unsigned long dccid, const mSocket & sock, const mstring & mynick, const mstring & source,
		 const mstring & filename, const size_t filesize, const size_t blocksize) : i_Socket(sock), i_Source(source),
i_Mynick(mynick), i_Filename(filename), i_Blocksize(Magick::instance().files.Blocksize()), i_Type(Get), i_DccId(dccid)
{
    BTCB();
    FT("DccXfer::DccXfer", (dccid, "(mSocket *) sock", mynick, source, filename, filesize, blocksize));

    // Setup Paramaters
    i_Transiant = NULL;
//    i_Blocksize = Magick::instance().files.Blocksize();
    if (blocksize > 0)
	i_Blocksize = blocksize;
    i_Tempfile.Format("%s%s%08x", Magick::instance().files.TempDir().c_str(), DirSlash.c_str(), i_DccId);
//    i_Filename = filename;

    // Verify Paramaters
    if (!Magick::instance().nickserv.IsLive(i_Source))
	return;

    map_entry < Nick_Live_t > nlive = Magick::instance().nickserv.GetLive(i_Source);
    if (nlive->InFlight.File() && !nlive->InFlight.InProg())
    {
	nlive->InFlight.SetInProg();
    }
    else
    {
	SEND(mynick, source, "DCC/NOREQUEST", ("GET"));
	return;
    }

    // Set 'Ready to Transfer'
    if (mFile::Exists(i_Tempfile))
	mFile::Erase(i_Tempfile);
    i_File.Open(i_Tempfile.c_str(), "wb");
    i_Filesize = filesize;

    // Initialize Transfer
    i_Transiant = new unsigned char [i_Blocksize + 1];

    memset(i_Transiant, 0, i_Blocksize + 1);
    i_Total = 0;
    i_XferTotal = 0;
    i_LastData = mDateTime::CurrentDateTime();
    DumpE();
    LOG(LM_DEBUG, "OTHER/DCC_INIT", (fmstring("%08x", i_DccId), i_Source, "GET"));
    CP(("DCC %d initialized", i_DccId));
    ETCB();
}

DccXfer::~DccXfer()
{
    BTCB();
    NFT("DccXfer::~DccXfer");

    if (i_Transiant != NULL)
	delete [] i_Transiant;
    i_Transiant = NULL;

    if (i_File.IsOpened())
	i_File.Close();

    if ((i_Filesize > 0) ? i_Total == i_Filesize : i_Total > 0)
    {
	LOG(LM_DEBUG, "OTHER/DCC_CLOSE", (fmstring("%08x", i_DccId)));
    }
    else if (i_DccId)
    {
	LOG(LM_DEBUG, "OTHER/DCC_CANCEL", (fmstring("%08x", i_DccId)));
    }

    if (!i_Socket.IsConnected())
    {
	return;
    }
    else
	i_Socket.close();

    CP(("DCC Xfer #%d Completed", i_DccId));
    // If we know the size, verify it, else we take
    // what we get!

    map_entry < Nick_Live_t > nlive;
    if (Magick::instance().nickserv.IsLiveAll(i_Source))
	nlive = Magick::instance().nickserv.GetLive(i_Source);

    if (i_Type == Get && nlive.entry() != NULL && nlive->InFlight.File() && nlive->InFlight.InProg())
    {
	if ((i_Filesize > 0) ? i_Total == i_Filesize : i_Total > 0)
	{
	    mstring tmp;

	    FileMap::FileType filetype = FileMap::Unknown;
	    if (nlive->InFlight.Memo())
		filetype = FileMap::MemoAttach;
	    else if (nlive->InFlight.Picture())
		filetype = FileMap::Picture;
	    else if (nlive->InFlight.Public())
		filetype = FileMap::Public;
	    unsigned long filenum = Magick::instance().filesys.NewFile(filetype, i_Filename);

	    if (filenum)
	    {
		if (filetype == FileMap::MemoAttach)
		    tmp.Format("%s%s%08x", Magick::instance().files.MemoAttach().c_str(), DirSlash.c_str(), filenum);
		else if (filetype == FileMap::Picture)
		    tmp.Format("%s%s%08x", Magick::instance().files.Picture().c_str(), DirSlash.c_str(), filenum);
		else if (filetype == FileMap::Public)
		    tmp.Format("%s%s%08x", Magick::instance().files.Public().c_str(), DirSlash.c_str(), filenum);

		if (mFile::Exists(i_Tempfile))
		{
		    mFile::Copy(i_Tempfile, tmp);
		    nlive->InFlight.File(filenum);
		    CP(("Added entry %d to FileMap", filenum));
		}
		else
		    nlive->InFlight.File(0);
	    }
	    else
		nlive->InFlight.File(0);
	}
	else
	    nlive->InFlight.File(0);
    }

    if (mFile::Exists(i_Tempfile))
	mFile::Erase(i_Tempfile);
    ETCB();
}

DccXfer &DccXfer::operator=(const DccXfer & in)
{
    BTCB();
    FT("DccXfer::operator=", ("(const DccXfer &) in"));

    // i_File=in.i_File;
    i_Source = in.i_Source;
    i_Mynick = in.i_Mynick;
    i_Tempfile = in.i_Tempfile;
    i_Filename = in.i_Filename;
    i_Blocksize = in.i_Blocksize;
    i_XferTotal = in.i_XferTotal;
    i_Total = in.i_Total;
    i_Filesize = in.i_Filesize;
    i_Type = in.i_Type;
    i_DccId = in.i_DccId;
    i_Socket = in.i_Socket;

    if (in.i_File.IsOpened())
    {
	in.i_File.Close();
	if (i_Type == Get)
	{
	    i_File.Open(i_Tempfile.c_str(), "ab");
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
    if (in.i_Transiant != NULL && in.i_XferTotal)
    {
	i_Transiant = new unsigned char [i_Blocksize + 1];

	memset(i_Transiant, 0, i_Blocksize + 1);
	memcpy(i_Transiant, in.i_Transiant, i_XferTotal);
    }
    i_LastData = in.i_LastData;
    NRET(DccXfer &, *this);
    ETCB();
}

bool DccXfer::Ready() const
{
    BTCB();
    NFT("DccXfer::Ready");
    RLOCK((lck_DccMap, lck_xfers, i_DccId, "i_File"));
    bool retval = i_File.IsOpened();

    RET(retval);
    ETCB();
}

DccXfer::XF_Type DccXfer::Type() const
{
    BTCB();
    NFT("DccXfer::Type");
    RLOCK((lck_DccMap, lck_xfers, i_DccId, "i_Type"));
    RET(i_Type);
    ETCB();
}

mstring DccXfer::Mynick() const
{
    BTCB();
    NFT("DccXfer::Mynick");
    RLOCK((lck_DccMap, lck_xfers, i_DccId, "i_Mynick"));
    RET(i_Mynick);
    ETCB();
}

mstring DccXfer::Source() const
{
    BTCB();
    NFT("DccXfer::Source");
    RLOCK((lck_DccMap, lck_xfers, i_DccId, "i_Source"));
    RET(i_Source);
    ETCB();
}

mstring DccXfer::Filename() const
{
    BTCB();
    NFT("DccXfer::Filename");
    RLOCK((lck_DccMap, lck_xfers, i_DccId, "i_Filename"));
    RET(i_Filename);
    ETCB();
}

size_t DccXfer::Filesize() const
{
    BTCB();
    NFT("DccXfer::Filesize");
    RLOCK((lck_DccMap, lck_xfers, i_DccId, "i_Filesize"));
    RET(i_Filesize);
    ETCB();
}

size_t DccXfer::Total() const
{
    BTCB();
    NFT("DccXfer::Total");
    RLOCK((lck_DccMap, lck_xfers, i_DccId, "i_Total"));
    RET(i_Total);
    ETCB();
}

mDateTime DccXfer::LastData() const
{
    BTCB();
    NFT("DccXfer::LastData");
    RLOCK((lck_DccMap, lck_xfers, i_DccId, "i_LastData"));
    RET(i_LastData);
    ETCB();
}

void DccXfer::ChgNick(const mstring & in)
{
    BTCB();
    FT("DccXfer::ChgNick", (in));
    WLOCK((lck_DccMap, lck_xfers, i_DccId, "i_Source"));
    MCB(i_Source);
    i_Source = in;
    MCE(i_Source);
    ETCB();
}

void DccXfer::Cancel()
{
    BTCB();
    NFT("DccXfer::Cancel");
    RLOCK((lck_DccMap, lck_xfers, i_DccId, "i_Source"));
    WLOCK((lck_DccMap, lck_xfers, i_DccId, "i_Total"));
    WLOCK2((lck_DccMap, lck_xfers, i_DccId, "i_File"));
    if (Magick::instance().nickserv.IsLiveAll(i_Source))
	Magick::instance().nickserv.GetLive(i_Source)->InFlight.Cancel();
    MCB(i_Total);
    CB(1, i_File.Length());
    i_Total = 0;
    i_File.Close();
    CE(1, i_File.Length());
    MCE(i_Total);
    ETCB();
}

void DccXfer::Action()
{
    BTCB();
    NFT("DccXfer::Action");
    long XferAmt = 0, TranSz = 0;
    unsigned long verify;

    WLOCK((lck_DccMap, lck_xfers, i_DccId));
    DumpB();
    if (i_Type == Get)
    {
	COM(("Executing action for DCC %d GET", i_DccId));
	XferAmt = 0;
	if (i_Traffic.size() &&
	    (Magick::instance().files.Max_Speed() == 0 || Average() <= Magick::instance().files.Max_Speed()))
	{
	    XferAmt = i_Socket.recv(reinterpret_cast < void * > (&i_Transiant[i_XferTotal]), i_Blocksize - i_XferTotal, 1);

	    COM(("%d: Bytes Transferred - %d, RECV Response %d (%s)", i_DccId, XferAmt, i_Socket.Last_Error(),
		 i_Socket.Last_Error_String().c_str()));
	}
	// Traffic Accounting ...
	map < time_t, size_t >::iterator iter;
	time_t now = time(NULL);

	for (iter = i_Traffic.begin();
	     iter != i_Traffic.end() && iter->first < now - static_cast < time_t > (Magick::instance().files.Sampletime() + 2);
	     iter = i_Traffic.begin())
	    i_Traffic.erase(iter->first);
	if (i_Traffic.find(now) == i_Traffic.end())
	    i_Traffic[now] = 0;
	i_Traffic[now] += XferAmt;
	if (XferAmt > 0)
	{
	    i_XferTotal += XferAmt;
	    i_LastData = mDateTime::CurrentDateTime();
	    if ((i_Filesize > 0 && i_Filesize == i_Total + i_XferTotal) || i_XferTotal == i_Blocksize)
	    {
		i_Total += i_XferTotal;
		if (!i_File.IsOpened())
		    return;
		if (i_File.Write(i_Transiant, i_XferTotal) < 1)
		{
		    SEND(i_Mynick, i_Source, "DCC/FAILED", ("GET"));
		    i_File.Close();
		}
		else
		{
		    i_XferTotal = 0;
		    memset(i_Transiant, 0, i_Blocksize);
		    verify = htonl(i_Total);
		    XferAmt = i_Socket.send(reinterpret_cast < void * > (&verify), 4, 1);

		    COM(("%d: Bytes Transferred - %d, SEND Response %d (%s)", i_DccId, XferAmt, i_Socket.Last_Error(),
			 i_Socket.Last_Error_String().c_str()));
		    if (i_Filesize == i_Total)
		    {
			SEND(i_Mynick, i_Source, "DCC/COMPLETED", ("GET", i_Total));
			i_File.Close();
		    }
		}
	    }
	}
	else
	{
	    switch (i_Socket.Last_Error())
	    {
	    case 0:
	    case EINTR:	// Interrupted System Call
	    case EAGAIN:	// Temporarily Unavailable
	    case ENOMEM:	// Not Enough Memory
	    case ETIME:	// Request Timed Out
	    case EINPROGRESS:	// Operation In Progress
		break;
	    default:
		SEND(i_Mynick, i_Source, "DCC/SOCKERR", ("GET", i_Socket.Last_Error(), i_Socket.Last_Error_String()));
		i_File.Close();
	    }
	}
	if (i_File.IsOpened() && i_Traffic.size() > Magick::instance().files.Sampletime() &&
	    Average() < Magick::instance().files.Min_Speed())
	{
	    SEND(i_Mynick, i_Source, "DCC/TOOSLOW", ("GET"));
	    i_File.Close();
	}
    }
    else if (i_Type == Send)
    {
	COM(("Executing action for DCC %d SEND", i_DccId));
	XferAmt = 0;
	if (i_Traffic.size() &&
	    (Magick::instance().files.Max_Speed() == 0 || Average() <= Magick::instance().files.Max_Speed()))
	{
	    if (i_XferTotal == i_Blocksize)
	    {
		XferAmt = i_Socket.recv(reinterpret_cast < void * > (&verify), 4, 1);

		COM(("%d: Bytes Transferred - %d, RECV Response %d (%s)", i_DccId, XferAmt, i_Socket.Last_Error(),
		     i_Socket.Last_Error_String().c_str()));
		if (XferAmt <= 0 || ntohl(verify) != i_Total)
		{
		    switch (i_Socket.Last_Error())
		    {
		    case 0:
		    case EINTR:	// Interrupted System Call
		    case EAGAIN:	// Temporarily Unavailable
		    case ENOMEM:	// Not Enough Memory
		    case ETIME:	// Request Timed Out
		    case EINPROGRESS:	// Operation In Progress
			break;
		    default:
			SEND(i_Mynick, i_Source, "DCC/SOCKERR", ("SEND", i_Socket.Last_Error(), i_Socket.Last_Error_String()));
			i_File.Close();
			return;
		    }
		}
		i_Total += i_XferTotal;
		i_XferTotal = 0;
	    }
	    if (!i_XferTotal && i_Total < i_Filesize)
	    {
		memset(i_Transiant, 0, i_Blocksize);
		if (!i_File.IsOpened())
		    return;
		if (i_Total + i_Blocksize > i_Filesize)
		    TranSz = i_File.Read(i_Transiant, i_Filesize - i_Total);
		else
		    TranSz = i_File.Read(i_Transiant, i_Blocksize);
		if (TranSz < 0)
		{
		    SEND(i_Mynick, i_Source, "DCC/FAILED", ("SEND"));
		    i_File.Close();
		    return;
		}
		i_XferTotal = 0;
	    }
	    CP(("Going to send %d bytes ...", TranSz));
	    XferAmt = i_Socket.send(reinterpret_cast < void * > (&i_Transiant[i_XferTotal]), TranSz - i_XferTotal, 1);
	    COM(("%d: Bytes Transferred - %d, SEND Response %d (%s)", i_DccId, XferAmt, i_Socket.Last_Error(),
		 i_Socket.Last_Error_String().c_str()));
	}
	// Traffic Accounting ...
	map < time_t, size_t >::iterator iter;
	time_t now = time(NULL);

	for (iter = i_Traffic.begin();
	     iter != i_Traffic.end() && iter->first < now - static_cast < time_t > (Magick::instance().files.Sampletime() + 2);
	     iter = i_Traffic.begin())
	    i_Traffic.erase(iter->first);
	if (i_Traffic.find(now) == i_Traffic.end())
	    i_Traffic[now] = 0;
	i_Traffic[now] += XferAmt;
	if (XferAmt > 0)
	{
	    i_XferTotal += XferAmt;
	    i_LastData = mDateTime::CurrentDateTime();
	    if (i_Filesize == i_Total + i_XferTotal)
	    {
		i_Total += i_XferTotal;
		SEND(i_Mynick, i_Source, "DCC/COMPLETED", ("SEND", i_Total));
		i_File.Close();
	    }
	}
	else
	{
	    switch (i_Socket.Last_Error())
	    {
	    case 0:
	    case EINTR:	// Interrupted System Call
	    case EAGAIN:	// Temporarily Unavailable
	    case ENOMEM:	// Not Enough Memory
	    case ETIME:	// Request Timed Out
	    case EINPROGRESS:	// Operation In Progress
		break;
	    default:
		SEND(i_Mynick, i_Source, "DCC/SOCKERR", ("SEND", i_Socket.Last_Error(), i_Socket.Last_Error_String()));
		i_File.Close();
	    }
	}
	if (i_File.IsOpened() && i_Traffic.size() > Magick::instance().files.Sampletime() &&
	    Average() < Magick::instance().files.Min_Speed())
	{
	    SEND(i_Mynick, i_Source, "DCC/TOOSLOW", ("GET"));
	    i_File.Close();
	}
    }
    DumpE();
    ETCB();
}

size_t DccXfer::Average(time_t secs) const
{
    BTCB();
    FT("DccXfer::Average", (secs));
    time_t now = time(NULL);
    size_t total = 0;
    int i = 0;

    map < time_t, size_t >::const_iterator iter;
    if (secs > static_cast < time_t > (Magick::instance().files.Sampletime()))
	secs = 0;

    RLOCK((lck_DccMap, lck_xfers, i_DccId, "i_Traffic"));
    for (iter = i_Traffic.begin(); iter != i_Traffic.end() && iter->first < now; iter++)
    {
	if (secs ? iter->first >= (now - 1) - secs : 1)
	{
	    total += iter->second;
	    i++;
	}
    }
    RET(total / (i ? i : 1));
    ETCB();
}

size_t DccXfer::Usage() const
{
    BTCB();
    size_t retval = 0;

    WLOCK((lck_DccMap, lck_xfers, i_DccId));
    retval += i_Source.capacity();
    retval += i_Mynick.capacity();
    retval += i_Tempfile.capacity();
    retval += i_Filename.capacity();
    retval += sizeof(i_Blocksize);
    retval += sizeof(i_XferTotal);
    retval += sizeof(i_Total);
    retval += sizeof(i_Filesize);
    retval += sizeof(i_Type);
    retval += sizeof(i_DccId);
    retval += sizeof(i_Transiant);
    retval += sizeof(i_LastData.Internal());

    map < time_t, size_t >::const_iterator iter;
    for (iter = i_Traffic.begin(); iter != i_Traffic.end(); iter++)
    {
	retval += sizeof(iter->first);
	retval += sizeof(iter->second);
    }

    return retval;
    ETCB();
}

void DccXfer::DumpB() const
{
    BTCB();
    MB(0,
       (i_Socket.Last_Error(), i_File.Length(), i_Source, i_Mynick, i_Tempfile, i_Filename, i_Blocksize, i_XferTotal, i_Total,
	i_Filesize, i_Type, i_DccId, i_Transiant, i_LastData, i_Traffic.size()));
    ETCB();
}

void DccXfer::DumpE() const
{
    BTCB();
    ME(0,
       (i_Socket.Last_Error(), i_File.Length(), i_Source, i_Mynick, i_Tempfile, i_Filename, i_Blocksize, i_XferTotal, i_Total,
	i_Filesize, i_Type, i_DccId, i_Transiant, i_LastData, i_Traffic.size()));
    ETCB();
}

int DccMap::open(void *in)
{
    BTCB();
    FT("DccMap::open", ("(void *) in"));
    magick_instance = reinterpret_cast < Magick * > (in);
    int retval = activate();

    RET(retval);
    ETCB();
}

int DccMap::close(const unsigned long in)
{
    BTCB();
    static_cast < void > (in);

    FT("DccMap::close", (in));

    // dump all and close open file handles.
    DccMap::xfers_t::iterator iter;
    RLOCK((lck_DccMap, lck_xfers));
    for (iter = XfersBegin(); iter != XfersEnd(); iter++)
    {
	RLOCK2((lck_DccMap, lck_xfers, iter->first));
	if (iter->second != NULL)
	{
	    SEND(iter->second->Mynick(), iter->second->Source(), "DCC/FAILED",
		 ((iter->second->Type() == DccXfer::Get) ? "GET" : "SEND"));
	    iter->second->Cancel();
	}
    }
    WLOCK((lck_DccMap, lck_xfers));
    for (iter = XfersBegin(); XfersSize(); iter = XfersBegin())
	RemXfers(iter->first);

    RET(0);
    ETCB();
}

int DccMap::svc(void)
{
    BTCB();
    mThread::Attach(tt_MAIN);
    Magick::register_instance(magick_instance);
    NFT("DccMap::svc");
    Magick::instance().hh.AddThread(Heartbeat_Handler::H_DCC);

    unsigned long WorkId;

    while (!Magick::instance().Shutdown())
    {
	/*COM(("Active Size is %d", active.size())); */

	Magick::instance().hh.Heartbeat();
	if (!active.size() || Magick::instance().Pause())
	{
	    ACE_OS::sleep(1);
	    continue;
	}

	{
	    WLOCK((lck_DccMap, "active"));
	    WorkId = active.front();
	    active.pop();
	}

#ifdef MAGICK_HAS_EXCEPTIONS
	try
	{
#endif
	    if (!IsXfers(WorkId))
		continue;
#ifdef MAGICK_HAS_EXCEPTIONS
	}
	catch (E_DccMap_Xfers & e)
	{
	    switch (e.type())
	    {
	    case E_DccMap_Xfers::T_Blank:
	    case E_DccMap_Xfers::T_Invalid:
		RemXfers(WorkId);
		break;
	    default:
		throw;
	    }
	    continue;
	}
#endif
	RLOCK((lck_DccMap, lck_xfers, WorkId));
	DccXfer &dcc = GetXfers(WorkId);

	if (!dcc.Ready())
	{
	    RemXfers(WorkId);
	    continue;
	}

	CP(("Executing ACTION for DCC #%d", WorkId));
	// Only do an action IF they have not exceeded the max speed
	dcc.Action();
	// Below LOW SPEED threshold OR
	// No data in X seconds...
	if (dcc.LastData().SecondsSince() > Magick::instance().files.Timeout())
	{
	    SEND(dcc.Mynick(), dcc.Source(), "DCC/TIMEOUT", ((dcc.Type() == DccXfer::Get) ? "GET" : "SEND"));
	    dcc.Cancel();
	    RemXfers(WorkId);
	    continue;
	}
	{
	    WLOCK((lck_DccMap, "active"));
	    active.push(WorkId);
	}
	FLUSH();		// Force TRACE output dump
    }
    Magick::instance().hh.RemoveThread();
    Magick::deregister_instance();
    DRET(0);
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
void DccMap::AddXfers(DccXfer * in) throw (E_DccMap_Xfers)
#else
void DccMap::AddXfers(DccXfer * in)
#endif
{
    BTCB();
    FT("DccMap::AddXfers", ("(DccXfer *) in"));

    if (in == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_DccMap_Xfers(E_DccMap_Xfers::W_Add, E_DccMap_Xfers::T_Invalid));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Nick", "Xfers", "Add", "Invalid"));
	return;
#endif
    }
    if (!in->DccId())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_DccMap_Xfers(E_DccMap_Xfers::W_Add, E_DccMap_Xfers::T_Blank));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Nick", "Xfers", "Add", "Blank"));
	return;
#endif
    }

    WLOCK((lck_DccMap, lck_xfers));
    xfers[in->DccId()] = in;
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
DccXfer &DccMap::GetXfers(const unsigned long in) throw (E_DccMap_Xfers)
#else
DccXfer &DccMap::GetXfers(const unsigned long in)
#endif
{
    BTCB();
    FT("DccMap::GetXfers", (in));

    RLOCK((lck_DccMap, lck_xfers, in));
    DccMap::xfers_t::iterator iter = xfers.find(in);
    if (iter == xfers.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_DccMap_Xfers(E_DccMap_Xfers::W_Get, E_DccMap_Xfers::T_NotFound));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("DccMap", "Xfers", "Get", "NotFound"));
	NRET(DccXfer &, GLOB_DccXfer);
#endif
    }
    if (iter->second == NULL)
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_DccMap_Xfers(E_DccMap_Xfers::W_Get, E_DccMap_Xfers::T_Invalid));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("DccMap", "Xfers", "Get", "Invalid"));
	NRET(DccXfer &, GLOB_DccXfer);
#endif
    }
    if (!iter->second->DccId())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_DccMap_Xfers(E_DccMap_Xfers::W_Get, E_DccMap_Xfers::T_Blank));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("DccMap", "Xfers", "Get", "Blank"));
	NRET(DccXfer &, GLOB_DccXfer);
#endif
    }

    NRET(DccXfer &, const_cast < DccXfer & > (*iter->second));
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
void DccMap::RemXfers(const unsigned long in) throw (E_DccMap_Xfers)
#else
void DccMap::RemXfers(const unsigned long in)
#endif
{
    BTCB();
    FT("DccMap::RemXfers", (in));

    WLOCK((lck_DccMap, lck_xfers));
    DccMap::xfers_t::iterator iter = xfers.find(in);
    if (iter == xfers.end())
    {
#ifdef MAGICK_HAS_EXCEPTIONS
	throw (E_DccMap_Xfers(E_DccMap_Xfers::W_Rem, E_DccMap_Xfers::T_NotFound));
#else
	LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("Nick", "Xfers", "Rem", "NotFound"));
	return;
#endif
    }
    WLOCK2((lck_DccMap, lck_xfers, iter->first));
    if (iter->second != NULL)
    {
	delete iter->second;
    }
    xfers.erase(iter);
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
bool DccMap::IsXfers(const unsigned long in) throw (E_DccMap_Xfers)
#else
bool DccMap::IsXfers(const unsigned long in)
#endif
{
    BTCB();
    FT("DccMap::IsXfers", (in));

    RLOCK((lck_DccMap, lck_xfers, in));
    DccMap::xfers_t::const_iterator iter = xfers.find(in);
    if (iter != xfers.end())
    {
	if (iter->second == NULL)
	{
#ifdef MAGICK_HAS_EXCEPTIONS
	    throw (E_DccMap_Xfers(E_DccMap_Xfers::W_Get, E_DccMap_Xfers::T_Invalid));
#else
	    LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("DccMap", "Xfers", "Get", "Invalid"));
	    RET(false);
#endif
	}
	if (!iter->second->DccId())
	{
#ifdef MAGICK_HAS_EXCEPTIONS
	    throw (E_DccMap_Xfers(E_DccMap_Xfers::W_Get, E_DccMap_Xfers::T_Blank));
#else
	    LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("DccMap", "Xfers", "Get", "Blank"));
	    RET(false);
#endif
	}
	RET(true);
    }
    RET(false);
    ETCB();
}

#ifdef MAGICK_HAS_EXCEPTIONS
vector < unsigned long > DccMap::GetList(const mstring & in) throw (E_DccMap_Xfers)
#else
vector < unsigned long > DccMap::GetList(const mstring & in)
#endif
{
    BTCB();
    FT("DccMap::GetList", (in));
    vector < unsigned long > retval;

    DccMap::xfers_t::const_iterator iter;
    RLOCK((lck_DccMap, lck_xfers));
    for (iter = xfers.begin(); iter != xfers.end(); iter++)
    {
	RLOCK2((lck_DccMap, lck_xfers, iter->first));
	if (iter->second == NULL)
	{
#ifdef MAGICK_HAS_EXCEPTIONS
	    throw (E_DccMap_Xfers(E_DccMap_Xfers::W_Get, E_DccMap_Xfers::T_Invalid));
#else
	    LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("DccMap", "Xfers", "Get", "Invalid"));
	    continue;
#endif
	}
	if (!iter->second->DccId())
	{
#ifdef MAGICK_HAS_EXCEPTIONS
	    throw (E_DccMap_Xfers(E_DccMap_Xfers::W_Get, E_DccMap_Xfers::T_Blank));
#else
	    LOG(LM_CRITICAL, "EXCEPTIONS/GENERIC", ("DccMap", "Xfers", "Get", "Blank"));
	    continue;
#endif
	}
	if (iter->second->Source().IsSameAs(in, true))
	    retval.push_back(iter->first);
    }
    NRET(vector < unsigned long >, retval);

    ETCB();
}

void *DccMap::Connect2(void *in)
{
    BTCB();
    mThread::Attach(tt_MAIN);
    FT("DccMap::Connect2", ("(void *) in"));

    NewSocket *val = reinterpret_cast < NewSocket * > (in);

    Magick::register_instance(val->magick_instance);

    mSocket DCC_SOCK(val->address, Magick::instance().files.Timeout());

    if (DCC_SOCK.IsConnected())
    {
	unsigned long WorkId;

	xfers_t::iterator iter;
	DCC_SOCK.Resolve(S_DCCFile, val->source);
	{
	    RLOCK((lck_DccMap, lck_xfers));
	    for (iter = XfersBegin(), WorkId = 1; iter != XfersEnd(); iter++, WorkId++)
	    {
		if (WorkId < iter->first)
		    break;
	    }
	}
	if (WorkId)
	{
	    AddXfers(new DccXfer(WorkId, DCC_SOCK, val->mynick, val->source, val->filename, val->filesize, val->blocksize));
	    {
		WLOCK((lck_DccMap, "active"));
		active.push(WorkId);
	    }
	    CP(("Created DCC entry #%d", WorkId));
	}
	else
	    SEND(val->mynick, val->source, "DCC/FAILED", ("GET"));
    }
    else
	SEND(val->mynick, val->source, "DCC/NOCONNECT", ("GET"));

    if (val != NULL)
	delete val;

    Magick::deregister_instance();
    DRET(0);
    ETCB();
}

void *DccMap::Accept2(void *in)
{
    BTCB();
    mThread::Attach(tt_MAIN);
    FT("DccMap::Accept2", ("(void *) in"));

    NewSocket *val = reinterpret_cast < NewSocket * > (in);

    Magick::register_instance(val->magick_instance);

    mSocket DCC_SOCK(val->port, Magick::instance().files.Timeout());

    if (DCC_SOCK.IsConnected())
    {
	unsigned long WorkId;

	xfers_t::iterator iter;
	DCC_SOCK.Resolve(S_DCCFile, val->source);
	{
	    RLOCK((lck_DccMap, lck_xfers));
	    for (iter = XfersBegin(), WorkId = 1; iter != XfersEnd(); iter++, WorkId++)
	    {
		if (WorkId < iter->first)
		    break;
	    }
	}
	if (WorkId)
	{
	    AddXfers(new DccXfer(WorkId, DCC_SOCK, val->mynick, val->source, val->filetype, val->filenum));
	    {
		WLOCK((lck_DccMap, "active"));
		active.push(WorkId);
	    }
	    CP(("Created DCC entry #%d", WorkId));
	}
	else
	    SEND(val->mynick, val->source, "DCC/FAILED", ("SEND"));
    }
    else
	SEND(val->mynick, val->source, "DCC/NOCONNECT", ("SEND"));

    if (val != NULL)
	delete val;

    Magick::deregister_instance();
    DRET(0);
    ETCB();
}

void DccMap::Connect(const ACE_INET_Addr & address, const mstring & mynick, const mstring & source, const mstring & filename,
		     const size_t filesize, const size_t blocksize)
{
    BTCB();
    FT("DccMap::Connect", ("(ACE_INET_Addr) address", mynick, source, filename, filesize, blocksize));
    NewSocket *tmp = new NewSocket;

    tmp->magick_instance = & Magick::instance();
    tmp->address = address;
    tmp->source = source;
    tmp->mynick = mynick;
    tmp->filename = filename;
    tmp->filesize = filesize;
    tmp->blocksize = blocksize;

    thr_mgr()->spawn(Connect2, reinterpret_cast < void * > (tmp), THR_NEW_LWP | THR_DETACHED);
    ETCB();
}

void DccMap::Accept(const unsigned short port, const mstring & mynick, const mstring & source,
		    const FileMap::FileType filetype, const unsigned long filenum)
{
    BTCB();
    FT("DccMap::Accept", (port, mynick, source, static_cast < int > (filetype), filenum));
    NewSocket *tmp = new NewSocket;

    tmp->magick_instance = & Magick::instance();
    tmp->port = port;
    tmp->source = source;
    tmp->mynick = mynick;
    tmp->filetype = filetype;
    tmp->filenum = filenum;

    thr_mgr()->spawn(Accept2, reinterpret_cast < void * > (tmp), THR_NEW_LWP | THR_DETACHED);
    ETCB();
}

void DccMap::Cancel(const unsigned long DccId, const bool silent)
{
    BTCB();
    FT("DccMap::Cancel", (DccId, silent));

#ifdef MAGICK_HAS_EXCEPTIONS
    try
    {
#endif
	if (IsXfers(DccId))
	{
	    RLOCK((lck_DccMap, lck_xfers, DccId));
	    DccXfer &dcc = GetXfers(DccId);

	    if (!silent)
		SEND(dcc.Mynick(), dcc.Source(), "DCC/FAILED", ((dcc.Type() == DccXfer::Get) ? "GET" : "SEND"));
	    dcc.Cancel();
	    RemXfers(DccId);
	}
#ifdef MAGICK_HAS_EXCEPTIONS
    }
    catch (E_DccMap_Xfers & e)
    {
	switch (e.type())
	{
	case E_DccMap_Xfers::T_Blank:
	case E_DccMap_Xfers::T_Invalid:
	    RemXfers(DccId);
	    break;
	default:
	    throw;
	}
    }
#endif
    thr_mgr()->cancel_all();
    ETCB();
}
