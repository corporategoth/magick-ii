//
//

import javax.swing.*;
import javax.swing.event.*;
import javax.swing.table.*;
import javax.swing.filechooser.*;
import java.awt.*;
import java.awt.event.*;
import java.util.*;
import java.net.*;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;

public class mct extends JApplet implements ActionListener
{
// private:
    private boolean command;
    private JMenuItem open, save, quit, about, time_fmt, space_fmt, wildcards;
    private JCheckBoxMenuItem tooltips;
    private ButtonGroup theme;
    private Vector laf;
    private TabbedPane startup, services, files, config, nickserv, chanserv,
		memoserv, operserv, commserv, servmsg;

    public mct(boolean c)
    {
	super();
	command = c;
    }

    public mct()
    {
	this(false);
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

    public void actionPerformed(ActionEvent e)
    {
	if (e.getSource() == open)
	{
	    JFileChooser open_file = new JFileChooser();

	    if (open_file.getAccessibleContext() == null)
	    {
		JOptionPane.showMessageDialog(null,
			"Due to security concerns, we cannot perform any files-based\n" +
			"actions on your system at this time.\n",
			"Error",
			JOptionPane.ERROR_MESSAGE);
	    }
	    else
	    {
		FileFilter cfg_filter = new FileFilter()
		{
		    public boolean accept(File f)
		    {
			if (f.isFile() && f.getName().endsWith(".ini") && f.exists() && f.canRead())
			    return true;
			return false;
		    }
		    public String getDescription()
		    {
			return "Config Files (*.ini)";
		    }
		};

		open_file.addChoosableFileFilter(cfg_filter);
		open_file.setFileFilter(cfg_filter);
		int rv = open_file.showOpenDialog(null);
		if (rv == JFileChooser.APPROVE_OPTION)
		{
		    byte[] array = null;
		    try
		    {
			File f = open_file.getSelectedFile();
			FileInputStream is = new FileInputStream(f);
			array = new byte[(int) f.length()];
			is.read(array);
		    }
		    catch (Exception ex)
		    {
		    }
		    String cfg = new String(array);
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
	    }
	}
	else if (e.getSource() == save)
	{
	    JFileChooser save_file = new JFileChooser();

	    if (save_file.getAccessibleContext() == null)
	    {
		JOptionPane.showMessageDialog(null,
			"Due to security concerns, we cannot perform any files-based\n" +
			"actions on your system at this time.\n",
			"Error",
			JOptionPane.ERROR_MESSAGE);
	    }
	    else
	    {
		FileFilter cfg_filter = new FileFilter()
		{
		    public boolean accept(File f)
		    {
			if (f.isFile() && f.getName().endsWith(".ini"))
			    return true;
			return false;
		    }
		    public String getDescription()
		    {
			return "Config Files (*.ini)";
		    }
		};

		save_file.addChoosableFileFilter(cfg_filter);
		save_file.setFileFilter(cfg_filter);
		int rv = save_file.showSaveDialog(null);
		if (rv == JFileChooser.APPROVE_OPTION)
		{
		    File f = save_file.getSelectedFile();
		    String cfg = getConfigData();
		    byte[] array = cfg.getBytes();
		    try
		    {
			f.createNewFile();
			FileOutputStream os = new FileOutputStream(f);
			os.write(array);
		    }
		    catch (Exception ex)
		    {
			JOptionPane.showMessageDialog(null,
			    "Could not write configuration file:\n" +
			    ex.getMessage() + "\n",
			    "Error",
			    JOptionPane.ERROR_MESSAGE);
		    }
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

	    open = new JMenuItem("Open");
	    open.setMnemonic(KeyEvent.VK_O);
	    open.setAccelerator(KeyStroke.getKeyStroke(
		KeyEvent.VK_S, ActionEvent.ALT_MASK));
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

	JPanel gb = new JPanel();
	GridBagConstraints gc = new GridBagConstraints();
	gb.setLayout(new GridBagLayout());
	gc.gridx=0;
	gc.gridy=0;
	gc.gridwidth = gc.REMAINDER;
	gc.anchor = gc.CENTER;
	URL url = mct.class.getResource("MagickMain.jpg");
	Image img=Toolkit.getDefaultToolkit().getImage(url); 
	JButton icon = new JButton("", new ImageIcon(img));
	icon.setBorderPainted(false);
	gb.add(icon, gc);
	gc.gridy++;
	gb.add(new JLabel("(c) 1997-2002 Preston A. Elder <prez@magick.tm>"), gc);
	gc.gridy++;
	gb.add(new JLabel("(c) 1997-2002 William King <ungod@magick.tm>"), gc);
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
    }

    public static void main(String argv[])
    {
	JApplet applet = new mct(true);
	JFrame frame = new JFrame("Magick Configuration Tool");
	frame.setDefaultCloseOperation(frame.EXIT_ON_CLOSE);
	frame.getContentPane().add(applet);
	frame.setSize(750, 500);
	applet.init();
	applet.start();
	frame.setVisible(true);
    }
}
