#ifndef WIN32
#pragma interface
#endif
/*  Magick IRC Services
**
** (c) 1997-2001 Preston Elder <prez@magick.tm>
** (c) 1998-2001r William King <ungod@magick.tm>
**
** The above copywright may not be removed under any
** circumstances, however it may be added to if any
** modifications are made to this file.  All modified
** code must be clearly documented and labelled.
**
** ========================================================== */
#ifndef _VARIANT_H
#define _VARIANT_H
//#include "pch.h"
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
** Revision 1.39  2001/04/09 07:55:26  prez
** Fixed type of Empty
**
** Revision 1.38  2001/04/05 05:59:51  prez
** Turned off -fno-default-inline, and split up server.cpp, it should
** compile again with no special options, and have default inlines :)
**
** Revision 1.37  2001/04/02 02:13:27  prez
** Added inlines, fixed more of the exception code.
**
** Revision 1.36  2001/03/08 08:07:40  ungod
** fixes for bcc 5.5
**
** Revision 1.35  2001/02/03 03:20:33  prez
** Fixed up some differences in previous committed versions ...
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
    friend class Magick;
    friend class mBase;

    enum value_t { BOOL, CHAR, INT, LONG, SHORT,
	FLOAT, DOUBLE, UCHAR, USHORT, UINT,
	ULONG, PTR, MSTRING, MDATETIME, EMPTY };

    mstring truevaluetype;
    value_t valuetype;

//    union value_union_t {
	bool BoolValue;
	char CharValue;
	short ShortValue;
	int IntValue;
	long LongValue;
	float FloatValue;
	double DoubleValue;
	unsigned char UCharValue;
	unsigned short UShortValue;
	unsigned int UIntValue;
	unsigned long ULongValue;
	void *PtrValue;

	// Magick ONLY types ...
	mstring StringValue;
	mDateTime DateTimeValue;
//    } value;
	
public:
    mVariant()
	: valuetype(EMPTY) {}
    mVariant(const mVariant& in)
	{ *this = in; }
    mVariant(const bool in)
	: truevaluetype("bool"),
	  valuetype(BOOL),
	  BoolValue(in) {}
    mVariant(const char in)
	: truevaluetype("char"),
	  valuetype(CHAR),
	  CharValue(in) {}
    mVariant(const short in)
	: truevaluetype("short"),
	  valuetype(SHORT),
	  ShortValue(in) {}
    mVariant(const int in)
	: truevaluetype("int"),
	  valuetype(INT),
	  IntValue(in) {}
    mVariant(const long in)
	: truevaluetype("long"),
	  valuetype(LONG),
	  LongValue(in) {}
    mVariant(const float in)
	: truevaluetype("float"),
	  valuetype(FLOAT),
	  FloatValue(in) {}
    mVariant(const double in)
	: truevaluetype("double"),
	  valuetype(DOUBLE),
	  DoubleValue(in) {}
    mVariant(const unsigned char in)
	: truevaluetype("unsigned char"),
	  valuetype(UCHAR),
	  UCharValue(in) {}
    mVariant(const unsigned short in)
	: truevaluetype("unsigned short"),
	  valuetype(USHORT),
	  UShortValue(in) {}
    mVariant(const unsigned int in)
	: truevaluetype("unsigned int"),
	  valuetype(UINT),
	  UIntValue(in) {}
    mVariant(const unsigned long in)
	: truevaluetype("unsigned long"),
	  valuetype(ULONG),
	  ULongValue(in) {}
    mVariant(void * in)
	: truevaluetype("void *"),
	  valuetype(PTR),
	  PtrValue(in) {}

    // Magick ONLY types ...
    mVariant(const mstring& in)
	: truevaluetype("mstring"),
	  valuetype(MSTRING),
	  StringValue(in) {}
    mVariant(const mDateTime& in)
	: truevaluetype("mDateTime"),
	  valuetype(MDATETIME),
	  DateTimeValue(in) {}

    // Aliases ...
    mVariant(const char * in)
	: truevaluetype("char *"),
	  valuetype(MSTRING),
	  StringValue(in) {}
    mVariant(const string& in)
	: truevaluetype("string"),
	  valuetype(MSTRING),
	  StringValue(in) {}

    ~mVariant() {}

    mVariant& operator=(const mVariant& in);
    bool operator==(const mVariant& in)const;
    bool operator!=(const mVariant& in)const
	{ return !operator==(in); }
    bool operator<(const mVariant& in)const;

    mstring AsString()const;
    mstring type()const
	{ return truevaluetype; }
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
    int count()const
	{ return values.size(); }
    const mVariant &operator[](int position)const
	{ return values[position]; }
    mVariant &operator[](int position)
	{ return values[position]; }
    static const mVarArray EmptyArray()
	{
	    const mVarArray Result;
	    return Result;
	}

};

// todo
#define AOC(x) mVarArray x

#endif
