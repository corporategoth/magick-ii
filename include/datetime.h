// $Id$
//
// Magick IRC Services
// (c) 1997-1999 Preston A. Elder <prez@magick.tm>
// (c) 1998-1999 W. King <ungod@magick.tm>
//
// The above copywright may not be removed under any
// circumstances, however it may be added to if any
// modifications are made to this file.  All modified
// code must be clearly documented and labelled.
//
// ===================================================

#ifndef _DATETIME_H
#define _DATETIME_H

#include "mstring.h"
#include "mstream.h"

class mDateTime
{
    double Val;
public:
    mstring timetstring()const;
    enum mDateTimeFlag {Date, Time, DateTime};

    static mDateTime CurrentDate();
    static mDateTime CurrentTime();
    static mDateTime CurrentDateTime();

    mDateTime() {Val=0.0;}
    mDateTime(const mDateTime& src) {Val=src.Val;}
    mDateTime(double src) {Val=src;}
    mDateTime(time_t src);
    mDateTime(const mstring& src, mDateTimeFlag flag=DateTime);
    mDateTime(unsigned int year, unsigned int month, unsigned int day);
    mDateTime(unsigned int hour, unsigned int min, unsigned int sec, unsigned int msec);
    mDateTime& operator=(const mDateTime& in);

    mDateTime& operator=(double in);
    mDateTime& operator=(time_t in);
    mDateTime& operator+=(const mDateTime& in);
    mDateTime& operator+=(double in);
    mDateTime& operator+=(time_t in);
    mDateTime& operator-=(const mDateTime& in);
    mDateTime& operator-=(double in);
    mDateTime& operator-=(time_t in);
    mDateTime& operator+(const mDateTime& in);
    mDateTime& operator+(double in);
    mDateTime& operator+(time_t in);
    mDateTime& operator-(const mDateTime& in);
    mDateTime& operator-(double in);
    mDateTime& operator-(time_t in);

    bool operator==(const mDateTime& in)const;
    bool operator!=(const mDateTime& in)const;
    bool operator>(const mDateTime& in)const;
    bool operator<(const mDateTime& in)const;
    bool operator>=(const mDateTime& in)const;
    bool operator<=(const mDateTime& in)const;

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
    mstring FormatString(const mstring& format)const;

    mstring DateString()const;
    mstring TimeString()const;
    mstring DateTimeString()const;

    operator double(){return Val;}
    operator time_t();
    operator mstring();

    int DayOfWeek()const;
    void DecodeDate(int &year, int &month, int &day)const;
    void DecodeTime(int &hour, int &min, int &sec, int& msec)const;

    int MSecond();
    int Second();
    int Minute();
    int Hour();
    int Day();
    int Month();
    int Year();
    mstring Ago(bool call);
    mstring Disect(bool call);
    int MSecondsSince();
    int SecondsSince();
    int MinutesSince();
    int HoursSince();
    int DaysSince();
    int YearsSince();

    friend bool DoEncodeDate(int Year, int Month, int Day, mDateTime& Date);
    friend bool DoEncodeTime(int Hour, int Min, int Sec, int MSec, mDateTime& Time);
    friend wxOutputStream& operator<<(wxOutputStream& os, const mDateTime& src);
    friend wxInputStream& operator>>(wxInputStream& is, mDateTime& src);

};

wxOutputStream& operator<<(wxOutputStream& os, const mDateTime& src);
wxInputStream& operator>>(wxInputStream& is, mDateTime& src);

extern mDateTime Now();
extern mDateTime Date();
extern mDateTime Time();
extern mDateTime StringToDate(const mstring& in);
extern mDateTime StringToTime(const mstring& in);
extern mDateTime StringToDateTime(const mstring& in);

extern mstring DateSeparator;
extern mstring ShortDateFormat;
extern mstring LongDateFormat;
extern mstring TimeSeparator;
extern mstring TimeAMString;
extern mstring TimePMString;
extern mstring ShortTimeFormat;
extern mstring LongTimeFormat;
extern mstring ShortMonthNames[12];
extern mstring LongMonthNames[12];
extern mstring ShortDayNames[7];
extern mstring LongDayNames[7];

#endif
