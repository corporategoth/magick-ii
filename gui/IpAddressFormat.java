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
import java.net.*;

public class IpAddressFormat extends JFormattedTextField.AbstractFormatter
{
    public Object stringToValue(String text) throws ParseException
    {
	String newtext;
	if (text == null || text.length() == 0)
	    newtext = new String("0.0.0.0");
	else
	    newtext = text;

	try
	{
	    InetAddress addr = InetAddress.getByName(newtext);
	    return addr;
	}
	catch (UnknownHostException e)
	{
	    throw new ParseException(newtext, 0);
	}
    }

    public String valueToString(Object value) throws ParseException
    {
	if (value == null)
	    return "0.0.0.0";

	InetAddress addr = ((InetAddress) value);
	return addr.getHostAddress();
    }
}
