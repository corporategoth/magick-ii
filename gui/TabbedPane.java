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
import java.util.*;
import java.util.zip.DataFormatException;
import java.text.ParseException;
import java.net.*;

abstract public class TabbedPane implements DocumentListener, ActionListener
{
    protected ToolTips tips;
    private myTableCellEditor tableEditor = null;

    public TabbedPane(ToolTips t)
    {
	tips = t;
    }

    public void insertUpdate(DocumentEvent e)
    {
	documentChanged(e);
    }
    public void removeUpdate(DocumentEvent e)
    {
	documentChanged(e);
    }
    public void changedUpdate(DocumentEvent e)
    {
	documentChanged(e);
    }

    abstract public String name();
    abstract public void documentChanged(DocumentEvent e);
    abstract public void actionPerformed(ActionEvent e);
    abstract public JComponent createPane();
    abstract public String createCfg() throws DataFormatException;
    abstract public void parseCfg(IniParser data);

    protected boolean isNonZero(String in)
    {
	if (in.length() == 0)
	    return false;
	try
	{
	    return (Integer.parseInt(in) != 0);
	}
	catch (NumberFormatException e)
	{
	    return false;
	}
    }

    protected boolean isNonZero(JFormattedTextField in)
    {
	if (in.getValue() == null)
	    return false;
	else if (in.getValue() instanceof String)
	{
	    return (in.getText().length() != 0);
	}
	else if (in.getValue() instanceof Integer)
	{
	    return (((Integer) in.getValue()).intValue() != 0);
	}
	else if (in.getValue() instanceof Float)
	{
	    return (((Float) in.getValue()).floatValue() != 0.0);
	}
	else
	{
	    return isNonZero(in.getText());
	}
    }

    protected JButton createButton(String tooltip, String text, boolean enabled)
    {
	JButton rv = new JButton(text)
		{
		    public JToolTip createToolTip()
		    {
			return new JMultiLineToolTip();
		    }
		};
	String tip = tips.getTip(name() + "/" + tooltip);
	if (tip.length() != 0)
	    rv.setToolTipText(tip);
	rv.addActionListener(this);
	rv.setEnabled(enabled);
	Insets i = rv.getMargin();
	i.left = i.right = i.bottom = i.top;
	rv.setMargin(i);
	return rv;
    }


    protected JTextField createTextField(String tooltip, int size, String def, boolean editable)
    {
	JTextField rv = new JTextField(size)
		{
		    public JToolTip createToolTip()
		    {
			return new JMultiLineToolTip();
		    }
		};
	String tip = tips.getTip(name() + "/" + tooltip);
	if (tip.length() != 0)
	    rv.setToolTipText(tip);
	if (def.length() != 0)
	    rv.setText(def);
	rv.addActionListener(this);
	rv.getDocument().addDocumentListener(this);
	rv.getDocument().putProperty("name", tooltip.toLowerCase());
	rv.setEditable(editable);
	rv.setEnabled(editable);
	return rv;
    }


    protected JFormattedTextField createFormattedTextField(String tooltip, int size, JFormattedTextField.AbstractFormatter f, String def, boolean editable)
    {
	JFormattedTextField rv = new JFormattedTextField(f)
		{
		    public JToolTip createToolTip()
		    {
			return new JMultiLineToolTip();
		    }
		};
	rv.setColumns(size);
	rv.setFocusLostBehavior(JFormattedTextField.COMMIT);
	String tip = tips.getTip(name() + "/" + tooltip);
	if (tip.length() != 0)
	    rv.setToolTipText(tip);
	if (def.length() != 0)
	    rv.setText(def);
	rv.addActionListener(this);
	rv.getDocument().addDocumentListener(this);
	rv.getDocument().putProperty("name", tooltip.toLowerCase());
	rv.setEditable(editable);
	rv.setEnabled(editable);
	return rv;
    }

    protected JCheckBox createCheckBox(String tooltip, boolean def, boolean enabled)
    {
	JCheckBox rv = new JCheckBox("", def)
		{
		    public JToolTip createToolTip()
		    {
			return new JMultiLineToolTip();
		    }
		};
	String tip = tips.getTip(name() + "/" + tooltip);
	if (tip.length() != 0)
	    rv.setToolTipText(tip);
	rv.addActionListener(this);
	rv.setEnabled(enabled);
	return rv;
    }

    protected JComboBox createComboBox(String tooltip, boolean editable, boolean enabled)
    {
	JComboBox rv = new JComboBox()
		{
		    public JToolTip createToolTip()
		    {
			return new JMultiLineToolTip();
		    }
		};
	String tip = tips.getTip(name() + "/" + tooltip);
	if (tip.length() != 0)
	    rv.setToolTipText(tip);
	rv.addActionListener(this);
	rv.setEditable(editable);
	rv.setEnabled(enabled);
	return rv;
    }

    protected JTable createTable(String tooltip, TableModel tm, TableCellRenderer tr, boolean rs)
    {
	JTable rv = new JTable(tm)
		{
		    public JToolTip createToolTip()
		    {
			return new JMultiLineToolTip();
		    }
		};
	String tip = tips.getTip(name() + "/" + tooltip);
	if (tip.length() != 0)
	    rv.setToolTipText(tip);
	rv.setCellSelectionEnabled(true);
	rv.setRowSelectionAllowed(rs);
	rv.setColumnSelectionAllowed(false);
	rv.setAutoResizeMode(rv.AUTO_RESIZE_ALL_COLUMNS);
	rv.setDefaultRenderer(String.class, tr);
	rv.setDefaultRenderer(Integer.class, tr);
	rv.setDefaultRenderer(InetAddress.class, tr);
	rv.setDefaultRenderer(Boolean.class, tr);
	if (tableEditor == null)
	    tableEditor = new myTableCellEditor();
	rv.setDefaultEditor(String.class, tableEditor);
	rv.setDefaultEditor(Integer.class, tableEditor);
	rv.setDefaultEditor(InetAddress.class, tableEditor);
	rv.setDefaultEditor(Boolean.class, tableEditor);

	TableColumnModel cm = rv.getColumnModel();
	Enumeration e = cm.getColumns();
	while (e.hasMoreElements())
	{
	    TableColumn c = (TableColumn) e.nextElement();
	    c.sizeWidthToFit();
	}
	rv.doLayout();
	return rv;
    }

    public GridBagConstraints createStandardConstraints()
    {
	GridBagConstraints gc = new GridBagConstraints();
	gc.gridx=0;
	gc.gridy=0;
	gc.weightx=1;
	gc.insets = new Insets(0, 5, 0, 5);
	gc.anchor = gc.EAST;
	return gc;
    }

    protected void addToGridBagTable(JPanel p, GridBagConstraints gc, String name, JTable t)
    {
	int oldalign = gc.anchor;
	addToGridBagLabel(p, gc, name);

	gc.gridwidth = gc.REMAINDER;
	gc.fill = gc.HORIZONTAL;
	t.setSize(t.getPreferredSize());
	p.add(new JTableHeader(t.getColumnModel()), gc);
	gc.gridy++;
	p.add(t, gc);
	gc.gridwidth = 1;
	gc.fill = gc.NONE;
	gc.anchor = oldalign;
	addGridBagLine(p, gc);
    }

    protected void addToGridBagField(JPanel p, GridBagConstraints gc, JComponent c)
    {
	p.add(c, gc);
	gc.gridx++;
    }

    protected void addToGridBagAligned(JPanel p, GridBagConstraints gc, JComponent c, int align)
    {
	int oldalign = gc.anchor;
	gc.anchor = align;
	p.add(c, gc);
	gc.gridx++;
	gc.anchor = oldalign;
    }

    protected void addToGridBagLabel(JPanel p, GridBagConstraints gc, String name)
    {
	if (gc.anchor == gc.WEST)
	    gc.anchor = gc.EAST;
	addToGridBagField(p, gc, new JLabel(name));
	gc.anchor = gc.WEST;
    }

    protected void addToGridBagLabel(JPanel p, GridBagConstraints gc, String name, Color col)
    {
	if (gc.anchor == gc.WEST)
	    gc.anchor = gc.EAST;
	JLabel l = new JLabel(name);
	l.setForeground(col);
	addToGridBagField(p, gc, l);
	gc.anchor = gc.WEST;
    }

    protected void addToGridBag(JPanel p, GridBagConstraints gc, String name, Color col, JComponent c)
    {
	addToGridBagLabel(p, gc, name, col);
	addToGridBagField(p, gc, c);
    }

    protected void addToGridBag(JPanel p, GridBagConstraints gc, String name, JComponent c)
    {
	addToGridBagLabel(p, gc, name);
	addToGridBagField(p, gc, c);
    }

    protected void addGridBagLine(JPanel p, GridBagConstraints gc)
    {
	gc.gridx=0;
	gc.gridy++;
    }

    protected void addToGridBagLine(JPanel p, GridBagConstraints gc, String name, JComponent c)
    {
	gc.gridwidth = 1;
	addToGridBagLabel(p, gc, name);
	gc.gridwidth = gc.REMAINDER;
	addToGridBagField(p, gc, c);
	gc.gridwidth = 1;
	addGridBagLine(p, gc);
    }

    protected void addToGridBagButton(JPanel p, GridBagConstraints gc, String name, Color col, JComponent c, JButton b)
    {
	addToGridBagLabel(p, gc, name, col);
	Box bx = Box.createHorizontalBox();
	bx.add(c);
	bx.add(b);
	addToGridBagField(p, gc, bx);
    }

    protected void addToGridBagButton(JPanel p, GridBagConstraints gc, String name, JComponent c, JButton b)
    {
	addToGridBagLabel(p, gc, name);
	Box bx = Box.createHorizontalBox();
	bx.add(c);
	bx.add(b);
	addToGridBagField(p, gc, bx);
    }

    protected void addToGridBagButtonLine(JPanel p, GridBagConstraints gc, String name, Color col, JComponent c, JButton b)
    {
	addToGridBagLabel(p, gc, name, col);
	Box bx = Box.createHorizontalBox();
	bx.add(c);
	bx.add(b);
	gc.gridwidth = gc.REMAINDER;
	addToGridBagField(p, gc, bx);
	gc.gridwidth = 1;
	addGridBagLine(p, gc);
    }

    protected void addToGridBagButtonLine(JPanel p, GridBagConstraints gc, String name, JComponent c, JButton b)
    {
	addToGridBagLabel(p, gc, name);
	Box bx = Box.createHorizontalBox();
	bx.add(c);
	bx.add(b);
	gc.gridwidth = gc.REMAINDER;
	addToGridBagField(p, gc, bx);
	gc.gridwidth = 1;
	addGridBagLine(p, gc);
    }

    protected boolean isEditValid(JFormattedTextField f)
    {
	try
	{
	    f.getFormatter().stringToValue(f.getText());
	    return true;
	}
	catch (ParseException e)
	{
	    return false;
	}
    }

    protected void setFmtField(JFormattedTextField f, IniParser data, String token)
    {
	String val = data.getValue(token);
	try {
	    Object o = f.getFormatter().stringToValue(val);
	    f.setValue(o);
	    f.commitEdit();
	}
	catch (Exception e)
	{
	    f.setText(val);
	    f.setBorder(BorderFactory.createLineBorder(Color.RED));
	}
    }
}

