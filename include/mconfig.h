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
#ifndef _MCONFIG_H
#define _MCONFIG_H
#include "pch.h"
RCSID(mconfig_h, "@(#) $Id$");

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
