//
//

import javax.swing.*;
import java.text.*;

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
	    throw new ParseException(text, 0);
	}
	if (f.floatValue() < 0.0 || f.floatValue() > 100.0)
	    throw new ParseException(text, 0);

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
