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
#ifndef _VARIANT_H
#define _VARIANT_H
#include "pch.h"
RCSID(variant_h, "@(#) $Id$");
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.34  2001/02/03 02:21:31  prez
** Loads of changes, including adding ALLOW to ini file, cleaning up
** the includes, RCSID, and much more.  Also cleaned up most warnings.
**
** Revision 1.32  2000/12/09 10:17:14  prez
** Added +h to unreal IRCD profile, and made variant more efficiant
** string wise by cutting out many operator= calls.
**
** Revision 1.31  2000/09/01 10:54:38  prez
** Added Changing and implemented Modify tracing, now just need to create
** DumpB() and DumpE() functions in all classes, and put MCB() / MCE() calls
** (or MB() / ME() or CB() / CE() where MCB() / MCE() not appropriate) in.
**
** Revision 1.30  2000/07/28 14:49:35  prez
** Ditched the old wx stuff, mconfig now in use, we're now ready to
** release (only got some conversion tests to do).
**
** Revision 1.29  2000/05/28 05:05:13  prez
** More makefile stuff ... Now we should work on all platforms.
** Added alot of checking for different .h files, functions, etc.
** So now all #define's are config.h based (also added a default
** windows config.h, which will need to be copied on these systems).
**
** Revision 1.28  2000/02/23 12:21:02  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.27  2000/02/15 10:37:47  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#include "datetime.h"

// based upon mechanisms prevalent in Delphi.
class mVariant
{
public:
    mstring truevaluetype;
    mstring valuetype;

private:
    //union value_union_t {
	bool BoolValue;
	short ShortValue;
	int IntValue;
	long LongValue;
	char CharValue;
	float FloatValue;
	double DoubleValue;
	unsigned char UCharValue;
	unsigned short UShortValue;
	unsigned int UIntValue;
	unsigned long ULongValue;
	void *PtrValue;

	// Magick only
	mstring StringValue;
	mDateTime DateValue;
    //} value;
	
public:
    mVariant();
    virtual ~mVariant() {}
    mVariant(const mVariant& in);
    mVariant(const bool in);
    mVariant(const char in);
    mVariant(const short in);
    mVariant(const int in);
    mVariant(const long in);
    mVariant(const float in);
    mVariant(const double in);
    mVariant(const unsigned char in);
    mVariant(const unsigned short in);
    mVariant(const unsigned int in);
    mVariant(const unsigned long in);
    mVariant(void *in);
    mVariant(const char *in);		// Stored as mstring
    mVariant(const string& in);		// Stored as mstring

    // Magick Only stuff
    mVariant(const mstring& in);
    mVariant(const mDateTime& in);

    mVariant& operator=(const mVariant& in);
    bool operator==(const mVariant& in)const;
    bool operator!=(const mVariant& in)const
	{ return !operator==(in); }
    bool operator<(const mVariant& in)const;

    mstring AsString()const;
    mstring type()const;
};

class mVarArray
{
    vector<mVariant> values;
public:
    mVarArray() { };
    mVarArray(const mVariant& one);
    mVarArray(const mVariant& one, const mVariant& two);
    mVarArray(const mVariant& one, const mVariant& two, const mVariant& three);
    mVarArray(const mVariant& one, const mVariant& two, const mVariant& three, const mVariant& four);
    mVarArray(const mVariant& one, const mVariant& two, const mVariant& three, const mVariant& four,
	const mVariant& five);
    mVarArray(const mVariant& one, const mVariant& two, const mVariant& three, const mVariant& four,
	const mVariant& five, const mVariant& six);
    mVarArray(const mVariant& one, const mVariant& two, const mVariant& three, const mVariant& four,
	const mVariant& five, const mVariant& six, const mVariant& seven);
    mVarArray(const mVariant& one, const mVariant& two, const mVariant& three, const mVariant& four,
	const mVariant& five, const mVariant& six, const mVariant& seven, const mVariant& eight);
    mVarArray(const mVariant& one, const mVariant& two, const mVariant& three, const mVariant& four,
	const mVariant& five, const mVariant& six, const mVariant& seven, const mVariant& eight,
	const mVariant& nine);
    mVarArray(const mVariant& one, const mVariant& two, const mVariant& three, const mVariant& four,
	const mVariant& five, const mVariant& six, const mVariant& seven, const mVariant& eight,
	const mVariant& nine, const mVariant& ten);
    mVarArray(const mVariant& one, const mVariant& two, const mVariant& three, const mVariant& four,
	const mVariant& five, const mVariant& six, const mVariant& seven, const mVariant& eight,
	const mVariant& nine, const mVariant& ten, const mVariant& eleven);
    mVarArray(const mVariant& one, const mVariant& two, const mVariant& three, const mVariant& four,
	const mVariant& five, const mVariant& six, const mVariant& seven, const mVariant& eight,
	const mVariant& nine, const mVariant& ten, const mVariant& eleven, const mVariant& twelve);
    mVarArray(const mVariant& one, const mVariant& two, const mVariant& three, const mVariant& four,
	const mVariant& five, const mVariant& six, const mVariant& seven, const mVariant& eight,
	const mVariant& nine, const mVariant& ten, const mVariant& eleven, const mVariant& twelve,
	const mVariant& thirteen);
    mVarArray(const mVariant& one, const mVariant& two, const mVariant& three, const mVariant& four,
	const mVariant& five, const mVariant& six, const mVariant& seven, const mVariant& eight,
	const mVariant& nine, const mVariant& ten, const mVariant& eleven, const mVariant& twelve,
	const mVariant& thirteen, const mVariant& fourteen);
    mVarArray(const mVariant& one, const mVariant& two, const mVariant& three, const mVariant& four,
	const mVariant& five, const mVariant& six, const mVariant& seven, const mVariant& eight,
	const mVariant& nine, const mVariant& ten, const mVariant& eleven, const mVariant& twelve,
	const mVariant& thirteen, const mVariant& fourteen, const mVariant& fifteen);
    mVarArray(const mVariant& one, const mVariant& two, const mVariant& three, const mVariant& four,
	const mVariant& five, const mVariant& six, const mVariant& seven, const mVariant& eight,
	const mVariant& nine, const mVariant& ten, const mVariant& eleven, const mVariant& twelve,
	const mVariant& thirteen, const mVariant& fourteen, const mVariant& fifteen,
	const mVariant& sixteen);
    // if we need any more, you get the drift
    int count()const{return values.size();};
    const mVariant &operator[](int position)const{return values[position];};
    mVariant &operator[](int position){return values[position];};
};

extern const mVarArray EmptyArray;
// todo
#define AOC(x) mVarArray x

#endif
