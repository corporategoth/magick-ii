// Magick IRC Services
//
// (c) 1997-2002 Preston Elder <prez@magick.tm>
// (c) 1998-2002 William King <ungod@magick.tm>
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
import javax.swing.event.*;
import javax.swing.text.*;
import java.awt.*;
import java.awt.event.*;

import java.util.zip.DataFormatException;

public class ServMsg extends TabbedPane
{
// private:
    public String name() { return "ServMsg"; }

    public ServMsg(ToolTips t)
    {
	super(t);

    }

    public void documentChanged(DocumentEvent e)
    {
	Document props = e.getDocument();
    }

    public void actionPerformed(ActionEvent e)
    {
    }

    public JComponent createPane()
    {
	JPanel gb = new JPanel();
	gb.setLayout(new GridBagLayout());
	GridBagConstraints gc = createStandardConstraints();

	addToGridBagLine(gb, gc, "", new JLabel(" "));
	addToGridBagLine(gb, gc, "nothing to configure", new JLabel(" "));
	addToGridBagLine(gb, gc, "", new JLabel(" "));

	JPanel p = new JPanel();
	p.setLayout(new BorderLayout());
	// p.add(gb, BorderLayout.NORTH);
	p.add(gb, BorderLayout.CENTER);
	JScrollPane rv = new JScrollPane(p);
	return rv;
    }	

    public String createCfg() throws DataFormatException
    {
	String rv = new String();

	rv += "[ServMsg]\n";

	return rv;
    }

    public void parseCfg(IniParser data)
    {
    }
}
