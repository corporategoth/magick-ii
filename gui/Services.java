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

public class Services extends TabbedPane
{
// private:
    private JTextField nickserv, chanserv, memoserv, operserv, commserv, servmsg;
    private JTextField nickserv_name, chanserv_name, memoserv_name, operserv_name,
		commserv_name, servmsg_name, enforcer_name, quit_message;
    private JCheckBox showsync;

    public String name() { return "Services"; }

    public Services(ToolTips t)
    {
	super(t);

	nickserv = createTextField("NickServ", 20, "NickServ", true);
	chanserv = createTextField("ChanServ", 20, "ChanServ", true);
	memoserv = createTextField("MemoServ", 20, "MemoServ", true);
	operserv = createTextField("OperServ", 20, "OperServ Magick-1", true);
	commserv = createTextField("CommServ", 20, "CommServ", true);
	servmsg = createTextField("ServMsg", 20, "GlobalMSG HelpServ DevNull", true);

	nickserv_name = createTextField("NickServ_Name", 30, "Nickname Service", nickserv.getText().length() != 0);
	chanserv_name = createTextField("ChanServ_Name", 30, "Channel Service", chanserv.getText().length() != 0);
	memoserv_name = createTextField("MemoServ_Name", 30, "Memo/News Service", memoserv.getText().length() != 0);
	operserv_name = createTextField("OperServ_Name", 30, "Operator Service", operserv.getText().length() != 0);
	commserv_name = createTextField("CommServ_Name", 30, "Committee Service", commserv.getText().length() != 0);
	servmsg_name = createTextField("ServMsg_Name", 30, "Magick <--> User", servmsg.getText().length() != 0);
	enforcer_name = createTextField("Enforcer_Name", 30, "Nickname Enforcer", nickserv.getText().length() != 0);
	quit_message = createTextField("QUIT_MESSAGE", 30, "Goodbye cruel world ...", true);

	showsync = createCheckBox("SHOWSYNC", true, true);
    }

    public void documentChanged(DocumentEvent e)
    {
	Document props = e.getDocument();
        if (props.getProperty("name").equals("nickserv"))
	{
	    nickserv_name.setEditable(nickserv.getText().length() != 0);
	    nickserv_name.setEnabled(nickserv.getText().length() != 0);
	    enforcer_name.setEditable(nickserv.getText().length() != 0);
	    enforcer_name.setEnabled(nickserv.getText().length() != 0);
	}
	else if (props.getProperty("name").equals("chanserv"))
	{
	    chanserv_name.setEditable(chanserv.getText().length() != 0);
	    chanserv_name.setEnabled(chanserv.getText().length() != 0);
	}
	else if (props.getProperty("name").equals("memoserv"))
	{
	    memoserv_name.setEditable(memoserv.getText().length() != 0);
	    memoserv_name.setEnabled(memoserv.getText().length() != 0);
	}
	else if (props.getProperty("name").equals("operserv"))
	{
	    operserv_name.setEditable(operserv.getText().length() != 0);
	    operserv_name.setEnabled(operserv.getText().length() != 0);
	}
	else if (props.getProperty("name").equals("commserv"))
	{
	    commserv_name.setEditable(commserv.getText().length() != 0);
	    commserv_name.setEnabled(commserv.getText().length() != 0);
	}
	else if (props.getProperty("name").equals("servmsg"))
	{
	    servmsg_name.setEditable(servmsg.getText().length() != 0);
	    servmsg_name.setEnabled(servmsg.getText().length() != 0);
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
	addToGridBagLine(gb, gc, "NickServ nicknames", nickserv);
	addToGridBagLine(gb, gc, "NickServ real name", nickserv_name);
	addToGridBagLine(gb, gc, "ChanServ nicknames", chanserv);
	addToGridBagLine(gb, gc, "ChanServ real name", chanserv_name);
	addToGridBagLine(gb, gc, "MemoServ nicknames", memoserv);
	addToGridBagLine(gb, gc, "MemoServ real name", memoserv_name);
	addToGridBagLine(gb, gc, "OperServ nicknames", operserv);
	addToGridBagLine(gb, gc, "OperServ real name", operserv_name);
	addToGridBagLine(gb, gc, "CommServ nicknames", commserv);
	addToGridBagLine(gb, gc, "CommServ real name", commserv_name);
	addToGridBagLine(gb, gc, "ServMsg nicknames", servmsg);
	addToGridBagLine(gb, gc, "ServMsg real name", servmsg_name);
	addToGridBagLine(gb, gc, "Enforcer real name", enforcer_name);
	addToGridBagLine(gb, gc, "Quit message", quit_message);
	addToGridBagLine(gb, gc, "Show DB Sync time", showsync);
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

	rv += "[Services]\n";
	rv += "NickServ = " + nickserv.getText() + "\n";
	rv += "NickServ_Name = " + nickserv_name.getText() + "\n";
	rv += "Enforcer_Name = " + enforcer_name.getText() + "\n";
	rv += "ChanServ = " + chanserv.getText() + "\n";
	rv += "ChanServ_Name = " + chanserv_name.getText() + "\n";
	rv += "MemoServ = " + memoserv.getText() + "\n";
	rv += "MemoServ_Name = " + memoserv_name.getText() + "\n";
	rv += "OperServ = " + operserv.getText() + "\n";
	rv += "OperServ_Name = " + operserv_name.getText() + "\n";
	rv += "CommServ = " + commserv.getText() + "\n";
	rv += "CommServ_Name = " + commserv_name.getText() + "\n";
	rv += "ServMsg = " + servmsg.getText() + "\n";
	rv += "ServMsg_Name = " + servmsg_name.getText() + "\n";
	rv += "SHOWSYNC = " + (showsync.isSelected() ? "TRUE" : "FALSE") + "\n";
	rv += "QUIT_MESSAGE = " + quit_message.getText() + "\n";

	return rv;
    }

    public void parseCfg(IniParser data)
    {
	nickserv.setText(data.getValue("Services/NickServ"));
	nickserv_name.setText(data.getValue("Services/NickServ_Name"));
	enforcer_name.setText(data.getValue("Services/Enforcer_Name"));
	chanserv.setText(data.getValue("Services/ChanServ"));
	chanserv_name.setText(data.getValue("Services/ChanServ_Name"));
	memoserv.setText(data.getValue("Services/MemoServ"));
	memoserv_name.setText(data.getValue("Services/MemoServ_Name"));
	operserv.setText(data.getValue("Services/OperServ"));
	operserv_name.setText(data.getValue("Services/OperServ_Name"));
	commserv.setText(data.getValue("Services/CommServ"));
	commserv_name.setText(data.getValue("Services/CommServ_Name"));
	servmsg.setText(data.getValue("Services/ServMsg"));
	servmsg_name.setText(data.getValue("Services/ServMsg_Name"));
	showsync.setSelected(IniParser.getBoolValue(data.getValue("Services/SHOWSYNC")));
	quit_message.setText(data.getValue("Services/QUIT_MESSAGE"));
    }
}
