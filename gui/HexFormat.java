//
//

import javax.swing.*;
import java.text.*;

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
	    return new Integer(Integer.parseInt(text, 16));
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

	String rv = new String("0x");
	if (in.intValue() != 0)
	    rv += Integer.toString(in.intValue(), 16);
	return rv;
    }
}
