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
//
// Author: Zafir Anjum
// Taken from: http://www.codeguru.com/java/articles/122.shtml

import javax.swing.*;
import javax.swing.plaf.*;

import java.awt.*;
import java.awt.font.*;
import java.awt.event.*;
import javax.swing.plaf.*;
import javax.swing.plaf.basic.BasicToolTipUI;
import javax.swing.text.*;


/**
 * @author Zafir Anjum
 */
public class JMultiLineToolTip extends JToolTip
{
	private static final String uiClassID = "ToolTipUI";
	
	String tipText;
	JComponent component;
	
	public JMultiLineToolTip() {
	    updateUI();
	}
	
	public void updateUI() {
	    setUI(MultiLineToolTipUI.createUI(this));
	}
	
	public void setColumns(int columns)
	{
		this.columns = columns;
		this.fixedwidth = 0;
	}
	
	public int getColumns()
	{
		return columns;
	}
	
	public void setFixedWidth(int width)
	{
		this.fixedwidth = width;
		this.columns = 0;
	}
	
	public int getFixedWidth()
	{
		return fixedwidth;
	}
	
	protected int columns = 0;
	protected int fixedwidth = 0;
}



class MultiLineToolTipUI extends BasicToolTipUI {
	static MultiLineToolTipUI sharedInstance = new MultiLineToolTipUI();
	Font smallFont; 			     
	static JToolTip tip;
	protected CellRendererPane rendererPane;
	
	private static JTextArea textArea ;
	
	public static ComponentUI createUI(JComponent c) {
	    return sharedInstance;
	}
	
	public MultiLineToolTipUI() {
	    super();
	}
	
	public void installUI(JComponent c) {
	    super.installUI(c);
		tip = (JToolTip)c;
	    rendererPane = new CellRendererPane();
	    c.add(rendererPane);
	}
	
	public void uninstallUI(JComponent c) {
		super.uninstallUI(c);
		
	    c.remove(rendererPane);
	    rendererPane = null;
	}
	
	public void paint(Graphics g, JComponent c) {
	    Dimension size = c.getSize();
	    textArea.setBackground(c.getBackground());
		rendererPane.paintComponent(g, textArea, c, 1, 1,
					    size.width - 1, size.height - 1, true);
	}
	
	public Dimension getPreferredSize(JComponent c) {
		String tipText = ((JToolTip)c).getTipText();
		if (tipText == null)
			return new Dimension(0,0);
		textArea = new JTextArea(tipText );
	    rendererPane.removeAll();
		rendererPane.add(textArea );
		textArea.setWrapStyleWord(true);
		int width = ((JMultiLineToolTip)c).getFixedWidth();
		int columns = ((JMultiLineToolTip)c).getColumns();
		
		if( columns > 0 )
		{
			textArea.setColumns(columns);
			textArea.setSize(0,0);
		textArea.setLineWrap(true);
			textArea.setSize( textArea.getPreferredSize() );
		}
		else if( width > 0 )
		{
		textArea.setLineWrap(true);
			Dimension d = textArea.getPreferredSize();
			d.width = width;
			d.height++;
			textArea.setSize(d);
		}
		else
			textArea.setLineWrap(false);


		Dimension dim = textArea.getPreferredSize();
		
		dim.height += 1;
		dim.width += 1;
		return dim;
	}
	
	public Dimension getMinimumSize(JComponent c) {
	    return getPreferredSize(c);
	}
	
	public Dimension getMaximumSize(JComponent c) {
	    return getPreferredSize(c);
	}
}


