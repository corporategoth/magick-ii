//
//

import javax.swing.*;
import java.text.*;

public class TimeFormat extends JFormattedTextField.AbstractFormatter
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
	    case 'Y':
	    case 'y':
		if (number != 0)
		{
		    total += (int) (number * 60 * 60 * 24 * 365.25);
		    number = 0;
		}
		else if (i <= 0 || str[i-1] != '0')
		    throw new ParseException(text, i);
		break;
	    case 'W':
	    case 'w':
		if (number != 0)
		{
		    total += number * 60 * 60 * 24 * 7;
		    number = 0;
		}
		else if (i <= 0 || str[i-1] != '0')
		    throw new ParseException(text, i);
		break;
	    case 'D':
	    case 'd':
		if (number != 0)
		{
		    total += number * 60 * 60 * 24;
		    number = 0;
		}
		else if (i <= 0 || str[i-1] != '0')
		    throw new ParseException(text, i);
		break;
	    case 'H':
	    case 'h':
		if (number != 0)
		{
		    total += number * 60 * 60;
		    number = 0;
		}
		else if (i <= 0 || str[i-1] != '0')
		    throw new ParseException(text, i);
		break;
	    case 'M':
	    case 'm':
		if (number != 0)
		{
		    total += number * 60;
		    number = 0;
		}
		else if (i <= 0 || str[i-1] != '0')
		    throw new ParseException(text, i);
		break;
	    case 'S':
	    case 's':
		if (number != 0)
		{
		    total += number;
		    number = 0;
		}
		else if (i <= 0 || str[i-1] != '0')
		    throw new ParseException(text, i);
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
		throw new ParseException(text, i);
	    }
	}
	if (number != 0)
	    total += number;
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
	int years = 0, weeks = 0, days = 0, hours = 0, mins = 0, secs = 0;

	while (total >= (int) (60 * 60 * 24 * 365.25))
	{
	    years++;
	    total -= (int) (60 * 60 * 24 * 365.25);
	}

	while (total >= 60 * 60 * 24 * 7)
	{
	    weeks++;
	    total -= 60 * 60 * 24 * 7;
	}

	while (total >= 60 * 60 * 24)
	{
	    days++;
	    total -= 60 * 60 * 24;
	}

	while (total >= 60 * 60)
	{
	    hours++;
	    total -= 60 * 60;
	}

	while (total >= 60)
	{
	    mins++;
	    total -= 60;
	}

	secs = total;

	if (years > 0)
	    rv += years + "y";
	if (weeks > 0)
	    rv += weeks + "w";
	if (days > 0)
	    rv += days + "d";
	if (hours > 0)
	    rv += hours + "h";
	if (mins > 0)
	    rv += mins + "m";
	if (secs > 0 || in.intValue() == 0)
	    rv += secs + "s";
	return rv;
    }
}
