#ifndef WIN32
#pragma interface
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
#ifndef _SXP_H
#define _SXP_H
#include "pch.h"
RCSID(sxp_h, "@(#) $Id$");

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

#define JUST_MFILE 1
#include "utils.h"
#include "filesys.h"
#undef JUST_MFILE

const char XML_STRING[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
const int INIT_BUFSIZE = 65535;

// expat
#include "xml/expat.h"

// errrr.... I've had too much COM lately...
#define interface struct

namespace SXP
{

    // ERRORS

    enum ErrCode
    {
	err_no_error,
	err_cant_open_file,
	err_no_memory,
	err_syntax,
	err_no_elements,
	err_invalid_token,
	err_unclosed_token,
	err_partial_char,
	err_tag_mismatch,
	err_duplicate_attribute,
	err_junk_after_doc_element,
	err_param_entity_ref,
	err_undefined_entity,
	err_recursive_entity_ref,
	err_async_entity,
	err_bad_char_ref,
	err_binary_entity_ref,
	err_attribute_external_entity_ref,
	err_misplaced_xml_pi,
	err_unknown_encoding,
	err_incorrect_encoding,
	err_unclosed_cdata_section,
	err_external_entity_handling,
	err_not_standalone,
	err_unknown
    };

    ErrCode RemapError(int expat_err);

    // a few shortcuts
    typedef std::map < mstring, mstring > dict;	// for the attribs
    extern pair < mstring, mstring > blank_mstring_pair;
    extern dict blank_dict;

    // Interface forwards

    interface IPersistObj;
    // wraps the element names ("tags") in a dual string-hash key
    // representation
    struct Tag
    {
	const char *ch;
	unsigned long dw;
	Tag(const char *name);
    };


    template < class T > interface IFilePointer
    {
	inline FILE *FP()
	{
	    return static_cast < T * > (this)->FP();
	}
	inline void Indent()
	{
	    static_cast < T * > (this)->Indent();
	}
    };

    template < class T > class IFilePrint
    {
    public:
	virtual ~ IFilePrint()
	{
	}
	virtual void Print(const char *format, ...)
	{
	    va_list argptr;

	    va_start(argptr, format);
	    static_cast < T * > (this)->PrintV(format, argptr);
	    va_end(argptr);
	}
	virtual void PrintV(const char *format, va_list argptr)
	{
	    static_cast < T * > (this)->PrintV(format, argptr);
	}
	virtual void Indent()
	{
	    static_cast < T * > (this)->Indent();
	}
    };

    interface IData
    {
	virtual ~ IData();
	virtual const char *Data() const = 0;
    };

    // helper functions

    // escape a char string - remove &<>" and replace with escape codes
    mstring XMLEscape(const char *pstr);

    // remove XML escapes (&amp; etc)
    mstring XMLUnEscape(const char *pstr);

#include "xml/sxp_data.h"

    // abstraction for the output stream - something which knows
    // how to begin the XML document, how to write object begin/end tags
    // and simple data elements
    template < class T > interface IOutStreamT : public IDataOutput < T >
    {
	virtual ~ IOutStreamT()
	{
	}

	virtual void BeginXML(void)
	{
	    static_cast < T * > (this)->BeginXML();
	}

	virtual void BeginObject(Tag & t, dict & attribs = blank_dict)
	{
	    static_cast < T * > (this)->BeginObject(t, attribs);
	}
	virtual void EndObject(Tag & t)
	{
	    static_cast < T * > (this)->EndObject(t);
	}

	// recursively write other objects
	virtual void WriteSubElement(IPersistObj * pObj, dict & attribs = blank_dict)
	{
	    static_cast < T * > (this)->WriteSubElement(pObj, attribs);
	}
    };

    // abstraction for an XML element, with extractor methods
    // for the name of the element, for the "character data"
    // (the free-floating characters between the opening and closing tags),
    // and a method to access the attribs by name
    // for want of a better place (and to simplify code in the user classes),
    // the methods to read from simple data elements (like <height>187.4</height>)
    // are stuffed here

    interface IElement : public IDataInput
    {
	virtual ~ IElement()
	{
	}
	virtual const char *Name();
	virtual const char *Attrib(const char *attrName);

	virtual bool IsA(const char *name);
	virtual bool IsA(Tag & t);
	virtual bool AttribIs(const char *attrName, const char *val);
    };

    // The user classes need only this much access to the parser:
    // a way to redirect parsing events into child element handlers
    // and a way to stop it on error or when it doesn't need more data
    interface IParser
    {
	virtual void ReadTo(IPersistObj * pTarget) = 0;
	virtual void Shutdown(void) = 0;
    };

    // Implementation follows. Please stop reading here :-)

    // the hashtable facility, used for element names

// hashtable size
// this should be at least 3-4 times the number of tags you use
// the more, the better
#define HASHTABLESIZE		1024

    // a singleton hashtable
    // for tags

    class TagHashtable;
    extern TagHashtable *g_pHashTable;

    class TagHashtable
    {
    private:
	TagHashtable()
	{
	}
    public:
	inline static TagHashtable &TagHT()
	{
	    if (g_pHashTable)
	    {
		return *g_pHashTable;
	    }
	    else
	    {
		g_pHashTable = new TagHashtable;
		return *g_pHashTable;
	    }
	}

	mstring table[HASHTABLESIZE];	// if you have lots of tags, increase this

	// the basic hash function
	inline unsigned long Hash(const char *ch)
	{
	    unsigned long dw = 0;

	    for (; *ch; dw = (dw << 5) - dw + *ch++) {}
	    return dw;
	}

	// add a tag's char* member to the hashtable and
	// fill the dword member;
	// plain vanilla hash, lookup, strcmp, linear search
	inline unsigned long Add(Tag & t)
	{
	    unsigned int dw = Hash(t.ch) % HASHTABLESIZE;

	    while (1)
	    {
		if (table[dw].empty())
		{
		    table[dw] = t.ch;
		    return t.dw = dw;
		}
		else if (table[dw].compare(t.ch) == 0)
		{
		    return t.dw = dw;
		}
		else
		{
		    dw++;
		    dw %= HASHTABLESIZE;
		}
	    }
	}

	// find the hash value of a string which shouldn't be added to the
	// table; return ~0 on "not found"
	inline unsigned long Lookup(const char *ch)
	{
	    unsigned int dw = TagHashtable::TagHT().Hash(ch) % HASHTABLESIZE;

	    while (1)
	    {
		if (table[dw].empty())
		{
		    return ~0;
		}
		else if (table[dw].compare(ch) == 0)
		{
		    return dw;
		}
		else
		{
		    dw++;
		    dw %= HASHTABLESIZE;
		}
	    }
	}
    };

    // IOutStream to an stdio file.
    class CFileOutStream : public IOutStreamT < CFileOutStream >
    {
    public:
	FILE * m_fp;
    private:
	int m_nIndent;

	// ugly...
    public:
	inline FILE * FP()
	{
	    return m_fp;
	}

	virtual void Indent()
	{
	    for (int i = 0; i < m_nIndent; i++)
		fputc('\t', m_fp);
	}

	inline CFileOutStream(const char *chFilename)
	{
	    m_fp = NULL;
	    m_fp = fopen(chFilename, "wt");
	    m_nIndent = 0;
	}

	inline CFileOutStream(FILE * fp)
	{
	    m_fp = fp;
	    m_nIndent = 0;
	}

	virtual ~ CFileOutStream()
	{
	    fclose(m_fp);
	}

	virtual void BeginXML(void)
	{
	    // UTF-8 encoding is used because it allows relatively painless
	    // support for storing widechars as character data, via
	    // conversion functions in IElement::Retrieve() and 
	    // IOutStream::WriteElement
	    fprintf(m_fp, "%s\n", XML_STRING);
	}

	virtual void BeginObject(Tag & t, dict & attribs = blank_dict)
	{
	    Indent();
	    m_nIndent++;
	    fprintf(m_fp, "<%s", t.ch);
	    for (dict::iterator i = attribs.begin(); i != attribs.end(); i++)
	    {
		fprintf(m_fp, " %s=\"%s\"", (*i).first.c_str(), (*i).second.c_str());
	    }
	    fprintf(m_fp, ">\n");
	}

	virtual void EndObject(Tag & t)
	{
	    m_nIndent--;
	    Indent();
	    fprintf(m_fp, "</%s>\n", t.ch);
	}

//      virtual void WriteSubElement(IPersistObj * pObj, dict & attribs = blank_dict)
//      {
//          pObj->WriteElement(this, attribs);
//      }
    };

    class MOutStream : public IOutStreamT < MOutStream >
    {
	int m_nIndent;
	size_t buf_sz;
	size_t buf_cnt;
	char *buffer;

	void ExpandBuf();

    public:
	virtual void Print(const char *format, ...);
	virtual void PrintV(const char *format, va_list argptr);
	virtual void Indent();

	MOutStream();
	virtual ~ MOutStream();
	inline size_t BufSize()
	{
	    return buf_cnt;
	}
	inline const char *Buf()
	{
	    return buffer;
	}
	virtual void BeginXML(void);
	virtual void BeginObject(Tag & t, dict & attribs = blank_dict);
	virtual void EndObject(Tag & t);
	virtual void WriteSubElement(IPersistObj * pObj, dict & attribs = blank_dict);
    };

    typedef IOutStreamT < MOutStream > IOutStream;

    // IPersistObj is implemented by the user classes;
    // it contains both reading and writing functionality
    interface IPersistObj
    {
	// this should return the identifier used for the elements
	// describing objects of the user class; this should be a
	// function used also by the IPersistObj::WriteElement implementation
	virtual Tag &GetClassTag() const = 0;

	// the user class should write itself out, using the IOutStream
	// members for its "simple" members and calling WriteElement
	// recursively for any contained objects
	// some of its attributes might be supplied from its container
	// (which is supposed to call WriteElement in the first place)
	virtual void WriteElement(IOutStream * pOut, dict & attribs) = 0;

	// this is called for each element within the "this" element,
	// for which the "this" element is immediate parent
	// it is called when the open element tag is encountered,
	// and only the Name() and Attrib() of pElement values are valid
	virtual void BeginElement(IParser * pIn, IElement * pElement) = 0;

	// this is called when the corresponding close element
	// tag is encountered, and the Data() member of pElement is
	// also valid
	// NOTE: each object receives both its own BeginElement so it can
	// process its own element tag attributes, and its own EndElement
	// so it can process its own character data
	virtual void EndElement(IParser * pIn, IElement * pElement) = 0;
    };

    // IElement implemented with STL strings

    class CElement : public IElement
    {
	mstring m_strName;
	mstring m_strData;
	dict m_Attribs;
	unsigned long m_dwTagHash;

    public:
	CElement()
	{
	}
	~CElement()
	{
	    m_strData.erase();
	}

	inline void Init(const XML_Char * pchName, const char **ppchAttrib)
	{
	    m_dwTagHash = ~0;
	    m_strName = pchName;
	    m_strData.erase();
	    m_Attribs.clear();

	    for (const char **pp = ppchAttrib; *pp != 0; pp += 2)
	    {
		m_Attribs[pp[0]] = pp[1];
	    }
	}

	inline void AddData(const char *pchData, int len)
	{
	    m_strData.append(pchData, len);
	}

	// IData
	inline const char *Data() const
	{
	    return m_strData.c_str();
	}

	// IElementT
	inline const char *Name()
	{
	    return m_strName.c_str();
	}
	inline const char *Attrib(const char *attrName)
	{
	    return (m_Attribs[attrName]).c_str();
	}
	virtual bool AttribIs(const char *attrName, const char *val)
	{
	    return ((m_Attribs[attrName]).compare(val) == 0);
	}
	virtual bool IsA(Tag & t)
	{
	    if (m_dwTagHash == static_cast < unsigned long > (~0))
	    {
		m_dwTagHash = TagHashtable::TagHT().Lookup(m_strName.c_str());
	    }
	    return (t.dw == m_dwTagHash);
	}
	virtual bool IsA(const char *pchName)
	{
	    return (m_strName.compare(pchName) == 0);
	}
    };

    // the mighty parser itself
    class CParser : public IParser
    {
	std::stack < IPersistObj * > m_EHStack;	// event handlers
	std::stack < CElement * > m_EStack;	// elements
	XML_Parser m_parser;

	int m_bShuttingDown;
	int m_nErrorLine, m_nErrorCol;	// position of error as reported by expat
	mstring m_strError;

    public:
	// The parser begins feeding element events into a "root" object -
	// typically an object factory of sorts
	CParser(IPersistObj * pRoot);
	virtual ~ CParser()
	{
	    DoShutdown();
	}

	int GetErrorLine()
	{
	    return m_nErrorLine;
	}
	int GetErrorCol()
	{
	    return m_nErrorCol;
	}
	mstring GetErrorStr()
	{
	    return m_strError;
	}
	void GetErrorPos(int &line, int &col)
	{
	    line = m_nErrorLine;
	    col = m_nErrorCol;
	}

	// give the parser food for thought
	ErrCode Feed(const char *pData, int nLen, int bFinal = 1);

	// give the parser a food for thought the lazy way
	ErrCode FeedFile(char *pchFilename);

	// IParser::ReadTo -> redirect event stream into a new IPersistObj
	inline void ReadTo(IPersistObj * pPI)
	{
	    m_EHStack.push(pPI);
	}

	// these need to be static so they can be registered as callbacks
	// with expat

	static void StaticStartElement(void *puserData, const XML_Char * name, const XML_Char ** atts);
	static void StaticEndElement(void *puserData, const XML_Char * name);
	static void StaticCharData(void *puserData, const XML_Char * data, int len);
	static int UEHandler(void *pData, const char *pchName, XML_Encoding * info);

	// closing tag for the root object element encountered, stop parsing
	// DoShutdown cannot be called inside a handler call initiated by
	// expat, so actual shutdown is deferred until the next iteration of
	// the feed loop; however, events are NOT received
	inline void Shutdown()
	{
	    m_bShuttingDown = 1;
	}

	void DoShutdown();

    private:
	// opening element tag encountered; push on element stack
	// and send to current handler
	inline void StartElement(const XML_Char * name, const char **atts)
	{
	    CElement *pElement = new CElement;

	    pElement->Init(name, atts);
	    m_EStack.push(pElement);
	    m_EHStack.top()->BeginElement(this, pElement);
	}

	// closing element tag encountered;
	//   if this is the closing tag for the current event handler,
	//   pop it off the handler stack; if this empties the stack,
	//   shut down parser
	// else - just feed the element with the already completed
	// data section to the current handler, then pop it off the element
	// stack
	inline void EndElement(const XML_Char * name)
	{
	    unsigned long dwHash = TagHashtable::TagHT().Lookup(name);
	    IElement *pElement = m_EStack.top();

	    m_EStack.pop();
	    m_EHStack.top()->EndElement(this, pElement);
	    if (dwHash == m_EHStack.top()->GetClassTag().dw)
	    {
		m_EHStack.pop();
		if (m_EHStack.empty())
		{
		    Shutdown();
		    delete pElement;

		    return;
		}
	    }
	    delete pElement;
	}

	// add it to the current element data
	inline void CharData(const XML_Char * data, int len)
	{
	    m_EStack.top()->AddData(data, len);
	}
    };


class WriteSubElement
{
    IOutStream *pOut;
    Tag &tag;
    
public:
    WriteSubElement(IOutStream *p, Tag &t) : pOut(p), tag(t) {}
    template<typename T> void operator()(const T &in)
    {
	pOut->BeginObject(tag);
	pOut->WriteSubElement(const_cast<T *>(&in));
	pOut->EndObject(tag);
    }
};

class WriteElement
{
    IOutStream *pOut;
    Tag &tag;
    
public:
    WriteElement(IOutStream *p, Tag &t) : pOut(p), tag(t) {}
    template<typename T> void operator()(const T &in)
    {
	pOut->WriteElement(tag, in);
    }
};

class WritePairElement
{
    IOutStream *pOut;
    Tag &tag;
    
public:
    WritePairElement(IOutStream *p, Tag &t) : pOut(p), tag(t) {}
    template<typename T> void operator()(const T &in)
    {
	pOut->WriteElement(tag, mstring(in.first) + "\n" + mstring(in.second));
    }
};

}

#endif // _SXP__H
