#ifndef WIN32
#pragma interface
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
#if !defined(__SXP__H)
#define __SXP__H
static const char *ident_sxp_h = "@(#) $Id$";
#include "pch.h"
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.7  2000/07/21 00:18:46  prez
** Fixed database loading, we can now load AND save databases...
**
** Almost ready to release now :)
**
** Revision 1.6  2000/07/11 13:22:18  prez
** Fixed loading/saving -- they now work with encryption and compression.
** Tested, it works too!  Now all we need to do is fix the loading, and
** we're set ... :))
**
** Revision 1.5  2000/05/28 05:20:46  prez
** More stuff ..
**
** Revision 1.4  2000/05/20 00:08:01  ungod
** getting ConfigEngine compiling and cleaning up SXP to stop circular includes of "datetime.h"
**
** Revision 1.3  2000/05/17 07:47:58  prez
** Removed all save_databases calls from classes, and now using XML only.
** To be worked on: DCC Xfer pointer transferal and XML Loading
**
** Revision 1.2  2000/04/15 11:11:44  ungod
** starting xmlage of magick
**
** Revision 1.56  2000/02/23 12:21:02  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.55  2000/02/15 10:37:47  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#define JUST_MFILE 1
#include "utils.h"
#undef JUST_MFILE

#define XML_STRING	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
#define SXP_TAG		0x01	// Required to force write.
#define SXP_COMPRESS	0x02
#define SXP_ENCRYPT	0x04

// configuration -- assume dos/win32 don't have GNU autoconf.
// Don't touch any of this unless you know what you're doing.
// in its present form it compiles on MSVC++ and Linux/GCC, and
// I assume that users of other compilers "know what they're doing".
/*
#ifndef SXP_HAVE_CONFIG_H
#	if (!defined(WIN32) && !defined(DJGPP))
#		define	SXP_HAVE_CONFIG_H	1
#	endif
#endif !SXP_HAVE_CONFIG_H */

#if SXP_HAVE_CONFIG_H
#	include <sxp-config.h>
#else /* SXP_HAVE_CONFIG_H */
	// Let's figure features out.
#	define HAVE_NAMESPACES	1 //Define if C++ compiler supports namespaces.
#	define HAVE_WSTRING	1 //Define if you have std::wstring.
#	define HAVE_SNPRINTF	1 //Define if you have the snprintf function.
	// AFAIK, MSVC++, DJGPP and MINGW32 lack snpintf. Maybe it's
	// common to all dos/win32 compilers.
#	if defined(_MSC_VER)
#		undef HAVE_SNPRINTF
#	endif /* defined(_MSC_VER) */

#	if defined(__GNUC__)
#		undef HAVE_WSTRING
		// FIXME: when exactly did the namespaces come in? egcs-1.1?
#		if __GNUC__ == 2 && __GNUC_MINOR__ < 91
#			undef	HAVE_NAMESPACES
#		endif /* __GNUC__ 2 && __GNUC_MINOR__ < 91 */
#		if defined(WIN32) || defined(DJGPP)
#			undef	HAVE_SNPRINTF
#		endif /* defined(WIN32) || defined(DJGPP) */
#	endif /* defined(__GNUC__) */
#endif /* !SXP_HAVE_CONFIG_H */

#if defined(_MSC_VER)
#	pragma comment(lib, "xmlparse.lib") // link expat lib too
#	pragma warning( disable: 4786 ) // damn identifier truncated's
#endif /* defined(_MSC_VER) */

// expat
#include "xmlparse.h"

// Be friendly to those without namespaces
#ifdef HAVE_NAMESPACES
#	define	SXP_NS_BEGIN	namespace SXP {
#	define	SXP_NS_END	}
#	define	SXP_NS_USING	using namespace SXP;
#	define	STD	std
#else /* HAVE_NAMESPACES */
#	define	SXP_NS_BEGIN
#	define	SXP_NS_END
#	define	SXP_NS_USING
#	define	STD
#	define	SXP
#endif /* !HAVE_NAMESPACES */

SXP_NS_BEGIN

	// a few shortcuts
#if HAVE_NAMESPACES
	typedef std::string string; // we deserve a string of our own!
#if HAVE_WSTRING
	typedef std::wstring wstring; // and a wchar_t variation
#endif /* HAVE_WSTRING */
#endif /* HAVE_NAMESPACES */
	typedef std::map<string, string> dict; // for the attribs

	// Interface forwards

	class IPersistObj;
	class Tag;

	template<class T>
	class IFilePointer {
    public:
		inline FILE *FP() { return ((T *)this)->FP(); }
		inline void Indent()   { ((T *)this)->Indent(); }
	};

	template<class T>
	class IFilePrint {
    public:
		inline void Print(char *format, ...)
		{
		    va_list argptr;
		    va_start(argptr, format);
		    ((T *)this)->PrintV(format, argptr);
		    va_end(argptr);
		}
		inline void PrintV(char *format, va_list argptr)
		{
		    ((T *)this)->PrintV(format, argptr);
		}
		inline void Indent()   { ((T *)this)->Indent(); }
	};

	template<class T>
	class IData {
    public:
		inline const char *Data() { return ((T *)this)->Data(); }
	};

#include "sxp_data.h"

	// abstraction for the output stream - something which knows
	// how to begin the XML document, how to write object begin/end tags
	// and simple data elements
	template<class T>
	class IOutStreamT:
	public IDataOutput<T> {
    public:
		inline void BeginXML(void) { ((T *)this)->BeginXML(); }

		inline void BeginObject(Tag& t, dict& attribs) { ((T *)this)->BeginObject(t, attribs); }
		inline void EndObject  (Tag& t) { ((T *)this)->EndObject(t); }

		// recursively write other objects
		inline void WriteSubElement(IPersistObj *pObj, dict& attribs) { 
			((T *)this)->WriteSubElement(pObj, attribs); 
		} 
	};

	// abstraction for an XML element, with extractor methods
	// for the name of the element, for the "character data"
	// (the free-floating characters between the opening and closing tags),
	// and a method to access the attribs by name
	// for want of a better place (and to simplify code in the user classes),
	// the methods to read from simple data elements (like <height>187.4</height>)
	// are stuffed here
	template<class T>
	class IElementT:
	public IDataInput<T>{
    public:
		inline const char *Name() { return ((T *)this)->Name(); }
		inline const char *Attrib(const char *attrName)
			{ return ((T *)this)->Attrib(attrName); }

		inline int IsA(const char *name)
			{ return ((T *)this)->IsA(name); }
		inline int IsA(Tag& t)
			{ return ((T *)this)->IsA(t); }
		inline int AttribIs(const char *attrName, const char *val)
			{ return ((T *)this)->AttribIs(attrName, val); }
	};

	// The user classes need only this much access to the parser:
	// a way to redirect parsing events into child element handlers
	// and a way to stop it on error or when it doesn't need more data
	class IParser {
    public:
		virtual void ReadTo( IPersistObj *pTarget ) = 0;
		virtual void Shutdown( void ) = 0;
	};

	// Implementation follows. Please stop reading here :-)

	// the pool allocator facility, used for elements
	template <typename T>
	class CPoolAllocator {
		T **pool;
		int top;
		int size;

	public:
		// create the pool with an initial size of sz
		CPoolAllocator(int sz = 32) {
			size = sz;
			pool = (T **)malloc(size * sizeof(T*));
			for(int i=0; i<size; i++) {
				pool[i] = new T; // construct objects
			}
			top = size;
		}

		~CPoolAllocator() {
			for(int i=0; i<top; i++) {
				delete pool[i];
			}
			free(pool);
		}

		inline T* Alloc() {
			if( top > 0 ) {
				return pool[--top]; // unused objects available
			} else {
				// pool exhausted, try to allocated new elements
				T **newpool;
				newpool = (T **)realloc(pool, size * sizeof(T*) * 2);
				if( newpool == 0 ) {
					return 0; // failed, can't allocate
				} else {
					pool = newpool; // construct new objects in the
					                // newly allocated part of the pool
					for(int i=0; i<size; i++) {
						pool[i] = new T;
					}
					top = size;
					size *= 2;
					return pool[--top]; // return one of the new objects
				}
			}
		}
		// return an object to the pool
		inline void Free(T *t) {
			pool[top++] = t;
		}
	};

	// the hashtable facility, used for element names

	// wraps the element names ("tags") in a dual string-hash key
	// representation
	class Tag {
    public:
		const char *ch;
		unsigned long dw;
		Tag(const char *name);
	};

// hashtable size
// this should be at least 3-4 times the number of tags you use
// the more, the better

#define SXP_HTSIZE		256

	// a singleton hashtable
	// for tags

	class TagHashtable;
	extern TagHashtable *g_pHashTable;

	class TagHashtable {
	private:
		TagHashtable() { }
	public:
		inline static TagHashtable& TagHT() {
			if( g_pHashTable ) {
				return *g_pHashTable;
			} else {
				g_pHashTable = new TagHashtable;
				return *g_pHashTable;
			}
		}

		string table[SXP_HTSIZE]; // if you have lots of tags, increase this

		// the basic hash function
		inline unsigned long Hash(const char *ch) {
			unsigned long dw = 0;
			for (; *ch; dw = (dw << 5) - dw + *ch++);
			return dw;
		}

		// add a tag's char* member to the hashtable and
		// fill the dword member;
		// plain vanilla hash, lookup, strcmp, linear search
		inline unsigned long Add(Tag& t) {
			unsigned int dw = Hash(t.ch) % SXP_HTSIZE;
			while(1) {
				if( table[dw].empty() ) {
					table[dw] = t.ch;
					return t.dw = dw;
				} else if( table[dw].compare( t.ch ) == 0 ) {
					return t.dw = dw;
				} else {
					dw++; 
					dw %= SXP_HTSIZE;
				}
			}
		}

		// find the hash value of a string which shouldn't be added to the
		// table; return ~0 on "not found"
		inline unsigned long Lookup(const char *ch) {
			unsigned int dw = TagHashtable::TagHT().Hash(ch) % SXP_HTSIZE;
			while(1) {
				if( table[dw].empty() ) {
					return ~0;
				} else if( table[dw].compare( ch ) == 0 ) {
					return dw;
				} else {
					dw++; 
					dw %= SXP_HTSIZE;
				}
			}
		}
	};

	// helper functions

	// escape a char string - remove &<>" and replace with escape codes
	inline string XMLEscape(const char *pstr) {
		const char *p;
		string ret;
		for(p = pstr; *p; p++) {
			switch( *p ) {
			case '&': ret.append("&amp;",  5); break;
			case '<': ret.append("&lt;",   4); break;
			case '>': ret.append("&gt;",   4); break;
			case '"': ret.append("&quot;", 6); break;
			default:  ret.append(1, *p);
			}
		}
		return ret;
	}

	// escape a wide character char string - remove &<>" and replace with escape codes, convert UCS-16 to UTF-8
	inline string XMLEscapeW(const wchar_t *pstr) {
		const wchar_t *p;
		string ret;
		for(p = pstr; *p; p++) {
			if( *p < 0x80 ) {
				switch( *p ) {
				case L'&': ret.append("&amp;"); break;
				case L'"': ret.append("&quot;"); break;
				case L'<': ret.append("&lt;"); break;
				case L'>': ret.append("&gt;"); break;
				default:   ret.append(1, (char)*p);
				}
			} else {
#define SXP_CHAR_CAST (char)
				if( *p < 0x800 ) {
					ret.append(1, SXP_CHAR_CAST (0xC0 | ((*p) >> 6) ));
					ret.append(1, SXP_CHAR_CAST (0x80 | ((*p) & 0x3F )));
				} else {
					ret.append(1, SXP_CHAR_CAST (0xE0 | ( (*p) >> 12)         ));
					ret.append(1, SXP_CHAR_CAST (0x80 | (((*p) >>  6) & 0x3F )));
					ret.append(1, SXP_CHAR_CAST (0x80 | ( (*p)        & 0x3F )));
				}
#undef SXP_CHAR_CAST
			}
		}
		return ret;
	}

	// remove XML escapes (&amp; etc)
	inline string XMLUnEscape(const char *pstr) {
		const char *p;
		string ret;
		for(p = pstr; *p; p++) {
			if( *p != '&' ) {
				ret.append(1, *p);
			} else {
				if( strcmp(p+1, "amp;") == 0 ) {
					p += 4;
					ret.append(1, '&');
				} else if( strcmp(p+1, "lt;") == 0 ) {
					p += 3;
					ret.append(1, '<');
				} else if( strcmp(p+1, "gt;") == 0 ) {
					p += 3;
					ret.append(1, '>');
				} else if( strcmp(p+1, "quot;") == 0 ) {
					p += 5;
					ret.append(1, '"');
				}
			}
		}
		return ret;
	}

#if HAVE_WSTRING
	// remove XML escapes (&amp; etc), convert UTF-8 to UCS-16
	inline wstring XMLUnEscapeW(const char *pstr) {
		std::wstring ret;
		unsigned len = strlen(pstr);
		wchar_t wch;
		for(unsigned pos = 0; pos<len; pos++) {
			char c0, c1, c2;
			c0 = pstr[pos];
			if( c0 == '&' ) {
				if( strncmp(pstr+pos+1, "amp;", 4) == 0) {
					ret.append(1, L'&');
				} else if( strncmp(pstr+pos+1, "lt;", 3) == 0 ) {
					ret.append(1, L'<');
				} else if( strncmp(pstr+pos+1, "gt;", 3) == 0 ) {
					ret.append(1, L'>');
				} else if( strncmp(pstr+pos+1, "quot;", 5) == 0 ) {
					ret.append(1, L'"');
				} else {
					ret.append(1, L'&');
				}
			} else {
				if( c0 < 0x80 ) {
					ret.append(1, wchar_t(pstr[pos]));
				} else {
					c1 = pstr[pos+1];
					if( (c0 & 0xE0) == 0xC0 ) {
						wch = (c0 & 0x1F);
						wch <<= 6;
						wch |= (c1 & 0x3F);
						ret.append(1, wch);
						pos++;
					} else {
						c2 = pstr[pos+2];
						wch = (c0 & 0xF);
						wch <<= 6;
						wch |= (c1 & 0x3F);
						wch <<= 6;
						wch |= (c2 & 0x3F);
						ret.append(1, wch);
						pos++; pos++;
					}
				}
			}
		}
		return ret;
	}
#endif /* HAVE_WSTRING */

	// IOutStream to an stdio file.
	class CFileOutStream : 
	public IOutStreamT<CFileOutStream> {
		FILE *m_fp;
		int m_nIndent;

		// ugly...
	public:
		inline FILE *FP() {
			return m_fp;
		}

		inline void Indent() {
			for(int i=0; i<m_nIndent; i++)
				fputc('\t', m_fp);
		}

		inline CFileOutStream(const char *chFilename) {
			m_fp = fopen(chFilename, "wt");
			m_nIndent = 0;
		}

		inline CFileOutStream(FILE *fp) {
			m_fp = fp;
			m_nIndent = 0;
		}

		~CFileOutStream() {
			fclose(m_fp);
		}

		inline void BeginXML(void) {
			// UTF-8 encoding is used because it allows relatively painless
			// support for storing widechars as character data, via
			// conversion functions in IElement::Retrieve() and 
			// IOutStream::WriteElement
			fprintf(m_fp, XML_STRING);
		}

		inline void BeginObject(Tag& t, dict& attribs) {
			Indent(); m_nIndent++;
			fprintf(m_fp, "<%s", t.ch);
			for(dict::iterator i=attribs.begin(); i!=attribs.end(); i++) {
				fprintf(m_fp, " %s=\"%s\"",
					(*i).first.c_str(),
					(*i).second.c_str() );
			}
			fprintf(m_fp, ">\n");
		}

		inline void EndObject  (Tag& t) {
			m_nIndent--;
			Indent();
			fprintf(m_fp, "</%s>\n", t.ch);
		}

		void WriteSubElement(IPersistObj *pObj, dict& attribs); 
	};

	class MFileOutStream : public IOutStreamT<MFileOutStream>
	{
		mstring filename;
		mFile out;
		int m_nIndent, compress;
		size_t buf_sz;
		char *buffer;
		mstring key;

		void ExpandBuf();
	public:
		void Print(char *format, ...);
		void PrintV(char *format, va_list argptr);
		void Indent();
		MFileOutStream(mstring chFilename, int comp = 0, mstring ikey = "");
		MFileOutStream(mstring chFilename, FILE *fp, int comp = 0, mstring ikey = "");
		~MFileOutStream();
		void BeginXML(void);
		void BeginObject(Tag& t, dict& attribs);
		void EndObject  (Tag& t);
		void WriteSubElement(IPersistObj *pObj, dict& attribs); 
	};

	typedef IOutStreamT<MFileOutStream> IOutStream;

	// IElement implemented with STL strings

	class CElement : public IElementT<CElement> {
		string m_strName;
		string m_strData;
		dict m_Attribs;
		unsigned long m_dwTagHash;

	public:
		CElement() {}
		
		inline void Init(const XML_Char *pchName, const char **ppchAttrib) 
		{
			m_dwTagHash = ~0;
			m_strName = pchName;
			m_strData.erase();
			m_Attribs.clear();

			for(const char **pp = ppchAttrib; *pp != 0; pp += 2) {
				m_Attribs[ string(pp[0]) ] =
					string( pp[1] );
			}
		}

		inline void AddData(const char *pchData, int len) {
			m_strData.append(pchData, len);
		}

		// IData
		inline const char *Data() {
			return m_strData.c_str();
		}

		// IElementT
		inline const char *Name() {
			return m_strName.c_str();
		}
		inline const char *Attrib(const char *attrName) {
			return (m_Attribs[ string(attrName) ]).c_str();
		}
		inline int AttribIs(const char *attrName, const char *val) {
			return ((m_Attribs[ string(attrName) ]).compare(val) == 0);
		}
		inline int IsA(Tag& t) {
			if( static_cast<int>(m_dwTagHash) == ~0 ) {
				m_dwTagHash = TagHashtable::TagHT().Lookup(m_strName.c_str());
			}
			return( t.dw == m_dwTagHash );
		}
		inline int IsA(const char *pchName) {
			return( !m_strName.compare(pchName) );
		}
	};

	typedef IElementT<CElement> IElement;

	// IPersistObj is implemented by the user classes;
	// it contains both reading and writing functionality
	class IPersistObj {
    public:
		// this should return the identifier used for the elements
		// describing objects of the user class; this should be a
		// function used also by the IPersistObj::WriteElement implementation
		virtual Tag& GetClassTag() const = 0;

		// the user class should write itself out, using the IOutStream
		// members for its "simple" members and calling WriteElement
		// recursively for any contained objects
		// some of its attributes might be supplied from its container
		// (which is supposed to call WriteElement in the first place)
		virtual void  WriteElement(IOutStream *pOut, dict& attribs) = 0;

		// this is called for each element within the "this" element,
		// for which the "this" element is immediate parent
		// it is called when the open element tag is encountered,
		// and only the Name() and Attrib() of pElement values are valid
		virtual void BeginElement(IParser *pIn, IElement *pElement) = 0;

		// this is called when the corresponding close element
		// tag is encountered, and the Data() member of pElement is
		// also valid
		// NOTE: each object receives both its own BeginElement so it can
		// process its own element tag attributes, and its own EndElement
		// so it can process its own character data
		virtual void EndElement(IParser *pIn, IElement *pElement) = 0;
	};

	// the mighty parser itself
	class CParser : public IParser {
		std::stack<IPersistObj *> m_EHStack; // event handlers
		std::stack<CElement *> m_EStack; // elements
		XML_Parser m_parser;

		CPoolAllocator<CElement> m_ElementAllocator; // pool alloc
		int m_bShuttingDown;

	public:
		// The parser begins feeding element events into a "root" object -
		// typically an object factory of sorts
		CParser(IPersistObj *pRoot) {
			m_parser = XML_ParserCreate(NULL);
			XML_SetUserData(m_parser, (void *)this);
			XML_SetElementHandler(m_parser, CParser::StaticStartElement, CParser::StaticEndElement);
			XML_SetCharacterDataHandler(m_parser, CParser::StaticCharData);
			ReadTo(pRoot);
			m_bShuttingDown = 0;
		}
		virtual ~CParser() {}

		// give the parser food for thought
		inline int Feed(const char *pData, int nLen, int bFinal = 1) {
			if( m_parser )
				return XML_Parse(m_parser, pData, nLen, bFinal);
			else
				return 0;
		}

		// give the parser a food for thought the lazy way
		int FeedFile(mstring chFilename, mstring ikey = "");

		// IParser::ReadTo -> redirect event stream into a new IPersistObj
		inline void ReadTo( IPersistObj *pPI ) {
			m_EHStack.push(pPI);
		}

		// these need to be static so they can be registered as callbacks
		// with expat
		
		static void StaticStartElement(void *puserData, const XML_Char *name, const XML_Char **atts) {
			CParser *pThis = (CParser *)puserData;
			if( !pThis->m_bShuttingDown ) {
				pThis->StartElement(name, atts);
			}
		}
		static void StaticEndElement(void *puserData, const XML_Char *name) {
			CParser *pThis = (CParser *)puserData;
			if( !pThis->m_bShuttingDown ) {
				pThis->EndElement(name);
			}
		}
		static void StaticCharData(void *puserData, const XML_Char *data, int len) {
			CParser *pThis = (CParser *)puserData;
			if( !pThis->m_bShuttingDown ) {
				pThis->CharData(data, len);
			}
		}

		// closing tag for the root object element encountered, stop parsing
		// DoShutdown cannot be called inside a handler call initiated by
		// expat, so actual shutdown is deferred until the next iteration of
		// the feed loop; however, events are NOT received
		inline void Shutdown() {
			m_bShuttingDown = 1;
		}

		void DoShutdown() {
			XML_ParserFree(m_parser);
			while( !m_EHStack.empty() ) {
				//delete m_EHStack.top();
				m_EHStack.pop();
			}
			while( !m_EStack.empty() ) {
				//delete m_EStack.top();
				m_EStack.pop();
			}
			m_parser = 0;
		}

	private:
		// opening element tag encountered; push on element stack
		// and send to current handler
		inline void StartElement(const XML_Char *name, const char **atts) {
			CElement *pElement = m_ElementAllocator.Alloc();
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
		inline void EndElement(const XML_Char *name) {
			unsigned long dwHash = TagHashtable::TagHT().Lookup(name);
			IElement *pElement = m_EStack.top();
			m_EStack.pop();
			m_EHStack.top()->EndElement(this, pElement);
			if( dwHash == m_EHStack.top()->GetClassTag().dw ) {
				m_EHStack.pop();
				if( m_EHStack.empty() ) {
					Shutdown();
					return;
				}
			}
			m_ElementAllocator.Free(static_cast<CElement *>(pElement));
		}

		// add it to the current element data
		inline void CharData(const XML_Char *data, int len) {
			m_EStack.top()->AddData(data, len);
		}
	};
SXP_NS_END
using namespace std;

#endif // __SXP__H
