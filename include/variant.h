// RCS-ID:      $Id$
#ifndef _VARIANT_H
#define _VARIANT_H
#include "mstring.h"
#include <vector>
using namespace std;

// based upon mechanisms prevalent in Delphi.


class mVariant
{
private:
	union value {
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

#endif