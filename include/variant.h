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
	union value {
		short ShortValue;
		int IntValue;
		char CharValue;
		float FloatValue;
		double DoubleValue;
		char *StringValue;
		// this will use the raw datatype
		//mDateTime DateValue;
		bool BoolValue;
		unsigned char UCharValue;
		unsigned short UShortValue;
		unsigned int UIntValue;
		void *PtrValue;
	};
public:
	enum typeenum { VarEmpty, VarShort, VarInt, VarChar, 
		VarFloat, VarDouble, VarString, VarNull, VarDate, VarBoolean, VarUChar, VarUShort, VarUInt, VarPtr};
private:
	typeenum valuetype;

	
};

class mVarArray
{
	//vector<mVariant> values;
};

// todo
#define AOC(x)

#endif