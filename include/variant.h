#ifndef WIN32
#pragma interface
#endif

/* Magick IRC Services
**
** (c) 1997-2003 Preston Elder <prez@magick.tm>
** (c) 1998-2003 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any circumstances,
** however it may be added to if any modifications are made to this
** file.  All modified code must be clearly documented and labelled.
**
** This code is released under the Artistic License v2.0 or better.
** The full text of this license should be contained in a file called
** COPYING distributed with this code.  If this file does not exist,
** it may be viewed here: http://www.magick.tm/m2/license.html
**
** ======================================================================= */
#ifndef _VARIANT_H
#define _VARIANT_H
//#include "pch.h"
RCSID(variant_h, "@(#) $Id$");

/* ======================================================================= **
**
** For official changes (by the Magick Development Team),please
** check the ChangeLog* files that come with this distribution.
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** ======================================================================= */

#include "datetime.h"

// based upon mechanisms prevalent in Delphi.
class mVariant
{
    friend class Magick;
    friend class mBase;

    enum value_t
    { BOOL, CHAR, INT, LONG, SHORT,
	FLOAT, DOUBLE, UCHAR, USHORT, UINT,
	ULONG, PTR, MSTRING, MDATETIME, EMPTY
    };

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
    mVariant() : valuetype(EMPTY)
    {
    }
    mVariant(const mVariant & in)
    {
	*this = in;
    }
    mVariant(const bool in) : truevaluetype("bool"), valuetype(BOOL), BoolValue(in)
    {
    }
    mVariant(const char in) : truevaluetype("char"), valuetype(CHAR), CharValue(in)
    {
    }
    mVariant(const short in) : truevaluetype("short"), valuetype(SHORT), ShortValue(in)
    {
    }
    mVariant(const int in) : truevaluetype("int"), valuetype(INT), IntValue(in)
    {
    }
    mVariant(const long in) : truevaluetype("long"), valuetype(LONG), LongValue(in)
    {
    }
    mVariant(const float in) : truevaluetype("float"), valuetype(FLOAT), FloatValue(in)
    {
    }
    mVariant(const double in) : truevaluetype("double"), valuetype(DOUBLE), DoubleValue(in)
    {
    }
    mVariant(const unsigned char in) : truevaluetype("unsigned char"), valuetype(UCHAR), UCharValue(in)
    {
    }
    mVariant(const unsigned short in) : truevaluetype("unsigned short"), valuetype(USHORT), UShortValue(in)
    {
    }
    mVariant(const unsigned int in) : truevaluetype("unsigned int"), valuetype(UINT), UIntValue(in)
    {
    }
    mVariant(const unsigned long in) : truevaluetype("unsigned long"), valuetype(ULONG), ULongValue(in)
    {
    }
    mVariant(void *in) : truevaluetype("void *"), valuetype(PTR), PtrValue(in)
    {
    }

    // Magick ONLY types ...
    mVariant(const mstring & in) : truevaluetype("mstring"), valuetype(MSTRING), StringValue(in)
    {
    }
    mVariant(const mDateTime & in) : truevaluetype("mDateTime"), valuetype(MDATETIME), DateTimeValue(in)
    {
    }

    // Aliases ...
    mVariant(const char *in) : truevaluetype("char *"), valuetype(MSTRING), StringValue(in)
    {
    }
    mVariant(const string & in) : truevaluetype("string"), valuetype(MSTRING), StringValue(in)
    {
    }

    ~mVariant()
    {
    }

    mVariant &operator=(const mVariant & in);
    bool operator==(const mVariant & in) const;
    bool operator!=(const mVariant & in) const
    {
	return !operator==(in);
    }
    bool operator<(const mVariant & in) const;

    mstring AsString() const;
    mstring type() const
    {
	return truevaluetype;
    }
};

class mVarArray
{
    vector < mVariant > values;
public:
    mVarArray()
    {
    };

    mVarArray(const mVariant & one);
    mVarArray(const mVariant & one, const mVariant & two);
    mVarArray(const mVariant & one, const mVariant & two, const mVariant & three);
    mVarArray(const mVariant & one, const mVariant & two, const mVariant & three, const mVariant & four);
    mVarArray(const mVariant & one, const mVariant & two, const mVariant & three, const mVariant & four,
	      const mVariant & five);
    mVarArray(const mVariant & one, const mVariant & two, const mVariant & three, const mVariant & four, const mVariant & five,
	      const mVariant & six);
    mVarArray(const mVariant & one, const mVariant & two, const mVariant & three, const mVariant & four, const mVariant & five,
	      const mVariant & six, const mVariant & seven);
    mVarArray(const mVariant & one, const mVariant & two, const mVariant & three, const mVariant & four, const mVariant & five,
	      const mVariant & six, const mVariant & seven, const mVariant & eight);
    mVarArray(const mVariant & one, const mVariant & two, const mVariant & three, const mVariant & four, const mVariant & five,
	      const mVariant & six, const mVariant & seven, const mVariant & eight, const mVariant & nine);
    mVarArray(const mVariant & one, const mVariant & two, const mVariant & three, const mVariant & four, const mVariant & five,
	      const mVariant & six, const mVariant & seven, const mVariant & eight, const mVariant & nine,
	      const mVariant & ten);
    mVarArray(const mVariant & one, const mVariant & two, const mVariant & three, const mVariant & four, const mVariant & five,
	      const mVariant & six, const mVariant & seven, const mVariant & eight, const mVariant & nine,
	      const mVariant & ten, const mVariant & eleven);
    mVarArray(const mVariant & one, const mVariant & two, const mVariant & three, const mVariant & four, const mVariant & five,
	      const mVariant & six, const mVariant & seven, const mVariant & eight, const mVariant & nine,
	      const mVariant & ten, const mVariant & eleven, const mVariant & twelve);
    mVarArray(const mVariant & one, const mVariant & two, const mVariant & three, const mVariant & four, const mVariant & five,
	      const mVariant & six, const mVariant & seven, const mVariant & eight, const mVariant & nine,
	      const mVariant & ten, const mVariant & eleven, const mVariant & twelve, const mVariant & thirteen);
    mVarArray(const mVariant & one, const mVariant & two, const mVariant & three, const mVariant & four, const mVariant & five,
	      const mVariant & six, const mVariant & seven, const mVariant & eight, const mVariant & nine,
	      const mVariant & ten, const mVariant & eleven, const mVariant & twelve, const mVariant & thirteen,
	      const mVariant & fourteen);
    mVarArray(const mVariant & one, const mVariant & two, const mVariant & three, const mVariant & four, const mVariant & five,
	      const mVariant & six, const mVariant & seven, const mVariant & eight, const mVariant & nine,
	      const mVariant & ten, const mVariant & eleven, const mVariant & twelve, const mVariant & thirteen,
	      const mVariant & fourteen, const mVariant & fifteen);
    mVarArray(const mVariant & one, const mVariant & two, const mVariant & three, const mVariant & four, const mVariant & five,
	      const mVariant & six, const mVariant & seven, const mVariant & eight, const mVariant & nine,
	      const mVariant & ten, const mVariant & eleven, const mVariant & twelve, const mVariant & thirteen,
	      const mVariant & fourteen, const mVariant & fifteen, const mVariant & sixteen);
    // if we need any more, you get the drift
    int count() const
    {
	return values.size();
    }
    const mVariant &operator [] (int position) const
    {
	return values[position];
    }
    mVariant &operator [] (int position)
    {
	return values[position];
    }
    static const mVarArray EmptyArray()
    {
	const mVarArray Result;

	return Result;
    }

};

// todo
#define AOC(x) mVarArray x

#endif
