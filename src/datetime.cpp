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
#define RCSID(x,y) const char *rcsid_datetime_cpp_ ## x () { return y; }
RCSID(datetime_cpp, "@(#)$Id$");
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.72  2001/11/12 01:05:02  prez
** Added new warning flags, and changed code to reduce watnings ...
**
** Revision 1.71  2001/08/04 18:32:02  prez
** Made some changes for Hybrid 6 -- we now work with it ... mostly.
**
** Revision 1.70  2001/05/05 17:33:58  prez
** Changed log outputs from printf-style to tokenized style files.
** Now use LOG/NLOG/SLOG/SNLOG rather than just LOG for output.  All
** formatting must be done BEFORE its sent to the logger (use fmstring).
**
** Revision 1.69  2001/04/13 00:46:38  prez
** Fixec channel registering
**
** Revision 1.68  2001/04/02 02:11:23  prez
** Fixed up some inlining, and added better excption handling
**
** Revision 1.67  2001/03/20 14:22:14  prez
** Finished phase 1 of efficiancy updates, we now pass mstring/mDateTime's
** by reference all over the place.  Next step is to stop using operator=
** to initialise (ie. use mstring blah(mstring) not mstring blah = mstring).
**
** Revision 1.66  2001/03/02 05:24:41  prez
** HEAPS of modifications, including synching up my own archive.
**
** Revision 1.65  2001/02/11 07:41:27  prez
** Enhansed support for server numerics, specifically for Unreal.
**
** Revision 1.63  2001/02/03 02:21:33  prez
** Loads of changes, including adding ALLOW to ini file, cleaning up
** the includes, RCSID, and much more.  Also cleaned up most warnings.
**
** Revision 1.62  2001/01/15 23:31:38  prez
** Added LogChan, HelpOp from helpserv, and changed all string != ""'s to
** !string.empty() to save processing.
**
** Revision 1.61  2001/01/01 05:32:44  prez
** Updated copywrights.  Added 'reversed help' syntax (so ACCESS HELP ==
** HELP ACCESS).
**
** Revision 1.60  2000/12/23 22:22:24  prez
** 'constified' all classes (ie. made all functions that did not need to
** touch another non-const function const themselves, good for data integrity).
**
** Revision 1.59  2000/12/19 07:24:53  prez
** Massive updates.  Linux works again, added akill reject threshold, and
** lots of other stuff -- almost ready for b6 -- first beta after the
** re-written strings class.  Also now using log adapter!
**
** Revision 1.58  2000/09/30 10:48:07  prez
** Some general code cleanups ... got rid of warnings, etc.
**
** Revision 1.57  2000/09/11 10:58:19  prez
** Now saves in in GMT
**
** Revision 1.56  2000/09/09 02:17:48  prez
** Changed time functions to actuallt accept the source nick as a param
** so that the time values (minutes, etc) can be customized.  Also added
** weeks to the time output.
**
** Revision 1.55  2000/07/29 21:58:53  prez
** Fixed XML loading of weird characters ...
** 2 known bugs now, 1) last_seen dates are loaded incorrectly on alot
** of nicknames, which means we expire lots of nicknames.  2) services
** wont rejoin a +i/+k channel when last user exits.
**
** Revision 1.54  2000/07/28 14:49:35  prez
** Ditched the old wx stuff, mconfig now in use, we're now ready to
** release (only got some conversion tests to do).
**
** Revision 1.53  2000/06/11 09:30:20  prez
** Added propper MaxLine length, no more hard-coded constants.
**
** Revision 1.52  2000/06/06 08:57:56  prez
** Finished off logging in backend processes except conver (which I will
** leave for now).  Also fixed some minor bugs along the way.
**
** Revision 1.51  2000/05/21 04:49:40  prez
** Removed all wxLog tags, now totally using our own logging.
**
** Revision 1.50  2000/05/20 15:17:00  prez
** Changed LOG system to use ACE's log system, removed wxLog, and
** added wrappers into pch.h and magick.cpp.
**
** Revision 1.49  2000/04/03 09:45:22  prez
** Made use of some config entries that were non-used, and
** removed some redundant ones ...
**
** Revision 1.48  2000/02/27 03:58:39  prez
** Fixed the WHAT program, also removed RegEx from Magick.
**
** Revision 1.47  2000/02/23 12:21:03  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.46  2000/02/16 12:59:39  ungod
** fixing for borland compilability
**
** Revision 1.45  2000/02/15 13:27:03  prez
** *** empty log message ***
**
** Revision 1.44  2000/02/15 10:37:49  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#include "magick.h"

mstring mDateTime::DateSeparator	= "/";
mstring mDateTime::ShortDateFormat	= "d/m/yyyy";
mstring mDateTime::LongDateFormat	= "mmmm d',' yyyy";
mstring mDateTime::TimeSeparator	= ":";
mstring mDateTime::TimeAMString		= "am";
mstring mDateTime::TimePMString		= "pm";
mstring mDateTime::ShortTimeFormat	= "h:nn AMPM";
mstring mDateTime::LongTimeFormat	= "hh:nn:ss AMPM";
mstring mDateTime::ShortMonthNames[12]	= { "Jan", "Feb", "Mar",
					    "Apr", "May", "Jun",
					    "Jul", "Aug", "Sep",
					    "Oct", "Nov", "Dec"  };
mstring mDateTime::LongMonthNames[12]	= { "January", "February", "March",
					    "April", "May", "June",
					    "July", "August", "September",
					    "October", "November", "December" };
mstring mDateTime::ShortDayNames[7]	= { "Mon", "Tue", "Wed",
					    "Thu", "Fri", "Sat",
					    "Sun" };
mstring mDateTime::LongDayNames[7]	= { "Monday", "Tuesday", "Wednesday",
					    "Thursday", "Friday", "Saturday",
					    "Sunday" };

static const int SecsPerDay		= 24 * 60 * 60;
static const int MSecsPerDay		= SecsPerDay * 1000;

typedef int mDayTable[12];
static mDayTable DayTable1		= { 31, 28, 31, 30, 31, 30,
					    31, 31, 30, 31, 30, 31 };
static mDayTable DayTable2		= { 31, 29, 31, 30, 31, 30,
					    31, 31, 30, 31, 30, 31 };

/* ---------------------------------------------------------- */

mDateTime mDateTime::CurrentDate()
{
    mDateTime Result;
    int iDay,iMonth,iYear;
    Result=mDateTime(time(NULL));
    Result.DecodeDate(iYear,iMonth,iDay);
    Result=mDateTime(iYear,iMonth,iDay);
    return Result;
}

mDateTime mDateTime::CurrentTime()
{
    mDateTime Result;
    int iHour,iMin,iSec,iMSec;
    Result=mDateTime(time(NULL));
    Result.DecodeTime(iHour,iMin,iSec,iMSec);
    Result=mDateTime(iHour,iMin,iSec,iMSec);
    return Result;
}

mDateTime::mDateTime(const mstring& src, const mDateTimeFlag flag)
{
    LOG(LM_ERROR, "SYS_ERRORS/NOT_IMPLEMENTED", (
	"mDateTime::mDateTime(const mstring& src, mDateTimeFlag flag)"));
#if 0
    if(flag==Date)
	*this=StringToDate(src);
    else if(flag=Time)
	*this=StringToTime(src);
    else if(flag=DateTime)
	*this=StringToDateTime(src);
#else
    static_cast<void>(src);
    static_cast<void>(flag);
#endif
}

static mDayTable &GetDayTable(int Year)
{
    if((Year % 4 == 0) && ((Year % 100 != 0) || (Year % 400 == 0)))
	return DayTable2;
    else 
	return DayTable1;
}

bool DoEncodeDate(const int iYear, const int iMonth, const int iDay, mDateTime& Date)
{
    int I;
    bool Result = false;
    int tmpDay=iDay-1;

    mDayTable &DayTable = GetDayTable(iYear);
    if ((iYear >= 1) && (iYear <= 9999) && (iMonth >= 1) && (iMonth <= 12) &&
	(iDay >= 1) && (iDay <= DayTable[iMonth-1]))
    {
	for(I = 1; I<iMonth;I++) 
	{
	    tmpDay+=DayTable[I-1];
	}
	I = iYear;
	tmpDay += ((I * 365) + (I / 4) - (I / 100) + (I / 400));
	Date.Val = static_cast<double>(tmpDay);
	Result = true;
    }
    return Result;
}

bool DoEncodeTime(const int iHour, const int iMin, const int iSec, const int iMSec, mDateTime& Time)
{
    bool Result = false;
    if ((iHour < 24) && (iMin < 60) && (iSec < 60) && (iMSec < 1000))
    {
	Time.Val = (static_cast<double>(iHour) * 3600000.0 +
		static_cast<double>(iMin) * 60000.0 +
		static_cast<double>(iSec) * 1000.0 +
		static_cast<double>(iMSec)) / static_cast<double>(MSecsPerDay);
	Result = true;
    }
    return Result;
}

mstring mDateTime::FormatString(const mstring& format)const
{
	mstring Result;
	int iYear, iMonth, iDay, iHour, iMin, iSec, iMSec;
	unsigned int i=0;
	int count=0;
	int ampmtype=0;
	DecodeDate(iYear,iMonth,iDay);
	DecodeTime(iHour,iMin,iSec,iMSec);

	if(format.Find("a/p")!=-1)
		ampmtype=1;
	else if(format.Find("am/pm")!=-1)
		ampmtype=2;
	else if(format.Find("AMPM")!=-1)
		ampmtype=3;

	while(i<format.size())
	{
	    switch(tolower(format[i]))
	    {
	    case 'c':
		Result += FormatString(ShortDateFormat);
		break;
	    case 'd':
		count=1;
		while(i+1<format.size()&&tolower(format[i+1])=='d')
		{
		    i++;
		    count++;
		}
		switch(count)
		{
		case 1:
		    Result += iDay;
		    break;
		case 2:
		    if(iDay<10)
			Result += "0";
		    Result += iDay;
		    break;
		case 3:
		    Result += ShortDayNames[DayOfWeek()];
		    break;
		case 4:
		    Result += LongDayNames[DayOfWeek()];
		    break;
		case 5:
		    Result += FormatString(ShortDateFormat);
		    break;
		case 6:
		    Result += FormatString(LongDateFormat);
		    break;
		default:
		    LOG(LM_ERROR, "SYS_ERRORS/INVALID_FORMAT", (
			count, format[i], format));
		};
		break;
	    case 'm':
		count=1;
		while(i+1<format.size()&&tolower(format[i+1])=='m')
		{
		    i++;
		    count++;
		}
		switch(count)
		{
		case 1:
		    Result += iMonth;
		    break;
		case 2:
		    if(iMonth<10)
			Result += "0";
		    Result += iMonth;
		    break;
		case 3:
		    Result += ShortMonthNames[iMonth-1];
		    break;
		case 4:
		    Result += LongMonthNames[iMonth-1];
		    break;
		default:
		    LOG(LM_ERROR, "SYS_ERRORS/INVALID_FORMAT", (
			count, format[i], format));
		}
		break;
	    case 'y':
		count=1;
		while(i+1<format.size()&&tolower(format[i+1])=='y')
		{
		    i++;
		    count++;
		}
		switch(count)
		{
		case 2:
		    Result += iYear%100;
		    break;
		case 4:
		    Result += iYear;
		    break;
		default:
		    LOG(LM_ERROR, "SYS_ERRORS/INVALID_FORMAT", (
			count, format[i], format));
		}
		break;
	    case 'h':
		if(i+1<format.size()&&tolower(format[i+1])=='h')
		{
		    i++;
		    if(ampmtype>0)
		    {
			if(iHour%12<10)
			    Result += "0";
		    }
		    else
		    {
			if(iHour<10)
			    Result += "0";
		    }
		}
		if(ampmtype>0)
		    Result += iHour%12;
		else
		    Result += iHour;
		break;
	    case 'n':
		if(i+1<format.size()&&tolower(format[i+1])=='n')
		{
		    i++;
		    if(iMin<10)
			Result += "0";
		}
		Result += iMin;
		break;
	    case 's':
		if(i+1<format.size()&&tolower(format[i+1])=='s')
		{
		    i++;
		    if(iSec<10)
			Result += "0";
		}
		Result += iSec;
		break;
	    case 'u':
		count=1;
		while(i+1<format.size()&&tolower(format[i+1])=='u')
		{
		    i++;
		    count++;
		}
		switch(count)
		{
		case 1:
		    break;
		case 3:
		    if(iMSec<100)
			Result += "0";
		    if(iMSec<10)
			Result += "0";
		    break;
		default:
		    LOG(LM_ERROR, "SYS_ERRORS/INVALID_FORMAT", (
			count, format[i], format));
		}
		Result += iMSec;
		break;
	    case 't':
		if(i+1<format.size()&&tolower(format[i+1])=='t')
		{
		    i++;
		    Result += FormatString(LongTimeFormat);
		}
		else
		    Result += FormatString(ShortTimeFormat);
		break;
	    case 'a':
		if(i+2<format.size() && format.substr(i+1, 2).IsSameAs("/p", true))
		{
		    //found a/p
		    i=i+2;
		    if(iHour<12)
			Result += "a";
		    else
			Result += "p";
		}
		else if(i+3<format.size() && format.substr(i+1, 3).IsSameAs("MPM", true))
		{
		    //found ampm
		    i=i+3;
		    if(iHour<12)
			Result += TimeAMString;
		    else
			Result += TimePMString;
		}
		else if(i+4<format.size() && format.substr(i+1, 4).IsSameAs("m/pm", true))
		{
		    //found am/pm
		    i=i+2;
		    if(iHour<12)
			Result += "am";
		    else
			Result += "pm";
		}
		else
		{
		    LOG(LM_ERROR, "SYS_ERRORS/INVALID_FORMAT", (
			1, format[i], format));
		    Result += "a";
		}
		break;
	    case '/':
		Result += DateSeparator;
		break;
	    case ':':
		Result += TimeSeparator;
		break;
	    case '\'':
		i++;
		while (i<format.size()&&format[i]!='\'')
		    Result += format[i++];
		break;
	    case '"':
		i++;
		while (i<format.size()&&format[i]!='\"')
		    Result += format[i++];
	    case ' ':
	    case '.':
	    case '\t':
	    case '\n':
		Result += format[i];
		break;
	    default:
		LOG(LM_TRACE, "SYS_ERRORS/NOT_LITERAL", (
		    format[i], format));
		Result += format[i];
	    };
	    i++;
	}
	return Result;
}

mstring mDateTime::DateTimeString()const
{
	mstring Result;
	int iYear,iMonth,iDay,iHour,iMin,iSec,iMSec;
	DecodeDate(iYear,iMonth,iDay);
	DecodeTime(iHour,iMin,iSec,iMSec);
	if(iYear!=0||iMonth!=0||iDay!=0)
		Result=FormatString(LongDateFormat);
	if(iHour!=0||iMin!=0||iSec!=0||iMSec!=0)
	{
	    if(!Result.empty())
		Result += " ";
	    Result += FormatString(LongTimeFormat);
	}

	return Result;
}

mDateTime::operator time_t() const
{
	int iYear,iMonth,iDay,iHour,iMin,iSec,iMSec;
	DecodeDate(iYear,iMonth,iDay);
	if (iYear < 1900)
	    return static_cast<time_t>(-1);

	DecodeTime(iHour,iMin,iSec,iMSec);
	tm localtm;
	localtm.tm_year=iYear-1900;
	localtm.tm_mon=iMonth-1;
	localtm.tm_mday=iDay;
	localtm.tm_hour=iHour;
	localtm.tm_min=iMin;
	localtm.tm_sec=iSec;
	localtm.tm_isdst=-1;
	return mktime(&localtm);
}

int mDateTime::DayOfWeek()const
{
	mDateTime knownmonday(1970,1,5);
	int Result=static_cast<int>(Val-knownmonday.Val) % 7;
	return Result;
}

void mDateTime::DecodeDate(int &year, int &month, int &day)const
{
    const int D1 = 365;
    const int D4 = D1 * 4 + 1;
    const int D100 = D4 * 25 - 1;
    const int D400 = D100 * 4 + 1;
    int NumDays = static_cast<int>(Val);
    int Y400,Y100,Y4,Y1,Y,M=1;
    int LeftOver;

    LeftOver=NumDays%D400;
    Y400=NumDays/D400;
    NumDays=LeftOver;

    LeftOver=NumDays%D100;
    Y100=NumDays/D100;
    NumDays=LeftOver;

    LeftOver=NumDays%D4;
    Y4=NumDays/D4;
    NumDays=LeftOver;
  
    LeftOver=NumDays%D1;
    Y1=NumDays/D1;
    NumDays=LeftOver;

    Y=Y400*400+Y100*100+Y4*4+Y1;
    year=Y;

    int i=0;
    mDayTable &DayTable=GetDayTable(year);
    while(DayTable[i]<NumDays)
    {
	M++;
	NumDays-=DayTable[i];
	i++;
    }
    day=NumDays+1;
    month=M;
}

void mDateTime::DecodeTime(int &hour, int &min, int &sec, int &msec)const
{
	//(iHour * 3600000 + iMin * 60000 + iSec * 1000 + iMSec) / MSecsPerDay;
	int CurrentVal=static_cast<int>(fmod(Val,1.0)*static_cast<double>(MSecsPerDay));
	int LeftOver;

	LeftOver=CurrentVal%3600000;
	hour=CurrentVal/3600000;
	CurrentVal=LeftOver;

	LeftOver=CurrentVal%60000;
	min=CurrentVal/60000;
	CurrentVal=LeftOver;

	LeftOver=CurrentVal%1000;
	sec=CurrentVal/1000;
	msec=LeftOver;
}

mDateTime StringToDate(const mstring& in)
{
	int year=0,month=0,day=0;
	int first=0,second=0,third=0;
	mstring formatspec;
	mstring shortdateformat=mDateTime::ShortDateFormat;
	shortdateformat.MakeLower();
	formatspec="%d"+mDateTime::DateSeparator+"%d"+mDateTime::DateSeparator+"%d";
	sscanf(in.c_str(),formatspec.c_str(),&first,&second,&third);
	if(shortdateformat.Before(mDateTime::DateSeparator)=="mm"||shortdateformat.Before(mDateTime::DateSeparator)=="m")
		month=first;
	else if(shortdateformat.Before(mDateTime::DateSeparator)=="dd"||shortdateformat.Before(mDateTime::DateSeparator)=="d")
		day=first;
	else if(shortdateformat.Before(mDateTime::DateSeparator)=="yyyy")
		year=first;
	else if(shortdateformat.Before(mDateTime::DateSeparator)=="yy")
		if(first+100 <= mDateTime::CurrentDateTime().Year2() + 150)
			year=first+mDateTime::CurrentDateTime().Century();
		else
			year=first+mDateTime::CurrentDateTime().Century()-100;
	if(shortdateformat.After(mDateTime::DateSeparator).Before(mDateTime::DateSeparator)=="mm"||shortdateformat.After(mDateTime::DateSeparator).Before(mDateTime::DateSeparator)=="m")
		month=second;
	else if(shortdateformat.After(mDateTime::DateSeparator).Before(mDateTime::DateSeparator)=="dd"||shortdateformat.After(mDateTime::DateSeparator).Before(mDateTime::DateSeparator)=="d")
		day=second;
	else if(shortdateformat.After(mDateTime::DateSeparator).Before(mDateTime::DateSeparator)=="yyyy")
		year=second;
	else if(shortdateformat.After(mDateTime::DateSeparator).Before(mDateTime::DateSeparator)=="yy")
		if(second+100 <= mDateTime::CurrentDateTime().Year2() + 150)
			year=second+mDateTime::CurrentDateTime().Century();
		else
			year=second+mDateTime::CurrentDateTime().Century()-100;
	if(shortdateformat.After(mDateTime::DateSeparator).After(mDateTime::DateSeparator)=="mm"||shortdateformat.After(mDateTime::DateSeparator).After(mDateTime::DateSeparator)=="m")
		month=third;
	else if(shortdateformat.After(mDateTime::DateSeparator).After(mDateTime::DateSeparator)=="dd"||shortdateformat.After(mDateTime::DateSeparator).After(mDateTime::DateSeparator)=="d")
		day=third;
	else if(shortdateformat.After(mDateTime::DateSeparator).After(mDateTime::DateSeparator)=="yyyy")
		year=third;
	else if(shortdateformat.After(mDateTime::DateSeparator).After(mDateTime::DateSeparator)=="yy")
		if(third+100 <= mDateTime::CurrentDateTime().Year2() + 150)
			year=third+mDateTime::CurrentDateTime().Century();
		else
			year=third+mDateTime::CurrentDateTime().Century()-100;
	return mDateTime(year,month,day);
}

#if 0
mDateTime StringToTime(const mstring& in)
{

}

mDateTime StringToDateTime(const mstring& in)
{

}

#endif


mstring mDateTime::timetstring()const
{
    mstring Result;
    time_t Res2;
    int iYear,iMonth,iDay,iHour,iMin,iSec,iMSec;
    DecodeDate(iYear,iMonth,iDay);
    if (iYear < 1900)
	return static_cast<time_t>(-1);
    DecodeTime(iHour,iMin,iSec,iMSec);
    tm localtm;
    localtm.tm_year=iYear-1900;
    localtm.tm_mon=iMonth-1;
    localtm.tm_mday=iDay;
    localtm.tm_hour=iHour;
    localtm.tm_min=iMin;
    localtm.tm_sec=iSec;
    localtm.tm_isdst=-1;
    Res2=mktime(&localtm);
    Result << static_cast<unsigned long>(Res2);
    return Result;
}

int mDateTime::MSecond() const
{
    int iHour,iSecond,iMinute,iMSec;
    DecodeTime(iHour,iSecond,iMinute,iMSec);
    return iMSec;
}
int mDateTime::Second() const
{
    int iHour,iSecond,iMinute,iMSec;
    DecodeTime(iHour,iSecond,iMinute,iMSec);
    return iSecond;
}
int mDateTime::Minute() const
{
    int iHour,iSecond,iMinute,iMSec;
    DecodeTime(iHour,iSecond,iMinute,iMSec);
    return iMinute;
}
int mDateTime::Hour() const
{
    int iHour,iSecond,iMinute,iMSec;
    DecodeTime(iHour,iSecond,iMinute,iMSec);
    return iHour;
}
int mDateTime::Day() const
{
    int iYear,iMonth,iDay;
    DecodeDate(iYear,iMonth,iDay);
    return iDay;
}
int mDateTime::Month() const
{
    int iYear,iMonth,iDay;
    DecodeDate(iYear,iMonth,iDay);
    return iMonth;
}
int mDateTime::Year() const
{
    int iYear,iMonth,iDay;
    DecodeDate(iYear,iMonth,iDay);
    return iYear;
}

unsigned long mDateTime::MSecondsSince() const
{
    mDateTime dummyvar=mDateTime::CurrentDateTime()-(*this);
    unsigned long CurrentVal=static_cast<unsigned long>(dummyvar.Val*static_cast<double>(MSecsPerDay));
    return CurrentVal;
}

unsigned long mDateTime::SecondsSince() const
{
    mDateTime dummyvar=mDateTime::CurrentDateTime()-(*this);
    unsigned long CurrentVal=static_cast<unsigned long>(dummyvar.Val*static_cast<double>(SecsPerDay));
    return CurrentVal;
}

mstring DisectTime(const long intime, const mstring &source)
{
    mstring Result="";
    long Years=0, Weeks=0, Days=0, Hours=0, Minutes=0, Seconds=0, negamt=0;

    if (intime < 0)
    {
	Result << "-";
	Seconds = intime * -1;
    }
    else
    {
	Seconds = intime;
    }

    negamt = static_cast<long>(60.0 * 60.0 * 24.0 * 365.25);
    while (Seconds >= negamt)
    {
	Years++;
	Seconds -= negamt;
    }

    negamt = 60 * 60 * 24 * 7;
    while (Seconds >= negamt)
    {
	Weeks++;
	Seconds -= negamt;
    }

    negamt = 60 * 60 * 24;
    while (Seconds >= negamt)
    {
	Days++;
	Seconds -= negamt;
    }

    negamt = 60 * 60;
    while (Seconds >= negamt)
    {
	Hours++;
	Seconds -= negamt;
    }

    negamt = 60;
    while (Seconds >= negamt)
    {
	Minutes++;
	Seconds -= negamt;
    }

    if(Years>0)
    {
	Result << Years << " " << (Years==1 ?
			Parent->getMessage(source, "VALS/TIME_YEAR") :
			Parent->getMessage(source, "VALS/TIME_YEARS"));
	if (Weeks)
	    Result << ", " << Weeks << " " << (Weeks==1 ?
			Parent->getMessage(source, "VALS/TIME_WEEK") :
			Parent->getMessage(source, "VALS/TIME_WEEKS"));
	if (Days)
	    Result << ", " << Days << " " << (Days==1 ?
			Parent->getMessage(source, "VALS/TIME_DAY") :
			Parent->getMessage(source, "VALS/TIME_DAYS"));
	if (Hours || Minutes || Seconds)
	    Result << ", " << (Hours>=10 ? "" : "0") << Hours << ":" <<
		    (Minutes>=10 ? "" : "0") << Minutes << ":" <<
		    (Seconds>=10 ? "" : "0") << Seconds;
    }
    else if(Weeks>0)
    {
	Result << Weeks << " " << (Weeks==1 ?
			Parent->getMessage(source, "VALS/TIME_WEEK") :
			Parent->getMessage(source, "VALS/TIME_WEEKS"));
	if (Days)
	    Result << ", " << Days << " " << (Days==1 ?
			Parent->getMessage(source, "VALS/TIME_DAY") :
			Parent->getMessage(source, "VALS/TIME_DAYS"));
	if (Hours || Minutes || Seconds)
	    Result << ", " << (Hours>=10 ? "" : "0") << Hours << ":" <<
		    (Minutes>=10 ? "" : "0") << Minutes << ":" <<
		    (Seconds>=10 ? "" : "0") << Seconds;
    }
    else if(Days>0)
    {
	Result << Days << " " << (Days==1 ?
			Parent->getMessage(source, "VALS/TIME_DAY") :
			Parent->getMessage(source, "VALS/TIME_DAYS"));
	if (Hours || Minutes || Seconds)
	    Result << ", " << (Hours>=10 ? "" : "0") << Hours << ":" <<
		    (Minutes>=10 ? "" : "0") << Minutes << ":" <<
		    (Seconds>=10 ? "" : "0") << Seconds;
    }
    else if(Hours>0)
    {
	Result << Hours << " " << (Hours==1 ?
			Parent->getMessage(source, "VALS/TIME_HOUR") :
			Parent->getMessage(source, "VALS/TIME_HOURS"));
	if (Minutes)
	    Result << ", " << Minutes << " " << (Minutes==1 ?
			Parent->getMessage(source, "VALS/TIME_MIN") :
			Parent->getMessage(source, "VALS/TIME_MINS"));
	if (Seconds)
	    Result << ", " << Seconds << " " << (Seconds==1 ?
			Parent->getMessage(source, "VALS/TIME_SEC") :
			Parent->getMessage(source, "VALS/TIME_SECS"));
    }
    else if(Minutes>0)
    {
	Result << Minutes << " " << (Minutes==1 ?
			Parent->getMessage(source, "VALS/TIME_MIN") :
			Parent->getMessage(source, "VALS/TIME_MINS"));
	if (Seconds)
	    Result << ", " << Seconds << " " << (Seconds==1 ?
			Parent->getMessage(source, "VALS/TIME_SEC") :
			Parent->getMessage(source, "VALS/TIME_SECS"));
    }
    else if(Seconds>0)
    {
	Result << Seconds << " " << (Seconds==1 ?
			Parent->getMessage(source, "VALS/TIME_SEC") :
			Parent->getMessage(source, "VALS/TIME_SECS"));
    }
    else
    {
	Result << Parent->getMessage(source, "VALS/TIME_NOW");
    }
    return Result;
}

mDateTime GMT(const mDateTime &in, const bool to)
{
    ACE_OS::tzset();
    long offset = ACE_OS::timezone() * (to ? 1 : -1);
    double val = in.Internal();
    int days = 0, secs = 0;
    days = static_cast<int>(val);
    val -= days;
    secs = static_cast<int>(val * static_cast<double>(SecsPerDay));

    if (secs + offset > SecsPerDay)
    {
	if (offset > 0)
	{
	    days++;
	    secs += offset - SecsPerDay;
	}
	else if (offset < 0)
	{
	    days--;
	    secs -= offset + SecsPerDay;
	}
    }
    else
    {
	secs += offset;
    }
    val = static_cast<double>(days) + (static_cast<double>(secs) *
		(1.0 / static_cast<double>(SecsPerDay)));
    return mDateTime(val);
}
