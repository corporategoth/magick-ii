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


#include "datetime.h"
#include "magick.h"

mstring DateSeparator="/";
mstring ShortDateFormat="d/m/yyyy";
mstring LongDateFormat="mmmm d',' yyyy";
mstring TimeSeparator=":";
mstring TimeAMString="am";
mstring TimePMString="pm";
mstring ShortTimeFormat="h:nn AMPM";
mstring LongTimeFormat="hh:nn:ss AMPM";
mstring ShortMonthNames[12] =
{"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
mstring LongMonthNames[12] =
{"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
mstring ShortDayNames[7] =
{"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
mstring LongDayNames[7] =
{"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

mDateTime mDateTime::CurrentDate()
{
    mDateTime Result;
    int Day,Month,Year;
    Result=mDateTime(time(NULL));
    Result.DecodeDate(Year,Month,Day);
    Result=mDateTime(Year,Month,Day);
    return Result;
}

mDateTime mDateTime::CurrentTime()
{
    mDateTime Result;
    int Hour,Min,Sec,MSec;
    Result=mDateTime(time(NULL));
    Result.DecodeTime(Hour,Min,Sec,MSec);
    Result=mDateTime(Hour,Min,Sec,MSec);
    return Result;
}

mDateTime mDateTime::CurrentDateTime()
{
	return mDateTime(time(NULL));
}

mDateTime::mDateTime(time_t src)
{
	*this=src;
}
mDateTime::mDateTime(const mstring& src, mDateTimeFlag flag)
{
	Log(LM_ERROR, Parent->getLogMessage("SYS_ERRORS/NOT_IMPLEMENTED"),
		"mDateTime::mDateTime(const mstring& src, mDateTimeFlag flag)");
#if 0
	if(flag==Date)
		*this=StringToDate(src);
	else if(flag=Time)
		*this=StringToTime(src);
	else if(flag=DateTime)
		*this=StringToDateTime(src);
#endif
}

const int SecsPerDay = 24 * 60 * 60;
const int MSecsPerDay = SecsPerDay * 1000;
//Days between 1/1/0001 and 12/31/1899
const int DateDelta = 693594;


bool IsLeapYear(int Year)
{
  return ((Year%4 == 0) && ((Year%100 != 0) || (Year%400 == 0)));
}

typedef int mDayTable[12];
mDayTable DayTable1={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
mDayTable DayTable2={31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

mDayTable &GetDayTable(int Year)
{
  if(IsLeapYear(Year))
	  return DayTable2;
  else 
	  return DayTable1;
}

bool DoEncodeDate(int Year, int Month, int Day, mDateTime& Date)
{
  int I;
  bool Result = false;
  int tmpDay=Day-1;

  mDayTable &DayTable = GetDayTable(Year);
  if ((Year >= 1) && (Year <= 9999) && (Month >= 1) && (Month <= 12) &&
    (Day >= 1) && (Day <= DayTable[Month-1]))
  {
    for(I = 1; I<Month;I++) 
		tmpDay+=DayTable[I-1];
    I = Year - 1;
    Date.Val = (double)(I * 365 + I / 4 - I / 100 + I / 400 + tmpDay - DateDelta);
    Result = true;
  }
  return Result;
}

mDateTime::mDateTime(unsigned int year, unsigned int month, unsigned int day)
{
	mDateTime tmp;
	if(!DoEncodeDate(year,month,day,tmp))
		tmp=0.0;
	*this=tmp;
}

bool DoEncodeTime(int Hour, int Min, int Sec, int MSec, mDateTime& Time)
{
  bool Result = false;
  if ((Hour < 24) && (Min < 60) && (Sec < 60) && (MSec < 1000))
  {
    Time.Val = ((double)Hour * 3600000.0 + (double)Min * 60000.0 + (double)Sec * 1000.0 + (double)MSec) / (double)MSecsPerDay;
    Result = true;
  }
  return Result;
}

mDateTime::mDateTime(unsigned int hour, unsigned int min, unsigned int sec, unsigned int msec)
{
	mDateTime tmp;
	if(!DoEncodeTime(hour,min,sec,msec,tmp))
		tmp=0.0;
	*this=tmp;
}
mDateTime& mDateTime::operator=(const mDateTime& in)
{
	Val=in.Val;
	return *this;
}
mDateTime& mDateTime::operator=(double in)
{
	Val=in;
	return *this;
}
mDateTime& mDateTime::operator=(time_t in)
{
	tm *tmst;
	tmst=localtime(&in);
	*this=mDateTime(tmst->tm_year+1900,tmst->tm_mon+1,tmst->tm_mday)+mDateTime(tmst->tm_hour,tmst->tm_min,tmst->tm_sec,0);
	return *this;
}
mDateTime& mDateTime::operator+=(const mDateTime& in)
{
	Val+=in.Val;
	return *this;
}
mDateTime& mDateTime::operator+=(double in)
{
	Val+=in;
	return *this;
}
mDateTime& mDateTime::operator+=(time_t in)
{
	*this+=mDateTime(in);
	return *this;
}
mDateTime& mDateTime::operator-=(const mDateTime& in)
{
	Val-=in.Val;
	return *this;
}
mDateTime& mDateTime::operator-=(double in)
{
	Val-=in;
	return *this;
}
mDateTime& mDateTime::operator-=(time_t in)
{
	*this-=mDateTime(in);
	return *this;
}
mDateTime& mDateTime::operator+(const mDateTime& in)
{
	Val+=in.Val;
	return *this;
}
mDateTime& mDateTime::operator+(double in)
{
	Val+=in;
	return *this;
}
mDateTime& mDateTime::operator+(time_t in)
{
	*this+=mDateTime(in);
	return *this;
}
mDateTime& mDateTime::operator-(const mDateTime& in)
{
	Val-=in.Val;
	return *this;
}
mDateTime& mDateTime::operator-(double in)
{
	Val-=in;
	return *this;
}
mDateTime& mDateTime::operator-(time_t in)
{
	*this-=mDateTime(in);
	return *this;
}

bool mDateTime::operator==(const mDateTime& in)const
{
	return Val==in.Val;
}

bool mDateTime::operator!=(const mDateTime& in)const
{
	return Val!=in.Val;
}

bool mDateTime::operator>(const mDateTime& in)const
{
	return Val>in.Val;
}

bool mDateTime::operator<(const mDateTime& in)const
{
	return Val<in.Val;
}

bool mDateTime::operator>=(const mDateTime& in)const
{
	return Val>=in.Val;
}

bool mDateTime::operator<=(const mDateTime& in)const
{
	return Val<=in.Val;
}

mstring mDateTime::FormatString(const mstring& format)const
{
	mstring Result;
	int Year, Month, Day, Hour, Min, Sec, MSec;
	unsigned int i=0;
	int count=0;
	int ampmtype=0;
	DecodeDate(Year,Month,Day);
	DecodeTime(Hour,Min,Sec,MSec);

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
			Result<<FormatString(ShortDateFormat);
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
				Result<<Day;
				break;
			case 2:
				if(Day<10)
					Result=Result+"0";
				Result<<Day;
				break;
			case 3:
				Result<<ShortDayNames[DayOfWeek()];
				break;
			case 4:
				Result<<LongDayNames[DayOfWeek()];
				break;
			case 5:
				Result<<FormatString(ShortDateFormat);
				break;
			case 6:
				Result<<FormatString(LongDateFormat);
				break;
			default:
				Log(LM_ERROR, Parent->getLogMessage("SYS_ERRORS/INVALID_FORMAT"),
					count, format[i], format.c_str());
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
				Result<<Month;
				break;
			case 2:
				if(Month<10)
					Result=Result+"0";
				Result<<Month;
				break;
			case 3:
				Result<<ShortMonthNames[Month-1];
				break;
			case 4:
				Result<<LongMonthNames[Month-1];
				break;
			default:
				Log(LM_ERROR, Parent->getLogMessage("SYS_ERRORS/INVALID_FORMAT"),
					count, format[i], format.c_str());
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
				Result<<Year%100;
				break;
			case 4:
				Result<<Year;
				break;
			default:
				Log(LM_ERROR, Parent->getLogMessage("SYS_ERRORS/INVALID_FORMAT"),
					count, format[i], format.c_str());
			}
			break;
		case 'h':
			if(i+1<format.size()&&tolower(format[i+1])=='h')
			{
				i++;
				if(ampmtype>0)
				{
					if(Hour%12<10)
						Result=Result+"0";
				}
				else
				{
					if(Hour<10)
						Result=Result+"0";
				}

			}
			if(ampmtype>0)
				Result<<Hour%12;
			else
				Result<<Hour;
			break;
		case 'n':
			if(i+1<format.size()&&tolower(format[i+1])=='n')
			{
				i++;
				if(Min<10)
					Result=Result+"0";
			}
			Result<<Min;
			break;
		case 's':
			if(i+1<format.size()&&tolower(format[i+1])=='s')
			{
				i++;
				if(Sec<10)
					Result=Result+"0";
			}
			Result<<Sec;
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
				if(MSec<100)
					Result=Result+"0";
				if(MSec<10)
					Result=Result+"0";
				break;
			default:
				Log(LM_ERROR, Parent->getLogMessage("SYS_ERRORS/INVALID_FORMAT"),
					count, format[i], format.c_str());
			}
			Result<<MSec;
			break;
		case 't':
			if(i+1<format.size()&&tolower(format[i+1])=='t')
			{
				i++;
				Result=Result+FormatString(LongTimeFormat);
			}
			else
				Result=Result+FormatString(ShortTimeFormat);
			break;
		case 'a':
			if(i+2<format.size()&&format[i+1]=='/'&&tolower(format[i+2])=='p')
			{
				//found a/p
				i=i+2;
				if(Hour<12)
					Result=Result+"a";
				else
					Result=Result+"p";
			}
			else if(i+3<format.size()&&tolower(format[i+1])=='m'&&tolower(format[i+2])=='p'&&tolower(format[i+3])=='m')
			{
				//found ampm
				i=i+3;
				if(Hour<12)
					Result=Result+TimeAMString;
				else
					Result=Result+TimePMString;
			}
			else if(i+4<format.size()&&tolower(format[i+1])=='m'&&format[i+2]=='/'&&tolower(format[i+3])=='p'&&tolower(format[i+4])=='m')
			{
				//found am/pm
				i=i+2;
				if(Hour<12)
					Result=Result+"am";
				else
					Result=Result+"pm";
			}
			else
			{
				Log(LM_ERROR, Parent->getLogMessage("SYS_ERRORS/INVALID_FORMAT"),
					1, format[i], format.c_str());
				Result=Result+"a";
			}
			break;
		case '/':
			Result=Result+DateSeparator;
			break;
		case ':':
			Result=Result+TimeSeparator;
			break;
		case '\'':
			i++;
			while(i<format.size()&&format[i]!='\'')
			{
				Result=Result+mstring(format[i]);
				i++;
			}
			break;
		case '"':
			i++;
			while(i<format.size()&&format[i]!='"')
			{
				Result=Result+mstring(format[i]);
				i++;
			}
			break;
		case ' ':
		case '.':
		case '\t':
		case '\n':
			Result=Result+mstring(format[i]);
			break;
		default:
			Log(LM_TRACE, Parent->getLogMessage("SYS_ERRORS/NOT_LITERAL"),
				format[i], format.c_str());
			Result=Result+mstring(format[i]);
		};
		i++;
	}
	return Result;
}
mstring mDateTime::DateString()const
{
	return FormatString(ShortDateFormat);
}
mstring mDateTime::TimeString()const
{
	return FormatString(LongTimeFormat);
}
mstring mDateTime::DateTimeString()const
{
	mstring Result;
	int Year,Month,Day,Hour,Min,Sec,MSec;
	DecodeDate(Year,Month,Day);
	DecodeTime(Hour,Min,Sec,MSec);
	if(Year!=0||Month!=0||Day!=0)
		Result=FormatString(LongDateFormat);
	if(Hour!=0||Min!=0||Sec!=0||MSec!=0)
	{
	    if(Result!="")
			Result<<" ";
	    Result<<FormatString(LongTimeFormat);
	}

	return Result;
}

mDateTime::operator time_t()
{
	int Year,Month,Day,Hour,Min,Sec,MSec;
	DecodeDate(Year,Month,Day);
	DecodeTime(Hour,Min,Sec,MSec);
	tm localtm;
	localtm.tm_year=Year-1900;
	localtm.tm_mon=Month-1;
	localtm.tm_mday=Day;
	localtm.tm_hour=Hour;
	localtm.tm_min=Min;
	localtm.tm_sec=Sec;
	localtm.tm_isdst=0;
	return mktime(&localtm);
}
mDateTime::operator mstring()
{
	return DateTimeString();
}

int mDateTime::DayOfWeek()const
{
	mDateTime knownmonday(1970,1,5);
	int Result=((int)(Val-knownmonday.Val))%7;
	return Result;
}

void mDateTime::DecodeDate(int &year, int &month, int &day)const
{
  const int D1 = 365;
  const int D4 = D1 * 4 + 1;
  const int D100 = D4 * 25 - 1;
  const int D400 = D100 * 4 + 1;
  int NumDays = (int)Val;
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
  year=Y+1900;

  int i=0;
  mDayTable &DayTable=GetDayTable(year);
  while(DayTable[i]<NumDays)
  {
	  M++;
	  NumDays-=DayTable[i];
	  i++;
  }
  day=NumDays;
  month=M;

}

void mDateTime::DecodeTime(int &hour, int &min, int &sec, int &msec)const
{
	//(Hour * 3600000 + Min * 60000 + Sec * 1000 + MSec) / MSecsPerDay;
	int CurrentVal=(int)(fmod(Val,1.0)*(double)MSecsPerDay);
	int LeftOver;
	//Time = ((double)Hour * 3600000.0 + (double)Min * 60000.0 + (double)Sec * 1000.0 + (double)MSec) / (double)MSecsPerDay;

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

mDateTime Now()
{
	return mDateTime::CurrentDateTime();
}
mDateTime Date()
{
	return mDateTime::CurrentDate();
}
mDateTime Time()
{
	return mDateTime::CurrentTime();
}

mDateTime StringToDate(const mstring& in)
{
	int year=0,month=0,day=0;
	int first=0,second=0,third=0;
	mstring formatspec;
	mstring shortdateformat=ShortDateFormat;
	shortdateformat.MakeLower();
	formatspec="%d"+DateSeparator+"%d"+DateSeparator+"%d";
	sscanf(in.c_str(),formatspec.c_str(),&first,&second,&third);
	if(shortdateformat.Before(DateSeparator)=="mm"||shortdateformat.Before(DateSeparator)=="m")
		month=first;
	else if(shortdateformat.Before(DateSeparator)=="dd"||shortdateformat.Before(DateSeparator)=="d")
		day=first;
	else if(shortdateformat.Before(DateSeparator)=="yyyy")
		year=first;
	else if(shortdateformat.Before(DateSeparator)=="yy")
		if(first+100 <= Now().Year2() + 150)
			year=first+Now().Century();
		else
			year=first+Now().Century()-100;
	if(shortdateformat.After(DateSeparator).Before(DateSeparator)=="mm"||shortdateformat.After(DateSeparator).Before(DateSeparator)=="m")
		month=second;
	else if(shortdateformat.After(DateSeparator).Before(DateSeparator)=="dd"||shortdateformat.After(DateSeparator).Before(DateSeparator)=="d")
		day=second;
	else if(shortdateformat.After(DateSeparator).Before(DateSeparator)=="yyyy")
		year=second;
	else if(shortdateformat.After(DateSeparator).Before(DateSeparator)=="yy")
		if(second+100 <= Now().Year2() + 150)
			year=second+Now().Century();
		else
			year=second+Now().Century()-100;
	if(shortdateformat.After(DateSeparator).After(DateSeparator)=="mm"||shortdateformat.After(DateSeparator).After(DateSeparator)=="m")
		month=third;
	else if(shortdateformat.After(DateSeparator).After(DateSeparator)=="dd"||shortdateformat.After(DateSeparator).After(DateSeparator)=="d")
		day=third;
	else if(shortdateformat.After(DateSeparator).After(DateSeparator)=="yyyy")
		year=third;
	else if(shortdateformat.After(DateSeparator).After(DateSeparator)=="yy")
		if(third+100 <= Now().Year2() + 150)
			year=third+Now().Century();
		else
			year=third+Now().Century()-100;
	return mDateTime(year,month,day);
}

#if 0
mDateTime StringToTime(const mstring& in)
{

}
mDateTime StringToDateTime(const mstring& in);

#endif


mstring mDateTime::timetstring()const
{
    mstring Result;
    time_t Res2;
	int Year,Month,Day,Hour,Min,Sec,MSec;
	DecodeDate(Year,Month,Day);
	DecodeTime(Hour,Min,Sec,MSec);
	tm localtm;
	localtm.tm_year=Year-1900;
	localtm.tm_mon=Month-1;
	localtm.tm_mday=Day;
	localtm.tm_hour=Hour;
	localtm.tm_min=Min;
	localtm.tm_sec=Sec+1;
	localtm.tm_isdst=0;
	Res2=mktime(&localtm);
    Result<<(unsigned long)Res2;
    return Result;
}

int mDateTime::MSecond()
{
    int Hours,Seconds,Minutes,Msecs;
    DecodeTime(Hours,Seconds,Minutes,Msecs);
    return Msecs;
}
int mDateTime::Second()
{
    int Hours,Seconds,Minutes,Msecs;
    DecodeTime(Hours,Seconds,Minutes,Msecs);
    return Seconds;
}
int mDateTime::Minute()
{
    int Hours,Seconds,Minutes,Msecs;
    DecodeTime(Hours,Seconds,Minutes,Msecs);
    return Minutes;
}
int mDateTime::Hour()
{
    int Hours,Seconds,Minutes,Msecs;
    DecodeTime(Hours,Seconds,Minutes,Msecs);
    return Hours;
}
int mDateTime::Day()
{
    int Year,Month,Day;
    DecodeDate(Year,Month,Day);
    return Day;
}
int mDateTime::Month()
{
    int Year,Month,Day;
    DecodeDate(Year,Month,Day);
    return Month;
}
int mDateTime::Year()
{
    int Year,Month,Day;
    DecodeDate(Year,Month,Day);
    return Year;
}
int mDateTime::Year2()
{
    int Year2=Year();
    while (Year2>100)
	Year2-=100;
    return Year2;
}
int mDateTime::Century()
{
    return Year()-Year2();
}
unsigned long mDateTime::MSecondsSince()
{
    mDateTime dummyvar=Now()-(*this);
    unsigned long CurrentVal=(unsigned long)(dummyvar.Val*(double)MSecsPerDay);
    return CurrentVal;
}

mstring mDateTime::Ago(bool gmt, mstring source)
{
    // Later we find out if this is a GMT time.
    return(DisectTime(SecondsSince(), source));
}

unsigned long mDateTime::SecondsSince()
{
    mDateTime dummyvar=Now()-(*this);
    unsigned long CurrentVal=(unsigned long)(dummyvar.Val*(double)SecsPerDay);
    return CurrentVal;
}

unsigned long mDateTime::MinutesSince()
{
    return (SecondsSince() / 60);
}

unsigned long mDateTime::HoursSince()
{
    return (MinutesSince() / 60);
}

unsigned long mDateTime::DaysSince()
{
    return (HoursSince() / 24);
}

unsigned long mDateTime::YearsSince()
{
    return (int)((double) DaysSince() / 365.25);
}

mstring DisectTime(long intime, mstring source)
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

    negamt = (long)(60.0 * 60.0 * 24.0 * 365.25);
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

mDateTime GMT(mDateTime in, bool to)
{
    ACE_OS::tzset();
    long offset = ACE_OS::timezone() * (to ? 1 : -1);
    double val = in.Internal();
    int days = 0, secs = 0;
    days = (int) val;
    val -= days;
    secs = (int) (val * (double) SecsPerDay);

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
    val = (double) days + ((double) secs * (1.0 / (double) SecsPerDay));
    return mDateTime(val);
}
