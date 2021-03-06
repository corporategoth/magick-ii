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

import javax.swing.table.*;
import java.util.zip.DataFormatException;

public class NickServ extends TabbedPane
{
// private:
    private JComboBox append_rename, def_language;
    private JFormattedTextField expire, delay, ident, release,
	passfail, picsize;
    private JTextField suffixes, picext;
    private JCheckBox lck_language;
    private JLabel suffixes_label;

    private final String[] optnames = {
		"PROTECT",
		"SECURE",
		"NOEXPIRE",
		"NOMEMO",
		"PRIVATE",
		"PRIVMSG",
	};

    private class OptionsTableModel extends AbstractTableModel
	{
	    private Object[][] internal = {
		{ new String("Protect"), new Boolean(true), new Boolean(false) },
		{ new String("Secure"), new Boolean(false), new Boolean(false) },
		{ new String("No Expire"), new Boolean(false), new Boolean(false) },
		{ new String("No Memo"), new Boolean(false), new Boolean(false) },
		{ new String("Private"), new Boolean(false), new Boolean(false) },
		{ new String("PRIVMSG"), new Boolean(false), new Boolean(false) },
		// { new String("Language"), new String("ENGLISH"), new Boolean(false) },
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
		    t.setEditable(table.isCellEditable(row, column));
		    return t;
		}
	    }
	}

    private JTable options;

    public String name() { return "NickServ"; }

    public NickServ(ToolTips t)
    {
	super(t);

	suffixes_label = new JLabel(" ");
	suffixes = createTextField("SUFFIXES", 10, "+=^`", suffixes_label.getText().length() != 0);

	append_rename = createComboBox("APPEND_RENAME", false, true);
	append_rename.addItem(new String("Append"));
	append_rename.addItem(new String("Rename"));

	expire = createFormattedTextField("EXPIRE", 4, new TimeFormat(), "4w", true);
	delay = createFormattedTextField("DELAY", 4, new TimeFormat(), "30s", true);
	ident = createFormattedTextField("IDENT", 4, new TimeFormat(), "2m", true);
	release = createFormattedTextField("RELEASE", 4, new TimeFormat(), "1m", true);
	passfail = createFormattedTextField("PASSFAIL", 2, new NumberRangeFormat(0, -1), "5", true);
	picext = createTextField("PICEXT", 30, "", true);
	picsize = createFormattedTextField("PICSIZE", 4, new SpaceFormat(), "0b", picext.getText().length() != 0);

	def_language = createComboBox("DEF_LANGUAGE", true, true);
	def_language.addItem(new String("English"));
	def_language.addItem(new String("French"));
	def_language.addItem(new String("Portuguese"));
	def_language.addItem(new String("Swedish"));
	def_language.addItem(new String("Turkish"));

	lck_language = createCheckBox("LCK_LANGUAGE", false, true);

	OptionsTableModel optionsModel = new OptionsTableModel();
	OptionsTableCellRenderer optionsRenderer = new OptionsTableCellRenderer();
	options = createTable("OPTIONS", optionsModel, optionsRenderer, false);
    }

    public void documentChanged(DocumentEvent e)
    {
	try
	{
	    Document props = e.getDocument();
	    if (props.getProperty("name").equals("picext"))
	    {
		picsize.setEditable(picext.getText().length() != 0);
		picsize.setEnabled(picext.getText().length() != 0);
	    }
	}
	catch (Exception ex)
	{
	}
    }

    public void actionPerformed(ActionEvent e)
    {
	if (e.getSource() == append_rename)
	{
	    if (append_rename.getSelectedIndex() == 0)
	    {
		suffixes_label.setText("Suffixes");
		suffixes.setToolTipText(tips.getTip(name() + "/SUFFIXES"));
	    }
	    else if (append_rename.getSelectedIndex() == 1)
	    {
		suffixes_label.setText("Prefix");
		suffixes.setToolTipText(tips.getTip(name() + "/PREFIX"));
	    }
	    else
		suffixes_label.setText("");
	    suffixes.setEditable(suffixes_label.getText().length() != 0);
	    suffixes.setEnabled(suffixes_label.getText().length() != 0);
	}
    }

    public JComponent createPane()
    {
	JPanel gb = new JPanel();
	gb.setLayout(new GridBagLayout());
	GridBagConstraints gc = createStandardConstraints();

	addToGridBagLine(gb, gc, "", new JLabel(" "));
	addToGridBag(gb, gc, "Registration Delay", delay);
	addToGridBag(gb, gc, "Time To Identify", ident);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Expiration Time", expire);
	addToGridBag(gb, gc, "Time To Keep Nickname", release);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Maximum Password Failures", passfail);
	addToGridBag(gb, gc, "Nickname Action", append_rename);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Picture Size", picsize);
	gc.anchor = gc.EAST;
	addToGridBagField(gb, gc, suffixes_label);
	gc.anchor = gc.WEST;
	addToGridBagField(gb, gc, suffixes);
	addGridBagLine(gb, gc);
	addToGridBagLine(gb, gc, "Picture Extensions", picext);
	addGridBagLine(gb, gc);
	addToGridBagTable(gb, gc, "Default Options", options);
	addToGridBag(gb, gc, "Default Language", def_language);
	addToGridBag(gb, gc, "Locked", lck_language);
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

	rv += "[NickServ]\n";
	rv += "APPEND_RENAME = " + (append_rename.getSelectedIndex() == 0 ? "TRUE" : "FALSE") + "\n";
	rv += "SUFFIXES = " + suffixes.getText() + "\n";
	if (!isEditValid(expire)) throw new DataFormatException("NickServ/EXPIRE");
	rv += "EXPIRE = " + expire.getText() + "\n";
	if (!isEditValid(delay)) throw new DataFormatException("NickServ/DELAY");
	rv += "DELAY = " + delay.getText() + "\n";
	if (!isEditValid(ident)) throw new DataFormatException("NickServ/IDENT");
	rv += "IDENT = " + ident.getText() + "\n";
	if (!isEditValid(release)) throw new DataFormatException("NickServ/RELEASE");
	rv += "RELEASE = " + release.getText() + "\n";
	if (!isEditValid(passfail)) throw new DataFormatException("NickServ/PASSFAIL");
	rv += "PASSFAIL = " + passfail.getText() + "\n";

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
	String lang = new String((String) def_language.getSelectedItem());
	rv += "DEF_LANGUAGE = " + lang.toUpperCase() + "\n";
	rv += "LCK_LANGUAGE = " + (lck_language.isSelected() ? "TRUE" : "FALSE") + "\n";
	if (!isEditValid(picsize)) throw new DataFormatException("NickServ/PICSIZE");
	rv += "PICSIZE = " + picsize.getText() + "\n";
	rv += "PICEXT = " + picext.getText() + "\n";

	return rv;
    }

    public void parseCfg(IniParser data)
    {
	int i;

	if (IniParser.getBoolValue(data.getValue("NickServ/APPEND_RENAME")))
	    append_rename.setSelectedIndex(0);
	else
	    append_rename.setSelectedIndex(1);
	suffixes.setText(data.getValue("NickServ/SUFFIXES"));
	setFmtField(expire, data, "NickServ/EXPIRE");
	setFmtField(delay, data, "NickServ/DELAY");
	setFmtField(ident, data, "NickServ/IDENT");
	setFmtField(release, data, "NickServ/RELEASE");
	setFmtField(passfail, data, "NickServ/PASSFAIL");

	for (i=0; i<optnames.length; i++)
	{
	    if (data.keyExists("NickServ/DEF_" + optnames[i]))
		options.setValueAt(new Boolean(IniParser.getBoolValue(data.getValue("NickServ/DEF_" + optnames[i]))), i, 1);
	    if (data.keyExists("NickServ/LCK_" + optnames[i]))
		options.setValueAt(new Boolean(IniParser.getBoolValue(data.getValue("NickServ/LCK_" + optnames[i]))), i, 2);
	}

	String lang = data.getValue("NickServ/DEF_LANGUAGE");
	for (i=0; i<def_language.getItemCount(); i++)
	{
	    if (((String) def_language.getItemAt(i)).equalsIgnoreCase(lang))
	    {
		def_language.setSelectedItem(def_language.getItemAt(i));
		break;
	    }
	}	
	if (i == def_language.getItemCount())
	    def_language.setSelectedItem(lang);

	lck_language.setSelected(IniParser.getBoolValue(data.getValue("NickServ/LCK_LANGUAGE")));
	setFmtField(picsize, data, "NickServ/PICSIZE");
	picext.setText(data.getValue("NickServ/PICEXT"));
    }
}
