#ifndef WIN32
#pragma interface
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
#ifndef _DATETIME_H
#define _DATETIME_H
#include "pch.h"
RCSID(datetime_h, "@(#) $Id$");

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

#include "mstring.h"

class mDateTime;
extern mstring DisectTime(const long intime, const mstring & source = "");
extern mDateTime GMT(const mDateTime & in, const bool to);
extern mDateTime StringToDate(const mstring & in);
extern mDateTime StringToTime(const mstring & in);
extern mDateTime StringToDateTime(const mstring & in);

class mDateTime
{
    double Val;

public:
    mstring timetstring() const;
    enum mDateTimeFlag
    { Date, Time, DateTime };

    static mDateTime CurrentDate();
    static mDateTime CurrentTime();
    static mDateTime CurrentDateTime()
    {
	return mDateTime(time(NULL));
    }
    static mstring DateSeparator;
    static mstring ShortDateFormat;
    static mstring LongDateFormat;
    static mstring TimeSeparator;
    static mstring TimeAMString;
    static mstring TimePMString;
    static mstring ShortTimeFormat;
    static mstring LongTimeFormat;
    static mstring ShortMonthNames[12];
    static mstring LongMonthNames[12];
    static mstring ShortDayNames[7];
    static mstring LongDayNames[7];

    mDateTime()
    {
	Val = 0.0;
    }
    mDateTime(const mDateTime & src)
    {
	Val = src.Val;
    }
    mDateTime(const double src)
    {
	Val = src;
    }
    mDateTime(const time_t src)
    {
	*this = src;
    }
    mDateTime(const struct tm src)
    {
	*this = src;
    }
    mDateTime(const mstring & src, const mDateTimeFlag flag = DateTime);
    mDateTime(const unsigned int year, const unsigned int month, const unsigned int day)
    {
	mDateTime tmp;

	if (!DoEncodeDate(year, month, day, tmp))
	    tmp = 0.0;
	*this = tmp;
    }
    mDateTime(const unsigned int hour, const unsigned int min, const unsigned int sec, const unsigned int msec)
    {
	mDateTime tmp;

	if (!DoEncodeTime(hour, min, sec, msec, tmp))
	    tmp = 0.0;
	*this = tmp;
    }
    ~mDateTime()
    {
    }

    mDateTime &operator=(const mDateTime & in)
    {
	Val = in.Val;
	return *this;
    }
    mDateTime &operator=(const double in)
    {
	Val = in;
	return *this;
    }
    mDateTime &operator=(const time_t in)
    {
	tm *tmst;

	tmst = localtime(&in);
	*this = * tmst;
	return *this;
    }
    mDateTime &operator=(const struct tm in)
    {
	*this = mDateTime(in.tm_year + 1900, in.tm_mon + 1, in.tm_mday) + mDateTime(in.tm_hour, in.tm_min, in.tm_sec, 0);
	return *this;
    }
    mDateTime &operator+=(const mDateTime & in)
    {
	Val += in.Val;
	return *this;
    }
    mDateTime &operator+=(const double in)
    {
	Val += in;
	return *this;
    }
    mDateTime &operator+=(const time_t in)
    {
	Val += mDateTime(in).Val;
	return *this;
    }
    mDateTime &operator+=(const struct tm in)
    {
	Val += mDateTime(in).Val;
	return *this;
    }
    mDateTime &operator-=(const mDateTime & in)
    {
	Val -= in.Val;
	return *this;
    }
    mDateTime &operator-=(const double in)
    {
	Val -= in;
	return *this;
    }
    mDateTime &operator-=(const time_t in)
    {
	Val -= mDateTime(in).Val;
	return *this;
    }
    mDateTime &operator-=(const struct tm in)
    {
	Val -= mDateTime(in).Val;
	return *this;
    }
    mDateTime operator+(const mDateTime & in) const
    {
	mDateTime retval(Val);

	retval += in;
	return retval;
    }
    mDateTime operator+(const double in) const
    {
	mDateTime retval(Val);

	retval += in;
	return retval;
    }
    mDateTime operator+(const time_t in) const
    {
	mDateTime retval(Val);

	retval += in;
	return retval;
    }
    mDateTime operator+(const struct tm in) const
    {
	mDateTime retval(Val);

	retval += in;
	return retval;
    }
    mDateTime operator-(const mDateTime & in) const
    {
	mDateTime retval(Val);

	retval -= in;
	return retval;
    }
    mDateTime operator-(const double in) const
    {
	mDateTime retval(Val);

	retval -= in;
	return retval;
    }
    mDateTime operator-(const time_t in) const
    {
	mDateTime retval(Val);

	retval -= in;
	return retval;
    }
    mDateTime operator-(const struct tm in) const
    {
	mDateTime retval(Val);

	retval -= in;
	return retval;
    }
    bool operator==(const mDateTime & in) const
    {
	return (Val == in.Val);
    }
    bool operator!=(const mDateTime & in) const
    {
	return (Val != in.Val);
    }
    bool operator>(const mDateTime & in) const
    {
	return (Val > in.Val);
    }
    bool operator<(const mDateTime & in) const
    {
	return (Val < in.Val);
    }
    bool operator>=(const mDateTime & in) const
    {
	return (Val >= in.Val);
    }
    bool operator<=(const mDateTime & in) const
    {
	return (Val <= in.Val);
    }

    operator      double () const
    {
	return Val;
    }
    operator      time_t() const;
    operator      mstring() const
    {
	return DateTimeString();
    }
    double Internal() const
    {
	return Val;
    }

/* FormatString formats the date-and-time using the format given by 
   format. The following format specifiers are supported:

  c       Displays the date using the format given by the ShortDateFormat
          global variable, followed by the time using the format given by
          the LongTimeFormat global variable. The time is not displayed if
          the fractional part of the DateTime value is zero.

  d       Displays the day as a number without a leading zero (1-31).

  dd      Displays the day as a number with a leading zero (01-31).

  ddd     Displays the day as an abbreviation (Sun-Sat) using the strings
          given by the ShortDayNames global variable.

  dddd    Displays the day as a full name (Sunday-Saturday) using the strings
          given by the LongDayNames global variable.

  ddddd   Displays the date using the format given by the ShortDateFormat
          global variable.

  dddddd  Displays the date using the format given by the LongDateFormat
          global variable.

  m       Displays the month as a number without a leading zero (1-12). If
          the m specifier immediately follows an h or hh specifier, the
          minute rather than the month is displayed.

  mm      Displays the month as a number with a leading zero (01-12). If
          the mm specifier immediately follows an h or hh specifier, the
          minute rather than the month is displayed.

  mmm     Displays the month as an abbreviation (Jan-Dec) using the strings
          given by the ShortMonthNames global variable.

  mmmm    Displays the month as a full name (January-December) using the
          strings given by the LongMonthNames global variable.

  yy      Displays the year as a two-digit number (00-99).

  yyyy    Displays the year as a four-digit number (0000-9999).

  h       Displays the hour without a leading zero (0-23).

  hh      Displays the hour with a leading zero (00-23).

  n       Displays the minute without a leading zero (0-59).

  nn      Displays the minute with a leading zero (00-59).

  s       Displays the second without a leading zero (0-59).

  ss      Displays the second with a leading zero (00-59).

  t       Displays the time using the format given by the ShortTimeFormat
          global variable.

  tt      Displays the time using the format given by the LongTimeFormat
          global variable.

  am/pm   Uses the 12-hour clock for the preceding h or hh specifier, and
          displays 'am' for any hour before noon, and 'pm' for any hour
          after noon. The am/pm specifier can use lower, upper, or mixed
          case, and the result is displayed accordingly.

  a/p     Uses the 12-hour clock for the preceding h or hh specifier, and
          displays 'a' for any hour before noon, and 'p' for any hour after
          noon. The a/p specifier can use lower, upper, or mixed case, and
          the result is displayed accordingly.

  ampm    Uses the 12-hour clock for the preceding h or hh specifier, and
          displays the contents of the TimeAMString global variable for any
          hour before noon, and the contents of the TimePMString global
          variable for any hour after noon.

  /       Displays the date separator character given by the DateSeparator
          global variable.

  :       Displays the time separator character given by the TimeSeparator
          global variable.

  'xx'    Characters enclosed in single or double quotes are displayed as-is,
  "xx"    and do not affect formatting.

  Format specifiers may be written in upper case as well as in lower case
  letters--both produce the same result.

  If the string given by the Format parameter is empty, the date and time
  value is formatted as if a 'c' format specifier had been given.

  The following example:

	
    S = StrToDateTime("2/15/95 10:30am").FormatString("'The meeting is on' dddd, mmmm d, yyyy, " +
      "'at' hh:mm AM/PM");

  assigns 'The meeting is on Wednesday, February 15, 1995 at 10:30 AM' to
  the string variable S. */
    mstring FormatString(const mstring & format) const;

    mstring DateString() const
    {
	return FormatString(ShortDateFormat);
    }
    mstring TimeString() const
    {
	return FormatString(LongTimeFormat);
    }
    mstring DateTimeString() const;

    int DayOfWeek() const;
    void DecodeDate(int &year, int &month, int &day) const;
    void DecodeTime(int &hour, int &min, int &sec, int &msec) const;

    int MSecond() const;
    int Second() const;
    int Minute() const;
    int Hour() const;
    int Day() const;
    int Month() const;
    int Year() const;
    int Year2() const
    {
	return Year() % 100;
    }
    int Century() const
    {
	return Year() - Year2();
    }

    unsigned long MSecondsSince() const;
    unsigned long SecondsSince() const;
    unsigned long MinutesSince() const
    {
	return (SecondsSince() / 60);
    }
    unsigned long HoursSince() const
    {
	return (MinutesSince() / 60);
    }
    unsigned long DaysSince() const
    {
	return (HoursSince() / 24);
    }
    unsigned long YearsSince() const
    {
	return static_cast < int > (static_cast < double > (DaysSince()) / 365.25);
    }
    mstring Ago(const mstring & source = "") const
    {
	return (DisectTime(SecondsSince(), source));
    }

    friend bool DoEncodeDate(const int Year, const int Month, const int Day, mDateTime & Date);
    friend bool DoEncodeTime(const int Hour, const int Min, const int Sec, const int MSec, mDateTime & Time);
};

#endif
