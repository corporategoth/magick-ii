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
import javax.swing.table.*;
import java.awt.*;
import java.awt.event.*;


public class myTableCellEditor extends AbstractCellEditor implements TableCellEditor
{
    private Component editor = null;

    public myTableCellEditor()
    {
	super();
    }

    public Component getComponent()
    {
	return editor;
    }

    public Object getCellEditorValue()
    {
	Object value = null;
	if (editor instanceof JFormattedTextField)
	    value = ((JFormattedTextField) editor).getValue();
	else if (editor instanceof JTextField)
	    value = ((JTextField) editor).getText();
	else if (editor instanceof JCheckBox)
	    value = new Boolean(((JCheckBox) editor).isSelected());
	return value;
    }

    public boolean stopCellEditing()
    {
	boolean rv = true;
	if (editor != null)
	{
	    if (editor instanceof JFormattedTextField)
	    {
		try
		{
		    ((JFormattedTextField) editor).commitEdit();
		}
		catch (Exception ex)
		{
		    rv = false;
		}
	    }
	}
	fireEditingStopped();
	return rv;
    }  

    public void  cancelCellEditing()
    {
	fireEditingCanceled(); 
    }


    public Component getTableCellEditorComponent(JTable table, Object value,
	boolean isSelected, int row, int column)
    {
	editor = table.getCellRenderer(row, column).getTableCellRendererComponent(table, value, isSelected, true, row, column);
	setValue(value);
	return editor;
    }

    private void setValue(Object value)
    {
	if (editor instanceof JFormattedTextField)
	    ((JFormattedTextField) editor).setValue(value);
	else if (editor instanceof JTextField)
	    ((JTextField) editor).setText(value.toString());
	else if (editor instanceof JCheckBox)
	    if (value instanceof Boolean)
		((JCheckBox) editor).setSelected(((Boolean) value).booleanValue());
	    else if (value instanceof String)
		((JCheckBox) editor).setSelected(((String) value).equals("true") ? true : false);
    }

}


