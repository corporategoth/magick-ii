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
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.11  2000/08/06 05:27:48  prez
** Fixed akill, and a few other minor bugs.  Also made trace TOTALLY optional,
** and infact disabled by default due to it interfering everywhere.
**
** Revision 1.10  2000/07/29 21:58:55  prez
** Fixed XML loading of weird characters ...
** 2 known bugs now, 1) last_seen dates are loaded incorrectly on alot
** of nicknames, which means we expire lots of nicknames.  2) services
** wont rejoin a +i/+k channel when last user exits.
**
** Revision 1.9  2000/07/21 01:10:17  prez
** Fixed the dbase corruption checking ...
**
** Revision 1.8  2000/07/21 00:18:50  prez
** Fixed database loading, we can now load AND save databases...
**
** Almost ready to release now :)
**
** Revision 1.7  2000/07/11 13:22:19  prez
** Fixed loading/saving -- they now work with encryption and compression.
** Tested, it works too!  Now all we need to do is fix the loading, and
** we're set ... :))
**
** Revision 1.6  2000/05/20 00:08:02  ungod
** getting ConfigEngine compiling and cleaning up SXP to stop circular includes of "datetime.h"
**
** Revision 1.5  2000/05/03 14:12:23  prez
** Added 'public' filesystem, ie. the ability to add
** arbitary files for download via. servmsg (sops may
** upload/download, and set the committees who can
** grab the file).
**
** Revision 1.4  2000/04/30 05:05:32  ungod
** fix prez's well meaning fuckup. ie *leave pch.h in there*,
** we need it for mdatetime support.
**
** Revision 1.3  2000/04/30 03:48:30  prez
** Replaced all system calls with ACE_OS equivilants,
** also removed any dependancy on ACE from sxp (xml)
**
** Revision 1.2  2000/04/15 11:11:45  ungod
** starting xmlage of magick
**
** Revision 1.89  2000/03/24 15:35:18  prez
** Fixed establishment of DCC transfers, and some other misc stuff
** (eg. small bug in trace, etc).  Still will not send or receive
** any data through DCC tho (will time out, but not receive data,
** error 14 - "Bad Access" -- to be investigated).
**
** Revision 1.88  2000/03/19 08:50:56  prez
** More Borlandization -- Added WHAT project, and fixed a bunch
** of minor warnings that appear in borland.
**
** Revision 1.87  2000/03/08 23:38:37  prez
** Added LIVE to nickserv/chanserv, added help funcitonality to all other
** services, and a bunch of other small changes (token name changes, etc)
**
** Revision 1.86  2000/02/27 03:58:40  prez
** Fixed the WHAT program, also removed RegEx from Magick.
**
** Revision 1.85  2000/02/23 12:21:04  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.84  2000/02/17 12:55:07  ungod
** still working on borlandization
**
** Revision 1.83  2000/02/16 12:59:41  ungod
** fixing for borland compilability
**
** Revision 1.82  2000/02/15 13:27:04  prez
** *** empty log message ***
**
** Revision 1.81  2000/02/15 10:37:51  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */
#include "xml/sxp.h"
#include "utils.h"

SXP_NS_BEGIN

//std::string TagHashtable::table[SXP_HTSIZE];
TagHashtable *g_pHashTable = 0;

// add the tag to the table right on construction
Tag::Tag(const char *name) 
{
	ch = name;
	dw = ~0;
	TagHashtable::TagHT().Add(*this);
}

/*void CFileOutStream::WriteSubElement(IPersistObj *pObj, dict& attribs)
{
	pObj->WriteElement(this, attribs);
} */


void MFileOutStream::ExpandBuf()
{
    buffer = (char *) ACE_OS::realloc(buffer, sizeof(char) * buf_sz * 2);
    ACE_OS::memset(&buffer[buf_sz], 0, buf_sz);
    buf_sz *= 2;
}


void MFileOutStream::Print(char *format, ...)
{
    va_list argptr;
    va_start(argptr, format);
    PrintV(format, argptr);
    va_end(argptr);
}

void MFileOutStream::PrintV(char *format, va_list argptr)
{
    mstring tmp;
    tmp.FormatV(format, argptr);
    if (buf_cnt + tmp.Len() >= buf_sz)
	ExpandBuf();
    ACE_OS::strcpy(&buffer[buf_cnt], tmp.c_str());
    buf_cnt+=tmp.Len();
}

void MFileOutStream::Indent()
{
	for(int i=0; i<m_nIndent; i++)
	{
	    if (buf_cnt+1 >= buf_sz)
		ExpandBuf();
	    buffer[buf_cnt] = '\t';
	    buf_cnt++;
	}
}

MFileOutStream::MFileOutStream(mstring chFilename, int comp, mstring ikey)
{
	out.Open(chFilename, "w");
	filename = chFilename;
	m_nIndent = 0;
	compress = comp;
	key = ikey;
	buf_sz = INIT_BUFSIZE;
	buf_cnt = 0;
	buffer = (char *) ACE_OS::malloc(sizeof(char) * buf_sz);
	ACE_OS::memset(buffer, 0, sizeof(char) * buf_sz);
}

MFileOutStream::MFileOutStream(mstring chFilename, FILE *fp, int comp, mstring ikey)
{
	out.Attach(chFilename, fp);
	filename = chFilename;
	m_nIndent = 0;
	compress = comp;
	key = ikey;
	buf_sz = INIT_BUFSIZE;
	buf_cnt = 0;
	buffer = (char *) ACE_OS::malloc(sizeof(char) * buf_sz);
	ACE_OS::memset(buffer, 0, sizeof(char) * buf_sz);
}

MFileOutStream::~MFileOutStream()
{
	unsigned char tag = SXP_TAG;
	char *outbuf = NULL;
	size_t new_sz = 0, attempt, length=buf_cnt;
	if (!out.IsOpened())
	    return;
	if (compress)
	{
	    z_streamp strm = new z_stream_s;
	    ACE_OS::memset(strm, 0, sizeof(z_stream_s));
	    strm->zalloc = Z_NULL;
	    strm->zfree  = Z_NULL;
	    strm->opaque = Z_NULL;

	    // From the zlib documentation:
	    // avail_out must be at least 0.1% larger
	    // than avail_in plus 12 bytes.
	    new_sz = (size_t) (length * 1.001 + 12);
	    deflateInit(strm, compress);
	    outbuf = (char *) ACE_OS::malloc(sizeof(char) * new_sz);
	    ACE_OS::memset(outbuf, 0, sizeof(char) * new_sz);

	    strm->next_in   = (unsigned char *) buffer;
	    strm->avail_in  = length;
	    strm->total_in  = 0;

	    strm->next_out  = (unsigned char *) outbuf;
	    strm->avail_out = new_sz;
	    strm->total_out = 0;

	    for (attempt=0; attempt < 10 &&
		deflate(strm, Z_FINISH) != Z_STREAM_END;
		attempt++)
	    {
		new_sz += 12;
		if (outbuf != NULL)
		    ACE_OS::free(outbuf);
		outbuf = (char *) ACE_OS::malloc(sizeof(char) * new_sz);
		ACE_OS::memset(outbuf, 0, sizeof(char) * new_sz);
		strm->next_out = (unsigned char *) outbuf;
	    }
	    deflateEnd(strm);
	    if (strm != NULL)
		delete strm;
	    if (attempt < 10)
	    {
		length = strm->total_out;
		buf_sz = length + 1;
		if (buffer != NULL)
		    ACE_OS::free(buffer);
		buffer = (char *) ACE_OS::malloc(sizeof(char) * buf_sz);
		ACE_OS::memset(buffer, 0, sizeof(char) * buf_sz);
		memcpy(buffer, outbuf, length);
		tag |= SXP_COMPRESS;
	    }
	    if (outbuf != NULL)
		ACE_OS::free(outbuf);
	    outbuf = NULL;
	    
	}
	if (key != "")
	{
	    des_key_schedule key1, key2;
	    des_cblock ckey1, ckey2;

	    new_sz = length + 8;
	    outbuf = (char *) ACE_OS::malloc(sizeof(char) * new_sz);
	    ACE_OS::memset(outbuf, 0, sizeof(char) * new_sz);

	    des_string_to_2keys((char *)key.c_str(), &ckey1, &ckey2);
	    des_set_key(&ckey1, key1);
	    des_set_key(&ckey2, key2);

	    mDES((unsigned char *) buffer, (unsigned char *) outbuf,
	    				length, key1, key2, 1);

	    if (outbuf != NULL)
	    {
		length = new_sz;
		while (outbuf[length-1]==0)
		    length--;
		if (buffer != NULL)
		    ACE_OS::free(buffer);
		buffer = outbuf;
		outbuf = NULL;
	    }
	    tag |= SXP_ENCRYPT;
	}
	
	if (buffer != NULL)
	{
	    out.Write(mstring(tag), false);
	    out.Write(buffer, length * sizeof(char));
	    ACE_OS::free(buffer);
	}
	out.Close();
}

void MFileOutStream::BeginXML(void)
{
	// UTF-8 encoding is used because it allows relatively painless
	// support for storing widechars as character data, via
	// conversion functions in IElement::Retrieve() and 
	// IOutStream::WriteElement
	mstring tmp = XML_STRING;
	if (buf_cnt + strlen(XML_STRING) >= buf_sz)
	    ExpandBuf();
	ACE_OS::strcpy(&buffer[buf_cnt], XML_STRING);
	buf_cnt+=strlen(XML_STRING);
}

void MFileOutStream::BeginObject(Tag& t, dict& attribs)
{
	Indent(); m_nIndent++;
	mstring tmp;
	tmp.Format("<%s", t.ch);
	if (buf_cnt + tmp.Len() >= buf_sz)
	    ExpandBuf();
	ACE_OS::strcpy(&buffer[buf_cnt], tmp.c_str());
	buf_cnt+=tmp.Len();
	for(dict::iterator i=attribs.begin(); i!=attribs.end(); i++) {
		tmp = "";
		tmp.Format(" %s=\"%s\"",
			(*i).first.c_str(),
			(*i).second.c_str() );
		if (buf_cnt + tmp.Len() >= buf_sz)
		    ExpandBuf();
		ACE_OS::strcpy(&buffer[buf_cnt], tmp.c_str());
		buf_cnt+=tmp.Len();
	}
	tmp = ">\n";
	if (buf_cnt + tmp.Len() >= buf_sz)
	    ExpandBuf();
	ACE_OS::strcpy(&buffer[buf_cnt], tmp.c_str());
	buf_cnt+=tmp.Len();
}

void MFileOutStream::EndObject  (Tag& t)
{
	m_nIndent--;
	Indent();
	mstring tmp;
	tmp.Format("</%s>\n", t.ch);
	if (buf_cnt + tmp.Len() >= buf_sz)
	    ExpandBuf();
	ACE_OS::strcpy(&buffer[buf_cnt], tmp.c_str());
	buf_cnt+=tmp.Len();
}

void MFileOutStream::WriteSubElement(IPersistObj *pObj, dict& attribs)
{
	pObj->WriteElement(this, attribs);
}


int CParser::FeedFile(mstring chFilename, mstring ikey)
{
    int retval = 0;
    mFile in(chFilename);
    if (in.IsOpened())
    {
	long filesize = in.Length(), new_sz;
	unsigned char tag = 0;
	char *tmpbuf, *buffer = (char *) ACE_OS::malloc(filesize * sizeof(char));
	ACE_OS::memset(buffer, 0, filesize * sizeof(char));
	new_sz = filesize + 8;

	in.Read(&tag, sizeof(unsigned char));
	ACE_OS::memset(buffer, 0, filesize);
	in.Read(buffer, (filesize-1) * sizeof(char));
	in.Close();

	if (tag & SXP_TAG)
	{
	    if ((tag & SXP_ENCRYPT) && ikey != "")
	    {
		des_key_schedule key1, key2;
		des_cblock ckey1, ckey2;

		tmpbuf = (char *) ACE_OS::malloc(new_sz * sizeof(char));
		ACE_OS::memset(tmpbuf, 0, new_sz * sizeof(char));
		des_string_to_2keys((char *)ikey.c_str(), &ckey1, &ckey2);
		des_set_key(&ckey1, key1);
		des_set_key(&ckey2, key2);

		mDES((unsigned char *) buffer, (unsigned char *) tmpbuf,
	    				filesize-1, key1, key2, 0);

		if (tmpbuf != NULL)
		{
		    if (buffer != NULL)
			ACE_OS::free(buffer);
		    buffer = tmpbuf;
		    tmpbuf = NULL;
		}
	    }
	    if (tag & SXP_COMPRESS)
	    {
		int bufsize = 1024, index = 0;
		z_streamp strm = new z_stream_s;
		ACE_OS::memset(strm, 0, sizeof(z_stream_s));

		strm->zalloc = Z_NULL;
		strm->zfree  = Z_NULL;
		strm->opaque = Z_NULL;
		inflateInit(strm);

		tmpbuf = (char *) ACE_OS::malloc(bufsize * sizeof(char));
		ACE_OS::memset(tmpbuf, 0, bufsize * sizeof(char));
		strm->next_in   = (unsigned char *) buffer;
		strm->avail_in  = new_sz;
		strm->total_in  = 0;

		strm->next_out  = (unsigned char *) &tmpbuf[index];
		strm->avail_out = bufsize;
		strm->total_out = 0;

		while ((retval = inflate(strm, Z_NO_FLUSH)) != Z_STREAM_END)
		{
		    if (retval == Z_OK && strm->avail_out == 0)
		    {
			index = bufsize;
			bufsize *= 2;
			tmpbuf = (char *) ACE_OS::realloc(tmpbuf, bufsize * sizeof(char));
			ACE_OS::memset(&tmpbuf[index], 0, index);
			strm->next_out  = (unsigned char *) &tmpbuf[index];
			strm->avail_out = index;
		    }
		}
		if (retval == Z_STREAM_END || retval == Z_OK)
		{
		    if (buffer)
			ACE_OS::free(buffer);
		    new_sz = strm->total_out;
		    buffer = (char *) ACE_OS::malloc(sizeof(char) * new_sz+1);
		    ACE_OS::memset(buffer, 0, sizeof(char) * new_sz+1);
		    memcpy(buffer, tmpbuf, new_sz);
		    if (tmpbuf != NULL)
			ACE_OS::free(tmpbuf);
		    tmpbuf = NULL;
		}
		inflateEnd(strm);
		if (strm != NULL)
		    delete strm;
	    }
	    if (strncmp(buffer, XML_STRING, strlen(XML_STRING))==0)
	    {
		retval = Feed(buffer, new_sz);
		if (retval == 0)
		{
		    Shutdown();
		    retval = 1;
		}
		if( m_bShuttingDown )
		    DoShutdown();
	    }
	}
	if (buffer != NULL)
	    ACE_OS::free(buffer);
    }
    return retval;
}

SXP_NS_END

