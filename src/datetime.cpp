// RCS-ID:      $Id$
#include "datetime.h"
#include "log.h"

mstring DateSeparator="/";
mstring ShortDateFormat="m/d/yy";
mstring LongDateFormat="mmmm d, yyyy";
mstring TimeSeparator=":";
mstring TimeAMString="am";
mstring TimePMString="pm";
mstring ShortTimeFormat="h:mm AMPM";
mstring LongTimeFormat="hh:mm:ss AMPM";
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
	return ::Date();
}

mDateTime mDateTime::CurrentTime()
{
	return ::Time();
}

mDateTime mDateTime::CurrentDateTime()
{
	return ::Now();
}

mDateTime::mDateTime(time_t src)
{
	*this=src;
}
mDateTime::mDateTime(const mstring& src, mDateTimeFlag flag)
{
	if(flag==Date)
		*this=StringToDate(src);
	else if(flag=Time)
		*this=StringToTime(src);
	else if(flag=DateTime)
		*this=StringToDateTime(src);
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

mDayTable *GetDayTable(int Year)
{
  if(IsLeapYear(Year))
	  return &DayTable2;
  else 
	  return &DayTable1;
}

bool DoEncodeDate(int Year, int Month, int Day, mDateTime& Date)
{
  int I;
  mDayTable *DayTable;
  bool Result;
  int tmpDay=Day;

  Result = false;
  DayTable = GetDayTable(Year);
  if ((Year >= 1) && (Year <= 9999) && (Month >= 1) && (Month <= 12) &&
    (Day >= 1) && (Day <= *DayTable[Month]))
  {
    for(I = 1; I<Month;I++) 
		tmpDay+=*DayTable[I];
    I = Year - 1;
    Date = (double)(I * 365 + I / 4 - I / 100 + I / 400 + tmpDay - DateDelta);
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
    Time = (double)((Hour * 3600000 + Min * 60000 + Sec * 1000 + MSec) / MSecsPerDay);
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
	*this=mDateTime(tmst->tm_year+1900,tmst->tm_mon,tmst->tm_mday)+mDateTime(tmst->tm_hour,tmst->tm_min,tmst->tm_sec,0);
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

bool mDateTime::operator==(const mDateTime& in)
{
	if(Val==in.Val)
		return true;
	else
		return false;
}
bool mDateTime::operator!=(const mDateTime& in)
{
	if(Val!=in.Val)
		return true;
	else
		return false;
}
bool mDateTime::operator>(const mDateTime& in)
{
	if(Val>in.Val)
		return true;
	else
		return false;
}
bool mDateTime::operator<(const mDateTime& in)
{
	if(Val<in.Val)
		return true;
	else
		return false;
}
bool mDateTime::operator>=(const mDateTime& in)
{
	if(Val>=in.Val)
		return true;
	else
		return false;
}
bool mDateTime::operator<=(const mDateTime& in)
{
	if(Val<=in.Val)
		return true;
	else
		return false;
}

mstring mDateTime::FormatString(const mstring& format)
{
	mstring Result;
	int Year, Month, Day, Hour, Min, Sec, MSec;
	int i=0;
	int count=0;
	int ampmtype=0;
	char buffer[20];
	DecodeDate(Year,Month,Day);
	DecodeTime(Hour,Min,Sec,MSec);
	if(format.Find("am/pm")!=-1)
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
			Result+=FormatString(ShortDateFormat);
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
				itoa(Day,buffer,10);
				Result+=buffer;
				break;
			case 2:
				if(Day<10)
					Result+="0";
				itoa(Day,buffer,10);
				Result+=buffer;
				break;
			case 3:
				Result+=ShortDayNames[DayOfWeek()];
				break;
			case 4:
				Result+=LongDayNames[DayOfWeek()];
				break;
			case 5:
				Result+=FormatString(ShortDateFormat);
				break;
			case 6:
				Result+=FormatString(LongDateFormat);
				break;
			default:
				wxLogError("mDateTime::FormatString Invalid date format string");
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
				itoa(Month,buffer,10);
				Result+=buffer;
				break;
			case 2:
				if(Month<10)
					Result+="0";
				itoa(Month,buffer,10);
				Result+=buffer;
				break;
			case 3:
				Result+=ShortMonthNames[Month-1];
				break;
			case 4:
				Result+=LongMonthNames[Month-1];
				break;
			default:
				wxLogError("mDateTime::FormatString Invalid month format string");
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
				itoa(Year%100,buffer,10);
				Result+=buffer;
				break;
			case 4:
				itoa(Year,buffer,10);
				Result+=buffer;
				break;
			default:
				wxLogError("mDateTime::FormatString Invalid year format string");
			}
			break;
		case 'h':
			if(i+1<format.size()&&tolower(format[i+1])=='h')
			{
				i++;
				if(ampmtype>0)
				{
					if(Hour%12<10)
						Result+="0";
				}
				else
				{
					if(Hour<10)
						Result+="0";
				}

			}
			if(ampmtype>0)
				itoa(Hour%12,buffer,10);
			else
				itoa(Hour,buffer,10);
			Result+=buffer;
			break;
		case 'n':
			if(i+1<format.size()&&tolower(format[i+1])=='n')
			{
				i++;
				if(Min<10)
					Result+="0";
			}
			itoa(Min,buffer,10);
			Result+=buffer;
			break;
		case 's':
			if(i+1<format.size()&&tolower(format[i+1])=='s')
			{
				i++;
				if(Sec<10)
					Result+="0";
			}
			itoa(Sec,buffer,10);
			Result+=buffer;
			break;
		case 't':
			if(i+1<format.size()&&tolower(format[i+1])=='t')
			{
				i++;
				Result+=FormatString(LongTimeFormat);
			}
			else
				Result+=FormatString(ShortTimeFormat);
			break;
		case 'a':
			if(i+2<format.size()&&format[i+1]=='/'&&tolower(format[i+2])=='p')
			{
				//found a/p
				i=i+2;
				if(Hour<12)
					Result+="a";
				else
					Result+="p";
			}
			else if(i+3<format.size()&&tolower(format[i+1])=='m'&&tolower(format[i+2])=='p'&&tolower(format[i+3])=='m')
			{
				//found ampm
				i=i+3;
				if(Hour<12)
					Result+=TimeAMString;
				else
					Result+=TimePMString;
			}
			else if(i+4<format.size()&&tolower(format[i+1])=='m'&&format[i+2]=='/'&&tolower(format[i+3])=='p'&&tolower(format[i+4])=='m')
			{
				//found am/pm
				i=i+2;
				if(Hour<12)
					Result+="am";
				else
					Result+="pm";
			}
			else
			{
				wxLogWarning("mDateTime::FormatString, charachter '%c' should be inside quotes, taken as literal");
				Result+="a";
			}
			break;
		case '/':
			Result+=DateSeparator;
			break;
		case ':':
			Result+=TimeSeparator;
			break;
		case '\'':
			i++;
			while(i<format.size()&&format[i]!='\'')
				Result+=mstring(format[i]);
			break;
		case '"':
			i++;
			while(i<format.size()&&format[i]!='"')
				Result+=mstring(format[i]);
			break;
		case ' ':
		case '\t':
		case '\n':
			Result+=mstring(format[i]);
			break;
		default:
			wxLogWarning("mDateTime::FormatString, charachter '%c' should be inside quotes, taken as literal");
			Result+=mstring(format[i]);
		};
		i++;
	}
	return Result;
}
mstring mDateTime::DateString()
{
	return FormatString(ShortDateFormat);
}
mstring mDateTime::TimeString()
{
	return FormatString(LongTimeFormat);
}
mstring mDateTime::DateTimeString()
{
	mstring Result;
	int Year,Month,Day,Hour,Min,Sec,MSec;
	DecodeDate(Year,Month,Day);
	DecodeTime(Hour,Min,Sec,MSec);
	if(Year!=0||Month!=0||Day!=0)
		Result=FormatString(ShortDateFormat);
	if(Hour!=0||Min!=0||Sec!=0||MSec!=0)
	{
		if(Result!="")
			Result+=" ";
		Result+=FormatString(LongTimeFormat);
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
	localtm.tm_mon=Month;
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
int mDateTime::DayOfWeek()
{
	mDateTime knownmonday(1970,1,5);
	int Result=((int)(Val-knownmonday.Val))%7;
	return Result;
}
void mDateTime::DecodeDate(int &year, int &month, int &day)
{
  const int D1 = 365;
  const int D4 = D1 * 4 + 1;
  const int D100 = D4 * 25 - 1;
  const int D400 = D100 * 4 + 1;
  int NumDays = (int)Val;
  int Y400,Y100,Y4,Y1,Y,M=0,D;
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

  int i=0;
  mDayTable *DayTable=GetDayTable(Y);
  while(*DayTable[i]>NumDays)
  {
	  M++;
	  i++;
	  NumDays-=*DayTable[i];
  }
  D=NumDays;
  year=Y;
  month=M;
  day=D;

}
#if 0
	void DecodeTime(int &hour, int &min, int &sec, int& msec); 

wxOutputStream& operator<<(wxOutputStream& os, const mDateTime& src);
wxInputStream& operator>>(wxInputStream& is, mDateTime& src);

extern mDateTime Now();
extern mDateTime Date();
extern mDateTime Time();
extern mDateTime StringToDate(const mstring& in);
extern mDateTime StringToTime(const mstring& in);
extern mDateTime StringToDateTime(const mstring& in);

#endif