#ifndef _MAGICK_H
#define _MAGICK_H

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include <vector>
#include <string>
using namespace std;

class Magick
{
	vector<string> argv;
public:
	Magick(int inargc, char **inargv);
	void Start();
};
#endif