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
#include "variant.h"

mVariant::mVariant()
{
	valuetype=VarEmpty;
}

mVariant::mVariant(short in)
{
	valuetype=VarShort;
	value.ShortValue=in;
}

mVariant::mVariant(int in)
{
	valuetype=VarInt;
	value.IntValue=in;
}

mVariant::mVariant(char in)
{
	valuetype=VarChar;
	value.CharValue=in;
}

mVariant::mVariant(float in)
{
	valuetype=VarFloat;
	value.FloatValue=in;
}

mVariant::mVariant(double in)
{
	valuetype=VarDouble;
	value.DoubleValue=in;
}

mVariant::mVariant(const char * in)
{
	if(in!=NULL)
	{
		valuetype=VarString;
		value.StringValue=in;
	}
	else
		valuetype=VarNull;
}

mVariant::mVariant(bool in)
{
	valuetype=VarBoolean;
	value.BoolValue=in;
}

mVariant::mVariant(unsigned char in)
{
	valuetype=VarUChar;
	value.UCharValue=in;
}

mVariant::mVariant(unsigned short in)
{
	valuetype=VarUShort;
	value.UShortValue=in;
}

mVariant::mVariant(unsigned int in)
{
	valuetype=VarUInt;
	value.UIntValue=in;
}

mVariant::mVariant(void * in)
{
	if(in!=NULL)
	{
		valuetype=VarPtr;
		value.PtrValue=in;
	}
	else
		valuetype=VarNull;
}

mVariant::mVariant(const mVariant & in)
{
	*this=in;
}

mVariant& mVariant::operator=(const mVariant& in)
{
	return *this;
}
bool mVariant::operator==(const mVariant& in)const
{
	if(valuetype!=in.valuetype)
		return false;
	switch(valuetype)
	{
	case VarBool:
		if(value.BoolValue==in.value.BoolValue)
			return true;
		else 
			return false;
	case VarChar:
		if(value.CharValue==in.value.CharValue)
			return true;
		else 
			return false;
	case VarDouble:
		if(value.DoubleValue==in.value.DoubleValue)
			return true;
		else 
			return false;
	case VarFloat:
		if(value.FloatValue==in.value.FloatValue)
			return true;
		else 
			return false;
	case VarInt:
		if(value.IntValue==in.value.IntValue)
			return true;
		else 
			return false;
	case VarPtr:
		if(value.PtrValue==in.value.PtrValue)
			return true;
		else 
			return false;
	case VarShort:
		if(value.ShortValue==in.value.ShortValue)
			return true;
		else 
			return false;
	case VarString:
		if(mstring(value)==mstring(in.value))
			return true;
		else 
			return false;
	case VarUChar:
		if(value.UCharValue==in.value.UCharValue)
			return true;
		else 
			return false;
	}
}
bool mVariant::operator<(const mVariant& in)const
{
	if(in.valuetype!=valuetype)
		return false;
	switch(valuetype)
	{
	}
}
