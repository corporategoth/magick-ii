// Magick IRC Services
//
// (c) 1997-2003 Preston Elder <prez@magick.tm>
// (c) 1998-2003 William King <ungod@magick.tm>
//
// The above copywright may not be removed under any circumstances,
// however it may be added to if any modifications are made to this
// file.  All modified code must be clearly documented and labelled.
//
// This code is released under the Artistic License v2.0 or better.
// The full text of this license should be contained in a file called
// COPYING distributed with this code.  If this file does not exist,
// it may be viewed here: http://www.magick.tm/m2/license.html
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
import java.awt.Color;

public class PercentageFormat extends JFormattedTextField.AbstractFormatter
{
    public Object stringToValue(String text) throws ParseException
    {
	if (text == null)
	    return new Float(0.0);

	Float f;
	try
	{
	    f = new Float(text);
	}
	catch (NumberFormatException e)
	{
	    getFormattedTextField().setBorder(BorderFactory.createLineBorder(Color.RED));
	    throw new ParseException(text, 0);
	}
	if (f.floatValue() < 0.0 || f.floatValue() > 100.0)
	{
	    getFormattedTextField().setBorder(BorderFactory.createLineBorder(Color.RED));
	    throw new ParseException(text, 0);
	}

	JFormattedTextField ftf = new JFormattedTextField();
	getFormattedTextField().setBorder(ftf.getBorder());
	return f;
    }

    public String valueToString(Object value) throws ParseException
    {
	if (value == null)
	    return new String("0.00");
	Float in = (Float) value;

	if (in.floatValue() < 0.0 || in.floatValue() > 100.0)
	    throw new ParseException(null, 0);

	int denominator = in.intValue();
	int numerator = ((int) (in.floatValue() * 100)) - (denominator * 100);

	String rv = new String();
	rv += denominator;
	rv += ".";
	if (numerator < 10)
	    rv += "0";
	rv += numerator;
	return rv;
    }
}
