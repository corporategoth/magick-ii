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

public class ConfigView implements DocumentListener, ActionListener
{
    private mct root;
    private JTextArea cfg_dump;
    private JButton generate, load;
    private JScrollPane scrollpane;

    public ConfigView(mct r)
    { 
	root = r;
	cfg_dump = new JTextArea("# Press the Generate button to generate current configuration\n" +
				 "# or paste configuration to be read and press Load.");
	cfg_dump.setEditable(true);
	cfg_dump.setMinimumSize(cfg_dump.getMaximumSize());
	cfg_dump.setPreferredSize(cfg_dump.getPreferredSize());
	cfg_dump.getDocument().addDocumentListener(this);
	cfg_dump.getDocument().putProperty("name", "cfg_dump");
	generate = new JButton("Generate");
	generate.addActionListener(this);
	load = new JButton("Load");
	load.addActionListener(this);
    }

    public void actionPerformed(ActionEvent e)
    {
	if (e.getSource() == generate)
	{
	    cfg_dump.setText(root.getConfigData());
	    cfg_dump.setRows(cfg_dump.getLineCount());
	}
	else if (e.getSource() == load)
	{
	    root.setConfigData(cfg_dump.getText());
	}
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

    public void documentChanged(DocumentEvent e)
    {
	Document props = e.getDocument();
	if (props.getProperty("name").equals("cfg_dump"))
	{
	    cfg_dump.setRows(cfg_dump.getLineCount());
	}
    }

    public JComponent createPane()
    {
	JPanel p = new JPanel();
	p.setLayout(new BorderLayout());
	scrollpane = new JScrollPane(cfg_dump);
	p.add(scrollpane, BorderLayout.CENTER);

	JPanel gb = new JPanel();
	GridBagConstraints gc = new GridBagConstraints();
	gb.setLayout(new GridBagLayout());
	gc.gridx=0;
	gc.gridy=0;
	gc.weightx = 1;
	gc.weighty = 1;
	gc.fill = gc.HORIZONTAL;

	gc.anchor = gc.WEST;
	gb.add(generate, gc);

	gc.gridx++;
	gc.anchor = gc.EAST;
	gb.add(load, gc);

	p.add(gb, BorderLayout.SOUTH);

	return p;
    }
}
