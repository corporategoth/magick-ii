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

class Magick
{
	vector<mstring> argv;
	Bob bob;
public:
	Magick(int inargc, char **inargv);
	int Start();
};
#endif