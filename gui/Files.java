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

import javax.swing.filechooser.*;
import java.util.zip.DataFormatException;
import java.io.File;

public class Files extends TabbedPane
{
// private:
    private JFormattedTextField umask, picture_size, memoattach_size,
		public_size, tempdir_size, blocksize, timeout,
		min_speed, max_speed, sampletime;
    private JTextField pidfile, protocol, logfile, logchan, motdfile, langdir,
		database, keyfile, picture, memoattach, f_public, tempdir;
    private JCheckBox verbose, encryption;
    private JComboBox compression;
    private int def_compression;
    private JButton b_logfile, b_pidfile, b_motdfile, b_keyfile, b_database,
		b_protocol, b_langdir, b_picture, b_memoattach, b_public,
		b_tempdir;

    public String name() { return "Files"; }

    public Files(ToolTips t)
    {
	super(t);

	verbose = createCheckBox("VERBOSE", false, true);
	encryption = createCheckBox("ENCRYPTION", false, true);

	umask = createFormattedTextField("UMASK", 4, new OctalFormat(), "027", true);
	protocol = createTextField("PROTOCOL", 30, "ircd" + File.separator + "ircd.rfc1459.ini", true);
	pidfile = createTextField("PIDFILE", 30, "magick.pid", true);
	logfile = createTextField("LOGFILE", 30, "magick.log", true);
	logchan = createTextField("LOGCHAN", 10, "", true);
	motdfile = createTextField("MOTDFILE", 30, "magick.motd", true);
	langdir = createTextField("LANGDIR", 20, "lang", true);
	database = createTextField("DATABASE", 30, "magick.mnd", true);
	keyfile = createTextField("KEYFILE", 30, "magick.key", encryption.isSelected());
	picture_size = createFormattedTextField("PICTURE_SIZE", 4, new SpaceFormat(), "0b", true);
	picture = createTextField("PICTURE", 20, "files" + File.separator + "pic", true);
	memoattach_size = createFormattedTextField("MEMOATTACH_SIZE", 4, new SpaceFormat(), "0b", true);
	memoattach = createTextField("MEMOATTACH", 20, "files" + File.separator + "memo", true);
	public_size = createFormattedTextField("PUBLIC_SIZE", 4, new SpaceFormat(), "0b", true);
	f_public = createTextField("PUBLIC", 20, "files" + File.separator + "public", true);
	tempdir_size = createFormattedTextField("TEMPDIR_SIZE", 4, new SpaceFormat(), "0b", true);
	tempdir = createTextField("TEMPDIR", 20, "files" + File.separator + "temp", true);
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

	if (mct.currentDirectory() != null)
	{
	    b_logfile = createButton("B_LOGFILE", "...", true);
	    b_logfile.setToolTipText("Browse ...");
	    b_database = createButton("B_DATABASE", "...", true);
	    b_database.setToolTipText("Browse ...");
	    b_keyfile = createButton("B_KEYFILE", "...", encryption.isSelected());
	    b_keyfile.setToolTipText("Browse ...");
	    b_motdfile = createButton("B_MOTDFILE", "...", true);
	    b_motdfile.setToolTipText("Browse ...");
	    b_protocol = createButton("B_PROTOCOL", "...", true);
	    b_protocol.setToolTipText("Browse ...");
	    b_pidfile = createButton("B_PIDFILE", "...", true);
	    b_pidfile.setToolTipText("Browse ...");
	    b_langdir = createButton("B_LANGDIR", "...", true);
	    b_langdir.setToolTipText("Browse ...");
	    b_picture = createButton("B_PICTURE", "...", true);
	    b_picture.setToolTipText("Browse ...");
	    b_memoattach = createButton("B_MEMOATTACH", "...", true);
	    b_memoattach.setToolTipText("Browse ...");
	    b_public = createButton("B_PUBLIC", "...", true);
	    b_public.setToolTipText("Browse ...");
	    b_tempdir = createButton("B_TEMPDIR", "...", true);
	    b_tempdir.setToolTipText("Browse ...");
	}
    }

    public void documentChanged(DocumentEvent e)
    {
	try
	{
	    Document props = e.getDocument();
	}
	catch (Exception ex)
	{
	}
    }

    public void actionPerformed(ActionEvent e)
    {
	if (e.getSource() == encryption)
	{
	    keyfile.setEditable(encryption.isSelected());
	    keyfile.setEnabled(encryption.isSelected());
	    b_keyfile.setEnabled(encryption.isSelected());
	}
	else if (e.getSource() == b_logfile)
	{
	    String rv = mct.showFileDialog(logfile.getText(), "Open", "Log Files (*.log)", ".log", false);
	    if (rv != null)
		logfile.setText(rv);
	}
	else if (e.getSource() == b_motdfile)
	{
	    String rv = mct.showFileDialog(motdfile.getText(), "Open", "MOTD Files (*.motd)", ".motd", false);
	    if (rv != null)
		motdfile.setText(rv);
	}
	else if (e.getSource() == b_database)
	{
	    String rv = mct.showFileDialog(database.getText(), "Open", "Magick New Database Files (*.mnd)", ".mnd", false);
	    if (rv != null)
		database.setText(rv);
	}
	else if (e.getSource() == b_keyfile)
	{
	    String rv = mct.showFileDialog(keyfile.getText(), "Open", "Key Files (*.key)", ".key", false);
	    if (rv != null)
		keyfile.setText(rv);
	}
	else if (e.getSource() == b_protocol)
	{
	    String rv = mct.showFileDialog(protocol.getText(), "Open", "Configuration Files (*.ini)", ".ini", true);
	    if (rv != null)
		protocol.setText(rv);
	}
	else if (e.getSource() == b_pidfile)
	{
	    String rv = mct.showFileDialog(pidfile.getText(), "Open", "Process ID Files (*.pid)", ".pid", false);
	    if (rv != null)
		pidfile.setText(rv);
	}
	else if (e.getSource() == b_langdir)
	{
	    String rv = mct.showDirectoryDialog(langdir.getText(), "Select", null, null, true);
	    if (rv != null)
		langdir.setText(rv);
	}
	else if (e.getSource() == b_picture)
	{
	    String rv = mct.showDirectoryDialog(picture.getText(), "Select", null, null, true);
	    if (rv != null)
		picture.setText(rv);
	}
	else if (e.getSource() == b_memoattach)
	{
	    String rv = mct.showDirectoryDialog(memoattach.getText(), "Select", null, null, true);
	    if (rv != null)
		memoattach.setText(rv);
	}
	else if (e.getSource() == b_public)
	{
	    String rv = mct.showDirectoryDialog(f_public.getText(), "Select", null, null, true);
	    if (rv != null)
		f_public.setText(rv);
	}
	else if (e.getSource() == b_tempdir)
	{
	    String rv = mct.showDirectoryDialog(tempdir.getText(), "Select", null, null, true);
	    if (rv != null)
		tempdir.setText(rv);
	}
    }

    public JComponent createPane()
    {
	JPanel gb = new JPanel();
	gb.setLayout(new GridBagLayout());
	GridBagConstraints gc = createStandardConstraints();

	addToGridBagLine(gb, gc, "", new JLabel(" "));
	addToGridBagLine(gb, gc, "File UMASK", umask);
	if (mct.currentDirectory() != null)
	    addToGridBagButtonLine(gb, gc, "Log File", logfile, b_logfile);
	else
	    addToGridBagLine(gb, gc, "Log File", logfile);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Log Channel", logchan);
	addToGridBag(gb, gc, "Verbose", verbose);
	addGridBagLine(gb, gc);
	if (mct.currentDirectory() != null)
	    addToGridBagButtonLine(gb, gc, "Database File", database, b_database);
	else
	    addToGridBagLine(gb, gc, "Database File", database);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Compression Level", compression);
	addToGridBag(gb, gc, "Encryption", encryption);
	addGridBagLine(gb, gc);
	if (mct.currentDirectory() != null)
	    addToGridBagButtonLine(gb, gc, "Key File", keyfile, b_keyfile);
	else
	    addToGridBagLine(gb, gc, "Key File", keyfile);
	addGridBagLine(gb, gc);
	if (mct.currentDirectory() != null)
	    addToGridBagButtonLine(gb, gc, "MOTD File", motdfile, b_motdfile);
	else
	    addToGridBagLine(gb, gc, "MOTD File", motdfile);
	addGridBagLine(gb, gc);
	if (mct.currentDirectory() != null)
	    addToGridBagButtonLine(gb, gc, "Protocol Definition", protocol, b_protocol);
	else
	    addToGridBagLine(gb, gc, "Protocol Definition", protocol);
	addGridBagLine(gb, gc);
	if (mct.currentDirectory() != null)
	    addToGridBagButtonLine(gb, gc, "PID File", pidfile, b_pidfile);
	else
	    addToGridBagLine(gb, gc, "PID File", pidfile);
	addGridBagLine(gb, gc);

	if (mct.currentDirectory() != null)
	    addToGridBagButtonLine(gb, gc, "Language Directory", langdir, b_langdir);
	else
	    addToGridBagLine(gb, gc, "Language Directory", langdir);
	addGridBagLine(gb, gc);
	if (mct.currentDirectory() != null)
	    addToGridBagButton(gb, gc, "Picture Directory", picture, b_picture);
	else
	    addToGridBag(gb, gc, "Picture Directory", picture);
	addToGridBag(gb, gc, "Size", picture_size);
	addGridBagLine(gb, gc);
	if (mct.currentDirectory() != null)
	    addToGridBagButton(gb, gc, "Memo Attach Directory", memoattach, b_memoattach);
	else
	    addToGridBag(gb, gc, "Memo Attach Directory", memoattach);
	addToGridBag(gb, gc, "Size", memoattach_size);
	addGridBagLine(gb, gc);
	if (mct.currentDirectory() != null)
	    addToGridBagButton(gb, gc, "Public Filesystem Directory", f_public, b_public);
	else
	    addToGridBag(gb, gc, "Public FS Directory", f_public);
	addToGridBag(gb, gc, "Size", public_size);
	addGridBagLine(gb, gc);
	if (mct.currentDirectory() != null)
	    addToGridBagButton(gb, gc, "Temp Directory", tempdir, b_tempdir);
	else
	    addToGridBag(gb, gc, "Temp Directory", tempdir);
	addToGridBag(gb, gc, "Size", tempdir_size);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Block Size", blocksize);
	addToGridBag(gb, gc, "Minimum Speed", min_speed);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Timeout", timeout);
	addToGridBag(gb, gc, "Maximum Speed", max_speed);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Sample Time", sampletime);
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

	rv += "[Files]\n";
	if (!isEditValid(umask)) throw new DataFormatException("Files/UMASK");
	rv += "UMASK = " + umask.getText() + "\n";
	rv += "PROTOCOL = " + protocol.getText() + "\n";
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
	if (!isEditValid(picture_size)) throw new DataFormatException("Files/PICTURE_SIZE");
	rv += "PICTURE_SIZE = " + picture_size.getText() + "\n";
	if (!isEditValid(memoattach_size)) throw new DataFormatException("Files/MEMOATTACH_SIZE");
	rv += "MEMOATTACH_SIZE = " + memoattach_size.getText() + "\n";
	if (!isEditValid(public_size)) throw new DataFormatException("Files/PUBLIC_SIZE");
	rv += "PUBLIC_SIZE = " + public_size.getText() + "\n";
	if (!isEditValid(tempdir_size)) throw new DataFormatException("Files/TEMPDIR_SIZE");
	rv += "TEMPDIR_SIZE = " + tempdir_size.getText() + "\n";
	if (!isEditValid(blocksize)) throw new DataFormatException("Files/BLOCKSIZE");
	rv += "BLOCKSIZE = " + blocksize.getText() + "\n";
	if (!isEditValid(timeout)) throw new DataFormatException("Files/TIMEOUT");
	rv += "TIMEOUT = " + timeout.getText() + "\n";
	if (!isEditValid(min_speed)) throw new DataFormatException("Files/MIN_SPEED");
	rv += "MIN_SPEED = " + min_speed.getText() + "\n";
	if (!isEditValid(max_speed)) throw new DataFormatException("Files/MAX_SPEED");
	rv += "MAX_SPEED = " + max_speed.getText() + "\n";
	if (!isEditValid(sampletime)) throw new DataFormatException("Files/SAMPLETIME");
	rv += "SAMPLETIME = " + sampletime.getText() + "\n";

	return rv;
    }

    public void parseCfg(IniParser data)
    {
	int i;

	setFmtField(umask, data, "Files/UMASK");
	protocol.setText(data.getValue("Files/PROTOCOL"));
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
	setFmtField(picture_size, data, "Files/PICTURE_SIZE");
	setFmtField(memoattach_size, data, "Files/MEMOATTACH_SIZE");
	setFmtField(public_size, data, "Files/PUBLIC_SIZE");
	setFmtField(tempdir_size, data, "Files/TEMPDIR_SIZE");
	setFmtField(blocksize, data, "Files/BLOCKSIZE");
	setFmtField(timeout, data, "Files/TIMEOUT");
	setFmtField(min_speed, data, "Files/MIN_SPEED");
	setFmtField(max_speed, data, "Files/MAX_SPEED");
	setFmtField(sampletime, data, "Files/SAMPLETIME");
    }
}
