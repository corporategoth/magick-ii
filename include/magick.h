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

const int MAGICK_RESTART = 1;
const int MAGICK_TERMINATE = 2;
class Magick
{
	vector<mstring> argv;
public:
	Bob bob;
	Magick(int inargc, char **inargv);
	int Start();
};
#endif