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
** Revision 1.2  2000/05/20 00:08:02  ungod
** getting ConfigEngine compiling and cleaning up SXP to stop circular includes of "datetime.h"
**
** Revision 1.1  2000/05/19 13:11:34  ungod
** initial checkin of the new config engine, class structure is in, but no code in it.
**
** Revision 1.13  2000/04/30 03:48:29  prez
** Replaced all system calls with ACE_OS equivilants,
** also removed any dependancy on ACE from sxp (xml)
**
** Revision 1.12  2000/03/28 16:20:58  prez
** LOTS of RET() fixes, they should now be safe and not do double
** calculations.  Also a few bug fixes from testing.
**
** Revision 1.11  2000/03/19 08:50:54  prez
** More Borlandization -- Added WHAT project, and fixed a bunch
** of minor warnings that appear in borland.
**
** Revision 1.10  2000/03/08 23:38:36  prez
** Added LIVE to nickserv/chanserv, added help funcitonality to all other
** services, and a bunch of other small changes (token name changes, etc)
**
** Revision 1.9  2000/02/27 03:58:39  prez
** Fixed the WHAT program, also removed RegEx from Magick.
**
** Revision 1.8  2000/02/23 12:21:03  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.7  2000/02/15 13:27:03  prez
** *** empty log message ***
**
** Revision 1.6  2000/02/15 10:37:49  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#include "mconfig.h"
#include "filesys.h"

ceNode::ceNode()
{
}

ceNode::ceNode(ceNode &in)
{
}

ceNode::~ceNode()
{
}

ceNode& ceNode::operator=(const ceNode &in)
{
}

bool ceNode::operator==(const ceNode &in)const
{
}

bool ceNode::operator<(const ceNode &in)const
{
}

void ceNode::SetKey(const mstring &KeyName, const mstring &Value)
{
}

void ceNode::DeleteKey(const mstring &KeyName)
{
}

void ceNode::CreateNode(const mstring &NodeName)
{
}

void ceNode::DeleteNode(const mstring &NodeName)
{
}

bool ceNode::NodeExists(const mstring &NodeName)
{
}

mConfigEngine::mConfigEngine()
{
}

mConfigEngine::mConfigEngine(const mstring& FileName)
{
}

bool mConfigEngine::LoadFile()
{
}

bool mConfigEngine::SaveFile()
{
}

void mConfigEngine::Empty()
{
}


mstring &mConfigEngine::Read(const mstring &key, const mstring Defailt)
{
}

bool mConfigEngine::Read(const mstring &key, mstring &outvar, mstring Default)
{
}

bool mConfigEngine::Read(const mstring &key, bool &outvar, bool Default)
{
}

bool mConfigEngine::Read(const mstring &key, int &outvar, int Default)
{
}

bool mConfigEngine::Read(const mstring &key, unsigned int &outvar, unsigned int Default)
{
}

bool mConfigEngine::Read(const mstring &key, long &outvar, int Default)
{
}

bool mConfigEngine::Read(const mstring &key, unsigned long &outvar, unsigned int Default)
{
}

bool mConfigEngine::Read(const mstring &key, double &outvar, double Default)
{
}


void mConfigEngine::Write(const mstring &key,const mstring &value)
{
}

void mConfigEngine::Write(const mstring &key,bool value)
{
}

void mConfigEngine::Write(const mstring &key,int value)
{
}

void mConfigEngine::Write(const mstring &key,unsigned int value)
{
}

void mConfigEngine::Write(const mstring &key,long value)
{
}

void mConfigEngine::Write(const mstring &key,unsigned long value)
{
}

void mConfigEngine::Write(const mstring &key,double value)
{
}


ceNode &mConfigEngine::GetNode(const mstring& NodeName)
{
}

bool mConfigEngine::DeleteNode(const mstring& NodeName)
{
}

bool mConfigEngine::DeleteKey(const mstring& KeyName)
{
}

bool mConfigEngine::LoadFromString(const mstring& configstring)
{
}

bool mConfigEngine::LoadFromArray(vector<mstring> configarray)
{
}

bool mConfigEngine::NodeExists(const mstring &NodeName)
{
}

