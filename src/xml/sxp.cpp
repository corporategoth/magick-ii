#include "pch.h"
#ifdef WIN32
  #pragma hdrstop
#else
  #pragma implementation
#endif

/*  Magick IRC Services
**
** (c) 1997-2001 Preston Elder <prez@magick.tm>
** (c) 1998-2001 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any
** circumstances, however it may be added to if any
** modifications are made to this file.  All modified
** code must be clearly documented and labelled.
**
** ========================================================== */
#define RCSID(x,y) const char *rcsid_sxp_cpp_ ## x () { return y; }
RCSID(sxp_cpp, "@(#)$Id$");
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.27  2001/11/12 01:05:03  prez
** Added new warning flags, and changed code to reduce watnings ...
**
** Revision 1.26  2001/11/05 05:10:20  prez
** Removed winbase.h from sxp.h ;P
**
** Revision 1.25  2001/11/05 04:19:18  prez
** more windows tuning, now it actually works without silly runtime dependancies
**
** Revision 1.24  2001/06/15 07:20:41  prez
** Fixed windows compiling -- now works with MS Visual Studio 6.0
**
** Revision 1.23  2001/06/02 16:27:04  prez
** Intergrated the staging system for dbase loading/saving.
**
** Revision 1.22  2001/05/17 19:18:55  prez
** Added ability to chose GETPASS or SETPASS.
**
** Revision 1.21  2001/05/14 04:46:32  prez
** Changed to use 3BF (3 * blowfish) encryption.  DES removed totally.
**
** Revision 1.20  2001/05/13 23:46:28  prez
** got rid of temp veriable, and revised decryption failure condition
**
** Revision 1.19  2001/05/13 18:45:15  prez
** Fixed up the keyfile validation bug, and added more error reporting to
** the db load (also made sure it did not hang on certain circumstances).
**
** Revision 1.18  2001/03/04 02:04:15  prez
** Made mstring a little more succinct ... and added vector/list operations
**
** Revision 1.17  2001/03/02 05:24:42  prez
** HEAPS of modifications, including synching up my own archive.
**
** Revision 1.16  2001/02/03 02:21:36  prez
** Loads of changes, including adding ALLOW to ini file, cleaning up
** the includes, RCSID, and much more.  Also cleaned up most warnings.
**
** Revision 1.15  2001/01/01 05:32:45  prez
** Updated copywrights.  Added 'reversed help' syntax (so ACCESS HELP ==
** HELP ACCESS).
**
** Revision 1.14  2000/10/10 11:47:53  prez
** mstring is re-written totally ... find or occurances
** or something has a problem, but we can debug that :)
**
** Revision 1.13  2000/09/13 12:45:34  prez
** Added intergration of mpatrol (memory leak finder).  Default is set OFF,
** must enable with --enable-mpatrol in configure (and have mpatrol in system).
**
** Revision 1.12  2000/08/08 09:58:56  prez
** Added ModeO to 4 pre-defined committees.
** Also added back some deletes in xml in the hope that it
** will free up some memory ...
**
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

dict blank_dict;
pair<mstring,mstring> blank_mstring_pair;

//std::string TagHashtable::table[SXP_HTSIZE];
TagHashtable *g_pHashTable = 0;

// add the tag to the table right on construction
Tag::Tag(const char *name) 
{
	ch = name;
	dw = ~0;
	TagHashtable::TagHT().Add(*this);
}

/* void CFileOutStream::WriteSubElement(IPersistObj *pObj, dict& attribs)
{
	pObj->WriteElement(this, attribs);
} */


void MOutStream::ExpandBuf()
{
    buffer = static_cast<char *>(realloc(buffer, sizeof(char) * (buf_sz + INIT_BUFSIZE)));
    memset(&buffer[buf_sz], 0, INIT_BUFSIZE);
    buf_sz += INIT_BUFSIZE;
}


void MOutStream::Print(const char *format, ...)
{
    va_list argptr;
    va_start(argptr, format);
    PrintV(format, argptr);
    va_end(argptr);
}

void MOutStream::PrintV(const char *format, va_list argptr)
{
    mstring tmp;
    tmp.FormatV(format, argptr);
    if (buf_cnt + tmp.length() >= buf_sz)
	ExpandBuf();
    ACE_OS::strcpy(&buffer[buf_cnt], tmp.c_str());
    buf_cnt+=tmp.length();
}

void MOutStream::Indent()
{
	for(int i=0; i<m_nIndent; i++)
	{
	    if (buf_cnt+1 >= buf_sz)
		ExpandBuf();
	    buffer[buf_cnt] = '\t';
	    buf_cnt++;
	}
}

MOutStream::MOutStream()
	: m_nIndent(0), buf_sz(INIT_BUFSIZE), buf_cnt(0)
{
	buffer = static_cast<char *>(malloc(sizeof(char) * buf_sz));
	memset(buffer, 0, sizeof(char) * buf_sz);
}

MOutStream::~MOutStream()
{
	if (buffer != NULL)
	    free(buffer);
}

void MOutStream::BeginXML(void)
{
	// UTF-8 encoding is used because it allows relatively painless
	// support for storing widechars as character data, via
	// conversion functions in IElement::Retrieve() and 
	// IOutStream::WriteElement
	mstring tmp(XML_STRING);
	if (buf_cnt + tmp.length() >= buf_sz)
	    ExpandBuf();
	ACE_OS::strcpy(&buffer[buf_cnt], tmp.c_str());
	buf_cnt+=tmp.length();
}

void MOutStream::BeginObject(Tag& t, dict& attribs)
{
	Indent(); m_nIndent++;
	mstring tmp;
	tmp.Format("<%s", t.ch);
	if (buf_cnt + tmp.length() >= buf_sz)
	    ExpandBuf();
	ACE_OS::strcpy(&buffer[buf_cnt], tmp.c_str());
	buf_cnt+=tmp.length();
	for(dict::iterator i=attribs.begin(); i!=attribs.end(); i++) {
		tmp = "";
		tmp.Format(" %s=\"%s\"",
			(*i).first.c_str(),
			(*i).second.c_str() );
		if (buf_cnt + tmp.length() >= buf_sz)
		    ExpandBuf();
		ACE_OS::strcpy(&buffer[buf_cnt], tmp.c_str());
		buf_cnt+=tmp.length();
	}
	tmp = ">\n";
	if (buf_cnt + tmp.length() >= buf_sz)
	    ExpandBuf();
	ACE_OS::strcpy(&buffer[buf_cnt], tmp.c_str());
	buf_cnt+=tmp.length();
}

void MOutStream::EndObject  (Tag& t)
{
	m_nIndent--;
	Indent();
	mstring tmp;
	tmp.Format("</%s>\n", t.ch);
	if (buf_cnt + tmp.length() >= buf_sz)
	    ExpandBuf();
	ACE_OS::strcpy(&buffer[buf_cnt], tmp.c_str());
	buf_cnt+=tmp.length();
}

void MOutStream::WriteSubElement(IPersistObj *pObj, dict& attribs)
{
	pObj->WriteElement(this, attribs);
}


int CParser::Feed(const char *pData, int nLen, int bFinal) {
    if( m_parser )
    {
	int retval = XML_Parse(m_parser, pData, nLen, bFinal);
	if (retval <= 0)
	    Shutdown();
	if( m_bShuttingDown )
	    DoShutdown();
	return retval;
    }
    else
	return 0;
}

SXP_NS_END

