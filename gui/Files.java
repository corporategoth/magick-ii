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

public class Files extends TabbedPane
{
// private:
    private JFormattedTextField umask, picture_size, memoattach_size,
		public_size, tempdir_size, blocksize, timeout,
		min_speed, max_speed, sampletime;
    private JTextField pidfile, logfile, logchan, motdfile, langdir,
		database, keyfile, picture, memoattach, f_public, tempdir;
    private JCheckBox verbose, encryption;
    private JComboBox compression;
    private int def_compression;

    public String name() { return "Files"; }

    public Files(ToolTips t)
    {
	super(t);

	verbose = createCheckBox("VERBOSE", false, true);
	encryption = createCheckBox("ENCRYPTION", false, true);

	umask = createFormattedTextField("UMASK", 4, new OctalFormat(), "027", true);
	pidfile = createTextField("PIDFILE", 30, "magick.pid", true);
	logfile = createTextField("LOGFILE", 30, "magick.log", true);
	logchan = createTextField("LOGCHAN", 10, "", true);
	motdfile = createTextField("MOTDFILE", 30, "magick.motd", true);
	langdir = createTextField("LANGDIR", 20, "lang", true);
	database = createTextField("DATABASE", 30, "magick.mnd", true);
	keyfile = createTextField("KEYFILE", 30, "magick.key", encryption.isSelected());
	picture_size = createFormattedTextField("PICTURE_SIZE", 4, new SpaceFormat(), "0b", true);
	picture = createTextField("PICTURE", 20, "files/pic", true);
	memoattach_size = createFormattedTextField("MEMOATTACH_SIZE", 4, new SpaceFormat(), "0b", true);
	memoattach = createTextField("MEMOATTACH", 20, "files/memo", true);
	public_size = createFormattedTextField("PUBLIC_SIZE", 4, new SpaceFormat(), "0b", true);
	f_public = createTextField("PUBLIC", 20, "files/public", true);
	tempdir_size = createFormattedTextField("TEMPDIR_SIZE", 4, new SpaceFormat(), "0b", true);
	tempdir = createTextField("TEMPDIR", 20, "files/temp", true);
	blocksize = createFormattedTextField("BLOCKSIZE", 4, new SpaceFormat(), "1k", true);
	timeout = createFormattedTextField("TIMEOUT", 4, new TimeFormat(), "2m", true);
	min_speed = createFormattedTextField("MIN_SPEED", 4, new SpaceFormat(), "0b", true);
	max_speed = createFormattedTextField("MAX_SPEED", 4, new SpaceFormat(), "0b", true);
	sampletime = createFormattedTextField("SAMPLETIME", 4, new TimeFormat(), "30s", true);

	compression = createComboBox("COMPRESSION", false, true);
	def_compression = 6;
	compression.addItem(new String("0 - Disabled"));
	compression.addItem(new String("1"));
	compression.addItem(new String("2"));
	compression.addItem(new String("3"));
	compression.addItem(new String("4"));
	compression.addItem(new String("5"));
	compression.addItem(new String("6 - Default"));
	compression.addItem(new String("7"));
	compression.addItem(new String("8"));
	compression.addItem(new String("9 - Maximum"));
	compression.setSelectedIndex(def_compression);
    }

    public void documentChanged(DocumentEvent e)
    {
	Document props = e.getDocument();
    }

    public void actionPerformed(ActionEvent e)
    {
	if (e.getSource() == encryption)
	{
	    keyfile.setEditable(encryption.isSelected());
	    keyfile.setEnabled(encryption.isSelected());
	}
    }

    public JComponent createPane()
    {
	JPanel gb = new JPanel();
	gb.setLayout(new GridBagLayout());
	GridBagConstraints gc = createStandardConstraints();

	addToGridBagLine(gb, gc, "", new JLabel(" "));
	addToGridBagLine(gb, gc, "File UMASK", umask);
	addToGridBagLine(gb, gc, "Log File", logfile);
	addToGridBag(gb, gc, "Log Channel", logchan);
	addToGridBag(gb, gc, "Verbose", verbose);
	addGridBagLine(gb, gc);
	addToGridBagLine(gb, gc, "Database File", database);
	addToGridBag(gb, gc, "Compression Level", compression);
	addToGridBag(gb, gc, "Encryption", encryption);
	addGridBagLine(gb, gc);
	addToGridBagLine(gb, gc, "Key File", keyfile);
	addToGridBagLine(gb, gc, "MOTD File", motdfile);
	addToGridBagLine(gb, gc, "PID File", pidfile);

	addToGridBagLine(gb, gc, "Language Dir", langdir);
	addToGridBag(gb, gc, "Picture Dir", picture);
	addToGridBag(gb, gc, "Size", picture_size);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Memo Attach Dir", memoattach);
	addToGridBag(gb, gc, "Size", memoattach_size);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Public FS Dir", f_public);
	addToGridBag(gb, gc, "Size", public_size);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Temp Dir", tempdir);
	addToGridBag(gb, gc, "Size", tempdir_size);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Block Size", blocksize);
	addToGridBag(gb, gc, "Min Speed", min_speed);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Timeout", timeout);
	addToGridBag(gb, gc, "Max Speed", max_speed);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Sample Time", sampletime);
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

	rv += "[Files]\n";
	rv += "UMASK = " + umask.getText() + "\n";
	rv += "PIDFILE = " + pidfile.getText() + "\n";
	rv += "LOGFILE = " + logfile.getText() + "\n";
	rv += "VERBOSE = " + (verbose.isSelected() ? "TRUE" : "FALSE") + "\n";
	rv += "LOGCHAN = " + logchan.getText() + "\n";
	rv += "MOTDFILE = " + motdfile.getText() + "\n";
	rv += "LANGDIR = " + langdir.getText() + "\n";
	rv += "DATABASE = " + database.getText() + "\n";
	rv += "COMPRESSION = " + compression.getSelectedIndex() + "\n";
	rv += "KEYFILE = " + keyfile.getText() + "\n";
	rv += "ENCRYPTION = " + (encryption.isSelected() ? "TRUE" : "FALSE") + "\n";
	rv += "PICTURE = " + picture.getText() + "\n";
	rv += "MEMOATTACH = " + memoattach.getText() + "\n";
	rv += "PUBLIC = " + f_public.getText() + "\n";
	rv += "TEMPDIR = " + tempdir.getText() + "\n";
	rv += "PICTURE_SIZE = " + picture_size.getText() + "\n";
	rv += "MEMOATTACH_SIZE = " + memoattach_size.getText() + "\n";
	rv += "PUBLIC_SIZE = " + public_size.getText() + "\n";
	rv += "TEMPDIR_SIZE = " + tempdir_size.getText() + "\n";
	rv += "BLOCKSIZE = " + blocksize.getText() + "\n";
	rv += "TIMEOUT = " + timeout.getText() + "\n";
	rv += "MIN_SPEED = " + min_speed.getText() + "\n";
	rv += "MAX_SPEED = " + max_speed.getText() + "\n";
	rv += "SAMPLETIME = " + sampletime.getText() + "\n";

	return rv;
    }

    public void parseCfg(IniParser data)
    {
	int i;

	umask.setText(data.getValue("Files/UMASK"));
	pidfile.setText(data.getValue("Files/PIDFILE"));
	logfile.setText(data.getValue("Files/LOGFILE"));
	verbose.setSelected(IniParser.getBoolValue(data.getValue("Files/VERBOSE")));
	logchan.setText(data.getValue("Files/LOGCHAN"));
	motdfile.setText(data.getValue("Files/MOTDFILE"));
	langdir.setText(data.getValue("Files/LANGDIR"));
	database.setText(data.getValue("Files/DATABASE"));

	String sel_compression = data.getValue("Files/COMPRESSION");
	for (i=0; i<compression.getItemCount(); i++)
	{
	    int idx = ((String) compression.getItemAt(i)).indexOf(" ");
	    String ri;
	    if (idx < 0)
		ri = (String) compression.getItemAt(i);
	    else
		ri = ((String) compression.getItemAt(i)).substring(0, idx);
	    if (ri.equalsIgnoreCase(sel_compression))
	    {
		compression.setSelectedItem(compression.getItemAt(i));
		break;
	    }
	}	
	if (i == compression.getItemCount())
	    compression.setSelectedIndex(def_compression);

	keyfile.setText(data.getValue("Files/KEYFILE"));
	encryption.setSelected(IniParser.getBoolValue(data.getValue("Files/ENCRYPTION")));
	picture.setText(data.getValue("Files/PICTURE"));
	memoattach.setText(data.getValue("Files/MEMOATTACH"));
	f_public.setText(data.getValue("Files/PUBLIC"));
	tempdir.setText(data.getValue("Files/TEMPDIR"));
	picture_size.setText(data.getValue("Files/PICTURE_SIZE"));
	memoattach_size.setText(data.getValue("Files/MEMOATTACH_SIZE"));
	public_size.setText(data.getValue("Files/PUBLIC_SIZE"));
	tempdir_size.setText(data.getValue("Files/TEMPDIR_SIZE"));
	blocksize.setText(data.getValue("Files/BLOCKSIZE"));
	timeout.setText(data.getValue("Files/TIMEOUT"));
	min_speed.setText(data.getValue("Files/MIN_SPEED"));
	max_speed.setText(data.getValue("Files/MAX_SPEED"));
	sampletime.setText(data.getValue("Files/SAMPLETIME"));
    }
}
