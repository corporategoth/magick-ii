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

public class Config extends TabbedPane
{
// private:
    private JFormattedTextField server_relink, squit_protect, squit_cancel,
		cycletime, savetime, checktime, ping_frequency,
		starthresh, listsize, maxlist, min_threads,
		max_threads, low_water_mark, high_water_mark,
		heartbeat_time, msg_seen_time, msg_check_time;

    public String name() { return "Config"; }

    public Config(ToolTips t)
    {
	super(t);

	server_relink = createFormattedTextField("SERVER_RELINK", 4, new TimeFormat(), "5m", true);
	squit_protect = createFormattedTextField("SQUIT_PROTECT", 4, new TimeFormat(), "2m", true);
	squit_cancel = createFormattedTextField("SQUIT_CANCEL", 4, new TimeFormat(), "10s", true);
	cycletime = createFormattedTextField("CYCLETIME", 4, new TimeFormat(), "15m", true);
	savetime = createFormattedTextField("SAVETIME", 4, new TimeFormat(), "5m", true);
	checktime = createFormattedTextField("CHECKTIME", 4, new TimeFormat(), "15s", true);
	ping_frequency = createFormattedTextField("PING_FREQUENCY", 4, new TimeFormat(), "30s", true);
	starthresh = createFormattedTextField("STARTHRESH", 4, new NumberRangeFormat(0, -1), "4", true);
	listsize = createFormattedTextField("LISTSIZE", 4, new NumberRangeFormat(0, -1), "50", true);
	maxlist = createFormattedTextField("MAXLIST", 4, new NumberRangeFormat(50, -1), "250", true);
	min_threads = createFormattedTextField("MIN_THREADS", 4, new NumberRangeFormat(1, -1), "3", true);
	max_threads = createFormattedTextField("MAX_THREADS", 4, new NumberRangeFormat(3, -1), "25", true);
	low_water_mark = createFormattedTextField("LOW_WATER_MARK", 4, new NumberRangeFormat(1, -1), "50", true);
	high_water_mark = createFormattedTextField("HIGH_WATER_MARK", 4, new NumberRangeFormat(50, -1), "75", true);
	heartbeat_time = createFormattedTextField("HEARTBEAT_TIME", 4, new TimeFormat(), "5m", true);
	msg_seen_time = createFormattedTextField("MSG_SEEN_TIME", 4, new TimeFormat(), "30s", true);
	msg_check_time = createFormattedTextField("MSG_CHECK_TIME", 4, new TimeFormat(), "1m", true);
    }

    public void documentChanged(DocumentEvent e)
    {
	Document props = e.getDocument();
        if (props.getProperty("name").equals("listsize") && listsize.getText().length() != 0)
	{
	    if (Integer.parseInt(maxlist.getText()) < Integer.parseInt(listsize.getText()))
		maxlist.setText(listsize.getText());
	    ((NumberRangeFormat) maxlist.getFormatter()).setLow(Integer.parseInt(listsize.getText()));
	}
        else if (props.getProperty("name").equals("min_threads") && min_threads.getText().length() != 0)
	{
	    if (Integer.parseInt(max_threads.getText()) < Integer.parseInt(min_threads.getText()))
		max_threads.setText(min_threads.getText());
	    ((NumberRangeFormat) max_threads.getFormatter()).setLow(Integer.parseInt(min_threads.getText()));
	}
        else if (props.getProperty("name").equals("low_water_mark") && low_water_mark.getText().length() != 0)
	{
	    if (Integer.parseInt(high_water_mark.getText()) < Integer.parseInt(low_water_mark.getText()))
		high_water_mark.setText(low_water_mark.getText());
	    ((NumberRangeFormat) high_water_mark.getFormatter()).setLow(Integer.parseInt(low_water_mark.getText()));
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
	addToGridBag(gb, gc, "Relink time", server_relink);
	addToGridBag(gb, gc, "SQUIT Protect time", squit_protect);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Active Cycle time", cycletime);
	addToGridBag(gb, gc, "SQUIT Cancel time", squit_cancel);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Hyperactive Cycle time", checktime);
	addToGridBag(gb, gc, "Default List size", listsize);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "DB Save time", savetime);
	addToGridBag(gb, gc, "Maximum List size", maxlist);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Heartbeat time", heartbeat_time);
	addToGridBag(gb, gc, "Server Ping time", ping_frequency);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Minimum Threads", min_threads);
	addToGridBag(gb, gc, "Low Water Mark", low_water_mark);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Maximum Threads", max_threads);
	addToGridBag(gb, gc, "High Water Mark", high_water_mark);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "MSG Expire time", msg_seen_time);
	addToGridBag(gb, gc, "Wildcard Threshold", starthresh);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "MSG Expire Check time", msg_check_time);
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

	rv += "[Config]\n";
	rv += "SERVER_RELINK = " + server_relink.getText() + "\n";
	rv += "SQUIT_PROTECT = " + squit_protect.getText() + "\n";
	rv += "SQUIT_CANCEL = " + squit_cancel.getText() + "\n";
	rv += "CYCLETIME = " + cycletime.getText() + "\n";
	rv += "SAVETIME = " + savetime.getText() + "\n";
	rv += "CHECKTIME = " + checktime.getText() + "\n";
	rv += "PING_FREQUENCY = " + ping_frequency.getText() + "\n";
	rv += "STARTHRESH = " + starthresh.getText() + "\n";
	rv += "LISTSIZE = " + listsize.getText() + "\n";
	rv += "MAXLIST = " + maxlist.getText() + "\n";
	rv += "MIN_THREADS = " + min_threads.getText() + "\n";
	rv += "MAX_THREADS = " + max_threads.getText() + "\n";
	rv += "LOW_WATER_MARK = " + low_water_mark.getText() + "\n";
	rv += "HIGH_WATER_MARK = " + high_water_mark.getText() + "\n";
	rv += "HEARTBEAT_TIME = " + heartbeat_time.getText() + "\n";
	rv += "MSG_SEEN_TIME = " + msg_seen_time.getText() + "\n";
	rv += "MSG_CHECK_TIME = " + msg_check_time.getText() + "\n";

	return rv;
    }

    public void parseCfg(String data)
    {
    }
}
