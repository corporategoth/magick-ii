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
import java.awt.Color;

public class HexFormat extends JFormattedTextField.AbstractFormatter
{
    public Object stringToValue(String text) throws ParseException
    {
	int number = 0;

	if (text == null)
	    return new Integer(0);

	if (text.startsWith("0x"))
	    text.replaceFirst("0x", "");
	try
	{
	    Integer i = new Integer(Integer.parseInt(text, 16));
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

	String rv = new String("0x");
	if (in.intValue() != 0)
	    rv += Integer.toString(in.intValue(), 16);
	return rv;
    }
}
