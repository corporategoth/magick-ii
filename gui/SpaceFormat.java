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

public class SpaceFormat extends JFormattedTextField.AbstractFormatter
{
    public Object stringToValue(String text) throws ParseException
    {
	int total = 0, number = 0;

	if (text == null)
	    return new Integer(0);

	char[] str = text.toCharArray();

	for (int i=0; i<str.length; i++)
	{
	    switch (str[i])
	    {
/*	    case 'T':
	    case 't':
		if (number != 0)
		{
		    total += number * 1024 * 1024 * 1024 * 1024;
		    number = 0;
		}
		else if (i <= 0 || str[i-1] != '0')
		{
		    getFormattedTextField().setBorder(BorderFactory.createLineBorder(Color.RED));
		    throw new ParseException(text, i);
		}
		break;
*/	    case 'G':
	    case 'g':
		if (number != 0)
		{
		    total += number * 1024 * 1024 * 1024;
		    number = 0;
		}
		else if (i <= 0 || str[i-1] != '0')
		{
		    getFormattedTextField().setBorder(BorderFactory.createLineBorder(Color.RED));
		    throw new ParseException(text, i);
		}
		break;
	    case 'M':
	    case 'm':
		if (number != 0)
		{
		    total += number * 1024 * 1024;
		    number = 0;
		}
		else if (i <= 0 || str[i-1] != '0')
		{
		    getFormattedTextField().setBorder(BorderFactory.createLineBorder(Color.RED));
		    throw new ParseException(text, i);
		}
		break;
	    case 'K':
	    case 'k':
		if (number != 0)
		{
		    total += number * 1024;
		    number = 0;
		}
		else if (i <= 0 || str[i-1] != '0')
		{
		    getFormattedTextField().setBorder(BorderFactory.createLineBorder(Color.RED));
		    throw new ParseException(text, i);
		}
		break;
	    case 'B':
	    case 'b':
		if (number != 0)
		{
		    total += number;
		    number = 0;
		}
		else if (i <= 0 || str[i-1] != '0')
		{
		    getFormattedTextField().setBorder(BorderFactory.createLineBorder(Color.RED));
		    throw new ParseException(text, i);
		}
		break;
	    case '0':
	    case '1':
	    case '2':
	    case '3':
	    case '4':
	    case '5':
	    case '6':
	    case '7':
	    case '8':
	    case '9':
		number *= 10;
		number += str[i] - '0';
		break;
	    default:
		getFormattedTextField().setBorder(BorderFactory.createLineBorder(Color.RED));
		throw new ParseException(text, i);
	    }
	}
	if (number != 0)
	    total += number;

	JFormattedTextField f = new JFormattedTextField();
	getFormattedTextField().setBorder(f.getBorder());

	return new Integer(total);
    }

    public String valueToString(Object value) throws ParseException
    {
	String rv = new String();
	Integer in;
	if (value == null)
	    in = new Integer(0);
	else
	    in = (Integer) value;
	int total = in.intValue();
	if (total < 0)
	    throw new ParseException(null, 0);

	int tbytes = 0, gbytes = 0, mbytes = 0, kbytes = 0, bytes = 0;

/*	while (total >= 1024 * 1024 * 1024 * 1024)
	{
	    tbytes++;
	    total -= 1024 * 1024 * 1024 * 1024;
	} */

	while (total >= 1024 * 1024 * 1024)
	{
	    gbytes++;
	    total -= 1024 * 1024 * 1024;
	}

	while (total >= 1024 * 1024)
	{
	    mbytes++;
	    total -= 1024 * 1024;
	}

	while (total >= 1024)
	{
	    kbytes++;
	    total -= 1024;
	}

	bytes = total;

	if (tbytes > 0)
	    rv += tbytes + "t";
	if (gbytes > 0)
	    rv += gbytes + "g";
	if (mbytes > 0)
	    rv += mbytes + "m";
	if (kbytes > 0)
	    rv += kbytes + "k";
	if (bytes > 0 || in.intValue() == 0)
	    rv += bytes + "b";
	return rv;
    }
}
