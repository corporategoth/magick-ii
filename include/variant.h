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
#include "mstring.h"
#include "datetime.h"
#include <vector>
using namespace std;

// based upon mechanisms prevalent in Delphi.


class mVariant
{
private:
	union value_union_t {
		short ShortValue;
		int IntValue;
		char CharValue;
		float FloatValue;
		double DoubleValue;
		const char *StringValue;
		// this will use the raw datatype
		//mDateTime DateValue;
		bool BoolValue;
		unsigned char UCharValue;
		unsigned short UShortValue;
		unsigned int UIntValue;
		void *PtrValue;
	} value;
public:
	enum typeenum { VarEmpty, VarShort, VarInt, VarChar, 
		VarFloat, VarDouble, VarString, VarNull, VarDate, VarBoolean, VarUChar, VarUShort, VarUInt, VarPtr};
private:
	typeenum valuetype;
public:
	 mVariant(const mVariant& in);
	 mVariant(void *in);
	 mVariant(unsigned int in);
	 mVariant(unsigned short in);
	 mVariant(unsigned char in);
	 mVariant(bool in);
	 mVariant(const char *in);
	 mVariant(double in);
	 mVariant(float in);
	 mVariant(char in);
	 mVariant(int in);
	 mVariant(short in);
	 mVariant();

	 mVariant& operator=(const mVariant& in);
	 bool operator==(const mVariant& in)const;
	 bool operator<(const mVariant& in)const;
	 mstring &AsString();
	
	 mstring &type();
};

class mVarArray
{
	vector<mVariant> values;
public:
	int count()const;
	mVariant &operator[](int position)const;
};

// todo
#define AOC(x)

#endif