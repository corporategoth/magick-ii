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
import javax.swing.event.*;
import javax.swing.text.*;
import java.awt.*;
import java.awt.event.*;

// import javax.swing.table.*;
// import java.util.*;

public class MemoServ extends TabbedPane
{
// private:
    private JFormattedTextField news_expire, inflight, delay, files, filesize;

    public String name() { return "MemoServ"; }

    public MemoServ(ToolTips t)
    {
	super(t);

	news_expire = createFormattedTextField("NEWS_EXPIRE", 4, new TimeFormat(), "3w", true);
	inflight = createFormattedTextField("INFLIGHT", 4, new TimeFormat(), "2m", true);
	delay = createFormattedTextField("DELAY", 4, new TimeFormat(), "10s", true);
	files = createFormattedTextField("FILES", 4, new NumberRangeFormat(0, -1), "0", true);
	filesize = createFormattedTextField("FILESIZE", 4, new SpaceFormat(), "0b", isNonZero(files));
    }

    public void documentChanged(DocumentEvent e)
    {
	Document props = e.getDocument();
	if (props.getProperty("name").equals("files"))
	{
	    if (files.isEditValid())
	    {
		try {
		    files.commitEdit();
		    filesize.setEditable(isNonZero(files));
		    filesize.setEnabled(isNonZero(files));
		}
		catch (Exception ex)
		{
		}
	    }
	}
    }

    public void actionPerformed(ActionEvent e)
    {
    }

    public JComponent createPane()
    {
	JPanel gb = new JPanel();
	GridBagConstraints gc = new GridBagConstraints();
	gb.setLayout(new GridBagLayout());
	gc.gridx=0;
	gc.gridy=0;
	gc.anchor = gc.EAST;

	addToGridBagLine(gb, gc, "", new JLabel(""));
	addToGridBag(gb, gc, "In Flight time", inflight);
	addToGridBag(gb, gc, "Send delay", delay);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Stored Files", files);
	addToGridBag(gb, gc, "File Size", filesize);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Expiration time", news_expire);
	addToGridBagLine(gb, gc, "", new JLabel(""));

	JPanel p = new JPanel();
	p.setLayout(new BorderLayout());
	p.add(gb, BorderLayout.NORTH);
	JScrollPane rv = new JScrollPane(p);
	return rv;
    }	

    public String createCfg()
    {
	String rv = new String();

	rv += "[MemoServ]\n";
	rv += "NEWS_EXPIRE = " + news_expire.getText() + "\n";
	rv += "INFLIGHT = " + inflight.getText() + "\n";
	rv += "DELAY = " + delay.getText() + "\n";
	rv += "FILES = " + files.getText() + "\n";
	rv += "FILESIZE = " + filesize.getText() + "\n";

	return rv;
    }

    public void parseCfg(IniParser data)
    {
	news_expire.setText(data.getValue("MemoServ/NEWS_EXPIRE"));
	inflight.setText(data.getValue("MemoServ/INFLIGHT"));
	delay.setText(data.getValue("MemoServ/DELAY"));
	files.setText(data.getValue("MemoServ/FILES"));
	filesize.setText(data.getValue("MemoServ/FILESIZE"));
    }
}
