//
//

import javax.swing.*;
import java.text.*;

public class OctalFormat extends JFormattedTextField.AbstractFormatter
{
    public Object stringToValue(String text) throws ParseException
    {
	int number = 0;

	if (text == null)
	    return new Integer(0);

	try
	{
	    return new Integer(Integer.parseInt(text, 8));
	}
	catch (NumberFormatException e)
	{
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
