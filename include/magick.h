// RCS-ID:      $Id$
#ifndef _MAGICK_H
#define _MAGICK_H

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include <vector>
#include <string>
using namespace std;
extern "C" {
#include "bob.h"
};

class Magick
{
	vector<string> argv;
	ObjectPtr bobval;
	UnwindTarget bobut;
	InterpreterContextPtr bobic;
	CompilerContext *bobcc;
	bool bobavail;
public:
	Magick(int inargc, const char **inargv);
	void Start();
	bool StartBob(const string& scriptname="");
};
#endif