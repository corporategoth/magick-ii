#ifndef WIN32
#pragma interface
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
#ifndef _DATETIME_H
#define _DATETIME_H
#include "pch.h"
RCSID(datetime_h, "@(#) $Id$");

/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.42  2002/01/14 07:16:54  prez
** More pretty printing with a newer indent with C++ fixes (not totally done)
**
** Revision 1.41  2002/01/12 14:42:08  prez
** Pretty-printed all code ... looking at implementing an auto-prettyprint.
**
** Revision 1.40  2001/11/12 01:05:01  prez
** Added new warning flags, and changed code to reduce watnings ...
**
** Revision 1.39  2001/08/04 18:32:01  prez
** Made some changes for Hybrid 6 -- we now work with it ... mostly.
**
** Revision 1.38  2001/04/05 05:59:50  prez
** Turned off -fno-default-inline, and split up server.cpp, it should
** compile again with no special options, and have default inlines :)
**
** Revision 1.37  2001/04/02 02:13:27  prez
** Added inlines, fixed more of the exception code.
**
** Revision 1.36  2001/03/20 14:22:14  prez
** Finished phase 1 of efficiancy updates, we now pass mstring/mDateTime's
** by reference all over the place.  Next step is to stop using operator=
** to initialise (ie. use mstring blah(mstring) not mstring blah = mstring).
**
** Revision 1.35  2001/02/11 07:41:27  prez
** Enhansed support for server numerics, specifically for Unreal.
**
** Revision 1.33  2001/02/03 03:20:33  prez
** Fixed up some differences in previous committed versions ...
**
** Revision 1.30  2000/12/23 22:22:23  prez
** 'constified' all classes (ie. made all functions that did not need to
** touch another non-const function const themselves, good for data integrity).
**
** Revision 1.29  2000/09/11 10:58:19  prez
** Now saves in in GMT
**
** Revision 1.28  2000/09/09 02:17:47  prez
** Changed time functions to actuallt accept the source nick as a param
** so that the time values (minutes, etc) can be customized.  Also added
** weeks to the time output.
**
** Revision 1.27  2000/07/28 14:49:34  prez
** Ditched the old wx stuff, mconfig now in use, we're now ready to
** release (only got some conversion tests to do).
**
** Revision 1.26  2000/06/12 06:07:49  prez
** Added Usage() functions to get ACCURATE usage stats from various
** parts of services.  However bare in mind DONT use this too much
** as it has to go through every data item to grab the usages.
**
** Revision 1.25  2000/06/06 08:57:54  prez
** Finished off logging in backend processes except conver (which I will
** leave for now).  Also fixed some minor bugs along the way.
**
** Revision 1.24  2000/05/28 05:05:13  prez
** More makefile stuff ... Now we should work on all platforms.
** Added alot of checking for different .h files, functions, etc.
** So now all #define's are config.h based (also added a default
** windows config.h, which will need to be copied on these systems).
**
** Revision 1.23  2000/02/23 12:21:01  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.22  2000/02/16 12:59:37  ungod
** fixing for borland compilability
**
** Revision 1.21  2000/02/15 10:37:47  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

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

    operator    double () const
    {
	return Val;
    }
    operator    time_t() const;
    operator    mstring() const
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
