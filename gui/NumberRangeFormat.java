//
//

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
