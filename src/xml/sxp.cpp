#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
#endif

/* Magick IRC Services
**
** (c) 1997-2003 Preston Elder <prez@magick.tm>
** (c) 1998-2003 William King <ungod@magick.tm>
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
#define RCSID(x,y) const char *rcsid_sxp_cpp_ ## x () { return y; }
RCSID(sxp_cpp, "@(#)$Id$");

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

    const char *IElement::Name()
    {
	return "";
    }
    const char *IElement::Attrib(const char *attrName)
    {
	static_cast<void>(attrName);
	return "";
    }

    bool IElement::IsA(const char *name)
    {
	static_cast<void>(name);
	return false;
    }
    bool IElement::IsA(Tag & t)
    {
	static_cast<void>(t);
	return false;
    }
    bool IElement::AttribIs(const char *attrName, const char *val)
    {
	static_cast<void>(attrName);
	static_cast<void>(val);
	return false;
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
		else if (strncmp(p + 1, "asc", 3) == 0)
		{
		    p += 4;
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
	static_cast<void>(pData);
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
	    delete [] buffer;
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
	    delete [] buffer;
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
