// RCS-ID:      $Id$
#ifndef _MAGICK_H
#define _MAGICK_H

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include <vector>
using namespace std;

#include "bob.hpp"
#include "mstring.h"

const int MAGICK_RET_NORMAL = 0;
const int MAGICK_RET_RESTART = 1;
const int MAGICK_RET_TERMINATE = 2;
const int MAGICK_RET_ERROR = -1;
class Magick
{
	vector<mstring> argv;
public:
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