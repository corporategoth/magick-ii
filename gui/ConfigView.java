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
import java.awt.*;
import java.awt.event.*;

public class ConfigView implements ActionListener
{
    private mct root;
    private JTextArea cfg_dump;
    private JButton update;

    public ConfigView(mct r)
    { 
	root = r;
	cfg_dump = new JTextArea("Press the Update button to generate current configuration.");
	cfg_dump.setEditable(false);
	cfg_dump.setMinimumSize(cfg_dump.getMaximumSize());
	cfg_dump.setPreferredSize(cfg_dump.getPreferredSize());
	update = new JButton("Update");
	update.addActionListener(this);
    }

    public void actionPerformed(ActionEvent e)
    {
	if (e.getSource() == update)
	{
	    cfg_dump.setText(root.getConfigData());
	    cfg_dump.setRows(cfg_dump.getLineCount());
	}
    }

    public JComponent createPane()
    {
	JPanel p = new JPanel();
	p.setLayout(new BorderLayout());
	p.add(new JScrollPane(cfg_dump), BorderLayout.CENTER);
	p.add(update, BorderLayout.SOUTH);

	return p;
    }
}
