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

public class OperServ extends TabbedPane
{
// private:
    private JFormattedTextField def_expire, expire_oper, expire_admin, expire_sop,
		expire_sadmin, akill_reject, max_clone, clone_limit, clone_trigger,
		clone_time, clone_akilltime, flood_time, flood_msgs, ignore_time,
		ignore_limit, ignore_remove, init_htm_gap, init_htm_thresh,
		max_htm_gap, htm_on_gap;
    private JTextField services_admin, def_clone, clone_akill;
    private JCheckBox secure, secureoper, log_ignore;
    private JComboBox ignore_method;
    private int def_ignore_method;

    public String name() { return "OperServ"; }

    public OperServ(ToolTips t)
    {
	super(t);

	services_admin = createTextField("SERVICES_ADMIN", 30, "PreZ", true);
	def_expire = createFormattedTextField("DEF_EXPIRE", 4, new TimeFormat(), "3h", true);
	expire_oper = createFormattedTextField("EXPIRE_OPER", 4, new TimeFormat(), "1d", true);
	expire_admin = createFormattedTextField("EXPIRE_ADMIN", 4, new TimeFormat(), "1w", true);
	expire_sop = createFormattedTextField("EXPIRE_SOP", 4, new TimeFormat(), "8w", true);
	expire_sadmin = createFormattedTextField("EXPIRE_SADMIN", 4, new TimeFormat(), "1y", true);
	akill_reject = createFormattedTextField("AKILL_REJECT", 5, new PercentageFormat(), "10.00", true);
	max_clone = createFormattedTextField("MAX_CLONE", 4, new NumberRangeFormat(1, -1), "50", true);
	clone_limit = createFormattedTextField("CLONE_LIMIT", 3, new NumberRangeFormat(1, -1), "2", true);
	def_clone = createTextField("DEF_CLONE", 30, "Maximum connections from one host exceeded", true);
	clone_trigger = createFormattedTextField("CLONE_TRIGGER", 2, new NumberRangeFormat(1, -1), "10", true);
	clone_time = createFormattedTextField("CLONE_TIME", 4, new TimeFormat(), "3h", true);
	clone_akill = createTextField("CLONE_AKILL", 30, "Clone trigger exceeded, Automatic AKILL", true);
	clone_akilltime = createFormattedTextField("CLONE_AKILLTIME", 4, new TimeFormat(), "30m", true);
	flood_time = createFormattedTextField("FLOOD_TIME", 4, new TimeFormat(), "10s", true);
	flood_msgs = createFormattedTextField("FLOOD_MSGS", 3, new NumberRangeFormat(1, -1), "5", true);
	ignore_time = createFormattedTextField("IGNORE_TIME", 4, new TimeFormat(), "20s", true);
	ignore_limit = createFormattedTextField("IGNORE_LIMIT", 3, new NumberRangeFormat(1, -1), "5", true);
	ignore_remove = createFormattedTextField("IGNORE_REMOVE", 4, new TimeFormat(), "5m", true);
	init_htm_gap = createFormattedTextField("INIT_HTM_GAP", 4, new TimeFormat(), "5s", true);
	init_htm_thresh = createFormattedTextField("INIT_HTM_THRESH", 4, new SpaceFormat(), "16k", true);
	max_htm_gap = createFormattedTextField("MAX_HTM_GAP", 4, new TimeFormat(), "1m", true);
	htm_on_gap = createFormattedTextField("HTM_ON_GAP", 4, new TimeFormat(), "30s", true);

	secure = createCheckBox("SECURE", false, true);
	secureoper = createCheckBox("SECUREOPER", false, true);
	log_ignore = createCheckBox("LOG_IGNORE", false, true);

	ignore_method = createComboBox("IGNORE_METHOD", false, true);
	def_ignore_method = 7;
	ignore_method.addItem(new String("1 - nick!*@*"));
	ignore_method.addItem(new String("2 - nick!user@port.host"));
	ignore_method.addItem(new String("3 - nick!*user@*.host"));
	ignore_method.addItem(new String("4 - nick!*@port.host"));
	ignore_method.addItem(new String("5 - nick!*@*.host"));
	ignore_method.addItem(new String("6 - *!user@port.host"));
	ignore_method.addItem(new String("7 - *!*user@*.host"));
	ignore_method.addItem(new String("8 - *!*@port.host"));
	ignore_method.addItem(new String("9 - *!*@*.host"));
	ignore_method.setSelectedIndex(def_ignore_method);
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
	addToGridBagLine(gb, gc, "Services Admins", services_admin);

	addToGridBag(gb, gc, "Secure", secure);
	addToGridBag(gb, gc, "Secure Operators", secureoper);
	addGridBagLine(gb, gc);

	addToGridBag(gb, gc, "AutoKills", new JLabel(" "));
	addToGridBag(gb, gc, "Clone Protection", new JLabel(" "));
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Default Expiry", def_expire);
	addToGridBag(gb, gc, "Clone Limit", clone_limit);

	addGridBagLine(gb, gc);

	addToGridBag(gb, gc, "OPER Expire", expire_oper);
	addToGridBag(gb, gc, "Maximum Override", max_clone);
	addGridBagLine(gb, gc);

	addToGridBag(gb, gc, "ADMIN Expire", expire_admin);
	addToGridBag(gb, gc, "AKILL Trigger", clone_trigger);
	addGridBagLine(gb, gc);

	addToGridBag(gb, gc, "SOP Expire", expire_sop);
	addToGridBag(gb, gc, "Trigger Memory", clone_time);
	addGridBagLine(gb, gc);

	addToGridBag(gb, gc, "SADMIN Expire", expire_sadmin);
	addToGridBag(gb, gc, "AKILL Time", clone_akilltime);
	addGridBagLine(gb, gc);

	addToGridBag(gb, gc, "Reject Match", akill_reject);
	addGridBagLine(gb, gc);

	addToGridBagLine(gb, gc, "Clone KILL reason", def_clone);
	addToGridBagLine(gb, gc, "Clone AKILL reason", clone_akill);

	addToGridBag(gb, gc, "Ignore Time", ignore_time);
	addToGridBag(gb, gc, "Flood Time", flood_time);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Ignore Limit", ignore_limit);
	addToGridBag(gb, gc, "Flood Messages", flood_msgs);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Ignore Memory", ignore_remove);
	addToGridBag(gb, gc, "Log Ignored", log_ignore);
	addGridBagLine(gb, gc);
	addToGridBagLine(gb, gc, "Ignore Method", ignore_method);
	addToGridBag(gb, gc, "Initial HTM gap", init_htm_gap);
	addToGridBag(gb, gc, "Initial HTM threshold", init_htm_thresh);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Maximum HTP gap", max_htm_gap);
	addToGridBag(gb, gc, "HTM ON gap", htm_on_gap);
	addGridBagLine(gb, gc);
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

	rv += "[OperServ]\n";
	rv += "SERVICES_ADMIN = " + services_admin.getText() + "\n";
	rv += "SECURE = " + (secure.isSelected() ? "TRUE" : "FALSE") + "\n";
	rv += "SECUREOPER = " + (secureoper.isSelected() ? "TRUE" : "FALSE") + "\n";
	if (!isEditValid(def_expire)) throw new DataFormatException("OperServ/DEF_EXPIRE");
	rv += "DEF_EXPIRE = " + def_expire.getText() + "\n";
	if (!isEditValid(expire_oper)) throw new DataFormatException("OperServ/EXPIRE_OPER");
	rv += "EXPIRE_OPER = " + expire_oper.getText() + "\n";
	if (!isEditValid(expire_admin)) throw new DataFormatException("OperServ/EXPIRE_ADMIN");
	rv += "EXPIRE_ADMIN = " + expire_admin.getText() + "\n";
	if (!isEditValid(expire_sop)) throw new DataFormatException("OperServ/EXPIRE_SOP");
	rv += "EXPIRE_SOP = " + expire_sop.getText() + "\n";
	if (!isEditValid(expire_sadmin)) throw new DataFormatException("OperServ/EXPIRE_SADMIN");
	rv += "EXPIRE_SADMIN = " + expire_sadmin.getText() + "\n";
	if (!isEditValid(akill_reject)) throw new DataFormatException("OperServ/AKILL_REJECT");
	rv += "AKILL_REJECT = " + akill_reject.getText() + "\n";
	if (!isEditValid(max_clone)) throw new DataFormatException("OperServ/MAX_CLONE");
	rv += "MAX_CLONE = " + max_clone.getText() + "\n";
	if (!isEditValid(clone_limit)) throw new DataFormatException("OperServ/CLONE_LIMIT");
	rv += "CLONE_LIMIT = " + clone_limit.getText() + "\n";
	rv += "DEF_CLONE = " + def_clone.getText() + "\n";
	if (!isEditValid(clone_trigger)) throw new DataFormatException("OperServ/CLONE_TRIGGER");
	rv += "CLONE_TRIGGER = " + clone_trigger.getText() + "\n";
	if (!isEditValid(clone_time)) throw new DataFormatException("OperServ/CLONE_TIME");
	rv += "CLONE_TIME = " + clone_time.getText() + "\n";
	rv += "CLONE_AKILL = " + clone_akill.getText() + "\n";
	if (!isEditValid(clone_akilltime)) throw new DataFormatException("OperServ/CLONE_AKILLTIME");
	rv += "CLONE_AKILLTIME = " + clone_akilltime.getText() + "\n";
	if (!isEditValid(flood_time)) throw new DataFormatException("OperServ/FLOOD_TIME");
	rv += "FLOOD_TIME = " + flood_time.getText() + "\n";
	if (!isEditValid(flood_msgs)) throw new DataFormatException("OperServ/FLOOD_MSGS");
	rv += "FLOOD_MSGS = " + flood_msgs.getText() + "\n";
	if (!isEditValid(ignore_time)) throw new DataFormatException("OperServ/IGNORE_TIME");
	rv += "IGNORE_TIME = " + ignore_time.getText() + "\n";
	if (!isEditValid(ignore_limit)) throw new DataFormatException("OperServ/IGNORE_LIMIT");
	rv += "IGNORE_LIMIT = " + ignore_limit.getText() + "\n";
	if (!isEditValid(ignore_remove)) throw new DataFormatException("OperServ/IGNORE_REMOVE");
	rv += "IGNORE_REMOVE = " + ignore_remove.getText() + "\n";
	rv += "IGNORE_METHOD = " + (ignore_method.getSelectedIndex() + 1) + "\n";
	rv += "LOG_IGNORE = " + (log_ignore.isSelected() ? "TRUE" : "FALSE") + "\n";
	if (!isEditValid(init_htm_gap)) throw new DataFormatException("OperServ/INIT_HTM_GAP");
	rv += "INIT_HTM_GAP = " + init_htm_gap.getText() + "\n";
	if (!isEditValid(init_htm_thresh)) throw new DataFormatException("OperServ/INIT_HTM_THRESH");
	rv += "INIT_HTM_THRESH = " + init_htm_thresh.getText() + "\n";
	if (!isEditValid(max_htm_gap)) throw new DataFormatException("OperServ/MAX_HTM_GAP");
	rv += "MAX_HTM_GAP = " + max_htm_gap.getText() + "\n";
	if (!isEditValid(htm_on_gap)) throw new DataFormatException("OperServ/HTM_ON_GAP");
	rv += "HTM_ON_GAP = " + htm_on_gap.getText() + "\n";

	return rv;
    }

    public void parseCfg(IniParser data)
    {
	int i;

	services_admin.setText(data.getValue("OperServ/SERVICES_ADMIN"));
	secure.setSelected(IniParser.getBoolValue(data.getValue("OperServ/SECURE")));
	secureoper.setSelected(IniParser.getBoolValue(data.getValue("OperServ/SECUREOPER")));
	setFmtField(def_expire, data, "OperServ/DEF_EXPIRE");
	setFmtField(expire_oper, data, "OperServ/EXPIRE_OPER");
	setFmtField(expire_admin, data, "OperServ/EXPIRE_ADMIN");
	setFmtField(expire_sop, data, "OperServ/EXPIRE_SOP");
	setFmtField(expire_sadmin, data, "OperServ/EXPIRE_SADMIN");
	setFmtField(akill_reject, data, "OperServ/AKILL_REJECT");
	setFmtField(max_clone, data, "OperServ/MAX_CLONE");
	setFmtField(clone_limit, data, "OperServ/CLONE_LIMIT");
	def_clone.setText(data.getValue("OperServ/DEF_CLONE"));
	setFmtField(clone_trigger, data, "OperServ/CLONE_TRIGGER");
	setFmtField(clone_time, data, "OperServ/CLONE_TIME");
	clone_akill.setText(data.getValue("OperServ/CLONE_AKILL"));
	setFmtField(clone_akilltime, data, "OperServ/CLONE_AKILLTIME");
	setFmtField(flood_time, data, "OperServ/FLOOD_TIME");
	setFmtField(flood_msgs, data, "OperServ/FLOOD_MSGS");
	setFmtField(ignore_time, data, "OperServ/IGNORE_TIME");
	setFmtField(ignore_limit, data, "OperServ/IGNORE_LIMIT");
	setFmtField(ignore_remove, data, "OperServ/IGNORE_REMOVE");

	String method = data.getValue("ChanServ/DEF_REVENGE");
	for (i=0; i<ignore_method.getItemCount(); i++)
	{
	    String ri = ((String) ignore_method.getItemAt(i)).substring(0, 
			((String) ignore_method.getItemAt(i)).indexOf(" "));
	    if (ri.equalsIgnoreCase(method))
	    {
		ignore_method.setSelectedItem(ignore_method.getItemAt(i));
		break;
	    }
	}	
	if (i == ignore_method.getItemCount())
	    ignore_method.setSelectedIndex(def_ignore_method);

	log_ignore.setSelected(IniParser.getBoolValue(data.getValue("OperServ/LOG_IGNORE")));
	setFmtField(init_htm_gap, data, "OperServ/INIT_HTM_GAP");
	setFmtField(init_htm_thresh, data, "OperServ/INIT_HTM_THRESH");
	setFmtField(max_htm_gap, data, "OperServ/MAX_HTM_GAP");
	setFmtField(htm_on_gap, data, "OperServ/HTM_ON_GAP");
    }
}
