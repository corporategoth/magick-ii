// RCS-ID:      $Id$
#ifndef _MAGICK_H
#define _MAGICK_H

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include <vector>
#include <string>
using namespace std;

#include "bob.hpp"

class Magick
{
	vector<string> argv;
	Bob bob;
public:
	Magick(int inargc, char **inargv);
	int Start();
};
#endif