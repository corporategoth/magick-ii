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
import java.util.*;
import java.net.*;

public class Startup extends TabbedPane
{
// private:
    private JTextField server_name, server_desc, services_user, services_host, setmode;
    private JFormattedTextField bind, lagtime, level;
    private JComboBox protocol;
    private JCheckBox ownuser, stop;

    private class RemotesTableModel extends AbstractTableModel
	{
	    private Vector internal = new Vector();

	    private void addBlankRecord()
	    {
		Vector v = new Vector();
		InetAddress addr = null;
		try
		{
		    addr = InetAddress.getByName("0.0.0.0");
		}
		catch (Exception ex)
		{
		}
		v.add(addr);
		v.add(new Integer(6667));
		v.add(new String(""));
		v.add(new Integer(1));
		v.add(new Integer(1));
		internal.add(v);
	    }

	    private TableModelListener tml = new TableModelListener()
	    {
		public void tableChanged(TableModelEvent e)
		{
		}
	    };

	    public RemotesTableModel()
	    {
		addTableModelListener(tml);
	    }

	    public int getColumnCount() { return 5; }
	    public int getRowCount()
	    {
		if (internal.size() == 0)
		{
		    addBlankRecord();
		}
		return internal.size();
	    }

	    public String getColumnName(int col)
	    {
		switch (col)
		{
		case 0:
		    return "Server";
		case 1:
		    return "Port";
		case 2:
		    return "Password";
		case 3:
		    return "Priority";
		case 4:
		    return "Numeric";
		}
		return "";
	    }
	    public Class getColumnClass(int col)
	    {
		switch (col)
		{
		case 0:
		    return InetAddress.class;
		case 1:
		    return Integer.class;
		case 2:
		    return String.class;
		case 3:
		    return Integer.class;
		case 4:
		    return Integer.class;
		}
		return Object.class;
	    }
	    public boolean isCellEditable(int row, int col)
	    {
		if (row >= internal.size() || col >= ((Vector) internal.get(row)).size())
		    return false;
		if (col != 0 && ((InetAddress) getValueAt(row, 0)).getHostAddress().equals("0.0.0.0"))
		    return false;
		return true;
	    }
	    public Object getValueAt(int row, int col)
	    {
		if (row >= internal.size())
		    return null;
		Vector v = (Vector) internal.get(row);
		if (col >= v.size())
		    return null;
		return v.get(col);
	    }
	    public void setValueAt(Object in, int row, int col)
	    {
		while (row >= internal.size())
		{
		    addBlankRecord();
		}

		Vector v = (Vector) internal.get(row);

		if (col >= v.size())
		    return;

		v.setElementAt(in, col);

		if (row + 1 == internal.size() && !((InetAddress) getValueAt(row, 0)).getHostAddress().equals("0.0.0.0"))
		{
		    addBlankRecord();
		}

		if (internal.size() > 1 && row + 1 != internal.size() && ((InetAddress) getValueAt(row, 0)).getHostAddress().equals("0.0.0.0"))
		{
		    internal.remove(row);
		}
	    }
	}
    private class RemotesTableCellRenderer implements TableCellRenderer
	{
	    public Component getTableCellRendererComponent(JTable table, Object value,
			boolean isSelected, boolean hasFocus, int row, int column)
	    {
		JFormattedTextField ft;
		switch (column)
		{
		case 4:
		case 3:
		    ft = new JFormattedTextField(new NumberRangeFormat(1, -1));
		    ft.setColumns(5);
		    if (value != null)
			ft.setValue((Integer) value);
		    ft.setEditable(table.isCellEditable(row, column));
		    ft.setEnabled(table.isCellEditable(row, column));
		    return ft;
		case 1:
		    ft = new JFormattedTextField(new NumberRangeFormat(1, 65535));
		    ft.setColumns(5);
		    if (value != null)
			ft.setValue((Integer) value);
		    ft.setEditable(table.isCellEditable(row, column));
		    ft.setEnabled(table.isCellEditable(row, column));
		    return ft;
		case 0:
		    ft = new JFormattedTextField(new IpAddressFormat());
		    ft.setColumns(10);
		    if (value != null)
			ft.setValue((InetAddress) value);
		    ft.setEditable(table.isCellEditable(row, column));
		    ft.setEnabled(table.isCellEditable(row, column));
		    return ft;
		default:
		    JTextField t = new JTextField(10);
		    t.setText((String) value);
		    t.setEditable(table.isCellEditable(row, column));
		    t.setEnabled(table.isCellEditable(row, column));
		    return t;
		}
	    }
	}

    private class AllowsTableModel extends AbstractTableModel
	{
	    private Vector internal = new Vector();

	    private void addBlankRecord()
	    {
		Vector v = new Vector();
		v.add(new String(""));
		v.add(new String(""));
		internal.add(v);
	    }

	    private TableModelListener tml = new TableModelListener()
	    {
		public void tableChanged(TableModelEvent e)
		{
		}
	    };

	    public AllowsTableModel()
	    {
		addTableModelListener(tml);
	    }

	    public int getColumnCount() { return 2; }
	    public int getRowCount()
	    {
		if (internal.size() == 0)
		{
		    addBlankRecord();
		}
		return internal.size();
	    }

	    public String getColumnName(int col)
	    {
		switch (col)
		{
		case 0:
		    return "Server";
		case 1:
		    return "Uplink(s)";
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
		if (row >= internal.size() || col >= ((Vector) internal.get(row)).size())
		    return false;
		if (col != 0 && ((String) getValueAt(row, 0)).length() == 0)
		    return false;
		return true;
	    }
	    public Object getValueAt(int row, int col)
	    {
		if (row >= internal.size())
		    return null;
		Vector v = (Vector) internal.get(row);
		if (col >= v.size())
		    return null;
		return v.get(col);
	    }
	    public void setValueAt(Object in, int row, int col)
	    {
		while (row >= internal.size())
		{
		    addBlankRecord();
		}

		Vector v = (Vector) internal.get(row);

		if (col >= v.size())
		    return;

		v.setElementAt(in, col);

		if (row + 1 == internal.size() && ((String) v.get(0)).length() != 0)
		{
		    addBlankRecord();
		}

		if (internal.size() > 1 && row + 1 != internal.size() && ((String) v.get(0)).length() == 0)
		{
		    internal.remove(row);
		}
	    }

	}


    private class AllowsTableCellRenderer implements TableCellRenderer
	{
	    public Component getTableCellRendererComponent(JTable table, Object value,
			boolean isSelected, boolean hasFocus, int row, int column)
	    {
		JTextField t = new JTextField(15);
		t.setText((String) value);
		t.setEditable(table.isCellEditable(row, column));
		t.setEnabled(table.isCellEditable(row, column));
		return t;
	    }
	}

    private JTable remotes, allows;

    public String name() { return "Startup"; }

    public Startup(ToolTips t)
    {
	super(t);

	ownuser = createCheckBox("OWNUSER", false, true);
	stop = createCheckBox("STOP", true, true);

	server_name = createTextField("SERVER_NAME", 20, "services.magick.tm", true);
	server_desc = createTextField("SERVER_DESC", 30, "Magick IRC Services", true);
	services_user = createTextField("SERVICES_USER", 10, "services", !ownuser.isSelected());
	services_host = createTextField("SERVICES_HOST", 20, "magick.tm", true);
	setmode = createTextField("SETMODE", 5, "", true);
	bind = createFormattedTextField("BIND", 10, new IpAddressFormat(), "", true);
	level = createFormattedTextField("LEVEL", 2, new NumberRangeFormat(1, -1), "1", true);
	lagtime = createFormattedTextField("LAGTIME", 4, new TimeFormat(), "15s", true);

	protocol = createComboBox("PROTOCOL", false, true);
	protocol.addItem(new String("0 - RFC 1459"));
	protocol.addItem(new String("1 - RFC 1459 + TS8"));
	protocol.addItem(new String("10 - DALnet < 4.4.15"));
	protocol.addItem(new String("11 - DALnet >= 4.4.15"));
	protocol.addItem(new String("12 - Bahamut"));
	protocol.addItem(new String("20 - UnderNet < 2.10.00"));
	protocol.addItem(new String("21 - UnderNet > 2.10.00"));
	protocol.addItem(new String("22 - UnderNet > 2.10.00 + Ext. Numerics"));
	protocol.addItem(new String("30 - Hybrid 5-6"));
	protocol.addItem(new String("31 - Hybrid 7"));
	protocol.addItem(new String("40 - EliteIRCD"));
	protocol.addItem(new String("50 - RelicNet 2"));
	protocol.addItem(new String("51 - RelicNet 2.1"));
	protocol.addItem(new String("53 - RelicNet 4.0"));
	protocol.addItem(new String("54 - RelicNet 5.0"));
	protocol.addItem(new String("60 - Aurora"));
	protocol.addItem(new String("70 - Unreal"));
	protocol.addItem(new String("80 - UltimateIRCD"));

	RemotesTableModel remotesModel = new RemotesTableModel();
	RemotesTableCellRenderer remotesRenderer = new RemotesTableCellRenderer();
	remotes = createTable("REMOTES", remotesModel, remotesRenderer, true);

	AllowsTableModel allowsModel = new AllowsTableModel();
	AllowsTableCellRenderer allowsRenderer = new AllowsTableCellRenderer();
	allows = createTable("ALLOWS", allowsModel, allowsRenderer, true);
    }

    public void documentChanged(DocumentEvent e)
    {
	Document props = e.getDocument();
    }

    public void actionPerformed(ActionEvent e)
    {
	if (e.getSource() == ownuser)
	{
	    services_user.setEditable(!ownuser.isSelected());
	    services_user.setEnabled(!ownuser.isSelected());
	}
    }

    public JComponent createPane()
    {
	JPanel gb = new JPanel();
	gb.setLayout(new GridBagLayout());
	GridBagConstraints gc = createStandardConstraints();

	addToGridBagLine(gb, gc, "", new JLabel(" "));
	addToGridBagLine(gb, gc, "Server Name", server_name);
	addToGridBagLine(gb, gc, "Server Description", server_desc);
	addToGridBag(gb, gc, "Bind", bind);
	addToGridBag(gb, gc, "STOP", Color.red, stop);
	addGridBagLine(gb, gc);
	addToGridBagLine(gb, gc, "Services User", services_user);
	addToGridBagLine(gb, gc, "Services Host", services_host);
	addToGridBag(gb, gc, "Set Mode", setmode);
	addToGridBag(gb, gc, "Own User", ownuser);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Level", level);
	addToGridBag(gb, gc, "Lag Time", lagtime);
	addGridBagLine(gb, gc);
	addToGridBagTable(gb, gc, "Remote Connections", remotes);
	addToGridBagTable(gb, gc, "Allowed Connections", allows);
	addToGridBagLine(gb, gc, "Protocol", protocol);
	addToGridBagLine(gb, gc, "", new JLabel(" "));

	JPanel p = new JPanel();
	p.setLayout(new BorderLayout());
	p.add(gb, BorderLayout.NORTH);
	JScrollPane rv = new JScrollPane(p);
	return rv;
    }	

    public String createCfg()
    {
	String rv = new String();

	rv += "[Startup]\n";
	rv += "SERVER_NAME = " + server_name.getText() + "\n";
	rv += "SERVER_DESC = " + server_desc.getText() + "\n";
	rv += "SERVICES_USER = " + services_user.getText() + "\n";
	rv += "SERVICES_HOST = " + services_host.getText() + "\n";
	rv += "OWNUSER = " + (ownuser.isSelected() ? "TRUE" : "FALSE") + "\n";
	rv += "SETMODE = " + setmode.getText() + "\n";
	rv += "BIND = " + bind.getText() + "\n";

	CellEditor ce;
	ce = remotes.getCellEditor();
	if (ce != null)
	    ce.stopCellEditing();
	for (int i=0; i<remotes.getRowCount(); i++)
	{
	    InetAddress addr = (InetAddress) remotes.getValueAt(i, 0);
	    if (!addr.getHostAddress().equals("0.0.0.0"))
		rv += "REMOTE_" + (i+1) + " = " + addr.getHostAddress() + ":" +
			remotes.getValueAt(i, 1) + ":" +
			remotes.getValueAt(i, 2) + ":" +
			remotes.getValueAt(i, 3) + ":" +
			remotes.getValueAt(i, 4) + "\n";
	}

	ce = allows.getCellEditor();
	if (ce != null)
	    ce.stopCellEditing();
	for (int i=0; i<allows.getRowCount(); i++)
	{
	    if (((String) allows.getValueAt(i, 0)).length() != 0)
		rv += "ALLOW_" + (i+1) + " = " + allows.getValueAt(i, 0) + ":" +
			allows.getValueAt(i, 1) + "\n";
	}

	rv += "PROTOCOL = " + ((String) protocol.getSelectedItem()).substring(0,
			((String) protocol.getSelectedItem()).indexOf(" ")) + "\n";
	rv += "LEVEL = " + level.getText() + "\n";
	rv += "LAGTIME = " + lagtime.getText() + "\n";
	rv += "STOP = " + (stop.isSelected() ? "TRUE" : "FALSE") + "\n";

	return rv;
    }

    public void parseCfg(IniParser data)
    {
	int i;

	server_name.setText(data.getValue("Startup/SERVER_NAME"));
	server_desc.setText(data.getValue("Startup/SERVER_DESC"));
	services_user.setText(data.getValue("Startup/SERVICES_USER"));
	services_host.setText(data.getValue("Startup/SERVICES_HOST"));
	ownuser.setSelected(IniParser.getBoolValue(data.getValue("Startup/OWNUSER")));
	setmode.setText(data.getValue("Startup/SETMODE"));
	bind.setText(data.getValue("Startup/BIND"));

	i=0;
	InetAddress addr = null;
	try
	{
	    addr = InetAddress.getByName("0.0.0.0");
	}
	catch (Exception ex)
	{
	}

	while (remotes.getRowCount() > 1)
	    remotes.setValueAt(addr, 0, 0);
	for (i=0; data.keyExists("Startup/REMOTE_" + (i+1)); i++)
	{
	    String fullLine = data.getValue("Startup/REMOTE_" + (i+1));
	    String[] fields = fullLine.split(":");
	    if (fields.length == 5)
	    {
		try
		{
		    addr = InetAddress.getByName(fields[0]);
		    remotes.setValueAt(addr, i, 0);
		    remotes.setValueAt(new Integer(fields[1]), i, 1);
		    remotes.setValueAt(fields[2], i, 2);
		    remotes.setValueAt(new Integer(fields[3]), i, 3);
		    remotes.setValueAt(new Integer(fields[4]), i, 4);
		}
		catch (Exception ex)
		{
		}
	    }
	}

	while (allows.getRowCount() > 1)
	    allows.setValueAt("", 0, 0);
	for (i=0; data.keyExists("Startup/ALLOW_" + (i+1)); i++)
	{
	    String fullLine = data.getValue("Startup/ALLOW_" + (i+1));
	    String[] fields = fullLine.split(":");
	    if (fields.length == 2)
	    {
		allows.setValueAt(fields[0], i, 0);
		allows.setValueAt(fields[1], i, 1);
	    }
	}

	String sel_protocol = data.getValue("Startup/PROTOCOL");
	for (i=0; i<protocol.getItemCount(); i++)
	{
	    String ri = ((String) protocol.getItemAt(i)).substring(0, 
			((String) protocol.getItemAt(i)).indexOf(" "));
	    if (ri.equalsIgnoreCase(sel_protocol))
	    {
		protocol.setSelectedItem(protocol.getItemAt(i));
		break;
	    }
	}	
	if (i == protocol.getItemCount())
	    protocol.setSelectedIndex(0);

	level.setText(data.getValue("Startup/LEVEL"));
	lagtime.setText(data.getValue("Startup/LAGTIME"));
	stop.setSelected(IniParser.getBoolValue(data.getValue("Startup/STOP")));
    }
}