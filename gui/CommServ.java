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

import javax.swing.table.*;

public class CommServ extends TabbedPane
{
// private:
    private JFormattedTextField max_logon;

    private final String[] optnames = {
		"SECURE",
		"PRIVATE",
		"OPENMEMOS",
	};

    private final String[] commnames = {
		"ALL",
		"REGD",
		"SADMIN",
		"SOP",
		"ADMIN",
		"OPER",
	};

    private final String[] ovrnames = {
		"OVR_VIEW",
		"OVR_OWNER",
		"OVR_CS_MODE",
		"OVR_CS_OP",
		"OVR_CS_HALFOP",
		"OVR_CS_VOICE",
		"OVR_CS_INVITE",
		"OVR_CS_UNBAN",
		"OVR_CS_CLEAR",
	};

    private class OptionsTableModel extends AbstractTableModel
	{
	    private Object[][] internal = {
		{ new String("Secure"), new Boolean(false), new Boolean(false) },
		{ new String("Private"), new Boolean(false), new Boolean(false) },
		{ new String("Open Memos"), new Boolean(true), new Boolean(false) },
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

    private class CommitteesTableModel extends AbstractTableModel
	{
	    private Object[][] internal = {
		{ new String("ALL"), new Boolean(false), new Boolean(false), new Boolean(false), new Boolean(false), new String("") },
		{ new String("REGD"), new Boolean(false), new Boolean(false), new Boolean(false), new Boolean(false), new String("") },
		{ new String("SADMIN"), new Boolean(true), new Boolean(false), new Boolean(true), new Boolean(true), new String("") },
		{ new String("SOP"), new Boolean(true), new Boolean(false), new Boolean(true), new Boolean(true), new String("") },
		{ new String("ADMIN"), new Boolean(true), new Boolean(false), new Boolean(true), new Boolean(true), new String("") },
		{ new String("OPER"), new Boolean(true), new Boolean(false), new Boolean(true), new Boolean(true), new String("") },
		};


	    private TableModelListener tml = new TableModelListener()
	    {
		public void tableChanged(TableModelEvent e)
		{
		}
	    };

	    public CommitteesTableModel()
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
		    return "Committee";
		case 1:
		    return "Secure";
		case 2:
		    return "Private";
		case 3:
		    return "Open Memos";
		case 4:
		    return "Mode +o";
		case 5:
		    return "Set Mode";
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
		case 3:
		    return Boolean.class;
		case 4:
		    return Boolean.class;
		case 5:
		    return String.class;
		}
		return Object.class;
	    }
	    public boolean isCellEditable(int row, int col)
	    {
		if (row >= internal.length || col >= internal[row].length)
		    return false;
		if (row < 2 && col >= 1 && col <= 4)
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
    private class CommitteesTableCellRenderer implements TableCellRenderer
	{
	    public Component getTableCellRendererComponent(JTable table, Object value,
			boolean isSelected, boolean hasFocus, int row, int column)
	    {
		switch (column)
		{
		case 4:
		case 3:
		case 2:
		case 1:
		    JCheckBox cb = new JCheckBox("", ((Boolean) value).booleanValue());
		    cb.setHorizontalAlignment(SwingConstants.CENTER);
		    cb.setEnabled(table.isCellEditable(row, column));
		    return cb;
		default:
		    JTextField t = new JTextField(10);
		    t.setText((String) value);
		    t.setEditable(table.isCellEditable(row, column));
		    t.setEnabled(table.isCellEditable(row, column));
		    return t;
		}
	    }
	}

    private class OverridesTableModel extends AbstractTableModel
	{
	    private Object[][] internal = {
		{ new String("View"), new String("OPER") }, 
		{ new String("Owner"), new String("SADMIN") }, 
		{ new String("CS Mode"), new String("SOP") }, 
		{ new String("CS Op"), new String("SOP") }, 
		{ new String("CS HalfOp"), new String("ADMIN") }, 
		{ new String("CS Voice"), new String("SOP") }, 
		{ new String("CS Invite"), new String("SOP") }, 
		{ new String("CS Unban"), new String("SOP") }, 
		{ new String("CS Clear"), new String("SADMIN") }, 
		};

	    private TableModelListener tml = new TableModelListener()
	    {
		public void tableChanged(TableModelEvent e)
		{
		}
	    };

	    public OverridesTableModel()
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
		    return "Override";
		case 1:
		    return "Committee";
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
		    return String.class;
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
    private class OverridesTableCellRenderer implements TableCellRenderer
	{
	    public Component getTableCellRendererComponent(JTable table, Object value,
			boolean isSelected, boolean hasFocus, int row, int column)
	    {
		JTextField t;
		switch (column)
		{
		case 1:
		    t = new JTextField(10);
		    t.setText((String) value);
		    t.setEditable(table.isCellEditable(row, column));
		    t.setEnabled(table.isCellEditable(row, column));
		    return t;
		case 0:
		    return new JLabel((String) value);
		default:
		    t = new JTextField(15);
		    t.setText((String) value);
		    t.setEditable(table.isCellEditable(row, column));
		    t.setEnabled(table.isCellEditable(row, column));
		    return t;
		}
	    }
	}

    private JTable options, committees, overrides;

    public String name() { return "CommServ"; }

    public CommServ(ToolTips t)
    {
	super(t);

	max_logon = createFormattedTextField("MAX_LOGON", 3, new NumberRangeFormat(0, -1), "5", true);

	OptionsTableModel optionsModel = new OptionsTableModel();
	OptionsTableCellRenderer optionsRenderer = new OptionsTableCellRenderer();
	options = createTable("OPTIONS", optionsModel, optionsRenderer, false);

	CommitteesTableModel committeesModel = new CommitteesTableModel();
	CommitteesTableCellRenderer committeesRenderer = new CommitteesTableCellRenderer();
	committees = createTable("COMMITTEES", committeesModel, committeesRenderer, false);

	OverridesTableModel overridesModel = new OverridesTableModel();
	OverridesTableCellRenderer overridesRenderer = new OverridesTableCellRenderer();
	overrides = createTable("OVERRIDES", overridesModel, overridesRenderer, false);
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
	addToGridBag(gb, gc, "Max Logon Messages", max_logon);
	addGridBagLine(gb, gc);
	addToGridBagTable(gb, gc, "Default Options", options);
	addToGridBagTable(gb, gc, "Committees", committees);
	addToGridBagTable(gb, gc, "Overrides", overrides);
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

	rv += "[CommServ]\n";
	rv += "MAX_LOGON = " + max_logon.getText() + "\n";
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

	ce = committees.getCellEditor();
	if (ce != null)
	    ce.stopCellEditing();
	for (int i=0; i<committees.getRowCount(); i++)
	{
	    rv += commnames[i] + "_NAME = " + committees.getValueAt(i, 0) + "\n";
	    if (i > 1)
	    {
		rv += commnames[i] + "_SECURE = " +
			(((Boolean) committees.getValueAt(i, 1)).booleanValue() ? "TRUE" : "FALSE") + "\n";
		rv += commnames[i] + "_PRVATE = " +
			(((Boolean) committees.getValueAt(i, 2)).booleanValue() ? "TRUE" : "FALSE") + "\n";
		rv += commnames[i] + "_OPENMEMOS = " +
			(((Boolean) committees.getValueAt(i, 3)).booleanValue() ? "TRUE" : "FALSE") + "\n";
		rv += commnames[i] + "_MODEO = " +
			(((Boolean) committees.getValueAt(i, 4)).booleanValue() ? "TRUE" : "FALSE") + "\n";
	    }
	    rv += commnames[i] + "_SETMODE = " + committees.getValueAt(i, 5) + "\n";
	}

	ce = overrides.getCellEditor();
	if (ce != null)
	    ce.stopCellEditing();
	for (int i=0; i<overrides.getRowCount(); i++)
	{
	    rv += ovrnames[i] + " = " + overrides.getValueAt(i, 1) + "\n";
	}

	return rv;
    }

    public void parseCfg(IniParser data)
    {
	int i;
	max_logon.setText(data.getValue("CommServ/MAX_LOGON"));

	for (i=0; i<optnames.length; i++)
	{
	    if (data.keyExists("CommServ/DEF_" + optnames[i]))
		options.setValueAt(new Boolean(IniParser.getBoolValue(data.getValue("CommServ/DEF_" + optnames[i]))), i, 1);
	    if (data.keyExists("CommServ/LCK_" + optnames[i]))
		options.setValueAt(new Boolean(IniParser.getBoolValue(data.getValue("CommServ/LCK_" + optnames[i]))), i, 2);
	}

	for (i=0; i<commnames.length; i++)
	{
	    if (data.keyExists("CommServ/" + commnames[i] + "_SECURE"))
		committees.setValueAt(new Boolean(IniParser.getBoolValue(data.getValue("CommServ/" + commnames[i] + "_SECURE"))), i, 1);
	    if (data.keyExists("CommServ/" + commnames[i] + "_PRIVATE"))
		committees.setValueAt(new Boolean(IniParser.getBoolValue(data.getValue("CommServ/" + commnames[i] + "_PRIVATE"))), i, 2);
	    if (data.keyExists("CommServ/" + commnames[i] + "_OPENMEMOS"))
		committees.setValueAt(new Boolean(IniParser.getBoolValue(data.getValue("CommServ/" + commnames[i] + "_OPENMEMOS"))), i, 3);
	    if (data.keyExists("CommServ/" + commnames[i] + "_MODEO"))
		committees.setValueAt(new Boolean(IniParser.getBoolValue(data.getValue("CommServ/" + commnames[i] + "_MODEO"))), i, 4);
	    if (data.keyExists("CommServ/" + commnames[i] + "_SETMODE"))
		committees.setValueAt(data.getValue("CommServ/" + commnames[i] + "_SETMODE"), i, 5);
	}

	for (i=0; i<ovrnames.length; i++)
	{
	    if (data.keyExists("CommServ/" + ovrnames[i]))
		options.setValueAt(new Boolean(IniParser.getBoolValue(data.getValue("CommServ/" + ovrnames[i]))), i, 1);
	}
    }
}
