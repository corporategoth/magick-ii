#ifndef WIN32
  #pragma interface
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
#ifndef _SXP_H
#define _SXP_H
#include "pch.h"
RCSID(sxp_h, "@(#) $Id$");
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.27  2001/12/27 04:54:46  prez
** Converted SXP to not use STL strings, use mstring instead.
**
** Revision 1.26  2001/12/26 23:30:35  prez
** More fixes to see if I can fix the memory leak ...
**
** Revision 1.25  2001/12/25 08:43:12  prez
** Fixed XML support properly ... it now works again with new version of
** expat (1.95.2) and sxp (1.1).  Also removed some of my const hacks.
**
** Revision 1.24  2001/12/25 06:26:57  prez
** More new SXP fixes ...
**
** Revision 1.23  2001/12/25 05:57:28  prez
** Updated SXP and EXPAT -- untested, but should work.
**
** Revision 1.22  2001/11/12 01:05:01  prez
** Added new warning flags, and changed code to reduce watnings ...
**
** Revision 1.21  2001/06/15 07:20:40  prez
** Fixed windows compiling -- now works with MS Visual Studio 6.0
**
** Revision 1.20  2001/06/02 16:27:04  prez
** Intergrated the staging system for dbase loading/saving.
**
** Revision 1.19  2001/05/17 19:18:53  prez
** Added ability to chose GETPASS or SETPASS.
**
** Revision 1.18  2001/05/14 04:46:31  prez
** Changed to use 3BF (3 * blowfish) encryption.  DES removed totally.
**
** Revision 1.17  2001/04/08 18:53:09  prez
** It now all compiles and RUNS with -fno-default-inline OFF.
**
** Revision 1.15  2001/03/02 05:24:41  prez
** HEAPS of modifications, including synching up my own archive.
**
** Revision 1.14  2001/02/03 02:21:31  prez
** Loads of changes, including adding ALLOW to ini file, cleaning up
** the includes, RCSID, and much more.  Also cleaned up most warnings.
**
** Revision 1.13  2001/01/01 05:32:44  prez
** Updated copywrights.  Added 'reversed help' syntax (so ACCESS HELP ==
** HELP ACCESS).
**
** Revision 1.12  2000/10/10 11:47:50  prez
** mstring is re-written totally ... find or occurances
** or something has a problem, but we can debug that :)
**
** Revision 1.11  2000/08/19 15:17:39  ungod
** no message
**
** Revision 1.10  2000/08/08 09:58:55  prez
** Added ModeO to 4 pre-defined committees.
** Also added back some deletes in xml in the hope that it
** will free up some memory ...
**
** Revision 1.9  2000/07/30 12:40:52  prez
** Updated windows project files.
**
** Revision 1.8  2000/07/29 21:58:52  prez
** Fixed XML loading of weird characters ...
** 2 known bugs now, 1) last_seen dates are loaded incorrectly on alot
** of nicknames, which means we expire lots of nicknames.  2) services
** wont rejoin a +i/+k channel when last user exits.
**
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
#include "filesys.h"
#undef JUST_MFILE

const char XML_STRING[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
const int INIT_BUFSIZE = 65535;

// expat
#include "xml/expat.h"

// errrr.... I've had too much COM lately...
#define interface struct

namespace SXP {

	// ERRORS

	enum ErrCode {
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
	typedef std::map<mstring, mstring> dict; // for the attribs
	extern pair<mstring,mstring> blank_mstring_pair;
	extern dict blank_dict;

	// Interface forwards

	interface IPersistObj;
	struct Tag;

	template<class T>
	interface IFilePointer {
		inline FILE *FP() { return ((T *)this)->FP(); }
		inline void Indent()   { ((T *)this)->Indent(); }
	};

	template<class T>
	class IFilePrint {
    public:
		inline void Print(const char *format, ...)
		{
		    va_list argptr;
		    va_start(argptr, format);
		    static_cast<T *>(this)->PrintV(format, argptr);
		    va_end(argptr);
		}
		inline void PrintV(const char *format, va_list argptr)
		{
		    static_cast<T *>(this)->PrintV(format, argptr);
		}
		inline void Indent()   { static_cast<T *>(this)->Indent(); }
	};

	interface IData {
		virtual ~IData();
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
	template<class T>
	interface IOutStreamT: 
	public IDataOutput<T> {
		inline void BeginXML(void) { ((T *)this)->BeginXML(); }

		inline void BeginObject(Tag& t, dict& attribs = blank_dict) { ((T *)this)->BeginObject(t, attribs); }
		inline void EndObject  (Tag& t) { ((T *)this)->EndObject(t); }

		// recursively write other objects
		inline void WriteSubElement(IPersistObj *pObj, dict& attribs = blank_dict) { 
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

	interface IElement : public IDataInput {
		virtual ~IElement() {}
		virtual const char *Name();
		virtual const char *Attrib(const char *attrName);

		virtual int IsA(const char *name);
		virtual int IsA(Tag& t);
		virtual int AttribIs(const char *attrName, const char *val);
	};

	// The user classes need only this much access to the parser:
	// a way to redirect parsing events into child element handlers
	// and a way to stop it on error or when it doesn't need more data
	interface IParser {
		virtual void ReadTo( IPersistObj *pTarget ) = 0;
		virtual void Shutdown( void ) = 0;
	};

	// Implementation follows. Please stop reading here :-)

	// the hashtable facility, used for element names

	// wraps the element names ("tags") in a dual string-hash key
	// representation
	struct Tag {
		const char *ch;
		unsigned long dw;
		Tag(const char *name);
	};

// hashtable size
// this should be at least 3-4 times the number of tags you use
// the more, the better
#define HASHTABLESIZE		1024

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

		mstring table[HASHTABLESIZE]; // if you have lots of tags, increase this

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
			unsigned int dw = Hash(t.ch) % HASHTABLESIZE;
			while(1) {
				if( table[dw].empty() ) {
					table[dw] = t.ch;
					return t.dw = dw;
				} else if( table[dw].compare( t.ch ) == 0 ) {
					return t.dw = dw;
				} else {
					dw++; 
					dw %= HASHTABLESIZE;
				}
			}
		}

		// find the hash value of a string which shouldn't be added to the
		// table; return ~0 on "not found"
		inline unsigned long Lookup(const char *ch) {
			unsigned int dw = TagHashtable::TagHT().Hash(ch) % HASHTABLESIZE;
			while(1) {
				if( table[dw].empty() ) {
					return ~0;
				} else if( table[dw].compare( ch ) == 0 ) {
					return dw;
				} else {
					dw++; 
					dw %= HASHTABLESIZE;
				}
			}
		}
	};

	// IOutStream to an stdio file.
	class CFileOutStream : 
	public IOutStreamT<CFileOutStream> {
	public:
		FILE *m_fp;
	private:
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
			m_fp = NULL;
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
			fprintf(m_fp, "%s\n", XML_STRING);
		}

		inline void BeginObject(Tag& t, dict& attribs = blank_dict) {
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

		void WriteSubElement(IPersistObj *pObj, dict& attribs = blank_dict); 
	};

	class MOutStream : public IOutStreamT<MOutStream>
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
		virtual ~MOutStream();
		inline size_t BufSize() { return buf_cnt; }
		inline const char *Buf() { return buffer; }
		virtual void BeginXML(void);
		virtual void BeginObject(Tag& t, dict& attribs = blank_dict);
		virtual void EndObject  (Tag& t);
		virtual void WriteSubElement(IPersistObj *pObj, dict& attribs = blank_dict); 
	};

	typedef IOutStreamT<MOutStream> IOutStream;

	// IElement implemented with STL strings

	class CElement : public IElement {
		mstring m_strName;
		mstring m_strData;
		dict m_Attribs;
		unsigned long m_dwTagHash;

	public:
		CElement() {}
		~CElement() { 
			m_strData.erase(); 
		}
		
		inline void Init(const XML_Char *pchName, const char **ppchAttrib) {
			m_dwTagHash = ~0;
			m_strName = pchName;
			m_strData.erase();
			m_Attribs.clear();

			for(const char **pp = ppchAttrib; *pp != 0; pp += 2) {
				m_Attribs[pp[0]] = pp[1];
			}
		}

		inline void AddData(const char *pchData, int len) {
			m_strData.append(pchData, len);
		}

		// IData
		inline const char *Data() const {
			return m_strData.c_str();
		}

		// IElementT
		inline const char *Name() {
			return m_strName.c_str();
		}
		inline const char *Attrib(const char *attrName) {
			return (m_Attribs[ attrName ]).c_str();
		}
		inline int AttribIs(const char *attrName, const char *val) {
			return ((m_Attribs[ attrName ]).compare(val) == 0);
		}
		inline int IsA(Tag& t) {
			if( m_dwTagHash == (unsigned long) ~0 ) {
				m_dwTagHash = TagHashtable::TagHT().Lookup(m_strName.c_str());
			}
			return( t.dw == m_dwTagHash );
		}
		inline int IsA(const char *pchName) {
			return( !m_strName.compare(pchName) );
		}
	};

	// IPersistObj is implemented by the user classes;
	// it contains both reading and writing functionality
	interface IPersistObj {
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

		int m_bShuttingDown;
		int m_nErrorLine, m_nErrorCol; // position of error as reported by expat
		mstring m_strError;

	public:
		// The parser begins feeding element events into a "root" object -
		// typically an object factory of sorts
		CParser(IPersistObj *pRoot);
		virtual ~CParser() { DoShutdown(); }

		int GetErrorLine() { return m_nErrorLine; }
		int GetErrorCol()  { return m_nErrorCol; }
		mstring GetErrorStr() { return m_strError; }
		void GetErrorPos(int& line, int& col) {
			line = m_nErrorLine;
			col  = m_nErrorCol;
		}

		// give the parser food for thought
		inline ErrCode Feed(const char *pData, int nLen, int bFinal = 1);

		// give the parser a food for thought the lazy way
		ErrCode FeedFile(char *pchFilename);

		// IParser::ReadTo -> redirect event stream into a new IPersistObj
		inline void ReadTo( IPersistObj *pPI ) {
			m_EHStack.push(pPI);
		}

		// these need to be static so they can be registered as callbacks
		// with expat
		
		static void StaticStartElement(void *puserData, const XML_Char *name, const XML_Char **atts);
		static void StaticEndElement(void *puserData, const XML_Char *name);
		static void StaticCharData(void *puserData, const XML_Char *data, int len);
		static int UEHandler(void *pData, const char *pchName, XML_Encoding *info);

		// closing tag for the root object element encountered, stop parsing
		// DoShutdown cannot be called inside a handler call initiated by
		// expat, so actual shutdown is deferred until the next iteration of
		// the feed loop; however, events are NOT received
		inline void Shutdown() {
			m_bShuttingDown = 1;
		}

		void DoShutdown();

	private:
		// opening element tag encountered; push on element stack
		// and send to current handler
		inline void StartElement(const XML_Char *name, const char **atts) {
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
		inline void EndElement(const XML_Char *name) {
			unsigned long dwHash = TagHashtable::TagHT().Lookup(name);
			IElement *pElement = m_EStack.top();
			m_EStack.pop();
			m_EHStack.top()->EndElement(this, pElement);
			if( dwHash == m_EHStack.top()->GetClassTag().dw ) {
				m_EHStack.pop();
				if( m_EHStack.empty() ) {
					Shutdown();
					delete pElement;
					return;
				}
			}
			delete pElement;
		}

		// add it to the current element data
		inline void CharData(const XML_Char *data, int len) {
			m_EStack.top()->AddData(data, len);
		}
	};

}

#endif // _SXP__H
