#ifndef WIN32
#pragma interface
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
#ifndef _DATETIME_H
#define _DATETIME_H
#include "pch.h"
static const char *ident_datetime_h = "@(#) $Id$";
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
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
    double Internal() { return Val; }

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
    int Year2();
    int Century();
    mstring Ago(bool call = false);
    unsigned long MSecondsSince();
    unsigned long SecondsSince();
    unsigned long MinutesSince();
    unsigned long HoursSince();
    unsigned long DaysSince();
    unsigned long YearsSince();

    friend bool DoEncodeDate(int Year, int Month, int Day, mDateTime& Date);
    friend bool DoEncodeTime(int Hour, int Min, int Sec, int MSec, mDateTime& Time);

};

mstring DisectTime(long intime);

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
