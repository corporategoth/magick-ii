// RCS-ID:      $Id$
#ifndef _MAGICK_H
#define _MAGICK_H

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include <vector>
#include <map>
using namespace std;
#include "ace/Synch.h"

#include "bob.hpp"
#include "mstring.h"
#include "fileconf.h"

const int MAGICK_RET_NORMAL = 0;
const int MAGICK_RET_RESTART = 1;
const int MAGICK_RET_TERMINATE = 2;
const int MAGICK_RET_ERROR = -1;

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
#endif