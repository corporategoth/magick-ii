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
    truevaluetype=valuetype="VarEmpty";
    
}

mVariant::mVariant(short in)
{
    truevaluetype=valuetype="VarShort";
    value.ShortValue=in;
}

mVariant::mVariant(int in)
{
    truevaluetype=valuetype="VarInt";
    value.IntValue=in;
}

mVariant::mVariant(long in)
{
    truevaluetype=valuetype="VarInt";
    value.IntValue=in;
}

mVariant::mVariant(char in)
{
    truevaluetype=valuetype="VarChar";
    value.CharValue=in;
}

mVariant::mVariant(float in)
{
    truevaluetype=valuetype="VarFloat";
    value.FloatValue=in;
}

mVariant::mVariant(double in)
{
    truevaluetype=valuetype="VarDouble";
    value.DoubleValue=in;
}

mVariant::mVariant(const char * in)
{
	if(in!=NULL)
	{
		truevaluetype=valuetype="VarString";
		value.StringValue=in;
	}
	else
		truevaluetype=valuetype="VarNull";
}

mVariant::mVariant(const mstring& in)
{
	truevaluetype="mstring";
	valuetype="VarString";
	value.StringValue=in.c_str();
}

mVariant::mVariant(bool in)
{
	truevaluetype=valuetype="VarBool";
	value.BoolValue=in;
}

mVariant::mVariant(unsigned char in)
{
	truevaluetype=valuetype="VarUChar";
	value.UCharValue=in;
}

mVariant::mVariant(unsigned short in)
{
	truevaluetype=valuetype="VarUShort";
	value.UShortValue=in;
}

mVariant::mVariant(unsigned int in)
{
	truevaluetype=valuetype="VarUInt";
	value.UIntValue=in;
}

mVariant::mVariant(unsigned long in)
{
	truevaluetype=valuetype="VarUInt";
	value.UIntValue=in;
}

mVariant::mVariant(void * in)
{
	if(in!=NULL)
	{
		truevaluetype=valuetype="VarPtr";
		value.PtrValue=in;
	}
	else
		truevaluetype=valuetype="VarNull";
}

mVariant::mVariant(const mVariant & in)
{
	*this=in;
}

mVariant::mVariant(wxTextFileType in)
{
    truevaluetype="wxTextFileType";
    valuetype="VarString";
    switch(in)
    {
    case wxTextFileType_None:
	value.StringValue="wxTextFileType_None";
	break;
    case wxTextFileType_Unix:
	value.StringValue="wxTextFileType_Unix";
	break;
    case wxTextFileType_Dos:
	value.StringValue="wxTextFileType_Dos";
	break;
    case wxTextFileType_Mac:
	value.StringValue="wxTextFileType_Mac";
	break;
    }
}

mVariant::mVariant(LineList *in)
{
	truevaluetype="LineList"
	valuetype="VarString";
	value.StringValue=(in->Text()).c_str();
}

mVariant::mVariant(ConfigEntry *in)
{
	truevaluetype="ConfigEntry"
	valuetype="VarString";
	value.StringValue=(in->Name() + "=" + in->Value()).c_str();
}

mVariant::mVariant(ConfigGroup *in)
{
	truevaluetype="ConfigGroup"
	valuetype="VarString";
	value.StringValue=(in->Name()).c_str();
}

mVariant& mVariant::operator=(const mVariant& in)
{
	return *this;
}
bool mVariant::operator==(const mVariant& in)const
{
	if(valuetype!=in.valuetype)
		return false;
	if(valuetype=="VarBool")
	{
	    if(value.BoolValue==in.value.BoolValue)
		return true;
	    else 
	    	return false;
	}
	else if(valuetype=="VarChar")
	{
	    if(value.CharValue==in.value.CharValue)
		return true;
	    else 
		return false;
	}
	else if(valuetype=="VarDouble")
	{
	    if(value.DoubleValue==in.value.DoubleValue)
		return true;
	    else 
		return false;
	}
	else if(valuetype=="VarFloat")
	{
	    if(value.FloatValue==in.value.FloatValue)
		return true;
	    else 
		return false;
	}
	else if(valuetype=="VarInt")
	{
	    if(value.IntValue==in.value.IntValue)
		return true;
	    else 
		return false;
	}
	else if(valuetype=="VarPtr")
	{
	    if(value.PtrValue==in.value.PtrValue)
		return true;
	    else 
	    	return false;
	}
	else if(valuetype=="VarShort")
	{
	    if(value.ShortValue==in.value.ShortValue)
	    	return true;
	    else 
	    	return false;
	}
	else if(valuetype=="VarString")
	{
	    if(mstring(value.StringValue)==mstring(in.value.StringValue))
		return true;
	    else 
		return false;
	}
	else if(valuetype=="VarUChar")
	{
	    if(value.UCharValue==in.value.UCharValue)
		return true;
	    else 
		return false;
	}
	else if(valuetype=="VarUInt")
	{
	    if(value.UIntValue==in.value.UIntValue)
		return true;
	    else 
		return false;
	}
	else if(valuetype=="VarUShort")
	{
	    if(value.UShortValue==in.value.UShortValue)
	    	return true;
	    else 
	    	return false;
	}
	else if(valuetype=="VarEmpty"||valuetype=="VarNull")
		return true;
	// fall through, if it get's here major fubar, so put in logging mechanism when it's done
	else return false;
}
bool mVariant::operator<(const mVariant& in)const
{
	if(in.valuetype!=valuetype)
	    return false;
	if(valuetype=="VarBool")
	{
	    if((int)value.BoolValue<(int)in.value.BoolValue)
	    	return true;
	    else 
	    	return false;
	}
	else if(valuetype=="VarChar")
	{
	    if(value.CharValue<in.value.CharValue)
		return true;
	    else 
		return false;
	}
	else if(valuetype=="VarDouble")
	{
	    if(value.DoubleValue<in.value.DoubleValue)
		return true;
	    else 
		return false;
	}
	else if(valuetype=="VarFloat")
	{
	    if(value.FloatValue<in.value.FloatValue)
		return true;
	    else 
		return false;
	}
	else if(valuetype=="VarInt")
	{
	    if(value.IntValue<in.value.IntValue)
		return true;
	    else 
		return false;
	}
	else if(valuetype=="VarPtr")
	{
	    if(value.PtrValue<in.value.PtrValue)
		return true;
	    else 
		return false;
	}
	else if(valuetype=="VarShort")
	{
	    if(value.ShortValue<in.value.ShortValue)
		return true;
	    else 
		return false;
	}
	else if(valuetype=="VarString")
	{
	    if(mstring(value.StringValue)<mstring(in.value.StringValue))
		return true;
	    else 
		return false;
	}
	else if(valuetype=="VarUChar")
	{
	    if(value.UCharValue<in.value.UCharValue)
		return true;
	    else 
		return false;
	}
	else if(valuetype=="VarUInt")
	{
	    if(value.UIntValue<in.value.UIntValue)
		return true;
	    else 
		return false;
	}
	else if(valuetype=="VarUShort")
	{
	    if(value.UShortValue<in.value.UShortValue)
		return true;
	    else 
		return false;
	}
	else if(valuetype=="VarEmpty"||valuetype=="VarNull")
	    return true;
	// fall through, if it get's here major fubar, so put in logging mechanism when it's done
	else 
	    return false;
}
mstring mVariant::AsString()const
{
    mstring dummystring="Empty";
    if(valuetype=="VarBool")
    {
	if(value.BoolValue==true)
	    dummystring="true";
	else
	    dummystring="false";
    }
    else if(valuetype=="VarChar")
	dummystring.Format("%c",value.CharValue);
    else if(valuetype=="VarDouble")
	dummystring.Format("%f",value.DoubleValue);
    else if(valuetype=="VarFloat")
	dummystring.Format("%f",value.FloatValue);
    else if(valuetype=="VarInt")
	dummystring.Format("%d",value.IntValue);
    else if(valuetype=="VarPtr")
	dummystring.Format("%p",value.PtrValue);
    else if(valuetype=="VarShort")
	dummystring.Format("%d",value.ShortValue);
    else if(valuetype=="VarString")
	dummystring=mstring(value.StringValue);
    else if(valuetype=="VarUChar")
	dummystring.Format("%u",value.UCharValue);
    else if(valuetype=="VarUInt")
	dummystring.Format("%u",value.UIntValue);
    else if(valuetype=="VarUShort")
	dummystring.Format("%u",value.UShortValue);
    else if(valuetype=="VarNull")
	dummystring="NULL";

    return dummystring;
}
	
mstring mVariant::type()const
{
    return truevaluetype;
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
