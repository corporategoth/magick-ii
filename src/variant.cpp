#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
#endif

/*  Magick IRC Services
**
** (c) 1997-2000 Preston Elder <prez@magick.tm>
** (c) 1998-2000 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any
** circumstances, however it may be added to if any
** modifications are made to this file.  All modified
** code must be clearly documented and labelled.
**
** ========================================================== */
static const char *ident = "@(#)$Id$";
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.38  2000/05/20 16:05:07  prez
** Finished off the log conversion (still via. wrappers)
**
** Revision 1.37  2000/03/28 16:20:59  prez
** LOTS of RET() fixes, they should now be safe and not do double
** calculations.  Also a few bug fixes from testing.
**
** Revision 1.36  2000/03/28 15:07:40  ungod
** fix for mVariant(long)
**
** Revision 1.35  2000/02/27 03:58:41  prez
** Fixed the WHAT program, also removed RegEx from Magick.
**
** Revision 1.34  2000/02/23 12:21:04  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.33  2000/02/16 12:59:41  ungod
** fixing for borland compilability
**
** Revision 1.32  2000/02/15 13:27:04  prez
** *** empty log message ***
**
** Revision 1.31  2000/02/15 10:37:51  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#include "variant.h"
#include "trace.h"

const mVarArray EmptyArray;

// tracing taken out of mVariant now that we know it works so that we don't get infinite recursion.
mVariant::mVariant()
{
    truevaluetype=valuetype="";
}

mVariant::mVariant(short in)
{
    truevaluetype=valuetype="short";
    ShortValue=in;
}

mVariant::mVariant(int in)
{
    truevaluetype=valuetype="int";
    IntValue=in;
}

mVariant::mVariant(long in)
{
    truevaluetype=valuetype="long";
    LongValue=in;
}

mVariant::mVariant(char in)
{
    truevaluetype=valuetype="char";
    CharValue=in;
}

mVariant::mVariant(float in)
{
    truevaluetype=valuetype="float";
    FloatValue=in;
}

mVariant::mVariant(double in)
{
    truevaluetype=valuetype="double";
    DoubleValue=in;
}

mVariant::mVariant(const char * in)
{
	truevaluetype="mstring";
	valuetype="string";
	StringValue=mstring(in);
}

mVariant::mVariant(const mstring& in)
{
	truevaluetype="mstring";
	valuetype="string";
	StringValue=in;
}

mVariant::mVariant(mDateTime in)
{
	truevaluetype="mDateTime";
	valuetype="string";
	StringValue=in.DateTimeString();
}

mVariant::mVariant(bool in)
{
	truevaluetype=valuetype="bool";
	BoolValue=in;
}

mVariant::mVariant(unsigned char in)
{
	truevaluetype=valuetype="unsigned char";
	UCharValue=in;
}

mVariant::mVariant(unsigned short in)
{
	truevaluetype=valuetype="unsigned short";
	UShortValue=in;
}

mVariant::mVariant(unsigned int in)
{
	truevaluetype=valuetype="unsigned int";
	UIntValue=in;
}

mVariant::mVariant(unsigned long in)
{
	truevaluetype=valuetype="unsigned long";
	ULongValue=in;
}

mVariant::mVariant(void * in)
{
	if(in!=NULL)
	{
		truevaluetype=valuetype="void *";
		PtrValue=in;
	}
	else
	{
		truevaluetype="void *";
		valuetype="NULL";
	}
}

mVariant::mVariant(const mVariant & in)
{
	*this=in;

}

mVariant::mVariant(wxTextFileType in)
{
    truevaluetype="wxTextFileType";
    valuetype="string";
    switch(in)
    {
    case wxTextFileType_None:
	StringValue="wxTextFileType_None";
	break;
    case wxTextFileType_Unix:
	StringValue="wxTextFileType_Unix";
	break;
    case wxTextFileType_Dos:
	StringValue="wxTextFileType_Dos";
	break;
    case wxTextFileType_Mac:
	StringValue="wxTextFileType_Mac";
	break;
    }
}

mVariant::mVariant(LineList *in)
{
    if(in!=NULL)
    {
        truevaluetype="LineList";
        valuetype="string";
        StringValue=(in->Text());
    }
    else
    {
        truevaluetype="LineList";
    	valuetype="NULL";
    }
}

mVariant::mVariant(ConfigEntry *in)
{
    if(in!=NULL)
    {
    	truevaluetype="ConfigEntry";
	valuetype="string";
	if(in->GetLine()!=NULL)
	    StringValue=in->GetLine()->Text();
	else
	    StringValue=in->Name();
    }
    else
    {
    	truevaluetype="ConfigEntry";
    	valuetype="NULL";
    }
}

mVariant::mVariant(ConfigGroup *in)
{
    if(in!=NULL)
    {
	truevaluetype="ConfigGroup";
	valuetype="string";
	StringValue=in->Name();
    }
    else
    {
	truevaluetype="ConfigGroup";
	valuetype="NULL";
    }
}

mVariant& mVariant::operator=(const mVariant& in)
{
    valuetype=in.valuetype;
    truevaluetype=in.truevaluetype;
    if(valuetype=="bool")
        BoolValue=in.BoolValue;
    else if(valuetype=="char")
	CharValue=in.CharValue;
    else if(valuetype=="double")
        DoubleValue=in.DoubleValue;
    else if(valuetype=="float")
        FloatValue=in.FloatValue;
    else if(valuetype=="int")
        IntValue=in.IntValue;
    else if(valuetype=="long")
        LongValue=in.LongValue;
    else if(valuetype=="void *")
        PtrValue=in.PtrValue;
    else if(valuetype=="short")
        ShortValue=in.ShortValue;
    else if(valuetype=="string")
        StringValue=in.StringValue;
    else if(valuetype=="unsigned char")
        UCharValue=in.UCharValue;
    else if(valuetype=="unsigned int")
        UIntValue=in.UIntValue;
    else if(valuetype=="unsigned long")
        ULongValue=in.ULongValue;
    else if(valuetype=="unsigned short")
        UShortValue=in.UShortValue;
    return *this;
}

bool mVariant::operator==(const mVariant& in)const
{
	if(valuetype!=in.valuetype)
		return false;
	if(valuetype=="bool")
	{
	    if(BoolValue==in.BoolValue)
		return true;
	    else 
	    	return false;
	}
	else if(valuetype=="char")
	{
	    if(CharValue==in.CharValue)
		return true;
	    else 
		return false;
	}
	else if(valuetype=="double")
	{
	    if(DoubleValue==in.DoubleValue)
		return true;
	    else 
		return false;
	}
	else if(valuetype=="float")
	{
	    if(FloatValue==in.FloatValue)
		return true;
	    else 
		return false;
	}
	else if(valuetype=="int")
	{
	    if(IntValue==in.IntValue)
		return true;
	    else 
		return false;
	}
	else if(valuetype=="long")
	{
	    if(LongValue==in.LongValue)
		return true;
	    else 
		return false;
	}
	else if(valuetype=="void *")
	{
	    if(PtrValue==in.PtrValue)
		return true;
	    else 
	    	return false;
	}
	else if(valuetype=="short")
	{
	    if(ShortValue==in.ShortValue)
	    	return true;
	    else 
	    	return false;
	}
	else if(valuetype=="string")
	{
	    if(StringValue==in.StringValue)
		return true;
	    else 
		return false;
	}
	else if(valuetype=="unsigned char")
	{
	    if(UCharValue==in.UCharValue)
		return true;
	    else 
		return false;
	}
	else if(valuetype=="unsigned int")
	{
	    if(UIntValue==in.UIntValue)
		return true;
	    else 
		return false;
	}
	else if(valuetype=="unsigned long")
	{
	    if(ULongValue==in.ULongValue)
		return true;
	    else 
		return false;
	}
	else if(valuetype=="unsigned short")
	{
	    if(UShortValue==in.UShortValue)
	    	return true;
	    else 
	    	return false;
	}
	else if(valuetype=="Empty"||valuetype=="NULL")
		return true;
	// fall through, if it get's here major fubar, so put in logging mechanism when it's done
	else return false;
}
bool mVariant::operator<(const mVariant& in)const
{
	if(in.valuetype!=valuetype)
	    return false;
	if(valuetype=="bool")
	{
	    if((int)BoolValue<(int)in.BoolValue)
	    	return true;
	    else 
	    	return false;
	}
	else if(valuetype=="char")
	{
	    if(CharValue<in.CharValue)
		return true;
	    else 
		return false;
	}
	else if(valuetype=="double")
	{
	    if(DoubleValue<in.DoubleValue)
		return true;
	    else 
		return false;
	}
	else if(valuetype=="float")
	{
	    if(FloatValue<in.FloatValue)
		return true;
	    else 
		return false;
	}
	else if(valuetype=="int")
	{
	    if(IntValue<in.IntValue)
		return true;
	    else 
		return false;
	}
	else if(valuetype=="long")
	{
	    if(LongValue<in.LongValue)
		return true;
	    else 
		return false;
	}
	else if(valuetype=="void *")
	{
	    if(PtrValue<in.PtrValue)
		return true;
	    else 
		return false;
	}
	else if(valuetype=="short")
	{
	    if(ShortValue<in.ShortValue)
		return true;
	    else 
		return false;
	}
	else if(valuetype=="string")
	{
	    if(StringValue<in.StringValue)
		return true;
	    else 
		return false;
	}
	else if(valuetype=="unsigned char")
	{
	    if(UCharValue<in.UCharValue)
		return true;
	    else 
		return false;
	}
	else if(valuetype=="unsigned int")
	{
	    if(UIntValue<in.UIntValue)
		return true;
	    else 
		return false;
	}
	else if(valuetype=="unsigned long")
	{
	    if(ULongValue<in.ULongValue)
		return true;
	    else 
		return false;
	}
	else if(valuetype=="unsigned short")
	{
	    if(UShortValue<in.UShortValue)
		return true;
	    else 
		return false;
	}
	else if(valuetype=="Empty"||valuetype=="NULL")
	    return true;
	// fall through, if it get's here major fubar, so put in logging mechanism when it's done
	else 
	    return false;
}

mstring mVariant::AsString()const
{
    mstring dummystring;
    if(valuetype=="bool")
    {
	if(BoolValue==true)
	    dummystring="true";
	else
	    dummystring="false";
    }
    else if(valuetype=="char")
	dummystring.Format("%c",CharValue);
    else if(valuetype=="double")
	dummystring.Format("%f",DoubleValue);
    else if(valuetype=="float")
	dummystring.Format("%f",FloatValue);
    else if(valuetype=="int")
	dummystring.Format("%d",IntValue);
    else if(valuetype=="long")
	dummystring.Format("%d",LongValue);
    else if(valuetype=="void *")
	dummystring.Format("%p",PtrValue);
    else if(valuetype=="short")
	dummystring.Format("%d",ShortValue);
    else if(valuetype=="string")
	dummystring=StringValue;
    else if(valuetype=="unsigned char")
	dummystring.Format("%u",UCharValue);
    else if(valuetype=="unsigned int")
	dummystring.Format("%u",UIntValue);
    else if(valuetype=="unsigned long")
	dummystring.Format("%u",ULongValue);
    else if(valuetype=="unsigned short")
	dummystring.Format("%u",UShortValue);
    else if(valuetype=="NULL")
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
