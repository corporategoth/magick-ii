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

#ifndef _MAGICK_H
#define _MAGICK_H

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include <vector>
#include <map>
using namespace std;
#include "ace/Synch.h"
#include "ace/Thread.h"

#include "bob.hpp"
#include "mstring.h"
#include "fileconf.h"
#include "datetime.h"	// Added by ClassView
#include "trace.h"

const int MAGICK_RET_NORMAL = 0;
const int MAGICK_RET_RESTART = 1;
const int MAGICK_RET_TERMINATE = 2;
const int MAGICK_RET_ERROR = -1;
const int MAGICK_RET_INVALID_SERVICES_DIR = -20;

typedef map<mstring,mstring> mapstringstring;
class Magick
{
	vector<mstring> argv;
	wxFileConfig* MagickIni;
	mapstringstring Messages;
	vector<mstring> MessageNamesLong;
	vector<mstring> MessageNamesShort;
	ACE_Thread_Mutex mutex;
public:
	map<ACE_thread_t,threadtype_enum> ThreadtoTypeMap;
	mDateTime StartTime;
	void LoadExternalMessages();
	mstring parseEscapes(const mstring& in);
	void LoadInternalMessages();
	void dump_help(mstring& progname);
	mstring getMessage(const mstring& name);
	mstring config_file;
	mstring services_dir;
	mstring ProgramName;
	Bob bob;
	Magick(int inargc, char **inargv);
	int Start();
};

extern Magick *MagickObject;

#endif