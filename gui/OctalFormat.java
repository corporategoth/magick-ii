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

public class OctalFormat extends JFormattedTextField.AbstractFormatter
{
    public Object stringToValue(String text) throws ParseException
    {
	int number = 0;

	if (text == null)
	    return new Integer(0);

	try
	{
	    Integer i = new Integer(Integer.parseInt(text, 8));
	    JFormattedTextField f = new JFormattedTextField();
	    getFormattedTextField().setBorder(f.getBorder());
	    return i;
	}
	catch (NumberFormatException e)
	{
	    getFormattedTextField().setBorder(BorderFactory.createLineBorder(Color.RED));
	    throw new ParseException(text, 0);
	}
    }

    public String valueToString(Object value) throws ParseException
    {
	Integer in;
	if (value == null)
	    in = new Integer(0);
	else
	    in = (Integer) value;

	String rv = new String("0");
	if (in.intValue() != 0)
	    rv += Integer.toString(in.intValue(), 8);
	return rv;
    }
}
