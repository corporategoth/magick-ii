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
#ifndef _MCONFIG_H
#define _MCONFIG_H
#include "pch.h"
RCSID(mconfig_h, "@(#) $Id$");

/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.25  2002/01/14 07:16:54  prez
** More pretty printing with a newer indent with C++ fixes (not totally done)
**
** Revision 1.24  2002/01/12 14:42:08  prez
** Pretty-printed all code ... looking at implementing an auto-prettyprint.
**
** Revision 1.23  2001/12/12 03:31:15  prez
** Re-wrote the occurances/find/replace functions in mstring to actually work
** with contents that includes a binary 0.  Also fixed PreParse in mconfig.
**
** Revision 1.22  2001/11/12 01:05:01  prez
** Added new warning flags, and changed code to reduce watnings ...
**
** Revision 1.21  2001/11/03 21:02:50  prez
** Mammoth change, including ALL changes for beta12, and all stuff done during
** the time GOTH.NET was down ... approx. 3 months.  Includes EPONA conv utils.
**
** Revision 1.20  2001/04/05 05:59:50  prez
** Turned off -fno-default-inline, and split up server.cpp, it should
** compile again with no special options, and have default inlines :)
**
** Revision 1.19  2001/04/02 02:13:27  prez
** Added inlines, fixed more of the exception code.
**
** Revision 1.18  2001/03/27 07:04:30  prez
** All maps have been hidden, and are now only accessable via. access functions.
**
** Revision 1.17  2001/03/20 14:22:14  prez
** Finished phase 1 of efficiancy updates, we now pass mstring/mDateTime's
** by reference all over the place.  Next step is to stop using operator=
** to initialise (ie. use mstring blah(mstring) not mstring blah = mstring).
**
** Revision 1.16  2001/02/03 03:20:33  prez
** Fixed up some differences in previous committed versions ...
**
** Revision 1.13  2000/12/23 22:22:23  prez
** 'constified' all classes (ie. made all functions that did not need to
** touch another non-const function const themselves, good for data integrity).
**
** Revision 1.12  2000/12/19 07:24:53  prez
** Massive updates.  Linux works again, added akill reject threshold, and
** lots of other stuff -- almost ready for b6 -- first beta after the
** re-written strings class.  Also now using log adapter!
**
** Revision 1.11  2000/07/28 14:49:34  prez
** Ditched the old wx stuff, mconfig now in use, we're now ready to
** release (only got some conversion tests to do).
**
** Revision 1.10  2000/07/24 17:50:35  ungod
** should be finished... not tested
**
** Revision 1.9  2000/06/23 14:23:37  ungod
** more completion of the ceNode class and more work done on mConfigEngine
**
** Revision 1.8  2000/06/23 12:49:13  ungod
** completion of the ceNode class
**
** Revision 1.7  2000/05/28 05:05:13  prez
** More makefile stuff ... Now we should work on all platforms.
** Added alot of checking for different .h files, functions, etc.
** So now all #define's are config.h based (also added a default
** windows config.h, which will need to be copied on these systems).
**
** Revision 1.6  2000/05/25 11:49:32  ungod
** even more mConfigEngine meaty bits. (should we change it's name to Good-O's?)
**
** Revision 1.5  2000/05/21 14:01:09  ungod
** body for decomment and load/save to files.
**
** Revision 1.4  2000/05/20 15:17:00  prez
** Changed LOG system to use ACE's log system, removed wxLog, and
** added wrappers into pch.h and magick.cpp.
**
** Revision 1.3  2000/05/20 01:20:59  ungod
** more meaty goodness in the quest for a better config engine.
**
** Revision 1.2  2000/05/20 00:08:01  ungod
** getting ConfigEngine compiling and cleaning up SXP to stop circular includes of "datetime.h"
**
** Revision 1.1  2000/05/19 13:11:34  ungod
** initial checkin of the new config engine, class structure is in, but no code in it.
**
**
** ========================================================== */

#include "mstring.h"

class ceNode
{
    friend class mConfigEngine;
private:
      mstring i_Name;
      map < mstring, mstring > i_keys;
      map < mstring, ceNode * > i_children;
public:
      ceNode()
    {
    }
    ceNode(ceNode & in)
    {
	*this = in;
    }
    ~ceNode();
    ceNode &operator=(const ceNode & in);
    bool operator==(const ceNode & in) const;
    bool operator<(const ceNode & in) const
    {
	return (i_Name < in.i_Name);
    }
    mstring Name()
    {
	return i_Name;
    }
    bool SetKey(const mstring & KeyName, const mstring & Value);
    bool DeleteKey(const mstring & KeyName);
    bool CreateNode(const mstring & NodeName);
    bool DeleteNode(const mstring & NodeName);
    bool NodeExists(const mstring & NodeName) const;
    bool KeyExists(const mstring & KeyName) const;
    mstring GetKey(const mstring & KeyName, const mstring & DefValue) const;
    ceNode *GetNode(const mstring & NodeName);
    mstring Write(const mstring & KeyName, const mstring & Value);
    map < mstring, mstring > GetMap() const;
};

class mConfigEngine
{
private:
    ceNode RootNode;
    mstring i_FileName;
      vector < mstring > PreParse(const vector < mstring > & in);
public:
      mConfigEngine()
    {
    }
    mConfigEngine(const mstring & FileName)
    {
	i_FileName = FileName;
	LoadFile();
    }

    mstring FileName()
    {
	return i_FileName;
    }
    bool LoadFile();
    bool LoadFromString(const mstring & configstring);
    bool LoadFromArray(const vector < mstring > & configarray);
    bool SaveFile();
    void Empty();

    map < mstring, mstring > GetMap() const
    {
	return RootNode.GetMap();
    }
    mstring Read(const mstring & key, const mstring & Defailt = "") const;
    bool Read(const mstring & key, mstring & outvar, const mstring & Default = "") const;
    bool Read(const mstring & key, bool &outvar, const bool Default = false) const;
    bool Read(const mstring & key, int &outvar, const int Default = 0) const;
    bool Read(const mstring & key, unsigned int &outvar, const unsigned int Default = 0) const;
    bool Read(const mstring & key, long &outvar, const long Default = 0) const;
    bool Read(const mstring & key, unsigned long &outvar, const unsigned long Default = 0) const;
    bool Read(const mstring & key, float &outvar, const float Default = 0.0) const;
    bool Read(const mstring & key, double &outvar, const double Default = 0.0) const;

    mstring Write(const mstring & key, const mstring & value);
    bool Write(const mstring & key, const bool value);
    int Write(const mstring & key, const int value);
    unsigned int Write(const mstring & key, const unsigned int value);
    long Write(const mstring & key, const long value);
    unsigned long Write(const mstring & key, const unsigned long value);
    double Write(const mstring & key, const double value);

    ceNode *GetNode(const mstring & NodeName);
    bool DeleteNode(const mstring & NodeName);
    bool DeleteKey(const mstring & KeyName);
    bool NodeExists(const mstring & NodeName) const;
};

#endif
