// $Id$
#include "xml/sxp.h"

SXP_NS_BEGIN
//	std::string TagHashtable::table[SXP_HTSIZE];
	TagHashtable *g_pHashTable = 0;

	// add the tag to the table right on construction
	Tag::Tag(const char *name) 
	{
		ch = name;
		dw = ~0;
		TagHashtable::TagHT().Add(*this);
	}

	void CFileOutStream::WriteSubElement(IPersistObj *pObj, dict& attribs) {
		pObj->WriteElement(this, attribs);
	}

SXP_NS_END

