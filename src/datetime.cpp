// RCS-ID:      $Id$
#include "datetime.h"

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

#if 0
mstring mDateTime::FormatString(const mstring& format)
{
	mstring Result;
	
}
	mstring DateString()const;
	mstring TimeString()const;
	mstring DateTimeString()const;

	operator double()const{return Val;}
	operator time_t()const;
	operator mstring()const;

	int DayOfWeek();
	void DecodeDate(int &year, int &month, int &day);
	void DecodeTime(int &hour, int &min, int &sec, int& msec); 
};

wxOutputStream& operator<<(wxOutputStream& os, const mDateTime& src);
wxInputStream& operator>>(wxInputStream& is, mDateTime& src);

extern mDateTime Now();
extern mDateTime Date();
extern mDateTime Time();
extern mDateTime StringToDate(const mstring& in);
extern mDateTime StringToTime(const mstring& in);
extern mDateTime StringToDateTime(const mstring& in);

#endif