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

import java.util.*;

public class IniParser
{
    private HashMap rep = new HashMap();

    public IniParser(String rawtext)
    {
	StringTokenizer st = new StringTokenizer(rawtext, "\n");

	String section = "";
	while (st.hasMoreTokens())
	{
	    String fullLine = st.nextToken();
	    if (fullLine.length() == 0 || fullLine.startsWith("#") || fullLine.startsWith(";"))
		continue;

	    // Ditch any trailing comments
	    String[] strippedLine = fullLine.split("[^\\\\];", 2);

	    // Blank line ...
	    if (strippedLine[0].matches("^\\s*$"))
		continue;

	    // Section Name ...
	    else if (strippedLine[0].matches("^\\s*\\[\\s*(\\s*[^\\s\\[\\]]+)+\\s*\\]\\s*$"))
	    {
		String[] splitLine = strippedLine[0].split("[\\[\\]]");
		for (int i=0; i<splitLine.length; i++)
		{
		    if (splitLine[i].trim().length() != 0)
		    {
			section = splitLine[i].trim();
			break;
		    }
		}
	    }

	    // Regular line ...
	    else if (strippedLine[0].matches("^\\s*(\\s*[^\\s\\[\\]=]+)+\\s*=.*$"))
	    {
		String[] splitLine = strippedLine[0].split("=", 2);
		String key = new String(section);
		if (key.length() != 0)
		    key += "/";
		key += splitLine[0].trim();
		key = key.toUpperCase();

		boolean trailingSpace = splitLine[1].endsWith("\\\\ ");
		String value = splitLine[1].trim();
		if (trailingSpace)
		    value.replaceFirst("\\\\$", " ");

		rep.put(key, value);
	    }

	    // error ...
	    else
	    {
	    }
	}
    }

    public String getValue(String inKey)
    {
	String key = inKey.toUpperCase();
	return (String) rep.get(key);
    }

    public boolean keyExists(String inKey)
    {
	String key = inKey.toUpperCase();
	return rep.containsKey(key);
    }

    public static boolean isBoolValue(String value)
    {
	if (value.equals("1") || value.equalsIgnoreCase("true") || value.equalsIgnoreCase("on") ||
	    value.equals("0") || value.equalsIgnoreCase("false") || value.equalsIgnoreCase("off"))
	    return true;
	return false;
    }

    public static boolean getBoolValue(String value)
    {
	if (value.equals("1") || value.equalsIgnoreCase("true") || value.equalsIgnoreCase("on"))
	    return true;
	return false;
    }
}
