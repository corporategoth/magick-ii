//
//

import javax.swing.*;
import javax.swing.event.*;
import javax.swing.table.*;
import javax.swing.text.*;
import java.awt.*;
import java.awt.event.*;
import java.util.*;

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
	max_clone = createFormattedTextField("MAX_CLONE", 3, new NumberRangeFormat(1, -1), "50", true);
	clone_limit = createFormattedTextField("CLONE_LIMIT", 4, new NumberRangeFormat(1, -1), "2", true);
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
	String def = new String("8 - *!*@port.host");
	ignore_method.addItem(new String("1 - nick!*@*"));
	ignore_method.addItem(new String("2 - nick!user@port.host"));
	ignore_method.addItem(new String("3 - nick!*user@*.host"));
	ignore_method.addItem(new String("4 - nick!*@port.host"));
	ignore_method.addItem(new String("5 - nick!*@*.host"));
	ignore_method.addItem(new String("6 - *!user@port.host"));
	ignore_method.addItem(new String("7 - *!*user@*.host"));
	ignore_method.addItem(def);
	ignore_method.addItem(new String("9 - *!*@*.host"));
	ignore_method.setSelectedItem(def);
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
	GridBagConstraints gc = new GridBagConstraints();
	gb.setLayout(new GridBagLayout());
	gc.gridx=0;
	gc.gridy=0;
	gc.anchor = gc.EAST;


	addToGridBagLine(gb, gc, "", new JLabel(""));
	addToGridBagLine(gb, gc, "Services Admins", services_admin);

	addToGridBag(gb, gc, "Secure", secure);
	addToGridBag(gb, gc, "Secure Operators", secureoper);
	addGridBagLine(gb, gc);

	addToGridBag(gb, gc, "AutoKills", new JLabel(""));
	addToGridBag(gb, gc, "Clone Protection", new JLabel(""));
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Default Expiry", def_expire);
	addToGridBag(gb, gc, "Maximum Clones", max_clone);

	addGridBagLine(gb, gc);

	addToGridBag(gb, gc, "OPER Expire", expire_oper);
	addToGridBag(gb, gc, "Maximum Override", clone_limit);
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

	rv += "[OperServ]\n";
	rv += "SERVICES_ADMIN = " + services_admin.getText() + "\n";
	rv += "SECURE = " + (secure.isSelected() ? "TRUE" : "FALSE") + "\n";
	rv += "SECUREOPER = " + (secureoper.isSelected() ? "TRUE" : "FALSE") + "\n";
	rv += "DEF_EXPIRE = " + def_expire.getText() + "\n";
	rv += "EXPIRE_OPER = " + expire_oper.getText() + "\n";
	rv += "EXPIRE_ADMIN = " + expire_admin.getText() + "\n";
	rv += "EXPIRE_SOP = " + expire_sop.getText() + "\n";
	rv += "EXPIRE_SADMIN = " + expire_sadmin.getText() + "\n";
	rv += "AKILL_REJECT = " + akill_reject.getText() + "\n";
	rv += "MAX_CLONE = " + max_clone.getText() + "\n";
	rv += "CLONE_LIMIT = " + clone_limit.getText() + "\n";
	rv += "DEF_CLONE = " + def_clone.getText() + "\n";
	rv += "CLONE_TRIGGER = " + clone_trigger.getText() + "\n";
	rv += "CLONE_TIME = " + clone_time.getText() + "\n";
	rv += "CLONE_AKILL = " + clone_akill.getText() + "\n";
	rv += "CLONE_AKILLTIME = " + clone_akilltime.getText() + "\n";
	rv += "FLOOD_TIME = " + flood_time.getText() + "\n";
	rv += "FLOOD_MSGS = " + flood_msgs.getText() + "\n";
	rv += "IGNORE_TIME = " + ignore_time.getText() + "\n";
	rv += "IGNORE_LIMIT = " + ignore_limit.getText() + "\n";
	rv += "IGNORE_REMOVE = " + ignore_remove.getText() + "\n";
	rv += "IGNORE_METHOD = " + (ignore_method.getSelectedIndex() + 1) + "\n";
	rv += "LOG_IGNORE = " + (log_ignore.isSelected() ? "TRUE" : "FALSE") + "\n";
	rv += "INIT_HTM_GAP = " + init_htm_gap.getText() + "\n";
	rv += "INIT_HTM_THRESH = " + init_htm_thresh.getText() + "\n";
	rv += "MAX_HTM_GAP = " + max_htm_gap.getText() + "\n";
	rv += "HTM_ON_GAP = " + htm_on_gap.getText() + "\n";

	return rv;
    }

    public void parseCfg(String data)
    {
    }
}
