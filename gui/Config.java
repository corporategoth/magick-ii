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
	try
	{
	    Document props = e.getDocument();
	    if (props.getProperty("name").equals("listsize") && listsize.getText().length() != 0 &&
		    maxlist.getText().length() != 0)
	    {
		if (Integer.parseInt(maxlist.getText()) < Integer.parseInt(listsize.getText()))
		    maxlist.setText(listsize.getText());
		((NumberRangeFormat) maxlist.getFormatter()).setLow(Integer.parseInt(listsize.getText()));
	    }
	    else if (props.getProperty("name").equals("min_threads") && min_threads.getText().length() != 0 &&
		 max_threads.getText().length() != 0)
	    {
		if (Integer.parseInt(max_threads.getText()) < Integer.parseInt(min_threads.getText()))
		    max_threads.setText(min_threads.getText());
		((NumberRangeFormat) max_threads.getFormatter()).setLow(Integer.parseInt(min_threads.getText()));
	    }
	    else if (props.getProperty("name").equals("low_water_mark") && low_water_mark.getText().length() != 0 &&
		 high_water_mark.getText().length() != 0)
	    {
		if (Integer.parseInt(high_water_mark.getText()) < Integer.parseInt(low_water_mark.getText()))
		    high_water_mark.setText(low_water_mark.getText());
		((NumberRangeFormat) high_water_mark.getFormatter()).setLow(Integer.parseInt(low_water_mark.getText()));
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
	addToGridBag(gb, gc, "Relink Time", server_relink);
	addToGridBag(gb, gc, "SQUIT Protect Time", squit_protect);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Active Cycle Time", cycletime);
	addToGridBag(gb, gc, "SQUIT Cancel Time", squit_cancel);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Hyperactive Cycle Time", checktime);
	addToGridBag(gb, gc, "Default List Size", listsize);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Database Save Time", savetime);
	addToGridBag(gb, gc, "Maximum List Size", maxlist);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Heartbeat Time", heartbeat_time);
	addToGridBag(gb, gc, "Server Ping Time", ping_frequency);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Minimum Threads", min_threads);
	addToGridBag(gb, gc, "Low Water Mark", low_water_mark);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Maximum Threads", max_threads);
	addToGridBag(gb, gc, "High Water Mark", high_water_mark);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Message Expire Time", msg_seen_time);
	addToGridBag(gb, gc, "Wildcard Threshold", starthresh);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Message Expire Check Time", msg_check_time);
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

	rv += "[Config]\n";
	if (!isEditValid(server_relink)) throw new DataFormatException("Config/SERVER_RELINK");
	rv += "SERVER_RELINK = " + server_relink.getText() + "\n";
	if (!isEditValid(squit_protect)) throw new DataFormatException("Config/SQUIT_PROTECT");
	rv += "SQUIT_PROTECT = " + squit_protect.getText() + "\n";
	if (!isEditValid(squit_cancel)) throw new DataFormatException("Config/SQUIT_CANCEL");
	rv += "SQUIT_CANCEL = " + squit_cancel.getText() + "\n";
	if (!isEditValid(cycletime)) throw new DataFormatException("Config/CYCLETIME");
	rv += "CYCLETIME = " + cycletime.getText() + "\n";
	if (!isEditValid(savetime)) throw new DataFormatException("Config/SAVETIME");
	rv += "SAVETIME = " + savetime.getText() + "\n";
	if (!isEditValid(checktime)) throw new DataFormatException("Config/CHECKTIME");
	rv += "CHECKTIME = " + checktime.getText() + "\n";
	if (!isEditValid(ping_frequency)) throw new DataFormatException("Config/PING_FREQUENCY");
	rv += "PING_FREQUENCY = " + ping_frequency.getText() + "\n";
	if (!isEditValid(starthresh)) throw new DataFormatException("Config/STARTHRESH");
	rv += "STARTHRESH = " + starthresh.getText() + "\n";
	if (!isEditValid(listsize)) throw new DataFormatException("Config/LISTSIZE");
	rv += "LISTSIZE = " + listsize.getText() + "\n";
	if (!isEditValid(maxlist)) throw new DataFormatException("Config/MAXLIST");
	rv += "MAXLIST = " + maxlist.getText() + "\n";
	if (!isEditValid(min_threads)) throw new DataFormatException("Config/MIN_THREADS");
	rv += "MIN_THREADS = " + min_threads.getText() + "\n";
	if (!isEditValid(max_threads)) throw new DataFormatException("Config/MAX_THREADS");
	rv += "MAX_THREADS = " + max_threads.getText() + "\n";
	if (!isEditValid(low_water_mark)) throw new DataFormatException("Config/LOW_WATER_MARK");
	rv += "LOW_WATER_MARK = " + low_water_mark.getText() + "\n";
	if (!isEditValid(high_water_mark)) throw new DataFormatException("Config/HIGH_WATER_MARK");
	rv += "HIGH_WATER_MARK = " + high_water_mark.getText() + "\n";
	if (!isEditValid(heartbeat_time)) throw new DataFormatException("Config/HEARTBEAT_TIME");
	rv += "HEARTBEAT_TIME = " + heartbeat_time.getText() + "\n";
	if (!isEditValid(msg_seen_time)) throw new DataFormatException("Config/MSG_SEEN_TIME");
	rv += "MSG_SEEN_TIME = " + msg_seen_time.getText() + "\n";
	if (!isEditValid(msg_check_time)) throw new DataFormatException("Config/MSG_CHECK_TIME");
	rv += "MSG_CHECK_TIME = " + msg_check_time.getText() + "\n";

	return rv;
    }

    public void parseCfg(IniParser data)
    {
	setFmtField(server_relink, data, "Config/SERVER_RELINK");
	setFmtField(squit_protect, data, "Config/SQUIT_PROTECT");
	setFmtField(squit_cancel, data, "Config/SQUIT_CANCEL");
	setFmtField(cycletime, data, "Config/CYCLETIME");
	setFmtField(savetime, data, "Config/SAVETIME");
	setFmtField(checktime, data, "Config/CHECKTIME");
	setFmtField(ping_frequency, data, "Config/PING_FREQUENCY");
	setFmtField(starthresh, data, "Config/STARTHRESH");
	setFmtField(listsize, data, "Config/LISTSIZE");
	setFmtField(maxlist, data, "Config/MAXLIST");
	setFmtField(min_threads, data, "Config/MIN_THREADS");
	setFmtField(max_threads, data, "Config/MAX_THREADS");
	setFmtField(low_water_mark, data, "Config/LOW_WATER_MARK");
	setFmtField(high_water_mark, data, "Config/HIGH_WATER_MARK");
	setFmtField(heartbeat_time, data, "Config/HEARTBEAT_TIME");
	setFmtField(msg_seen_time, data, "Config/MSG_SEEN_TIME");
	setFmtField(msg_check_time, data, "Config/MSG_CHECK_TIME");
    }
}
