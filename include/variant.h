// $Id$
//
// Magick IRC Services
// (c) 1997-1999 Preston A. Elder <prez@magick.tm>
// (c) 1998-1999 W. King <ungod@magick.tm>
//
// The above copywright may not be removed under any
// circumstances, however it may be added to if any
// modifications are made to this file.  All modified
// code must be clearly documented and labelled.
//
// ===================================================
#ifndef _VARIANT_H
#define _VARIANT_H

#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include "mstring.h"
#include "datetime.h"
#include "textfile.h"
#include "fileconf.h"

// based upon mechanisms prevalent in Delphi.


class mVariant
{
private:
	//union value_union_t {
		short ShortValue;
		int IntValue;
		char CharValue;
		float FloatValue;
		double DoubleValue;
		//const char *StringValue;
		mstring StringValue;
		// this will use the raw datatype
		mDateTime DateValue;
		bool BoolValue;
		unsigned char UCharValue;
		unsigned short UShortValue;
		unsigned int UIntValue;
		void *PtrValue;
	//} value;
	
public:
	 mVariant(const char *in);
	mstring valuetype;
	mstring truevaluetype;
	// gonna change this to a mstring so we can use custom defined types
	mVariant(const mVariant& in);
	mVariant(void *in);
	mVariant(unsigned int in);
	mVariant(unsigned long in);
	mVariant(unsigned short in);
	mVariant(unsigned char in);
	mVariant(bool in);
	mVariant(double in);
	mVariant(float in);
	mVariant(char in);
	mVariant(int in);
	mVariant(long in);
	mVariant(short in);
	mVariant();
	// Magick Only stuff
	mVariant(const mstring& in);
	mVariant(mDateTime in);
	mVariant(wxTextFileType in);
	mVariant(LineList *in);
	mVariant(ConfigEntry *in);
	mVariant(ConfigGroup *in);

	mVariant& operator=(const mVariant& in);
	bool operator==(const mVariant& in)const;
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
    // if we need any more, you get the drift
    int count()const{return values.size();};
    const mVariant &operator[](int position)const{return values[position];};
    mVariant &operator[](int position){return values[position];};
};

extern const mVarArray EmptyArray;
// todo
#define AOC(x) mVarArray x

#endif