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
import javax.swing.event.*;
import javax.swing.text.*;
import java.awt.*;
import java.awt.event.*;

import java.util.zip.DataFormatException;
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
	try
	{
	    Document props = e.getDocument();
	    if (props.getProperty("name").equals("files"))
	    {
		if (files.isEditValid())
		{
		    files.commitEdit();
		    filesize.setEditable(isNonZero(files));
		    filesize.setEnabled(isNonZero(files));
		}
	    }
	}
	catch (Exception ex)
	{
	}
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
	addToGridBag(gb, gc, "In Flight Time", inflight);
	addToGridBag(gb, gc, "Send Delay", delay);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Stored Files", files);
	addToGridBag(gb, gc, "File Size", filesize);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Expiration Time", news_expire);
	addToGridBagLine(gb, gc, "", new JLabel(" "));

	JPanel p = new JPanel();
	p.setLayout(new BorderLayout());
	p.add(gb, BorderLayout.NORTH);
	JScrollPane rv = new JScrollPane(p);
	return rv;
    }	

    public String createCfg() throws DataFormatException
    {
	String rv = new String();

	rv += "[MemoServ]\n";
	if (!isEditValid(news_expire)) throw new DataFormatException("MemoServ/NEWS_EXPIRE");
	rv += "NEWS_EXPIRE = " + news_expire.getText() + "\n";
	if (!isEditValid(inflight)) throw new DataFormatException("MemoServ/INFLIGHT");
	rv += "INFLIGHT = " + inflight.getText() + "\n";
	if (!isEditValid(delay)) throw new DataFormatException("MemoServ/DELAY");
	rv += "DELAY = " + delay.getText() + "\n";
	if (!isEditValid(files)) throw new DataFormatException("MemoServ/FILES");
	rv += "FILES = " + files.getText() + "\n";
	if (!isEditValid(filesize)) throw new DataFormatException("MemoServ/FILESIZE");
	rv += "FILESIZE = " + filesize.getText() + "\n";

	return rv;
    }

    public void parseCfg(IniParser data)
    {
	setFmtField(news_expire, data, "MemoServ/NEWS_EXPIRE");
	setFmtField(inflight, data, "MemoServ/INFLIGHT");
	setFmtField(delay, data, "MemoServ/DELAY");
	setFmtField(files, data, "MemoServ/FILES");
	setFmtField(filesize, data, "MemoServ/FILESIZE");
    }
}
