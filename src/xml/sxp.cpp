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
** Revision 1.35  2002/01/14 07:19:59  prez
** Fixed delete[] calls
**
** Revision 1.34  2002/01/14 07:16:56  prez
** More pretty printing with a newer indent with C++ fixes (not totally done)
**
** Revision 1.33  2002/01/13 05:18:42  prez
** More formatting, changed style slightly
**
** Revision 1.32  2002/01/12 14:42:09  prez
** Pretty-printed all code ... looking at implementing an auto-prettyprint.
**
** Revision 1.31  2001/12/30 11:53:08  prez
** Added thread safety for STL to pch.h, and started using ACE to do it.
**
** Revision 1.30  2001/12/27 04:54:46  prez
** Converted SXP to not use STL strings, use mstring instead.
**
** Revision 1.29  2001/12/26 23:30:35  prez
** More fixes to see if I can fix the memory leak ...
**
** Revision 1.28  2001/12/25 05:57:28  prez
** Updated SXP and EXPAT -- untested, but should work.
**
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

namespace SXP
{

    dict blank_dict;
      pair < mstring, mstring > blank_mstring_pair;

    TagHashtable *g_pHashTable = 0;

    // add the tag to the table right on construction
      Tag::Tag(const char *name)
    {
	ch = name;
	dw = ~0;
	TagHashtable::TagHT().Add(*this);
    }

//      void CFileOutStream::WriteSubElement(IPersistObj *pObj, dict& attribs) {
//              pObj->WriteElement(this, attribs);
//      }

    const char *IElement::Name()
    {
	return 0;
    }
    const char *IElement::Attrib(const char *attrName)
    {
	return 0;
    }

    int IElement::IsA(const char *name)
    {
	return 0;
    }
    int IElement::IsA(Tag & t)
    {
	return 0;
    }
    int IElement::AttribIs(const char *attrName, const char *val)
    {
	return 0;
    }

    ErrCode RemapError(int expat_err)
    {
	switch (expat_err)
	{
	case XML_ERROR_NONE:
	    return err_no_error;
	case XML_ERROR_NO_MEMORY:
	    return err_no_memory;
	case XML_ERROR_SYNTAX:
	    return err_syntax;
	case XML_ERROR_NO_ELEMENTS:
	    return err_no_elements;
	case XML_ERROR_INVALID_TOKEN:
	    return err_invalid_token;
	case XML_ERROR_UNCLOSED_TOKEN:
	    return err_unclosed_token;
	case XML_ERROR_PARTIAL_CHAR:
	    return err_partial_char;
	case XML_ERROR_TAG_MISMATCH:
	    return err_tag_mismatch;
	case XML_ERROR_DUPLICATE_ATTRIBUTE:
	    return err_duplicate_attribute;
	case XML_ERROR_JUNK_AFTER_DOC_ELEMENT:
	    return err_junk_after_doc_element;
	case XML_ERROR_PARAM_ENTITY_REF:
	    return err_param_entity_ref;
	case XML_ERROR_UNDEFINED_ENTITY:
	    return err_undefined_entity;
	case XML_ERROR_RECURSIVE_ENTITY_REF:
	    return err_recursive_entity_ref;
	case XML_ERROR_ASYNC_ENTITY:
	    return err_async_entity;
	case XML_ERROR_BAD_CHAR_REF:
	    return err_bad_char_ref;
	case XML_ERROR_BINARY_ENTITY_REF:
	    return err_binary_entity_ref;
	case XML_ERROR_ATTRIBUTE_EXTERNAL_ENTITY_REF:
	    return err_attribute_external_entity_ref;
	case XML_ERROR_MISPLACED_XML_PI:
	    return err_misplaced_xml_pi;
	case XML_ERROR_UNKNOWN_ENCODING:
	    return err_unknown_encoding;
	case XML_ERROR_INCORRECT_ENCODING:
	    return err_incorrect_encoding;
	case XML_ERROR_UNCLOSED_CDATA_SECTION:
	    return err_unclosed_cdata_section;
	case XML_ERROR_EXTERNAL_ENTITY_HANDLING:
	    return err_external_entity_handling;
	case XML_ERROR_NOT_STANDALONE:
	    return err_not_standalone;
	default:
	    return err_unknown;
	}
    }

    IData::~IData()
    {
    }

    // escape a char string - remove &<>" and replace with escape codes
    mstring XMLEscape(const char *pstr)
    {
	const char *p;
	mstring ret;

	for (p = pstr; *p; p++)
	{
	    switch (*p)
	    {
	    case '&':
		ret.append("&amp;");
		break;
	    case '<':
		ret.append("&lt;");
		break;
	    case '>':
		ret.append("&gt;");
		break;
	    case '"':
		ret.append("&quot;");
		break;
	    default:
		if (*p < 32 || *p > 126)
		{
		    mstring tmp;
		    tmp.Format("&#%d;", static_cast < unsigned char > (*p));

		    ret.append(tmp);
		}
		else
		    ret.append(*p);
	    }
	}
	return ret;
    }

    mstring XMLUnEscape(const char *pstr)
    {
	const char *p;
	mstring ret;

	for (p = pstr; *p; p++)
	{
	    if (*p != '&')
	    {
		ret.append(*p);
	    }
	    else
	    {
		if (strcmp(p + 1, "amp;") == 0)
		{
		    p += 4;
		    ret.append('&');
		}
		else if (strcmp(p + 1, "lt;") == 0)
		{
		    p += 3;
		    ret.append('<');
		}
		else if (strcmp(p + 1, "gt;") == 0)
		{
		    p += 3;
		    ret.append('>');
		}
		else if (strcmp(p + 1, "quot;") == 0)
		{
		    p += 5;
		    ret.append('"');
		}
		else if (strncmp(p + 1, "#", 1) == 0)
		{
		    p += 2;
		    char tmp[4];

		    memset(tmp, 0, 4);
		    for (short i = 0; *p != ';' && i < 4; p++, i++)
			tmp[i] = * p;
		    if (strlen(tmp))
			ret.append(static_cast < char > (atoi(tmp)));
		}
		else
		{
		    ret.append('&');
		}
	    }
	}
	return ret;
    }

    // the mighty parser itself
    CParser::CParser(IPersistObj * pRoot)
    {
	m_parser = XML_ParserCreate(0);
//              XML_SetUnknownEncodingHandler(m_parser, CParser::UEHandler, 0);
	XML_SetUserData(m_parser, (void *) this);
	XML_SetElementHandler(m_parser, CParser::StaticStartElement, CParser::StaticEndElement);
	XML_SetCharacterDataHandler(m_parser, CParser::StaticCharData);
	ReadTo(pRoot);
	m_bShuttingDown = 0;
    }

    // give the parser food for thought
    ErrCode CParser::Feed(const char *pData, int nLen, int bFinal)
    {
	if (m_parser)
	{
	    int i;

	    if ((i = XML_Parse(m_parser, pData, nLen, bFinal)))
	    {
		return err_no_error;
	    }
	    else
	    {
		XML_Error e = XML_GetErrorCode(m_parser);

		m_nErrorLine = XML_GetCurrentLineNumber(m_parser);
		m_nErrorCol = XML_GetCurrentColumnNumber(m_parser);
		m_strError = XML_ErrorString(e);
		return RemapError(e);
	    }
	}
	else
	{
	    return err_no_error;
	}
    }

    // give the parser a food for thought the lazy way
    ErrCode CParser::FeedFile(char *pchFilename)
    {
	char buf[8192];
	FILE *fp;
	int done;
	ErrCode err = err_no_error;

	m_strError = "No error";
	m_nErrorLine = -1;
	m_nErrorCol = -1;

	fp = fopen((const char *) pchFilename, "rt");
	if (NULL == fp)
	{
	    // free already created parser
	    XML_ParserFree(m_parser);
	    m_parser = 0;

	    m_strError = "Can\'t open file";
	    m_nErrorLine = -1;
	    m_nErrorCol = -1;
	    return err_cant_open_file;
	}

	do
	{
	    size_t len = fread(buf, 1, sizeof(buf), fp);

	    done = len < sizeof(buf);
	    err = Feed(buf, len, done);
	    done = done || m_bShuttingDown || !m_parser || (err != err_no_error);
	} while (!done);

	fclose(fp);

	// free the parsers and the stacks
	DoShutdown();

	return err;
    }

    // these need to be static so they can be registered as callbacks
    // with expat

    void CParser::StaticStartElement(void *puserData, const XML_Char * name, const XML_Char ** atts)
    {
	CParser *pThis = (CParser *) puserData;

	if (!pThis->m_bShuttingDown)
	{
	    pThis->StartElement(name, atts);
	}
    }
    void CParser::StaticEndElement(void *puserData, const XML_Char * name)
    {
	CParser *pThis = (CParser *) puserData;

	if (!pThis->m_bShuttingDown)
	{
	    pThis->EndElement(name);
	}
    }
    void CParser::StaticCharData(void *puserData, const XML_Char * data, int len)
    {
	CParser *pThis = (CParser *) puserData;

	if (!pThis->m_bShuttingDown)
	{
	    pThis->CharData(data, len);
	}
    }

    int CParser::UEHandler(void *pData, const char *pchName, XML_Encoding * info)
    {
	if (strcmp(pchName, "sxp_encoding") == 0)
	{
	    for (int i = 0; i < 256; i++)
	    {
		info->map[i] = i;
	    }
	    info->convert = 0;
	    info->release = 0;
	    return 1;
	}
	else
	{
	    return 0;
	}
    }

    // closing tag for the root object element encountered, stop parsing
    // DoShutdown cannot be called inside a handler call initiated by
    // expat, so actual shutdown is deferred until the next iteration of
    // the feed loop; however, events are NOT received

    void CParser::DoShutdown()
    {
	if (m_parser != 0)
	    XML_ParserFree(m_parser);
	while (!m_EHStack.empty())
	{
	    //delete m_EHStack.top(); 
	    m_EHStack.pop();
	}
	while (!m_EStack.empty())
	{
	    delete m_EStack.top();

	    m_EStack.pop();
	}
	m_parser = 0;
    }

    void MOutStream::ExpandBuf()
    {
	char *newbuf = new char [buf_sz + INIT_BUFSIZE];

	memcpy(newbuf, buffer, buf_sz);
	memset(&newbuf[buf_sz], 0, INIT_BUFSIZE);
	buf_sz += INIT_BUFSIZE;

	if (buffer != NULL)
	    delete[] buffer;
	buffer = newbuf;
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
	buf_cnt += tmp.length();
    }

    void MOutStream::Indent()
    {
	while (buf_cnt + m_nIndent >= buf_sz)
	    ExpandBuf();
	for (int i = 0; i < m_nIndent; i++)
	    buffer[buf_cnt++] = '\t';
    }

    MOutStream::MOutStream() : m_nIndent(0), buf_sz(INIT_BUFSIZE), buf_cnt(0)
    {
	buffer = new char [buf_sz];

	memset(buffer, 0, buf_sz);
    }

    MOutStream::~MOutStream()
    {
	if (buffer != NULL)
	    delete[] buffer;
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
	buf_cnt += tmp.length();
    }

    void MOutStream::BeginObject(Tag & t, dict & attribs)
    {
	Indent();
	m_nIndent++;
	mstring tmp;

	tmp.Format("<%s", t.ch);
	if (buf_cnt + tmp.length() >= buf_sz)
	    ExpandBuf();
	ACE_OS::strcpy(&buffer[buf_cnt], tmp.c_str());
	buf_cnt += tmp.length();
	for (dict::iterator i = attribs.begin(); i != attribs.end(); i++)
	{
	    tmp = "";
	    tmp.Format(" %s=\"%s\"", (*i).first.c_str(), (*i).second.c_str());
	    if (buf_cnt + tmp.length() >= buf_sz)
		ExpandBuf();
	    ACE_OS::strcpy(&buffer[buf_cnt], tmp.c_str());
	    buf_cnt += tmp.length();
	}
	tmp = ">\n";
	if (buf_cnt + tmp.length() >= buf_sz)
	    ExpandBuf();
	ACE_OS::strcpy(&buffer[buf_cnt], tmp.c_str());
	buf_cnt += tmp.length();
    }

    void MOutStream::EndObject(Tag & t)
    {
	m_nIndent--;
	Indent();
	mstring tmp;

	tmp.Format("</%s>\n", t.ch);
	if (buf_cnt + tmp.length() >= buf_sz)
	    ExpandBuf();
	ACE_OS::strcpy(&buffer[buf_cnt], tmp.c_str());
	buf_cnt += tmp.length();
    }

    void MOutStream::WriteSubElement(IPersistObj * pObj, dict & attribs)
    {
	pObj->WriteElement(this, attribs);
    }
};
