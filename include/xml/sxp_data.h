// $Id$
// this file is included by sxp.h
// here "cleanly" separated is the functionality for converting
// between char * data and simple data types
// adding methods to these two interfaces automagically adds them respectively
// to the IElement and IOutStream interfaces passed to your user classes

// IDataOutput: bundles together methods for printing out simple data
// to a tag, e.g. <height>188</height> to the
// stdio.h file pointer returned by the FP() method, aided by an Indent()
// method which knows how many tabs to print before your tag.

#include "datetime.h"

template < class T > class IDataOutput : public IFilePrint < T >
{
public:
    // add more simple type writers here

    inline void WriteElement(Tag & t, const bool val, dict & attribs = blank_dict)
    {
	Indent();
	mstring param;

	  dict::iterator iter;
	for (iter = attribs.begin(); iter != attribs.end(); iter++)
	      param << " " << iter->first.c_str() << "=\"" << iter->second.c_str() << "\"";
	  Print("<%s%s>%d</%s>\n", t.ch, param.c_str(), val, t.ch);
    }

    inline void WriteElement(Tag & t, const int val, dict & attribs = blank_dict)
    {
	Indent();
	mstring param;

	dict::iterator iter;
	for (iter = attribs.begin(); iter != attribs.end(); iter++)
	    param << " " << iter->first.c_str() << "=\"" << iter->second.c_str() << "\"";
	Print("<%s%s>%d</%s>\n", t.ch, param.c_str(), val, t.ch);
    }

    inline void WriteElement(Tag & t, const long val, dict & attribs = blank_dict)
    {
	Indent();
	mstring param;

	dict::iterator iter;
	for (iter = attribs.begin(); iter != attribs.end(); iter++)
	    param << " " << iter->first.c_str() << "=\"" << iter->second.c_str() << "\"";
	Print("<%s%s>%d</%s>\n", t.ch, param.c_str(), val, t.ch);
    }

    inline void WriteElement(Tag & t, const double val, dict & attribs = blank_dict)
    {
	Indent();
	mstring param;

	dict::iterator iter;
	for (iter = attribs.begin(); iter != attribs.end(); iter++)
	    param << " " << iter->first.c_str() << "=\"" << iter->second.c_str() << "\"";
	Print("<%s%s>%7.5f</%s>\n", t.ch, param.c_str(), val, t.ch);
    }

    inline void WriteElement(Tag & t, const unsigned int val, dict & attribs = blank_dict)
    {
	Indent();
	mstring param;

	dict::iterator iter;
	for (iter = attribs.begin(); iter != attribs.end(); iter++)
	    param << " " << iter->first.c_str() << "=\"" << iter->second.c_str() << "\"";
	Print("<%s%s>%u</%s>\n", t.ch, param.c_str(), val, t.ch);
    }

    inline void WriteElement(Tag & t, const unsigned long val, dict & attribs = blank_dict)
    {
	Indent();
	mstring param;

	dict::iterator iter;
	for (iter = attribs.begin(); iter != attribs.end(); iter++)
	    param << " " << iter->first.c_str() << "=\"" << iter->second.c_str() << "\"";
	Print("<%s%s>%u</%s>\n", t.ch, param.c_str(), val, t.ch);
    }

    inline void WriteElement(Tag & t, const mstring & val, dict & attribs = blank_dict)
    {
	Indent();
	mstring param;

	dict::iterator iter;
	for (iter = attribs.begin(); iter != attribs.end(); iter++)
	    param << " " << iter->first.c_str() << "=\"" << iter->second.c_str() << "\"";
	if (!val.empty())
	    Print("<%s%s>%s</%s>\n", t.ch, param.c_str(), XMLEscape(val.c_str()).c_str(), t.ch);
	else
	    Print("<%s%s/>\n", t.ch, param.c_str());
    }

    inline void WriteElement(Tag & t, const mDateTime & val, dict & attribs = blank_dict)
    {
	WriteElement(t, static_cast < double > (GMT(val, true)), attribs);
    }
};

// IDataInput: bundles together methods for converting from the char*
// returned by the Data() method (inherited from IData).

class IDataInput : public IData
{
public:
    inline void Retrieve(bool &val) const
    {
	val = (atoi(Data()) != 0);
    }

    inline void Retrieve(int &val) const
    {
	val = atoi(Data());
    }

    inline void Retrieve(long &val) const
    {
	val = atol(Data());
    }

    inline void Retrieve(double &val) const
    {
	val = atof(Data());
    }

    inline void Retrieve(unsigned int &val) const
    {
	val = atoi(Data()) - static_cast < unsigned int > (0);
    }

    inline void Retrieve(unsigned long &val) const
    {
	val = atol(Data()) - static_cast < unsigned long > (0);
    }

    inline void Retrieve(mstring & val) const
    {
	val = XMLUnEscape(Data());
    }

    void Retrieve(mDateTime & val) const
    {
	double temp;
	  Retrieve(temp);
	// If the dates are before 1900, we're probably
	// converting an old database.  Just add 1900
	// years, and be done with it.   Dont forget
	// the leap years :)
	if (temp < static_cast < double > (1900 * 365))
	      temp += static_cast < double > ((1900 * 365) + (1900 / 4) - (1900 / 100) + (1900 / 400));
	  val = GMT(mDateTime(temp), false);
    }

};
