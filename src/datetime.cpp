#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
#endif

/* Magick IRC Services
**
** (c) 1997-2002 Preston Elder <prez@magick.tm>
** (c) 1998-2002 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any circumstances,
** however it may be added to if any modifications are made to this
** file.  All modified code must be clearly documented and labelled.
**
** This code is released under the GNU General Public License, which
** means (in short), it may be distributed freely, and may not be sold
** or used as part of any closed-source product.  Please check the
** COPYING file for full rights and restrictions of this software.
**
** ======================================================================= */
#define RCSID(x,y) const char *rcsid_datetime_cpp_ ## x () { return y; }
RCSID(datetime_cpp, "@(#)$Id$");

/* ======================================================================= **
**
** For official changes (by the Magick Development Team),please
** check the ChangeLog* files that come with this distribution.
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** ======================================================================= */

#include "magick.h"

mstring mDateTime::DateSeparator = "/";
mstring mDateTime::ShortDateFormat = "d/m/yyyy";
mstring mDateTime::LongDateFormat = "mmmm d',' yyyy";
mstring mDateTime::TimeSeparator = ":";
mstring mDateTime::TimeAMString = "am";
mstring mDateTime::TimePMString = "pm";
mstring mDateTime::ShortTimeFormat = "h:nn AMPM";
mstring mDateTime::LongTimeFormat = "hh:nn:ss AMPM";
mstring mDateTime::ShortMonthNames[12] =
{
"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
mstring mDateTime::LongMonthNames[12] =
{
"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
mstring mDateTime::ShortDayNames[7] =
{
"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
mstring mDateTime::LongDayNames[7] =
{
"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

static const int SecsPerDay = 24 * 60 * 60;
static const int MSecsPerDay = SecsPerDay * 1000;

typedef int mDayTable[12];
static mDayTable DayTable1 =
{
31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static mDayTable DayTable2 =
{
31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/* ---------------------------------------------------------- */

mDateTime mDateTime::CurrentDate()
{
    mDateTime Result;
    int iDay, iMonth, iYear;

    Result = mDateTime(time(NULL));
    Result.DecodeDate(iYear, iMonth, iDay);
    Result = mDateTime(iYear, iMonth, iDay);
    return Result;
}

mDateTime mDateTime::CurrentTime()
{
    mDateTime Result;
    int iHour, iMin, iSec, iMSec;

    Result = mDateTime(time(NULL));
    Result.DecodeTime(iHour, iMin, iSec, iMSec);
    Result = mDateTime(iHour, iMin, iSec, iMSec);
    return Result;
}

mDateTime::mDateTime(const mstring & src, const mDateTimeFlag flag)
{
    LOG(LM_ERROR, "SYS_ERRORS/NOT_IMPLEMENTED", ("mDateTime::mDateTime(const mstring& src, mDateTimeFlag flag)"));
#if 0
    if (flag == Date)
	*this = StringToDate(src);
    else if (flag = Time)
	*this = StringToTime(src);
    else if (flag = DateTime)
	*this = StringToDateTime(src);
#else
    static_cast < void > (src);
    static_cast < void > (flag);
#endif
}

static mDayTable &GetDayTable(int Year)
{
    if ((Year % 4 == 0) && ((Year % 100 != 0) || (Year % 400 == 0)))
	return DayTable2;
    else
	return DayTable1;
}

bool DoEncodeDate(const int iYear, const int iMonth, const int iDay, mDateTime & Date)
{
    int I;
    bool Result = false;
    int tmpDay = iDay - 1;

    mDayTable &DayTable = GetDayTable(iYear);

    if ((iYear >= 1) && (iYear <= 9999) && (iMonth >= 1) && (iMonth <= 12) && (iDay >= 1) && (iDay <= DayTable[iMonth - 1]))
    {
	for (I = 1; I < iMonth; I++)
	{
	    tmpDay += DayTable[I - 1];
	}
	I = iYear;
	tmpDay += ((I * 365) + (I / 4) - (I / 100) + (I / 400));
	Date.Val = static_cast < double > (tmpDay);

	Result = true;
    }
    return Result;
}

bool DoEncodeTime(const int iHour, const int iMin, const int iSec, const int iMSec, mDateTime & Time)
{
    bool Result = false;

    if ((iHour < 24) && (iMin < 60) && (iSec < 60) && (iMSec < 1000))
    {
	Time.Val =
	    (static_cast < double > (iHour) * 3600000.0 + static_cast < double > (iMin) * 60000.0 + static_cast < double >
	     (iSec) * 1000.0 + static_cast < double > (iMSec)) /static_cast < double > (MSecsPerDay);
	Result = true;
    }
    return Result;
}

mstring mDateTime::FormatString(const mstring & format) const
{
    mstring Result;
    int iYear, iMonth, iDay, iHour, iMin, iSec, iMSec;
    unsigned int i = 0;
    int count = 0;
    int ampmtype = 0;

    DecodeDate(iYear, iMonth, iDay);
    DecodeTime(iHour, iMin, iSec, iMSec);

    if (format.Find("a/p") != -1)
	ampmtype = 1;
    else if (format.Find("am/pm") != -1)
	ampmtype = 2;
    else if (format.Find("AMPM") != -1)
	ampmtype = 3;

    while (i < format.size())
    {
	switch (tolower(format[i]))
	{
	case 'c':
	    Result += FormatString(ShortDateFormat);
	    break;
	case 'd':
	    count = 1;
	    while (i + 1 < format.size() && tolower(format[i + 1]) == 'd')
	    {
		i++;
		count++;
	    }
	    switch (count)
	    {
	    case 1:
		Result += iDay;
		break;
	    case 2:
		if (iDay < 10)
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
		LOG(LM_ERROR, "SYS_ERRORS/INVALID_FORMAT", (count, format[i], format));
	    };
	    break;
	case 'm':
	    count = 1;
	    while (i + 1 < format.size() && tolower(format[i + 1]) == 'm')
	    {
		i++;
		count++;
	    }
	    switch (count)
	    {
	    case 1:
		Result += iMonth;
		break;
	    case 2:
		if (iMonth < 10)
		    Result += "0";
		Result += iMonth;
		break;
	    case 3:
		Result += ShortMonthNames[iMonth - 1];
		break;
	    case 4:
		Result += LongMonthNames[iMonth - 1];
		break;
	    default:
		LOG(LM_ERROR, "SYS_ERRORS/INVALID_FORMAT", (count, format[i], format));
	    }
	    break;
	case 'y':
	    count = 1;
	    while (i + 1 < format.size() && tolower(format[i + 1]) == 'y')
	    {
		i++;
		count++;
	    }
	    switch (count)
	    {
	    case 2:
		Result += iYear % 100;
		break;
	    case 4:
		Result += iYear;
		break;
	    default:
		LOG(LM_ERROR, "SYS_ERRORS/INVALID_FORMAT", (count, format[i], format));
	    }
	    break;
	case 'h':
	    if (i + 1 < format.size() && tolower(format[i + 1]) == 'h')
	    {
		i++;
		if (ampmtype > 0)
		{
		    if (iHour % 12 < 10)
			Result += "0";
		}
		else
		{
		    if (iHour < 10)
			Result += "0";
		}
	    }
	    if (ampmtype > 0)
		Result += iHour % 12;
	    else
		Result += iHour;
	    break;
	case 'n':
	    if (i + 1 < format.size() && tolower(format[i + 1]) == 'n')
	    {
		i++;
		if (iMin < 10)
		    Result += "0";
	    }
	    Result += iMin;
	    break;
	case 's':
	    if (i + 1 < format.size() && tolower(format[i + 1]) == 's')
	    {
		i++;
		if (iSec < 10)
		    Result += "0";
	    }
	    Result += iSec;
	    break;
	case 'u':
	    count = 1;
	    while (i + 1 < format.size() && tolower(format[i + 1]) == 'u')
	    {
		i++;
		count++;
	    }
	    switch (count)
	    {
	    case 1:
		break;
	    case 3:
		if (iMSec < 100)
		    Result += "0";
		if (iMSec < 10)
		    Result += "0";
		break;
	    default:
		LOG(LM_ERROR, "SYS_ERRORS/INVALID_FORMAT", (count, format[i], format));
	    }
	    Result += iMSec;
	    break;
	case 't':
	    if (i + 1 < format.size() && tolower(format[i + 1]) == 't')
	    {
		i++;
		Result += FormatString(LongTimeFormat);
	    }
	    else
		Result += FormatString(ShortTimeFormat);
	    break;
	case 'a':
	    if (i + 2 < format.size() && format.substr(i + 1, 2).IsSameAs("/p", true))
	    {
		//found a/p
		i = i + 2;
		if (iHour < 12)
		    Result += "a";
		else
		    Result += "p";
	    }
	    else if (i + 3 < format.size() && format.substr(i + 1, 3).IsSameAs("MPM", true))
	    {
		//found ampm
		i = i + 3;
		if (iHour < 12)
		    Result += TimeAMString;
		else
		    Result += TimePMString;
	    }
	    else if (i + 4 < format.size() && format.substr(i + 1, 4).IsSameAs("m/pm", true))
	    {
		//found am/pm
		i = i + 2;
		if (iHour < 12)
		    Result += "am";
		else
		    Result += "pm";
	    }
	    else
	    {
		LOG(LM_ERROR, "SYS_ERRORS/INVALID_FORMAT", (1, format[i], format));
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
	    while (i < format.size() && format[i] != '\'')
		Result += format[i++];
	    break;
	case '"':
	    i++;
	    while (i < format.size() && format[i] != '\"')
		Result += format[i++];
	case ' ':
	case '.':
	case '\t':
	case '\n':
	    Result += format[i];
	    break;
	default:
	    LOG(LM_TRACE, "SYS_ERRORS/NOT_LITERAL", (format[i], format));
	    Result += format[i];
	};
	i++;
    }
    return Result;
}

mstring mDateTime::DateTimeString() const
{
    mstring Result;
    int iYear, iMonth, iDay, iHour, iMin, iSec, iMSec;

    DecodeDate(iYear, iMonth, iDay);
    DecodeTime(iHour, iMin, iSec, iMSec);
    if (iYear != 0 || iMonth != 0 || iDay != 0)
	Result = FormatString(LongDateFormat);
    if (iHour != 0 || iMin != 0 || iSec != 0 || iMSec != 0)
    {
	if (!Result.empty())
	    Result += " ";
	Result += FormatString(LongTimeFormat);
    }

    return Result;
}

mDateTime::operator             time_t() const
{
    int iYear, iMonth, iDay, iHour, iMin, iSec, iMSec;

    DecodeDate(iYear, iMonth, iDay);
    if (iYear < 1900)
	return static_cast < time_t > (-1);

    DecodeTime(iHour, iMin, iSec, iMSec);
    tm localtm;

    localtm.tm_year = iYear - 1900;
    localtm.tm_mon = iMonth - 1;
    localtm.tm_mday = iDay;
    localtm.tm_hour = iHour;
    localtm.tm_min = iMin;
    localtm.tm_sec = iSec;
    localtm.tm_isdst = -1;
    return mktime(&localtm);
}

int mDateTime::DayOfWeek() const
{
    mDateTime knownmonday(1970, 1, 5);
    int Result = static_cast < int > (Val - knownmonday.Val) % 7;

    return Result;
}

void mDateTime::DecodeDate(int &year, int &month, int &day) const
{
    const int D1 = 365;
    const int D4 = D1 * 4 + 1;
    const int D100 = D4 * 25 - 1;
    const int D400 = D100 * 4 + 1;
    int NumDays = static_cast < int > (Val);
    int Y400, Y100, Y4, Y1, Y, M = 1;
    int LeftOver;

    LeftOver = NumDays % D400;
    Y400 = NumDays / D400;
    NumDays = LeftOver;

    LeftOver = NumDays % D100;
    Y100 = NumDays / D100;
    NumDays = LeftOver;

    LeftOver = NumDays % D4;
    Y4 = NumDays / D4;
    NumDays = LeftOver;

    LeftOver = NumDays % D1;
    Y1 = NumDays / D1;
    NumDays = LeftOver;

    Y = Y400 * 400 + Y100 * 100 + Y4 * 4 + Y1;
    year = Y;

    int i = 0;

    mDayTable &DayTable = GetDayTable(year);

    while (DayTable[i] < NumDays)
    {
	M++;
	NumDays -= DayTable[i];
	i++;
    }
    day = NumDays + 1;
    month = M;
}

void mDateTime::DecodeTime(int &hour, int &min, int &sec, int &msec) const
{
    //(iHour * 3600000 + iMin * 60000 + iSec * 1000 + iMSec) / MSecsPerDay;
    int CurrentVal = static_cast < int > (fmod(Val, 1.0) * static_cast < double > (MSecsPerDay));
    int LeftOver;

    LeftOver = CurrentVal % 3600000;
    hour = CurrentVal / 3600000;
    CurrentVal = LeftOver;

    LeftOver = CurrentVal % 60000;
    min = CurrentVal / 60000;
    CurrentVal = LeftOver;

    LeftOver = CurrentVal % 1000;
    sec = CurrentVal / 1000;
    msec = LeftOver;
}

mDateTime StringToDate(const mstring & in)
{
    int year = 0, month = 0, day = 0;
    int first = 0, second = 0, third = 0;
    mstring formatspec;
    mstring shortdateformat = mDateTime::ShortDateFormat;

    shortdateformat.MakeLower();
    formatspec = "%d" + mDateTime::DateSeparator + "%d" + mDateTime::DateSeparator + "%d";
    sscanf(in.c_str(), formatspec.c_str(), &first, &second, &third);
    if (shortdateformat.Before(mDateTime::DateSeparator) == "mm" || shortdateformat.Before(mDateTime::DateSeparator) == "m")
	month = first;
    else if (shortdateformat.Before(mDateTime::DateSeparator) == "dd" ||
	     shortdateformat.Before(mDateTime::DateSeparator) == "d")
	day = first;
    else if (shortdateformat.Before(mDateTime::DateSeparator) == "yyyy")
	year = first;
    else if (shortdateformat.Before(mDateTime::DateSeparator) == "yy")
	if (first + 100 <= mDateTime::CurrentDateTime().Year2() + 150)
	    year = first + mDateTime::CurrentDateTime().Century();
	else
	    year = first + mDateTime::CurrentDateTime().Century() - 100;
    if (shortdateformat.After(mDateTime::DateSeparator).Before(mDateTime::DateSeparator) == "mm" ||
	shortdateformat.After(mDateTime::DateSeparator).Before(mDateTime::DateSeparator) == "m")
	month = second;
    else if (shortdateformat.After(mDateTime::DateSeparator).Before(mDateTime::DateSeparator) == "dd" ||
	     shortdateformat.After(mDateTime::DateSeparator).Before(mDateTime::DateSeparator) == "d")
	day = second;
    else if (shortdateformat.After(mDateTime::DateSeparator).Before(mDateTime::DateSeparator) == "yyyy")
	year = second;
    else if (shortdateformat.After(mDateTime::DateSeparator).Before(mDateTime::DateSeparator) == "yy")
	if (second + 100 <= mDateTime::CurrentDateTime().Year2() + 150)
	    year = second + mDateTime::CurrentDateTime().Century();
	else
	    year = second + mDateTime::CurrentDateTime().Century() - 100;
    if (shortdateformat.After(mDateTime::DateSeparator).After(mDateTime::DateSeparator) == "mm" ||
	shortdateformat.After(mDateTime::DateSeparator).After(mDateTime::DateSeparator) == "m")
	month = third;
    else if (shortdateformat.After(mDateTime::DateSeparator).After(mDateTime::DateSeparator) == "dd" ||
	     shortdateformat.After(mDateTime::DateSeparator).After(mDateTime::DateSeparator) == "d")
	day = third;
    else if (shortdateformat.After(mDateTime::DateSeparator).After(mDateTime::DateSeparator) == "yyyy")
	year = third;
    else if (shortdateformat.After(mDateTime::DateSeparator).After(mDateTime::DateSeparator) == "yy")
	if (third + 100 <= mDateTime::CurrentDateTime().Year2() + 150)
	    year = third + mDateTime::CurrentDateTime().Century();
	else
	    year = third + mDateTime::CurrentDateTime().Century() - 100;
    return mDateTime(year, month, day);
}

#if 0
mDateTime StringToTime(const mstring & in)
{

}

mDateTime StringToDateTime(const mstring & in)
{

}

#endif

mstring mDateTime::timetstring() const
{
    mstring Result;
    time_t Res2;
    int iYear, iMonth, iDay, iHour, iMin, iSec, iMSec;

    DecodeDate(iYear, iMonth, iDay);
    if (iYear < 1900)
	return static_cast < time_t > (-1);
    DecodeTime(iHour, iMin, iSec, iMSec);
    tm localtm;

    localtm.tm_year = iYear - 1900;
    localtm.tm_mon = iMonth - 1;
    localtm.tm_mday = iDay;
    localtm.tm_hour = iHour;
    localtm.tm_min = iMin;
    localtm.tm_sec = iSec;
    localtm.tm_isdst = -1;
    Res2 = mktime(&localtm);
    Result << static_cast < unsigned long > (Res2);

    return Result;
}

int mDateTime::MSecond() const
{
    int iHour, iSecond, iMinute, iMSec;

    DecodeTime(iHour, iSecond, iMinute, iMSec);
    return iMSec;
}

int mDateTime::Second() const
{
    int iHour, iSecond, iMinute, iMSec;

    DecodeTime(iHour, iSecond, iMinute, iMSec);
    return iSecond;
}

int mDateTime::Minute() const
{
    int iHour, iSecond, iMinute, iMSec;

    DecodeTime(iHour, iSecond, iMinute, iMSec);
    return iMinute;
}

int mDateTime::Hour() const
{
    int iHour, iSecond, iMinute, iMSec;

    DecodeTime(iHour, iSecond, iMinute, iMSec);
    return iHour;
}

int mDateTime::Day() const
{
    int iYear, iMonth, iDay;

    DecodeDate(iYear, iMonth, iDay);
    return iDay;
}

int mDateTime::Month() const
{
    int iYear, iMonth, iDay;

    DecodeDate(iYear, iMonth, iDay);
    return iMonth;
}

int mDateTime::Year() const
{
    int iYear, iMonth, iDay;

    DecodeDate(iYear, iMonth, iDay);
    return iYear;
}

unsigned long mDateTime::MSecondsSince() const
{
    mDateTime dummyvar = mDateTime::CurrentDateTime() - (*this);
    unsigned long CurrentVal = static_cast < unsigned long > (dummyvar.Val * static_cast < double > (MSecsPerDay));

    return CurrentVal;
}

unsigned long mDateTime::SecondsSince() const
{
    mDateTime dummyvar = mDateTime::CurrentDateTime() - (*this);
    unsigned long CurrentVal = static_cast < unsigned long > (dummyvar.Val * static_cast < double > (SecsPerDay));

    return CurrentVal;
}

mstring DisectTime(const long intime, const mstring & source)
{
    mstring Result = "";
    long Years = 0, Weeks = 0, Days = 0, Hours = 0, Minutes = 0, Seconds = 0, negamt = 0;

    if (intime < 0)
    {
	Result << "-";
	Seconds = intime * -1;
    }
    else
    {
	Seconds = intime;
    }

    negamt = static_cast < long > (60.0 * 60.0 * 24.0 * 365.25);

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

    if (Years > 0)
    {
	Result << Years << " " << (Years ==
				   1 ? Magick::instance().getMessage(source,
								     "VALS/TIME_YEAR") : Magick::instance().getMessage(source,
														       "VALS/TIME_YEARS"));
	if (Weeks)
	    Result << ", " << Weeks << " " << (Weeks ==
					       1 ? Magick::instance().getMessage(source,
										 "VALS/TIME_WEEK") : Magick::instance().
					       getMessage(source, "VALS/TIME_WEEKS"));
	if (Days)
	    Result << ", " << Days << " " << (Days ==
					      1 ? Magick::instance().getMessage(source,
										"VALS/TIME_DAY") : Magick::instance().
					      getMessage(source, "VALS/TIME_DAYS"));
	if (Hours || Minutes || Seconds)
	    Result << ", " << (Hours >= 10 ? "" : "0") << Hours << ":" << (Minutes >=
									   10 ? "" : "0") << Minutes << ":" << (Seconds >=
														10 ? "" : "0")
		<< Seconds;
    }
    else if (Weeks > 0)
    {
	Result << Weeks << " " << (Weeks ==
				   1 ? Magick::instance().getMessage(source,
								     "VALS/TIME_WEEK") : Magick::instance().getMessage(source,
														       "VALS/TIME_WEEKS"));
	if (Days)
	    Result << ", " << Days << " " << (Days ==
					      1 ? Magick::instance().getMessage(source,
										"VALS/TIME_DAY") : Magick::instance().
					      getMessage(source, "VALS/TIME_DAYS"));
	if (Hours || Minutes || Seconds)
	    Result << ", " << (Hours >= 10 ? "" : "0") << Hours << ":" << (Minutes >=
									   10 ? "" : "0") << Minutes << ":" << (Seconds >=
														10 ? "" : "0")
		<< Seconds;
    }
    else if (Days > 0)
    {
	Result << Days << " " << (Days ==
				  1 ? Magick::instance().getMessage(source,
								    "VALS/TIME_DAY") : Magick::instance().getMessage(source,
														     "VALS/TIME_DAYS"));
	if (Hours || Minutes || Seconds)
	    Result << ", " << (Hours >= 10 ? "" : "0") << Hours << ":" << (Minutes >=
									   10 ? "" : "0") << Minutes << ":" << (Seconds >=
														10 ? "" : "0")
		<< Seconds;
    }
    else if (Hours > 0)
    {
	Result << Hours << " " << (Hours ==
				   1 ? Magick::instance().getMessage(source,
								     "VALS/TIME_HOUR") : Magick::instance().getMessage(source,
														       "VALS/TIME_HOURS"));
	if (Minutes)
	    Result << ", " << Minutes << " " << (Minutes ==
						 1 ? Magick::instance().getMessage(source,
										   "VALS/TIME_MIN") : Magick::instance().
						 getMessage(source, "VALS/TIME_MINS"));
	if (Seconds)
	    Result << ", " << Seconds << " " << (Seconds ==
						 1 ? Magick::instance().getMessage(source,
										   "VALS/TIME_SEC") : Magick::instance().
						 getMessage(source, "VALS/TIME_SECS"));
    }
    else if (Minutes > 0)
    {
	Result << Minutes << " " << (Minutes ==
				     1 ? Magick::instance().getMessage(source,
								       "VALS/TIME_MIN") : Magick::instance().getMessage(source,
															"VALS/TIME_MINS"));
	if (Seconds)
	    Result << ", " << Seconds << " " << (Seconds ==
						 1 ? Magick::instance().getMessage(source,
										   "VALS/TIME_SEC") : Magick::instance().
						 getMessage(source, "VALS/TIME_SECS"));
    }
    else if (Seconds > 0)
    {
	Result << Seconds << " " << (Seconds ==
				     1 ? Magick::instance().getMessage(source,
								       "VALS/TIME_SEC") : Magick::instance().getMessage(source,
															"VALS/TIME_SECS"));
    }
    else
    {
	Result << Magick::instance().getMessage(source, "VALS/TIME_NOW");
    }
    return Result;
}

mDateTime GMT(const mDateTime & in, const bool to)
{
    ACE_OS::tzset();
    long offset = ACE_OS::timezone() * (to ? 1 : -1);
    double val = in.Internal();
    int days = 0, secs = 0;
    days = static_cast < int > (val);

    val -= days;
    secs = static_cast < int > (val * static_cast < double > (SecsPerDay));

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
    val = static_cast < double > (days) + (static_cast < double > (secs) * (1.0 / static_cast < double > (SecsPerDay)));

    return mDateTime(val);
}
