// Magick IRC Services
//
// (c) 1997-2002 Preston Elder <prez@magick.tm>
// (c) 1998-2002 William King <ungod@magick.tm>
//
// The above copywright may not be removed under any circumstances,
// however it may be added to if any modifications are made to this
// file.  All modified code must be clearly documented and labelled.
//
// This code is released under the GNU General Public License, which
// means (in short), it may be distributed freely, and may not be sold
// or used as part of any closed-source product.  Please check the
// COPYING file for full rights and restrictions of this software.
//
// =======================================================================
// rcsid = @(#)$Id$
// =======================================================================
//
// For official changes (by the Magick Development Team),please
// check the ChangeLog* files that come with this distribution.
//
// Third Party Changes (please include e-mail address):
//
// N/A
//
// =======================================================================

import javax.swing.*;
import java.text.*;

public class NumberRangeFormat extends JFormattedTextField.AbstractFormatter
{
    private int low, high;

    public NumberRangeFormat(int l, int h)
    {
	super();
	low = l;
	high = h;
    }

    public int getLow()
    {
	return low;
    }

    public int getHigh()
    {
	return high;
    }

    public void setLow(int in)
    {
	low = in;
    }

    public void setHigh(int in)
    {
	high = in;
    }

    public Object stringToValue(String text) throws ParseException
    {
	if (text == null)
	    return new Integer(low);

	Integer i;
	try
	{
	    i = new Integer(text);
	}
	catch (NumberFormatException e)
	{
	    throw new ParseException(text, 0);
	}

	if (i.intValue() < low)
	    return new Integer(low);
	if (i.intValue() > high && high >= low)
	    return new Integer(high);

	return i;
    }

    public String valueToString(Object value) throws ParseException
    {
	Integer in;
	if (value == null)
	    in = new Integer(low);
	else
	    in = (Integer) value;

	if (in.intValue() < low)
	    return String.valueOf(low);
	if (in.intValue() > high && high >= low)
	    return String.valueOf(high);

	return new String(in.toString());
    }
}
