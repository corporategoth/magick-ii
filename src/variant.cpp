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
** Revision 1.42  2000/12/09 10:17:14  prez
** Added +h to unreal IRCD profile, and made variant more efficiant
** string wise by cutting out many operator= calls.
**
** Revision 1.41  2000/09/01 10:54:39  prez
** Added Changing and implemented Modify tracing, now just need to create
** DumpB() and DumpE() functions in all classes, and put MCB() / MCE() calls
** (or MB() / ME() or CB() / CE() where MCB() / MCE() not appropriate) in.
**
** Revision 1.40  2000/08/31 06:25:09  prez
** Added our own socket class (wrapper around ACE_SOCK_Stream,
** ACE_SOCK_Connector and ACE_SOCK_Acceptor, with tracing).
**
** Revision 1.39  2000/07/28 14:49:36  prez
** Ditched the old wx stuff, mconfig now in use, we're now ready to
** release (only got some conversion tests to do).
**
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
{}

mVariant::mVariant(const short in)
	: truevaluetype("short"),
	  valuetype("short"),
	  ShortValue(in)
{}

mVariant::mVariant(const int in)
	: truevaluetype("int"),
	  valuetype("int"),
	  IntValue(in)
{}

mVariant::mVariant(const long in)
	: truevaluetype("long"),
	  valuetype("long"),
	  LongValue(in)
{}

mVariant::mVariant(const char in)
	: truevaluetype("char"),
	  valuetype("char"),
	  CharValue(in)
{}

mVariant::mVariant(const float in)
	: truevaluetype("float"),
	  valuetype("float"),
	  FloatValue(in)
{}

mVariant::mVariant(const double in)
	: truevaluetype("double"),
	  valuetype("double"),
	  DoubleValue(in)
{}

mVariant::mVariant(const char * in)
	: truevaluetype("char *"),
	  valuetype("mstring"),
	  StringValue(in)
{}

mVariant::mVariant(const string& in)
	: truevaluetype("string"),
	  valuetype("mstring"),
	  StringValue(in)
{}

mVariant::mVariant(const mstring& in)
	: truevaluetype("mstring"),
	  valuetype("mstring"),
	  StringValue(in)
{}

mVariant::mVariant(const mDateTime in)
	: truevaluetype("mDateTime"),
	  valuetype("mstring"),
	  StringValue(in.DateTimeString())
{}

mVariant::mVariant(const bool in)
	: truevaluetype("bool"),
	  valuetype("bool"),
	  BoolValue(in)
{}

mVariant::mVariant(const unsigned char in)
	: truevaluetype("unsigned char"),
	  valuetype("unsigned char"),
	  UCharValue(in)
{}

mVariant::mVariant(const unsigned short in)
	: truevaluetype("unsigned short"),
	  valuetype("unsigned short"),
	  UShortValue(in)
{}

mVariant::mVariant(const unsigned int in)
	: truevaluetype("unsigned int"),
	  valuetype("unsigned int"),
	  UIntValue(in)
{}

mVariant::mVariant(const unsigned long in)
	: truevaluetype("unsigned long"),
	  valuetype("unsigned long"),
	  ULongValue(in)
{}

mVariant::mVariant(void * in)
	: truevaluetype("void *")
{
	if(in!=NULL)
	{
		valuetype="void *";
	}
	else
	{
		truevaluetype="void *";
		valuetype="NULL";
	}
	PtrValue=in;
}

mVariant::mVariant(const mVariant & in)
{
	*this=in;
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
    else if(valuetype=="mstring")
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
	else if(valuetype=="mstring")
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
	else if(valuetype=="mstring")
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
    else if(valuetype=="mstring")
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
mVarArray::mVarArray(const mVariant& one, const mVariant& two, const mVariant& three, const mVariant& four,
	const mVariant& five, const mVariant& six, const mVariant& seven, const mVariant& eight,
	const mVariant& nine, const mVariant& ten, const mVariant& eleven, const mVariant& twelve,
	const mVariant& thirteen, const mVariant& fourteen, const mVariant& fifteen,
	const mVariant& sixteen)
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
