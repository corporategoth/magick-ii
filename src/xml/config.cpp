// $Id$
// In this example the use of SXP for maintaing simple configuration
// files is demonstrated.

// Suppose you have an application which needs some type of global
// configuration values, e.g. the size and position of several windows
// You could store them in the Win32 Registry, but that makes it
// a bit inconvenient to view and edit these values during debugging.
// You can persist the configuration in an XML file

#include "xml/sxp.h"
#include <string>

// We'll declare a class for storing the config info for a single window,
// at let it persist under its own tag, like this:
// <window>
//    <top>0</top>
//    <left>0</left>
//    <width>320</width>
//    <height>240</height>
//    <visible>0</visible>
// </window>

struct CRect {
	int t, l, w, h;
};

// of course, you can do a "using namespace SXP" to avoid all the
// SXP:: scope resolutions, but that defeats the whole idea of using
// namespaces and purists won't be happy

class CWindowConfig :
public SXP::IPersistObj {
public:

	// the CWindowConfig members
	CRect m_rcPos;
	int m_bVisible;

	// The members needed for the SXP mechanism

	// the tags are a property of the class, not of the objects,
	// so they are declared static
	// tags
	static SXP::Tag tag_t, tag_l, tag_w, tag_h, tag_v;
	static SXP::Tag tag_own;
	SXP::Tag& GetClassTag() const { return tag_own; }

	// here you can handle "complex" subtags - see below in CConfig
	// we don't need it here
	void BeginElement(SXP::IParser *pIn, SXP::IElement *pElement) {}
	// here you handle the "simple" subtags to init the member vars
	void EndElement(SXP::IParser *pIn, SXP::IElement *pElement) {
		if( pElement->IsA(tag_t) )   pElement->Retrieve(m_rcPos.t);
		if( pElement->IsA(tag_l) )   pElement->Retrieve(m_rcPos.l);
		if( pElement->IsA(tag_w) )   pElement->Retrieve(m_rcPos.w);
		if( pElement->IsA(tag_h) )   pElement->Retrieve(m_rcPos.h);
		if( pElement->IsA(tag_v) )   pElement->Retrieve(m_bVisible);
	}
	// here you describe how to write out the state of the object
	void WriteElement(SXP::IOutStream *pOut, SXP::dict& attribs) {
		// no attributes, don't add anything to attribs
		pOut->BeginObject(tag_own, attribs);

		pOut->WriteElement(tag_t, m_rcPos.t);
		pOut->WriteElement(tag_l, m_rcPos.l);
		pOut->WriteElement(tag_w, m_rcPos.w);
		pOut->WriteElement(tag_h, m_rcPos.h);
		pOut->WriteElement(tag_v, m_bVisible);

		pOut->EndObject(tag_own);
	}
};

// unfortunately, the C++ syntax doesn't allow us to construct the
// static members together with their declaration at class scope,
// so we must repeat their declarations here at the global scope.
// The constructor of the tags takes their names as a parameter,
// and registers them with the parser.
SXP::Tag CWindowConfig::tag_t("top");
SXP::Tag CWindowConfig::tag_l("left");
SXP::Tag CWindowConfig::tag_w("width");
SXP::Tag CWindowConfig::tag_h("height");
SXP::Tag CWindowConfig::tag_v("visible");
SXP::Tag CWindowConfig::tag_own("window");

class CConfig:
public SXP::IPersistObj {
public:
	// the configuration consists of the info about three windows,
	// the main window, a floating toolbar, and a debug console
	// the pathnames of the 4 most recently used files,
	// and a few user preferences
	CWindowConfig m_wndMain, m_wndToolbar, m_wndDebugConsole;
	std::string m_strLRU[4];
	int m_nTabSize, m_bAutosave, m_bCreateBackups;

	// the members needed for the SXP mechanism
	// tags
	static SXP::Tag tag_LRU[4];
	static SXP::Tag tag_tabsize, tag_autosave, tag_backups;
	static SXP::Tag tag_own;
	// attributes
	static char *attr_wndMain, *attr_wndToolbar, *attr_wndDbgCon;
	SXP::Tag& GetClassTag() const { return tag_own; }

	// here you can handle "complex" members - which will read themselves
	void BeginElement(SXP::IParser *pIn, SXP::IElement *pElement) {
		// all the member windows come with the same tag, so their
		// identity is given via the id attribute
		if( pElement->IsA(CWindowConfig::tag_own) ) {
			if( pElement->AttribIs("id", attr_wndMain) ) {
				// redirect tag stream to wndMain
				pIn->ReadTo( &m_wndMain );
				// send wndMain its own start tag
				m_wndMain.BeginElement(pIn, pElement); 
			}
			if( pElement->AttribIs("id", attr_wndToolbar)  ) {
				pIn->ReadTo( &m_wndToolbar );
				m_wndToolbar.BeginElement(pIn, pElement); 
			}
			if( pElement->AttribIs("id", attr_wndDbgCon)  ) {
				pIn->ReadTo( &m_wndDebugConsole );
				m_wndDebugConsole.BeginElement(pIn, pElement); 
			}
		}
	}
	// here you handle the "simple" subtags to init the member vars
	void EndElement(SXP::IParser *pIn, SXP::IElement *pElement) {
		if( pElement->IsA(tag_tabsize) )   
			pElement->Retrieve(m_nTabSize);
		if( pElement->IsA(tag_backups) )   pElement->Retrieve(m_bCreateBackups);
		if( pElement->IsA(tag_autosave) )   pElement->Retrieve(m_bAutosave);
		for(int i=0; i<4; i++) 
			if( pElement->IsA(tag_LRU[i]) )   pElement->Retrieve(m_strLRU[i]);
	}

	// here you describe how to write out the state of the object
	void WriteElement(SXP::IOutStream *pOut, SXP::dict& attribs) {
		pOut->BeginObject(tag_own, attribs);

		// write out "simple" members"
		pOut->WriteElement(tag_tabsize,  m_nTabSize);
		pOut->WriteElement(tag_autosave, m_bAutosave);
		pOut->WriteElement(tag_backups,  m_bCreateBackups);
		for(int i=0; i<4; i++)
			pOut->WriteElement(tag_LRU[i], m_strLRU[i]);

		// write out "complex" members
		// the three CConfigWindow members will be saved using
		// the same <window> tag, so they must be differentiated
		// by the use of a "id" attrib, like this:
		// <window id="toolbar">
		SXP::dict wndAttribs;
		wndAttribs["id"] = attr_wndMain;
		pOut->WriteSubElement(&m_wndMain, wndAttribs);
		wndAttribs["id"] = attr_wndToolbar;
		pOut->WriteSubElement(&m_wndToolbar, wndAttribs);
		wndAttribs["id"] = attr_wndDbgCon;
		pOut->WriteSubElement(&m_wndDebugConsole, wndAttribs);

		pOut->EndObject(tag_own);
	}
};

char *CConfig::attr_wndMain = "main_window";
char *CConfig::attr_wndToolbar = "toolbar";
char *CConfig::attr_wndDbgCon = "debug_console";

SXP::Tag CConfig::tag_LRU[4] = {
	"lru0", "lru1", "lru2", "lru3"
};
SXP::Tag CConfig::tag_tabsize("tab_size");
SXP::Tag CConfig::tag_autosave("autosave");
SXP::Tag CConfig::tag_backups("backups");
SXP::Tag CConfig::tag_own("configuration");

void main()
{
	CConfig cfg;

	// read config first thing in the morning 
	
	SXP::CParser p( &cfg ); // let the parser know which is the object
	// to receive the root element
	p.FeedFile(	"config.xml" );

	// work, work, work

	// at the end of the day write config
	SXP::CFileOutStream *o = new SXP::CFileOutStream("config.xml");
	o->BeginXML();

	// you might add a namespace attrib to the root element, if you 
	// wish; if you don't know what it is, you don't need it, just
	// add anything to attribs
	SXP::dict attribs;
	cfg.WriteElement(o, attribs);
	delete o; // the destructor of o closes the file
}
