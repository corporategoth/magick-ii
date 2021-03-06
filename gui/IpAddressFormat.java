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
import java.net.*;

public class IpAddressFormat extends JFormattedTextField.AbstractFormatter
{
    public Object stringToValue(String text) throws ParseException
    {
	String newtext;
	if (text == null || text.length() == 0)
	    return null;
	else
	    newtext = text;

	try
	{
	    InetAddress addr = InetAddress.getByName(newtext);
	    JFormattedTextField f = new JFormattedTextField();
	    getFormattedTextField().setBorder(f.getBorder());
	    return addr;
	}
	catch (UnknownHostException e)
	{
	    getFormattedTextField().setBorder(BorderFactory.createLineBorder(Color.RED));
	    throw new ParseException(newtext, 0);
	}
    }

    public String valueToString(Object value) throws ParseException
    {
	if (value == null)
	    return "";

	InetAddress addr = ((InetAddress) value);
	return addr.getHostAddress();
    }
}
