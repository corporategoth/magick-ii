// RCS-ID:      $Id$
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
		mstring StringValue;
		mDateTime DateValue;
		bool BoolValue;
		unsigned char UCharValue;
		unsigned short UShortValue;
		unsigned int UIntValue;
		void *PtrValue;
	};
	typeenum valuetype;
public:
	enum typeenum { VarEmpty, VarShort, VarInt, VarChar, 
		VarFloat, VarDouble, VarString, VarNull, VarDate, VarBoolean, VarUChar, VarUShort, VarUInt, VarPtr};
	
};

class mVarArray
{
	//vector<mVariant> values;
};

// todo
#define AOC(x)

#endif