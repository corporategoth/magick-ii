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

import javax.swing.filechooser.*;
import java.util.*;
import java.net.*;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.PrintStream;

public class mct extends JApplet implements ActionListener
{
// private:
    private JMenuItem cwd, open, save, quit, about, time_fmt, space_fmt, wildcards;
    private JCheckBoxMenuItem tooltips;
    private ButtonGroup theme;
    private Vector laf;
    private TabbedPane startup, services, files, config, nickserv, chanserv,
		memoserv, operserv, commserv, servmsg;
    private boolean command;
    private static String pwd;

    public static String currentDirectory()
    {
	return pwd;
    }

    public mct(String argv[])
    {
	super();
	command = true;
	if (argv != null && argv.length > 0)
	{
	    File f = new File(argv[0]);
	    if (f.exists() && f.isDirectory())
	    {
		try
		{
		    pwd = f.getCanonicalPath();
		}
		catch (Exception ex)
		{
		}
	    }
	}
    }

    public mct()
    {
	this(null);
	command = false;
    }

    public String getConfigData()
    {
	String cfg = new String();
	cfg += "# Automatically generated configuration file for\n";
	cfg += "# Magick IRC Services v2.0.\n";
	cfg += "#\n";
	cfg += "# Generated at " + new Date() + "\n";
	cfg += "\n";
	cfg += startup.createCfg();
	cfg += "\n" + services.createCfg();
	cfg += "\n" + files.createCfg();
	cfg += "\n" + config.createCfg();
	cfg += "\n" + nickserv.createCfg();
	cfg += "\n" + chanserv.createCfg();
	cfg += "\n" + memoserv.createCfg();
	cfg += "\n" + operserv.createCfg();
	cfg += "\n" + commserv.createCfg();
	cfg += "\n" + servmsg.createCfg();
	return cfg;
    }

    public void setConfigData(String s)
    {
	IniParser cfg = new IniParser(s);
	startup.parseCfg(cfg);
	services.parseCfg(cfg);
	files.parseCfg(cfg);
	config.parseCfg(cfg);
	nickserv.parseCfg(cfg);
	chanserv.parseCfg(cfg);
	memoserv.parseCfg(cfg);
	operserv.parseCfg(cfg);
	commserv.parseCfg(cfg);
	servmsg.parseCfg(cfg);
    }

    public void actionPerformed(ActionEvent e)
    {
	if (e.getSource() == cwd)
	{
	    JFileChooser select_dir = new JFileChooser(currentDirectory());

	    if (select_dir.getAccessibleContext() == null)
	    {
		JOptionPane.showMessageDialog(null,
			"Due to security concerns, we cannot perform any files-based\n" +
			"actions on your system at this time.\n",
			"Error",
			JOptionPane.ERROR_MESSAGE);
	    }
	    else
	    {
		select_dir.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
		int rv = select_dir.showDialog(null, "Select Working Directory");
		if (rv == JFileChooser.APPROVE_OPTION)
		{
		    try
		    {
			pwd = select_dir.getSelectedFile().getCanonicalPath();
		    }
		    catch (Exception ex)
		    {
			JOptionPane.showMessageDialog(null,
			    "Could not open directory:\n" +
			    ex.getMessage() + "\n",
			    "Error",
			    JOptionPane.ERROR_MESSAGE);
			return;
		    }
		}
		else if (pwd == null)
		{
		    System.exit(0);
		}
	    }
	}
	else if (e.getSource() == open)
	{
	    String rv = showFileDialog("magick.ini", "Open", "Configuration Files (*.ini)", ".ini", true);
	    if (rv != null)
	    {
		byte[] array = null;
		try
		{
		   
		    File f = new File(rv);
		    if (!f.isAbsolute())
			f = new File(currentDirectory() + File.separator + rv);
		    FileInputStream is = new FileInputStream(f);
		    array = new byte[(int) f.length()];
		    is.read(array);
		}
		catch (Exception ex)
		{
		    JOptionPane.showMessageDialog(null,
			"Could not read configuration file:\n" +
			ex.getMessage() + "\n",
			"Error",
			JOptionPane.ERROR_MESSAGE);
		    return;
		}
		setConfigData(new String(array));
	    }
	}
	else if (e.getSource() == save)
	{
	    String rv = showFileDialog("magick.ini", "Save", "Configuration Files (*.ini)", ".ini", false);
	    if (rv != null)
	    {
		File f = new File(rv);
		if (!f.isAbsolute())
		    f = new File(currentDirectory() + File.separator + rv);
		String cfg = getConfigData();
		cfg = cfg.replaceAll("\n\n", "\n \n");
		try
		{
		    f.createNewFile();
		    PrintStream os = new PrintStream(new FileOutputStream(f), true);
		    StringTokenizer st = new StringTokenizer(cfg, "\n");
		    while (st.hasMoreTokens())
			os.println(st.nextToken());
		}
		catch (Exception ex)
		{
		    JOptionPane.showMessageDialog(null,
			"Could not write configuration file:\n" +
			ex.getMessage() + "\n",
			"Error",
			JOptionPane.ERROR_MESSAGE);
		    return;
		}
	    }
	}
	else if (e.getSource() == quit)
	{
	    System.exit(0);
	}
	else if (e.getSource() == tooltips)
	{
	    ToolTipManager.sharedInstance().setEnabled(tooltips.isSelected());
	}
	else if (e.getSource() == about)
	{
	    JOptionPane.showMessageDialog(null,
			"This tool was written to configure Magick IRC Services version 2.0\n" +
			"\n" +
			"This tool was written by Preston A. Elder <prez@magick.tm>, and is\n" +
			"released under the terms of the GNU General Public License.\n" +
			"\n" +
			"The Magick home page can be found at: http://www.magick.tm\n" +
			"\n" +
			"Magick is dedicated to the memory of PreZ's beloved sister, Stacey\n" +
			"Louise Elder (Jan 1975 - Feb 1998) and Ungod's brother Edward Kevin\n" +
			"King (Aug 1982 - Nov 1997).\n",
			"About Magick Configuration Tool",
			JOptionPane.INFORMATION_MESSAGE);
	}
	else if (e.getSource() == time_fmt)
	{
	    JOptionPane.showMessageDialog(null,
			"When specifying a time value, you must enter a number, and then a modifier.\n" +
			"\n" +
			"The modifiers you may use are:\n" +
			"s (seconds), m (minutes), h (hours), d (days), w (weeks) and y (years)\n" +
			"\n" +
			"If you do not place a modifier after a number, it defaults to seconds.\n" +
			"You may chain modified numbers together in any order you wish.  You may\n" +
			"also exceed the 'boundary' value of any number, and it will be re-calculated.\n",
			"Time Formats",
			JOptionPane.INFORMATION_MESSAGE);
	}
	else if (e.getSource() == space_fmt)
	{
	    JOptionPane.showMessageDialog(null,
			"When specifying a space value, you must enter a number, and then a modifier.\n" +
			"\n" +
			"The modifiers you may use are:\n" +
			"b (bytes), k (kilobytes), m (megabytes) and g (gigabytes)\n" +
			"\n" +
			"If you do not place a modifier after a number, it defaults to bytes.\n" +
			"You may chain modified numbers together in any order you wish.  You may\n" +
			"also exceed the 'boundary' value of any number, and it will be re-calculated.\n" +
			"Please note that the boundary values are always multipals of 1024.\n",
			"Space Formats",
			JOptionPane.INFORMATION_MESSAGE);
	}
	else if (e.getSource() == wildcards)
	{
	    JOptionPane.showMessageDialog(null,
			"Magick supports standard unix 'globbing' wildcards.  This means that an\n" +
			"asterisk (*) represents zero or more of any character, and a question mark\n" +
			"(?) represents exactly one of any character.\n",
			"Wildcard Usage",
			JOptionPane.INFORMATION_MESSAGE);
	}
	else if (e.getSource() instanceof AbstractButton)
	{
	    Enumeration el = theme.getElements();
	    int i=0;
	    while (el.hasMoreElements())
	    {
		if ((AbstractButton) el.nextElement() == (AbstractButton) e.getSource())
		{
		    try
		    {
			UIManager.setLookAndFeel((LookAndFeel) laf.get(i));
			SwingUtilities.updateComponentTreeUI(this);
		    }
		    catch (Exception ex)
		    {
		    }
		}
		i++;
	    }
	}
    }

    public void init()
    {
	// Ground work ... (Look and Feel)
	laf = new Vector();
	UIManager.LookAndFeelInfo[] feels = UIManager.getInstalledLookAndFeels();
	LookAndFeel fdef = null;
	theme = new ButtonGroup();
	for (int i=0; i<feels.length; i++)
	{
	    LookAndFeel f = null;
	    try
	    {
		UIManager.setLookAndFeel(feels[i].getClassName());
		f = UIManager.getLookAndFeel();
	    }
	    catch (Exception ex)
	    {
	    }

	    if (f != null && f.isSupportedLookAndFeel())
	    {
		laf.add(f);
		if (fdef == null || f.isNativeLookAndFeel())
		    fdef = f;
	    }
	}
	try
	{
	    if (fdef != null)
		UIManager.setLookAndFeel(fdef);
	}
	catch(Exception ex)
	{
	}

	Container cp = getContentPane();
	cp.setLayout(new BorderLayout());

	// Menu Bar ...
	JPanel top = new JPanel();
	top.setLayout(new BorderLayout());
	JPanel tmp = new JPanel();
	tmp.setLayout(new BorderLayout());

	JMenuBar mb = new JMenuBar();
	mb.setBorderPainted(false);
	JMenu submenu;

	if (command)
	{
	    submenu = new JMenu("File");
	    submenu.setMnemonic(KeyEvent.VK_F);

	    cwd = new JMenuItem("Current Directory");
	    cwd.setMnemonic(KeyEvent.VK_D);
	    cwd.setAccelerator(KeyStroke.getKeyStroke(
		KeyEvent.VK_D, ActionEvent.ALT_MASK));
	    cwd.addActionListener(this);
	    submenu.add(cwd);

	    open = new JMenuItem("Open");
	    open.setMnemonic(KeyEvent.VK_O);
	    open.setAccelerator(KeyStroke.getKeyStroke(
		KeyEvent.VK_O, ActionEvent.ALT_MASK));
	    open.addActionListener(this);
	    submenu.add(open);

	    save = new JMenuItem("Save");
	    save.setMnemonic(KeyEvent.VK_S);
	    save.setAccelerator(KeyStroke.getKeyStroke(
		KeyEvent.VK_S, ActionEvent.ALT_MASK));
	    save.addActionListener(this);
	    submenu.add(save);

	    quit = new JMenuItem("Exit");
	    quit.setMnemonic(KeyEvent.VK_X);
	    quit.setAccelerator(KeyStroke.getKeyStroke(
		KeyEvent.VK_X, ActionEvent.ALT_MASK));
	    quit.addActionListener(this);
	    submenu.add(quit);

	    mb.add(submenu);

	    if (currentDirectory() == null)
		actionPerformed(new ActionEvent(cwd, 0, ""));
	}

	submenu = new JMenu("Look and Feel");
	submenu.setMnemonic(KeyEvent.VK_L);

	JMenu theme_menu = new JMenu("Theme");
	submenu.add(theme_menu);

	for (int i=0; i<laf.size(); i++)
	{
	    LookAndFeel f = (LookAndFeel) laf.get(i);
	    JRadioButtonMenuItem mi = new JRadioButtonMenuItem(f.getName());
	    mi.addActionListener(this);
	    if (f == fdef)
		mi.setSelected(true);
	    theme.add(mi);
	    theme_menu.add(mi);
	}

	tooltips = new JCheckBoxMenuItem("Enable ToolTips", true)
		{
		    public JToolTip createToolTip()
		    {
			return new JMultiLineToolTip();
		    }
		};
	tooltips.setMnemonic(KeyEvent.VK_T);
	tooltips.addActionListener(this);
	submenu.add(tooltips);

	mb.add(submenu);
	tmp.add(mb, BorderLayout.WEST);

	mb = new JMenuBar();
	mb.setBorderPainted(false);
	submenu = new JMenu("Help");
	submenu.setMnemonic(KeyEvent.VK_H);

	about = new JMenuItem("About");
	about.setMnemonic(KeyEvent.VK_A);
	about.addActionListener(this);
	submenu.add(about);

	submenu.addSeparator();

	time_fmt = new JMenuItem("Time Formats");
	time_fmt.setMnemonic(KeyEvent.VK_T);
	time_fmt.addActionListener(this);
	submenu.add(time_fmt);

	space_fmt = new JMenuItem("Space Formats");
	space_fmt.setMnemonic(KeyEvent.VK_S);
	space_fmt.addActionListener(this);
	submenu.add(space_fmt);

	wildcards = new JMenuItem("Wildcard Usage");
	wildcards.setMnemonic(KeyEvent.VK_W);
	wildcards.addActionListener(this);
	submenu.add(wildcards);

	mb.add(submenu);
	tmp.add(mb, BorderLayout.EAST);

	top.add(tmp, BorderLayout.NORTH);

	JTabbedPane middle = new JTabbedPane();

	Color background = new Color(0x40, 0x40, 0x40);
	Color foreground = new Color(0xA2, 0xA2, 0xA2);

	JPanel gb = new JPanel();
	GridBagConstraints gc = new GridBagConstraints();
	gb.setLayout(new GridBagLayout());
	gb.setBackground(background);
	gc.gridx=0;
	gc.gridy=0;
	gc.gridwidth = gc.REMAINDER;
	gc.anchor = gc.CENTER;
	URL url = mct.class.getResource("splash.jpg");
	Image img = Toolkit.getDefaultToolkit().getImage(url).getScaledInstance(-1, 300, Image.SCALE_DEFAULT);
	JButton icon = new JButton("", new ImageIcon(img));
	icon.setBorderPainted(false);
	icon.setBackground(background);
	gb.add(icon, gc);
	gc.gridy++;
	JLabel lab = new JLabel("(c) 1997-2002 Preston A. Elder <prez@magick.tm>");
	lab.setForeground(foreground);
	gb.add(lab, gc);
	gc.gridy++;
	lab = new JLabel("(c) 1997-2002 William King <ungod@magick.tm>");
	lab.setForeground(foreground);
	gb.add(lab, gc);
	gc.gridy++;
	gc.gridwidth = 1;

	ToolTips tips = new ToolTips();
	startup = new Startup(tips);
	services = new Services(tips);
	files = new Files(tips);
	config = new Config(tips);
	nickserv = new NickServ(tips);
	chanserv = new ChanServ(tips);
	memoserv = new MemoServ(tips);
	operserv = new OperServ(tips);
	commserv = new CommServ(tips);
	servmsg = new ServMsg(tips);
        ConfigView cfgview = new ConfigView(this);

	middle.add("Home", gb);
	middle.add(startup.name(), startup.createPane());
	middle.add(services.name(), services.createPane());
	middle.add(files.name(), files.createPane());
	middle.add(config.name(), config.createPane());
	middle.add(nickserv.name(), nickserv.createPane());
	middle.add(chanserv.name(), chanserv.createPane());
	middle.add(memoserv.name(), memoserv.createPane());
	middle.add(operserv.name(), operserv.createPane());
	middle.add(commserv.name(), commserv.createPane());
	middle.add(servmsg.name(), servmsg.createPane());
	middle.add("Current File", cfgview.createPane());

	cp.add(top, BorderLayout.NORTH);
	cp.add(middle, BorderLayout.CENTER);
	// cp.add(bottom, BorderLayout.SOUTH);

	if (currentDirectory() != null)
	{
	    File f = new File(currentDirectory() + "/magick.ini");
	    if (f.exists() && f.canRead())
	    {
		try
		{
		    FileInputStream is = new FileInputStream(f);
		    byte[] array = new byte[(int) f.length()];
		    is.read(array);
		    setConfigData(new String(array));
		}
		catch (Exception ex)
		{
		}
	    }
	}
    }

    public static void main(String argv[])
    {
	JApplet applet = new mct(argv);
	JFrame frame = new JFrame("Magick Configuration Tool");
	frame.setDefaultCloseOperation(frame.EXIT_ON_CLOSE);
	frame.getContentPane().add(applet);
	frame.setSize(750, 500);
	applet.init();
	applet.start();
	frame.setVisible(true);
    }

    public static String showFileDialog(String path, String approve, String filtname, String fileext, boolean readable)
    {
	String value = null;

	String directory = null, selected = null;

	if (path != null)
	{
	    try
	    {
		File f = new File(path);
		if (!f.isAbsolute())
		    f = new File(currentDirectory() + File.separator + path);
		if (f.isDirectory())
		    directory = f.getCanonicalPath();
		else
		{
		    selected = f.getName();
		    directory = f.getParent();
		    if (directory == null)
			directory = currentDirectory();
		    else
		    {
			f = new File(directory);
			directory = null;
			directory = f.getCanonicalPath();
		    }
		}
	    }
	    catch (Exception ex)
	    {
	    }
	}
	if (directory == null)
	    directory = currentDirectory();

	JFileChooser chooser = new JFileChooser(directory);

	if (chooser.getAccessibleContext() == null)
	{
	    JOptionPane.showMessageDialog(null,
		"Due to security concerns, we cannot perform any files-based\n" +
		"actions on your system at this time.\n",
		"Error",
		JOptionPane.ERROR_MESSAGE);
	}
	else
	{
	    if (filtname != null && fileext != null)
	    {
		class MyFileFilter extends FileFilter
		{
		    private String name, ext;
		    boolean read;

		    public MyFileFilter(String filtname, String fileext, boolean readable)
		    {
			super();
			name = filtname;
			ext = fileext;
			read = readable;
		    }
		    public boolean accept(File f)
		    {
			if (f.isDirectory())
			    return true;
			if (f.isFile() && f.getName().endsWith(ext))
			    if (read)
				return (f.exists() && f.canRead());
			    else
				return true;
			return false;
		    }
		    public String getDescription()
		    {
			return name;
		    }
		};
		MyFileFilter filt = new MyFileFilter(filtname, fileext, readable);

		chooser.addChoosableFileFilter(filt);
		chooser.setFileFilter(filt);
	    }
	    if (selected != null)
		chooser.setSelectedFile(new File(directory + File.separator + selected));
	    int rv = chooser.showDialog(null, approve);
	    if (rv == JFileChooser.APPROVE_OPTION)
	    {
		try
		{
		    value = chooser.getSelectedFile().getCanonicalPath();
		    if (value.startsWith(currentDirectory()))
			value = value.substring(currentDirectory().length() + 1);
		}
		catch (Exception ex)
		{
		    JOptionPane.showMessageDialog(null,
			"Due to security concerns, we cannot perform any files-based\n" +
			"actions on your system at this time.\n",
			"Error",
			JOptionPane.ERROR_MESSAGE);
		}
	    }
	}
	return value;
    }
}
