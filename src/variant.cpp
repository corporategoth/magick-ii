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
#include "trace.h"

const mVarArray EmptyArray;

// tracing taken out of mVariant now that we know it works so that we don't get infinite recursion.
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

mVariant::mVariant(const mstring& in)
{
	valuetype=VarString;
	value.StringValue=in.c_str();
}

mVariant::mVariant(bool in)
{
	valuetype=VarBool;
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
		if(mstring(value.StringValue)==mstring(in.value.StringValue))
			return true;
		else 
			return false;
	case VarUChar:
		if(value.UCharValue==in.value.UCharValue)
			return true;
		else 
			return false;
	case VarUInt:
		if(value.UIntValue==in.value.UIntValue)
			return true;
		else 
			return false;
	case VarUShort:
		if(value.UShortValue==in.value.UShortValue)
			return true;
		else 
			return false;
	case VarEmpty:
	case VarNull:
		return true;
	}
	// fall through, if it get's here major fubar, so put in logging mechanism when it's done
	return false;
}
bool mVariant::operator<(const mVariant& in)const
{
	if(in.valuetype!=valuetype)
		return false;
	switch(valuetype)
	{
	case VarBool:
		if((int)value.BoolValue<(int)in.value.BoolValue)
			return true;
		else 
			return false;
	case VarChar:
		if(value.CharValue<in.value.CharValue)
			return true;
		else 
			return false;
	case VarDouble:
		if(value.DoubleValue<in.value.DoubleValue)
			return true;
		else 
			return false;
	case VarFloat:
		if(value.FloatValue<in.value.FloatValue)
			return true;
		else 
			return false;
	case VarInt:
		if(value.IntValue<in.value.IntValue)
			return true;
		else 
			return false;
	case VarPtr:
		if(value.PtrValue<in.value.PtrValue)
			return true;
		else 
			return false;
	case VarShort:
		if(value.ShortValue<in.value.ShortValue)
			return true;
		else 
			return false;
	case VarString:
		if(mstring(value.StringValue)<mstring(in.value.StringValue))
			return true;
		else 
			return false;
	case VarUChar:
		if(value.UCharValue<in.value.UCharValue)
			return true;
		else 
			return false;
	case VarUInt:
		if(value.UIntValue<in.value.UIntValue)
			return true;
		else 
			return false;
	case VarUShort:
		if(value.UShortValue<in.value.UShortValue)
			return true;
		else 
			return false;
	case VarEmpty:
	case VarNull:
		return true;
	}
	// fall through, if it get's here major fubar, so put in logging mechanism when it's done
	return false;
}
mstring mVariant::AsString()const
{
    mstring dummystring="Empty";
    switch(valuetype)
    {
    case VarBool:
	if(value.BoolValue==true)
	    dummystring="true";
	else
	    dummystring="false";
	break;
    case VarChar:
	dummystring.Format("%c",value.CharValue);
	break;
    case VarDouble:
	dummystring.Format("%f",value.DoubleValue);
	break;
    case VarFloat:
	dummystring.Format("%f",value.FloatValue);
	break;
    case VarInt:
	dummystring.Format("%d",value.IntValue);
	break;
    case VarPtr:
	dummystring.Format("%p",value.PtrValue);
	break;
    case VarShort:
	dummystring.Format("%d",value.ShortValue);
	break;
    case VarString:
	dummystring=mstring(value.StringValue);
	break;
    case VarUChar:
	dummystring.Format("%u",value.UCharValue);
	break;
    case VarUInt:
	dummystring.Format("%u",value.UIntValue);
	break;
    case VarUShort:
	dummystring.Format("%u",value.UShortValue);
	break;
    case VarEmpty:
	break;
    case VarNull:
	dummystring="NULL";
	break;
    }
    return dummystring;
}
	
mstring mVariant::type()const
{
    switch(valuetype)
    {
    case VarBool:
	return "Bool";
    case VarChar:
	return "Char";
    case VarDouble:
	return "Double";
    case VarFloat:
	return "Float";
    case VarInt:
	return "Int";
    case VarPtr:
	return "Pointer";
    case VarShort:
	return "Short";
    case VarString:
	return "String";
    case VarUChar:
	return "Unsigned Char";
    case VarUInt:
	return "Unsigned Int";
    case VarUShort:
	return "Unsigned Short";
    case VarEmpty:
	return "Empty";
    case VarNull:
	return "Null Pointer";
    }
    // fall through, if it get's here major fubar, so put in logging mechanism when it's done
    return mstring("Error");
}

mVarArray::mVarArray(const mVariant& one)
{
    values.push_back(one);
}

mVarArray::mVarArray(const mVariant & one, const mVariant & two)
{
    values.push_back(one);
    values.push_back(two);
}
mVarArray::mVarArray(const mVariant& one, const mVariant& two, const mVariant& three)
{
    values.push_back(one);
    values.push_back(two);
    values.push_back(three);
}
mVarArray::mVarArray(const mVariant& one, const mVariant& two, const mVariant& three, const mVariant& four)
{
    values.push_back(one);
    values.push_back(two);
    values.push_back(three);
    values.push_back(four);
}
mVarArray::mVarArray(const mVariant& one, const mVariant& two, const mVariant& three, const mVariant& four,
	const mVariant& five)
{
    values.push_back(one);
    values.push_back(two);
    values.push_back(three);
    values.push_back(four);
    values.push_back(five);
}
mVarArray::mVarArray(const mVariant& one, const mVariant& two, const mVariant& three, const mVariant& four,
	const mVariant& five, const mVariant& six)
{
    values.push_back(one);
    values.push_back(two);
    values.push_back(three);
    values.push_back(four);
    values.push_back(five);
    values.push_back(six);
}
mVarArray::mVarArray(const mVariant& one, const mVariant& two, const mVariant& three, const mVariant& four,
	const mVariant& five, const mVariant& six, const mVariant& seven)
{
    values.push_back(one);
    values.push_back(two);
    values.push_back(three);
    values.push_back(four);
    values.push_back(five);
    values.push_back(six);
    values.push_back(seven);
}
mVarArray::mVarArray(const mVariant& one, const mVariant& two, const mVariant& three, const mVariant& four,
	const mVariant& five, const mVariant& six, const mVariant& seven, const mVariant& eight)
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
mVarArray::mVarArray(const mVariant& one, const mVariant& two, const mVariant& three, const mVariant& four,
	const mVariant& five, const mVariant& six, const mVariant& seven, const mVariant& eight,
	const mVariant& nine)
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
mVarArray::mVarArray(const mVariant& one, const mVariant& two, const mVariant& three, const mVariant& four,
	const mVariant& five, const mVariant& six, const mVariant& seven, const mVariant& eight,
	const mVariant& nine, const mVariant& ten)
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
mVarArray::mVarArray(const mVariant& one, const mVariant& two, const mVariant& three, const mVariant& four,
	const mVariant& five, const mVariant& six, const mVariant& seven, const mVariant& eight,
	const mVariant& nine, const mVariant& ten, const mVariant& eleven)
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
mVarArray::mVarArray(const mVariant& one, const mVariant& two, const mVariant& three, const mVariant& four,
	const mVariant& five, const mVariant& six, const mVariant& seven, const mVariant& eight,
	const mVariant& nine, const mVariant& ten, const mVariant& eleven, const mVariant& twelve)
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
mVarArray::mVarArray(const mVariant& one, const mVariant& two, const mVariant& three, const mVariant& four,
	const mVariant& five, const mVariant& six, const mVariant& seven, const mVariant& eight,
	const mVariant& nine, const mVariant& ten, const mVariant& eleven, const mVariant& twelve,
	const mVariant& thirteen)
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
mVarArray::mVarArray(const mVariant& one, const mVariant& two, const mVariant& three, const mVariant& four,
	const mVariant& five, const mVariant& six, const mVariant& seven, const mVariant& eight,
	const mVariant& nine, const mVariant& ten, const mVariant& eleven, const mVariant& twelve,
	const mVariant& thirteen, const mVariant& fourteen)
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
mVarArray::mVarArray(const mVariant& one, const mVariant& two, const mVariant& three, const mVariant& four,
	const mVariant& five, const mVariant& six, const mVariant& seven, const mVariant& eight,
	const mVariant& nine, const mVariant& ten, const mVariant& eleven, const mVariant& twelve,
	const mVariant& thirteen, const mVariant& fourteen, const mVariant& fifteen)
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
