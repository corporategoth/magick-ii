#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
#endif

/*  Magick IRC Services
**
** (c) 1997-2001 Preston Elder <prez@magick.tm>
** (c) 1998-2001 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any
** circumstances, however it may be added to if any
** modifications are made to this file.  All modified
** code must be clearly documented and labelled.
**
** ========================================================== */
#define RCSID(x,y) const char *rcsid_variant_cpp_ ## x () { return y; }
RCSID(variant_cpp, "@(#)$Id$");
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.48  2001/04/02 02:11:23  prez
** Fixed up some inlining, and added better excption handling
**
** Revision 1.47  2001/03/08 14:34:29  prez
** Fixed some OS incompatabilities
**
** Revision 1.46  2001/03/08 08:07:41  ungod
** fixes for bcc 5.5
**
** Revision 1.45  2001/03/02 05:24:42  prez
** HEAPS of modifications, including synching up my own archive.
**
** Revision 1.44  2001/02/03 02:21:35  prez
** Loads of changes, including adding ALLOW to ini file, cleaning up
** the includes, RCSID, and much more.  Also cleaned up most warnings.
**
** Revision 1.43  2001/01/01 05:32:45  prez
** Updated copywrights.  Added 'reversed help' syntax (so ACCESS HELP ==
** HELP ACCESS).
**
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

// gah microsoft sucks, the below is to make sure our header get's included, not ms's
#include "../include/variant.h"

mVariant& mVariant::operator=(const mVariant& in)
{
    valuetype=in.valuetype;
    truevaluetype=in.truevaluetype;
    switch (valuetype)
    {
    case BOOL:
        BoolValue=in.BoolValue;
	break;
    case CHAR:
	CharValue=in.CharValue;
	break;
    case SHORT:
        ShortValue=in.ShortValue;
	break;
    case INT:
        IntValue=in.IntValue;
	break;
    case LONG:
        LongValue=in.LongValue;
	break;
    case FLOAT:
        FloatValue=in.FloatValue;
	break;
    case DOUBLE:
        DoubleValue=in.DoubleValue;
	break;
    case UCHAR:
        UCharValue=in.UCharValue;
	break;
    case USHORT:
        UShortValue=in.UShortValue;
	break;
    case UINT:
        UIntValue=in.UIntValue;
	break;
    case ULONG:
        ULongValue=in.ULongValue;
	break;
    case PTR:
        PtrValue=in.PtrValue;
	break;
    case MSTRING:
        StringValue=in.StringValue;
	break;
    case MDATETIME:
        DateTimeValue=in.DateTimeValue;
	break;
    case EMPTY:
	break;
    }
    return *this;
}

bool mVariant::operator==(const mVariant& in)const
{
    if(valuetype!=in.valuetype)
	return false;

    switch (valuetype)
    {
    case BOOL:
	return (BoolValue==in.BoolValue);
    case CHAR:
	return (CharValue==in.CharValue);
    case SHORT:
	return (ShortValue==in.ShortValue);
    case INT:
	return (IntValue==in.IntValue);
    case LONG:
	return (LongValue==in.LongValue);
    case FLOAT:
	return (FloatValue==in.FloatValue);
    case DOUBLE:
	return (DoubleValue==in.DoubleValue);
    case UCHAR:
	return (UCharValue==in.UCharValue);
    case USHORT:
	return (UShortValue==in.UShortValue);
    case UINT:
	return (UIntValue==in.UIntValue);
    case ULONG:
	return (ULongValue==in.ULongValue);
    case PTR:
	return (PtrValue==in.PtrValue);
    case MSTRING:
	return (StringValue==in.StringValue);
    case MDATETIME:
	return (DateTimeValue==in.DateTimeValue);
    case EMPTY:
	return true;
    default:
	return false;
    }
}
bool mVariant::operator<(const mVariant& in)const
{
    if(valuetype!=in.valuetype)
	return false;


    switch (valuetype)
    {
    case BOOL:
	return (static_cast<int>(BoolValue)<static_cast<int>(in.BoolValue));
    case CHAR:
	return (CharValue<in.CharValue);
    case SHORT:
	return (ShortValue<in.ShortValue);
    case INT:
	return (IntValue<in.IntValue);
    case LONG:
	return (LongValue<in.LongValue);
    case FLOAT:
	return (FloatValue<in.FloatValue);
    case DOUBLE:
	return (DoubleValue<in.DoubleValue);
    case UCHAR:
	return (UCharValue<in.UCharValue);
    case USHORT:
	return (UShortValue<in.UShortValue);
    case UINT:
	return (UIntValue<in.UIntValue);
    case ULONG:
	return (ULongValue<in.ULongValue);
    case PTR:
	return (PtrValue<in.PtrValue);
    case MSTRING:
	return (StringValue<in.StringValue);
    case MDATETIME:
	return (DateTimeValue<in.DateTimeValue);
    case EMPTY:
	return true;
    default:
	return false;
    }
}

mstring mVariant::AsString()const
{
    mstring dummystring;

    switch (valuetype)
    {
    case BOOL:
	if(BoolValue==true)
	    dummystring="true";
	else
	    dummystring="false";
	break;
    case CHAR:
	dummystring.Format("%c",CharValue);
	break;
    case SHORT:
	dummystring.Format("%d",ShortValue);
	break;
    case INT:
	dummystring.Format("%d",IntValue);
	break;
    case LONG:
	dummystring.Format("%d",LongValue);
	break;
    case FLOAT:
	dummystring.Format("%f",FloatValue);
	break;
    case DOUBLE:
	dummystring.Format("%f",DoubleValue);
	break;
    case UCHAR:
	dummystring.Format("%u",UCharValue);
	break;
    case USHORT:
	dummystring.Format("%u",UShortValue);
	break;
    case UINT:
	dummystring.Format("%u",UIntValue);
	break;
    case ULONG:
	dummystring.Format("%u",ULongValue);
	break;
    case PTR:
	dummystring.Format("%p",PtrValue);
	break;
    case MSTRING:
	dummystring=StringValue;
	break;
    case MDATETIME:
	dummystring=DateTimeValue.DateTimeString();
	break;
    case EMPTY:
	break;
    }

    return dummystring;
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
