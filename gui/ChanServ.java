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

import javax.swing.table.*;
import java.util.zip.DataFormatException;

public class ChanServ extends TabbedPane
{
// private:
    private JCheckBox hide, lck_bantime, lck_parttime, lck_revenge;
    private JFormattedTextField expire, delay, max_per_nick, max_messages,
		passfail, chankeep, def_bantime, def_parttime, level_max, level_min;
    private JTextField def_akick, def_mlock, lck_mlock, ovr_per_nick, ovr_messages;
    private JComboBox def_revenge;

    private final String[] optnames = {
		"KEEPTOPIC",
		"TOPICLOCK",
		"PRIVATE",
		"SECUREOPS",
		"SECURE",
		"NOEXPIRE",
		"ANARCHY",
		"KICKONBAN",
		"RESTRICTED",
		"JOIN",
	};

    private final String[] levelnames = {
		"AUTODEOP",
		"AUTOVOICE",
		"AUTOHALFOP",
		"AUTOOP",
		"READMEMO",
		"WRITEMEMO",
		"DELMEMO",
		"GREET",
		"OVERGREET",
		"MESSAGE",
		"AKICK",
		"SUPER",
		"UNBAN",
		"ACCESS",
		"SET",
		"VIEW",
		"CMDINVITE",
		"CMDUNBAN",
		"CMDVOICE",
		"CMDHALFOP",
		"CMDOP",
		"CMDKICK",
		"CMDMODE",
		"CMDCLEAR",
	};

    private class OptionsTableModel extends AbstractTableModel
	{
	    private Object[][] internal = {
		// { new String("Ban Time"), new String("0"), new Boolean(false) },
		// { new String("Part Time"), new String("3m"), new Boolean(false) },
		{ new String("Keep Topic"), new Boolean(true), new Boolean(false) },
		{ new String("Topic Lock"), new Boolean(false), new Boolean(false) },
		{ new String("Private"), new Boolean(false), new Boolean(false) },
		{ new String("Secure Ops"), new Boolean(false), new Boolean(false) },
		{ new String("Secure"), new Boolean(false), new Boolean(false) },
		{ new String("No Expire"), new Boolean(false), new Boolean(false) },
		{ new String("Anarchy"), new Boolean(false), new Boolean(false) },
		{ new String("Kick On Ban"), new Boolean(false), new Boolean(false) },
		{ new String("Restricted"), new Boolean(false), new Boolean(false) },
		{ new String("Join"), new Boolean(false), new Boolean(false) },
		// { new String("Revenge"), new String("NONE"), new Boolean(false) },
		};

	    private TableModelListener tml = new TableModelListener()
	    {
		public void tableChanged(TableModelEvent e)
		{
		}
	    };

	    public OptionsTableModel()
	    {
		addTableModelListener(tml);
	    }

	    public int getColumnCount() { return internal[0].length; }
	    public int getRowCount() { return internal.length; }
	    public String getColumnName(int col)
	    {
		switch (col)
		{
		case 0:
		    return "Option";
		case 1:
		    return "Default";
		case 2:
		    return "Locked";
		}
		return "";
	    }
	    public Class getColumnClass(int col)
	    {
		switch (col)
		{
		case 0:
		    return String.class;
		case 1:
		    return Boolean.class;
		case 2:
		    return Boolean.class;
		}
		return Object.class;
	    }
	    public boolean isCellEditable(int row, int col)
	    {
		if (row >= internal.length || col == 0 || col >= internal[row].length)
		    return false;
		return true;
	    }
	    public Object getValueAt(int row, int col)
	    {
		if (row >= internal.length)
		    return null;
		if (col >= internal[row].length)
		    return null;
		return internal[row][col];
	    }
	    public void setValueAt(Object in, int row, int col)
	    {
		if (row >= internal.length)
		    return;
		if (col >= internal[row].length)
		    return;
		internal[row][col] = in;
	    }

	}
    private class OptionsTableCellRenderer implements TableCellRenderer
	{
	    public Component getTableCellRendererComponent(JTable table, Object value,
			boolean isSelected, boolean hasFocus, int row, int column)
	    {
		switch (column)
		{
		case 2:
		case 1:
		    JCheckBox cb = new JCheckBox("", ((Boolean) value).booleanValue());
		    cb.setHorizontalAlignment(SwingConstants.CENTER);
		    cb.setEnabled(table.isCellEditable(row, column));
		    return cb;
		case 0:
		    return new JLabel((String) value);
		default:
		    JTextField t = new JTextField(10);
		    t.setText((String) value);
		    t.setEditable(table.isCellEditable(row, column));
		    t.setEnabled(table.isCellEditable(row, column));
		    return t;
		}
	    }
	}

    private class LevelsTableModel extends AbstractTableModel
	{
	    private Object[][] internal = {
		{ new String("Auto Deop"), new Integer(-1) }, 
		{ new String("Auto Voice"), new Integer(5) }, 
		{ new String("Auto Half-Op"), new Integer(10) }, 
		{ new String("Auto Op"), new Integer(10) }, 
		{ new String("Read Memos"), new Integer(0) }, 
		{ new String("Write Memos"), new Integer(15) }, 
		{ new String("Delete Memos"), new Integer(25) }, 
		{ new String("Greet"), new Integer(1) }, 
		{ new String("Override Greet"), new Integer(25) }, 
		{ new String("On-Join Messages"), new Integer(20) }, 
		{ new String("AutoKick"), new Integer(20) }, 
		{ new String("Super"), new Integer(25) }, 
		{ new String("Unban"), new Integer(10) }, 
		{ new String("Access List"), new Integer(5) }, 
		{ new String("Set"), new Integer(25) }, 
		{ new String("View"), new Integer(1) }, 
		{ new String("Invite Command"), new Integer(5) }, 
		{ new String("Unban Command"), new Integer(10) }, 
		{ new String("Voice Command"), new Integer(5) }, 
		{ new String("Halfop Command"), new Integer(10) }, 
		{ new String("Op Command"), new Integer(10) }, 
		{ new String("Kick Command"), new Integer(15) }, 
		{ new String("Mode Command"), new Integer(15) }, 
		{ new String("Clear Command"), new Integer(20) }, 
		};

	    private TableModelListener tml = new TableModelListener()
	    {
		public void tableChanged(TableModelEvent e)
		{
		}
	    };

	    public LevelsTableModel()
	    {
		addTableModelListener(tml);
	    }

	    public int getColumnCount() { return internal[0].length; }
	    public int getRowCount() { return internal.length; }
	    public String getColumnName(int col)
	    {
		switch (col)
		{
		case 0:
		    return "Level";
		case 1:
		    return "Default";
		}
		return "";
	    }
	    public Class getColumnClass(int col)
	    {
		switch (col)
		{
		case 0:
		    return String.class;
		case 1:
		    return Integer.class;
		}
		return Object.class;
	    }
	    public boolean isCellEditable(int row, int col)
	    {
		if (row >= internal.length || col == 0 || col >= internal[row].length)
		    return false;
		return true;
	    }
	    public Object getValueAt(int row, int col)
	    {
		if (row >= internal.length)
		    return null;
		if (col >= internal[row].length)
		    return null;
		return internal[row][col];
	    }
	    public void setValueAt(Object in, int row, int col)
	    {
		if (row >= internal.length)
		    return;
		if (col >= internal[row].length)
		    return;
		internal[row][col] = in;
	    }

	}
    private class LevelsTableCellRenderer implements TableCellRenderer
	{
	    JFormattedTextField low, high;
	    public LevelsTableCellRenderer(JFormattedTextField l, JFormattedTextField h)
	    {
		super();
		low = l;
		high = h;
	    }

	    public Component getTableCellRendererComponent(JTable table, Object value,
			boolean isSelected, boolean hasFocus, int row, int column)
	    {
		switch (column)
		{
		case 1:
		    JFormattedTextField ft = new JFormattedTextField(new NumberRangeFormat(
			Integer.parseInt(low.getText()), Integer.parseInt(high.getText()) + 2));
		    ft.setFocusLostBehavior(JFormattedTextField.COMMIT);
		    ft.setColumns(5);
		    if (value != null)
			ft.setValue((Integer) value);
		    ft.setEditable(table.isCellEditable(row, column));
		    ft.setEnabled(table.isCellEditable(row, column));
		    return ft;
		case 0:
		    return new JLabel((String) value);
		default:
		    JTextField t = new JTextField(15);
		    t.setText((String) value);
		    t.setEditable(table.isCellEditable(row, column));
		    t.setEnabled(table.isCellEditable(row, column));
		    return t;
		}
	    }
	}

    private JTable options, levels;

    public String name() { return "ChanServ"; }

    public ChanServ(ToolTips t)
    {
	super(t);

	hide = createCheckBox("HIDE", false, true);
	lck_bantime = createCheckBox("LCK_BANTIME", false, true);
	lck_parttime = createCheckBox("LCK_PARTTIME", false, true);
	lck_revenge = createCheckBox("LCK_REVENGE", false, true);

	expire = createFormattedTextField("EXPIRE", 4, new TimeFormat(), "2w", true);
	delay = createFormattedTextField("DELAY", 4, new TimeFormat(), "30s", true);
	max_per_nick = createFormattedTextField("MAX_PER_NICK", 3, new NumberRangeFormat(0, -1), "15", true);
	ovr_per_nick = createTextField("OVR_PER_NICK", 20, "", true);
	max_messages = createFormattedTextField("MAX_MESSAGES", 3, new NumberRangeFormat(0, -1), "15", true);
	ovr_messages = createTextField("OVR_PER_NICK", 20, "", true);
	def_akick = createTextField("DEF_AKICK", 30, "You have been banned from channel", true);
	passfail = createFormattedTextField("PASSFAIL", 2, new NumberRangeFormat(0, -1), "5", true);
	chankeep = createFormattedTextField("CHANKEEP", 4, new TimeFormat(), "15s", true);
	def_mlock = createTextField("DEF_MLOCK", 10, "+nt", true);
	lck_mlock = createTextField("LCK_MLOCK", 5, "", true);
	def_bantime = createFormattedTextField("DEF_BANTIME", 4, new TimeFormat(), "0s", true);
	def_parttime = createFormattedTextField("DEF_PARTTIME", 4, new TimeFormat(), "3m", true);
	level_min = createFormattedTextField("LEVEL_MIN", 5, new NumberRangeFormat(-10000, 10000), "-1", true);
	level_max = createFormattedTextField("LEVEL_MAX", 5, new NumberRangeFormat(-10000, 10000), "30", true);

	OptionsTableModel optionsModel = new OptionsTableModel();
	OptionsTableCellRenderer optionsRenderer = new OptionsTableCellRenderer();
	options = createTable("OPTIONS", optionsModel, optionsRenderer, false);

	LevelsTableModel levelsModel = new LevelsTableModel();
	LevelsTableCellRenderer levelsRenderer = new LevelsTableCellRenderer(level_min, level_max);
	levels = createTable("LEVELS", levelsModel, levelsRenderer, false);

	def_revenge = createComboBox("DEF_REVENGE", false, true);
	def_revenge.addItem(new String("NONE - Disabled"));
	def_revenge.addItem(new String("REVERSE - Reverse Activity"));
	def_revenge.addItem(new String("DEOP - Deop Only"));
	def_revenge.addItem(new String("KICK - Kick Only"));
	def_revenge.addItem(new String("BAN1 - Nick Ban (nick!*@*)"));
	def_revenge.addItem(new String("BAN2 - Mask Ban (*!*user@*.host)"));
	def_revenge.addItem(new String("BAN3 - Port Ban (*!*@port.host)"));
	def_revenge.addItem(new String("BAN4 - Host Ban (*!*@*.host)"));
	def_revenge.addItem(new String("MIRROR - Mirror Activity"));
    }

    public void documentChanged(DocumentEvent e)
    {
	try
	{
	    Document props = e.getDocument();
	    if (props.getProperty("name").equals("level_min") && level_min.getText().length() != 0 &&
			level_max.getText().length() != 0)
	    {
		if (Integer.parseInt(level_max.getText()) < Integer.parseInt(level_min.getText()))
		    level_max.setText(level_min.getText());
		((NumberRangeFormat) level_max.getFormatter()).setLow(Integer.parseInt(level_min.getText()));
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
	addToGridBag(gb, gc, "Registration Delay", delay);
	addToGridBag(gb, gc, "Hide", hide);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Expiration Time", expire);
	addToGridBag(gb, gc, "Keep Time", chankeep);
	addGridBagLine(gb, gc);

	addToGridBag(gb, gc, "Maximum Per Nickname Override", ovr_per_nick);
	addToGridBag(gb, gc, "Maximum Per Nickname", max_per_nick);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Maximum On-joins Override", ovr_messages);
	addToGridBag(gb, gc, "Maximum On-joins", max_messages);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Maximum Pass Failures", passfail);
	addGridBagLine(gb, gc);
	addToGridBagLine(gb, gc, "Default AutoKick", def_akick);
	addToGridBag(gb, gc, "Default ModeLock", def_mlock);
	addToGridBag(gb, gc, "Locked", lck_mlock);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Default Ban Time", def_bantime);
	addToGridBag(gb, gc, "Locked", lck_bantime);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Default Part Time", def_parttime);
	addToGridBag(gb, gc, "Locked", lck_parttime);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Default Revenge Level", def_revenge);
	addToGridBag(gb, gc, "Locked", lck_revenge);
	addGridBagLine(gb, gc);
	addToGridBagTable(gb, gc, "Default Options", options);
	addToGridBag(gb, gc, "Minimum Level", level_min);
	addToGridBag(gb, gc, "Maximum Level", level_max);
	addGridBagLine(gb, gc);
	addToGridBagTable(gb, gc, "Access Levels", levels);
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

	rv += "[ChanServ]\n";
	rv += "HIDE = " + (hide.isSelected() ? "TRUE" : "FALSE") + "\n";
	if (!isEditValid(expire)) throw new DataFormatException("ChanServ/EXPIRE");
	rv += "EXPIRE = " + expire.getText() + "\n";
	if (!isEditValid(delay)) throw new DataFormatException("ChanServ/DELAY");
	rv += "DELAY = " + delay.getText() + "\n";
	if (!isEditValid(max_per_nick)) throw new DataFormatException("ChanServ/MAX_PER_NICK");
	rv += "MAX_PER_NICK = " + max_per_nick.getText() + "\n";
	rv += "OVR_PER_NICK = " + ovr_per_nick.getText() + "\n";
	if (!isEditValid(max_messages)) throw new DataFormatException("ChanServ/MAX_MESSAGES");
	rv += "MAX_MESSAGES = " + max_messages.getText() + "\n";
	rv += "OVR_MESSAGES = " + ovr_messages.getText() + "\n";
	rv += "DEF_AKICK = " + def_akick.getText() + "\n";
	if (!isEditValid(passfail)) throw new DataFormatException("ChanServ/PASSFAIL");
	rv += "PASSFAIL = " + passfail.getText() + "\n";
	if (!isEditValid(chankeep)) throw new DataFormatException("ChanServ/CHANKEEP");
	rv += "CHANKEEP = " + chankeep.getText() + "\n";
	rv += "DEF_MLOCK = " + def_mlock.getText() + "\n";
	rv += "LCK_MLOCK = " + lck_mlock.getText() + "\n";
	if (!isEditValid(def_bantime)) throw new DataFormatException("ChanServ/DEF_BANTIME");
	rv += "DEF_BANTIME = " + def_bantime.getText() + "\n";
	rv += "LCK_BANTIME = " + (lck_bantime.isSelected() ? "TRUE" : "FALSE") + "\n";
	if (!isEditValid(def_parttime)) throw new DataFormatException("ChanServ/DEF_PARTTIME");
	rv += "DEF_PARTTIME = " + def_parttime.getText() + "\n";
	rv += "LCK_PARTTIME = " + (lck_parttime.isSelected() ? "TRUE" : "FALSE") + "\n";
	CellEditor ce;
	ce = options.getCellEditor();
	if (ce != null)
	    ce.stopCellEditing();
	for (int i=0; i<options.getRowCount(); i++)
	{
	    rv += "DEF_" + optnames[i] + " = " +
			(((Boolean) options.getValueAt(i, 1)).booleanValue() ? "TRUE" : "FALSE") + "\n";
	    rv += "LCK_" + optnames[i] + " = " +
			(((Boolean) options.getValueAt(i, 2)).booleanValue() ? "TRUE" : "FALSE") + "\n";
	}
	rv += "DEF_REVENGE = " + ((String) def_revenge.getSelectedItem()).substring(0,
			((String) def_revenge.getSelectedItem()).indexOf(" ")) + "\n";
	rv += "LCK_REVENGE = " + (lck_revenge.isSelected() ? "TRUE" : "FALSE") + "\n";
	if (!isEditValid(level_min)) throw new DataFormatException("ChanServ/LEVEL_MIN");
	rv += "LEVEL_MIN = " + level_min.getText() + "\n";
	if (!isEditValid(level_max)) throw new DataFormatException("ChanServ/LEVEL_MAX");
	rv += "LEVEL_MAX = " + level_max.getText() + "\n";
	ce = levels.getCellEditor();
	if (ce != null)
	    ce.stopCellEditing();
	for (int i=0; i<levels.getRowCount(); i++)
	{
	    rv += "LVL_" + levelnames[i] + " = " +
			((Integer) levels.getValueAt(i, 1)).intValue() + "\n";
	}

	return rv;
    }

    public void parseCfg(IniParser data)
    {
	int i;

	hide.setSelected(IniParser.getBoolValue(data.getValue("ChanServ/HIDE")));
	setFmtField(expire, data, "ChanServ/EXPIRE");
	setFmtField(delay, data, "ChanServ/DELAY");
	setFmtField(max_per_nick, data, "ChanServ/MAX_PER_NICK");
	ovr_per_nick.setText(data.getValue("ChanServ/OVR_PER_NICK"));
	setFmtField(max_messages, data, "ChanServ/MAX_MESSAGES");
	ovr_messages.setText(data.getValue("ChanServ/OVR_MESSAGES"));
	def_akick.setText(data.getValue("ChanServ/DEF_AKICK"));
	setFmtField(passfail, data, "ChanServ/PASSFAIL");
	setFmtField(chankeep, data, "ChanServ/CHANKEEP");
	def_mlock.setText(data.getValue("ChanServ/DEF_MLOCK"));
	lck_mlock.setText(data.getValue("ChanServ/LCK_MLOCK"));
	setFmtField(def_bantime, data, "ChanServ/DEF_BANTIME");
	lck_bantime.setSelected(IniParser.getBoolValue(data.getValue("ChanServ/LCK_BANTIME")));
	setFmtField(def_parttime, data, "ChanServ/DEF_PARTTIME");
	lck_parttime.setSelected(IniParser.getBoolValue(data.getValue("ChanServ/LCK_PARTTIME")));

	for (i=0; i<optnames.length; i++)
	{
	    if (data.keyExists("ChanServ/DEF_" + optnames[i]))
		options.setValueAt(new Boolean(IniParser.getBoolValue(data.getValue("ChanServ/DEF_" + optnames[i]))), i, 1);
	    if (data.keyExists("ChanServ/LCK_" + optnames[i]))
		options.setValueAt(new Boolean(IniParser.getBoolValue(data.getValue("ChanServ/LCK_" + optnames[i]))), i, 2);
	}

	String revenge = data.getValue("ChanServ/DEF_REVENGE");
	for (i=0; i<def_revenge.getItemCount(); i++)
	{
	    String ri = ((String) def_revenge.getItemAt(i)).substring(0, 
			((String) def_revenge.getItemAt(i)).indexOf(" "));
	    if (ri.equalsIgnoreCase(revenge))
	    {
		def_revenge.setSelectedItem(def_revenge.getItemAt(i));
		break;
	    }
	}	
	if (i == def_revenge.getItemCount())
	    def_revenge.setSelectedIndex(0);

	lck_revenge.setSelected(IniParser.getBoolValue(data.getValue("ChanServ/LCK_REVENGE")));
	setFmtField(level_min, data, "ChanServ/LEVEL_MIN");
	setFmtField(level_max, data, "ChanServ/LEVEL_MAX");

	for (i=0; i<levelnames.length; i++)
	{
	    if (data.keyExists("ChanServ/LVL_" + levelnames[i]))
		levels.setValueAt(new Integer(data.getValue("ChanServ/LVL_" + levelnames[i])), i, 1);
	}
    }
}
