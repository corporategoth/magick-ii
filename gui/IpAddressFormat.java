//
//

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
