#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
#endif

/* Magick IRC Services
**
** (c) 1997-2002 Preston Elder <prez@magick.tm>
** (c) 1998-2002 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any circumstances,
** however it may be added to if any modifications are made to this
** file.  All modified code must be clearly documented and labelled.
**
** This code is released under the GNU General Public License, which
** means (in short), it may be distributed freely, and may not be sold
** or used as part of any closed-source product.  Please check the
** COPYING file for full rights and restrictions of this software.
**
** ======================================================================= */
#define RCSID(x,y) const char *rcsid_variant_cpp_ ## x () { return y; }
RCSID(variant_cpp, "@(#)$Id$");

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

// gah microsoft sucks, the below is to make sure our header get's included, not ms's
#include "../include/variant.h"

mVariant &mVariant::operator=(const mVariant & in)
{
    valuetype = in.valuetype;
    truevaluetype = in.truevaluetype;
    switch (valuetype)
    {
    case BOOL:
	BoolValue = in.BoolValue;
	break;
    case CHAR:
	CharValue = in.CharValue;
	break;
    case SHORT:
	ShortValue = in.ShortValue;
	break;
    case INT:
	IntValue = in.IntValue;
	break;
    case LONG:
	LongValue = in.LongValue;
	break;
    case FLOAT:
	FloatValue = in.FloatValue;
	break;
    case DOUBLE:
	DoubleValue = in.DoubleValue;
	break;
    case UCHAR:
	UCharValue = in.UCharValue;
	break;
    case USHORT:
	UShortValue = in.UShortValue;
	break;
    case UINT:
	UIntValue = in.UIntValue;
	break;
    case ULONG:
	ULongValue = in.ULongValue;
	break;
    case PTR:
	PtrValue = in.PtrValue;
	break;
    case MSTRING:
	StringValue = in.StringValue;
	break;
    case MDATETIME:
	DateTimeValue = in.DateTimeValue;
	break;
    case EMPTY:
	break;
    }
    return *this;
}

bool mVariant::operator==(const mVariant & in) const
{
    if (valuetype != in.valuetype)
	return false;

    switch (valuetype)
    {
    case BOOL:
	return (BoolValue == in.BoolValue);
    case CHAR:
	return (CharValue == in.CharValue);
    case SHORT:
	return (ShortValue == in.ShortValue);
    case INT:
	return (IntValue == in.IntValue);
    case LONG:
	return (LongValue == in.LongValue);
    case FLOAT:
	return (FloatValue == in.FloatValue);
    case DOUBLE:
	return (DoubleValue == in.DoubleValue);
    case UCHAR:
	return (UCharValue == in.UCharValue);
    case USHORT:
	return (UShortValue == in.UShortValue);
    case UINT:
	return (UIntValue == in.UIntValue);
    case ULONG:
	return (ULongValue == in.ULongValue);
    case PTR:
	return (PtrValue == in.PtrValue);
    case MSTRING:
	return (StringValue == in.StringValue);
    case MDATETIME:
	return (DateTimeValue == in.DateTimeValue);
    case EMPTY:
	return true;
    default:
	return false;
    }
}
bool mVariant::operator<(const mVariant & in) const
{
    if (valuetype != in.valuetype)
	return false;

    switch (valuetype)
    {
    case BOOL:
	return (static_cast < int > (BoolValue) < static_cast < int > (in.BoolValue));
    case CHAR:
	return (CharValue < in.CharValue);
    case SHORT:
	return (ShortValue < in.ShortValue);
    case INT:
	return (IntValue < in.IntValue);
    case LONG:
	return (LongValue < in.LongValue);
    case FLOAT:
	return (FloatValue < in.FloatValue);
    case DOUBLE:
	return (DoubleValue < in.DoubleValue);
    case UCHAR:
	return (UCharValue < in.UCharValue);
    case USHORT:
	return (UShortValue < in.UShortValue);
    case UINT:
	return (UIntValue < in.UIntValue);
    case ULONG:
	return (ULongValue < in.ULongValue);
    case PTR:
	return (PtrValue < in.PtrValue);
    case MSTRING:
	return (StringValue < in.StringValue);
    case MDATETIME:
	return (DateTimeValue < in.DateTimeValue);
    case EMPTY:
	return true;
    default:
	return false;
    }
}

mstring mVariant::AsString() const
{
    mstring dummystring;

    switch (valuetype)
    {
    case BOOL:
	if (BoolValue == true)
	    dummystring = "true";
	else
	    dummystring = "false";
	break;
    case CHAR:
	dummystring.Format("%c", CharValue);
	break;
    case SHORT:
	dummystring.Format("%d", ShortValue);
	break;
    case INT:
	dummystring.Format("%d", IntValue);
	break;
    case LONG:
	dummystring.Format("%d", LongValue);
	break;
    case FLOAT:
	dummystring.Format("%f", FloatValue);
	break;
    case DOUBLE:
	dummystring.Format("%f", DoubleValue);
	break;
    case UCHAR:
	dummystring.Format("%u", UCharValue);
	break;
    case USHORT:
	dummystring.Format("%u", UShortValue);
	break;
    case UINT:
	dummystring.Format("%u", UIntValue);
	break;
    case ULONG:
	dummystring.Format("%u", ULongValue);
	break;
    case PTR:
	dummystring.Format("%p", PtrValue);
	break;
    case MSTRING:
	dummystring = StringValue;
	break;
    case MDATETIME:
	dummystring = DateTimeValue.DateTimeString();
	break;
    case EMPTY:
	break;
    }

    return dummystring;
}

mVarArray::mVarArray(const mVariant & one)
{
    values.push_back(one);
}

mVarArray::mVarArray(const mVariant & one, const mVariant & two)
{
    values.push_back(one);
    values.push_back(two);
}
mVarArray::mVarArray(const mVariant & one, const mVariant & two, const mVariant & three)
{
    values.push_back(one);
    values.push_back(two);
    values.push_back(three);
}
mVarArray::mVarArray(const mVariant & one, const mVariant & two, const mVariant & three, const mVariant & four)
{
    values.push_back(one);
    values.push_back(two);
    values.push_back(three);
    values.push_back(four);
}
mVarArray::mVarArray(const mVariant & one, const mVariant & two, const mVariant & three, const mVariant & four,
		     const mVariant & five)
{
    values.push_back(one);
    values.push_back(two);
    values.push_back(three);
    values.push_back(four);
    values.push_back(five);
}
mVarArray::mVarArray(const mVariant & one, const mVariant & two, const mVariant & three, const mVariant & four,
		     const mVariant & five, const mVariant & six)
{
    values.push_back(one);
    values.push_back(two);
    values.push_back(three);
    values.push_back(four);
    values.push_back(five);
    values.push_back(six);
}
mVarArray::mVarArray(const mVariant & one, const mVariant & two, const mVariant & three, const mVariant & four,
		     const mVariant & five, const mVariant & six, const mVariant & seven)
{
    values.push_back(one);
    values.push_back(two);
    values.push_back(three);
    values.push_back(four);
    values.push_back(five);
    values.push_back(six);
    values.push_back(seven);
}
mVarArray::mVarArray(const mVariant & one, const mVariant & two, const mVariant & three, const mVariant & four,
		     const mVariant & five, const mVariant & six, const mVariant & seven, const mVariant & eight)
{
    values.push_back(one);
    values.push_back(two);
    values.push_back(three);
    values.push_back(four);
    values.push_back(five);
    values.push_back(six);
    values.push_back(seven);
    values.push_back(eight);
}
mVarArray::mVarArray(const mVariant & one, const mVariant & two, const mVariant & three, const mVariant & four,
		     const mVariant & five, const mVariant & six, const mVariant & seven, const mVariant & eight,
		     const mVariant & nine)
{
    values.push_back(one);
    values.push_back(two);
    values.push_back(three);
    values.push_back(four);
    values.push_back(five);
    values.push_back(six);
    values.push_back(seven);
    values.push_back(eight);
    values.push_back(nine);
}
mVarArray::mVarArray(const mVariant & one, const mVariant & two, const mVariant & three, const mVariant & four,
		     const mVariant & five, const mVariant & six, const mVariant & seven, const mVariant & eight,
		     const mVariant & nine, const mVariant & ten)
{
    values.push_back(one);
    values.push_back(two);
    values.push_back(three);
    values.push_back(four);
    values.push_back(five);
    values.push_back(six);
    values.push_back(seven);
    values.push_back(eight);
    values.push_back(nine);
    values.push_back(ten);
}
mVarArray::mVarArray(const mVariant & one, const mVariant & two, const mVariant & three, const mVariant & four,
		     const mVariant & five, const mVariant & six, const mVariant & seven, const mVariant & eight,
		     const mVariant & nine, const mVariant & ten, const mVariant & eleven)
{
    values.push_back(one);
    values.push_back(two);
    values.push_back(three);
    values.push_back(four);
    values.push_back(five);
    values.push_back(six);
    values.push_back(seven);
    values.push_back(eight);
    values.push_back(nine);
    values.push_back(ten);
    values.push_back(eleven);
}
mVarArray::mVarArray(const mVariant & one, const mVariant & two, const mVariant & three, const mVariant & four,
		     const mVariant & five, const mVariant & six, const mVariant & seven, const mVariant & eight,
		     const mVariant & nine, const mVariant & ten, const mVariant & eleven, const mVariant & twelve)
{
    values.push_back(one);
    values.push_back(two);
    values.push_back(three);
    values.push_back(four);
    values.push_back(five);
    values.push_back(six);
    values.push_back(seven);
    values.push_back(eight);
    values.push_back(nine);
    values.push_back(ten);
    values.push_back(eleven);
    values.push_back(twelve);
}
mVarArray::mVarArray(const mVariant & one, const mVariant & two, const mVariant & three, const mVariant & four,
		     const mVariant & five, const mVariant & six, const mVariant & seven, const mVariant & eight,
		     const mVariant & nine, const mVariant & ten, const mVariant & eleven, const mVariant & twelve,
		     const mVariant & thirteen)
{
    values.push_back(one);
    values.push_back(two);
    values.push_back(three);
    values.push_back(four);
    values.push_back(five);
    values.push_back(six);
    values.push_back(seven);
    values.push_back(eight);
    values.push_back(nine);
    values.push_back(ten);
    values.push_back(eleven);
    values.push_back(twelve);
    values.push_back(thirteen);
}
mVarArray::mVarArray(const mVariant & one, const mVariant & two, const mVariant & three, const mVariant & four,
		     const mVariant & five, const mVariant & six, const mVariant & seven, const mVariant & eight,
		     const mVariant & nine, const mVariant & ten, const mVariant & eleven, const mVariant & twelve,
		     const mVariant & thirteen, const mVariant & fourteen)
{
    values.push_back(one);
    values.push_back(two);
    values.push_back(three);
    values.push_back(four);
    values.push_back(five);
    values.push_back(six);
    values.push_back(seven);
    values.push_back(eight);
    values.push_back(nine);
    values.push_back(ten);
    values.push_back(eleven);
    values.push_back(twelve);
    values.push_back(thirteen);
    values.push_back(fourteen);
}
mVarArray::mVarArray(const mVariant & one, const mVariant & two, const mVariant & three, const mVariant & four,
		     const mVariant & five, const mVariant & six, const mVariant & seven, const mVariant & eight,
		     const mVariant & nine, const mVariant & ten, const mVariant & eleven, const mVariant & twelve,
		     const mVariant & thirteen, const mVariant & fourteen, const mVariant & fifteen)
{
    values.push_back(one);
    values.push_back(two);
    values.push_back(three);
    values.push_back(four);
    values.push_back(five);
    values.push_back(six);
    values.push_back(seven);
    values.push_back(eight);
    values.push_back(nine);
    values.push_back(ten);
    values.push_back(eleven);
    values.push_back(twelve);
    values.push_back(thirteen);
    values.push_back(fourteen);
    values.push_back(fifteen);
}
mVarArray::mVarArray(const mVariant & one, const mVariant & two, const mVariant & three, const mVariant & four,
		     const mVariant & five, const mVariant & six, const mVariant & seven, const mVariant & eight,
		     const mVariant & nine, const mVariant & ten, const mVariant & eleven, const mVariant & twelve,
		     const mVariant & thirteen, const mVariant & fourteen, const mVariant & fifteen, const mVariant & sixteen)
{
    values.push_back(one);
    values.push_back(two);
    values.push_back(three);
    values.push_back(four);
    values.push_back(five);
    values.push_back(six);
    values.push_back(seven);
    values.push_back(eight);
    values.push_back(nine);
    values.push_back(ten);
    values.push_back(eleven);
    values.push_back(twelve);
    values.push_back(thirteen);
    values.push_back(fourteen);
    values.push_back(fifteen);
    values.push_back(sixteen);
}
