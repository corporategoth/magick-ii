//
//

import javax.swing.*;
import javax.swing.event.*;
import javax.swing.table.*;
import javax.swing.text.*;
import java.awt.*;
import java.awt.event.*;
import java.util.*;

public class MemoServ extends TabbedPane
{
// private:
    private JFormattedTextField news_expire, inflight, delay, files, filesize;

    public String name() { return "MemoServ"; }

    public MemoServ(ToolTips t)
    {
	super(t);

	news_expire = createFormattedTextField("NEWS_EXPIRE", 4, new TimeFormat(), "3w", true);
	inflight = createFormattedTextField("INFLIGHT", 4, new TimeFormat(), "2m", true);
	delay = createFormattedTextField("DELAY", 4, new TimeFormat(), "10s", true);
	files = createFormattedTextField("FILES", 4, new NumberRangeFormat(0, -1), "0", true);
	filesize = createFormattedTextField("FILESIZE", 4, new SpaceFormat(), "0b", isNonZero(files));
    }

    public void documentChanged(DocumentEvent e)
    {
	Document props = e.getDocument();
	if (props.getProperty("name").equals("files"))
	{
	    if (files.isEditValid())
	    {
		try {
		    files.commitEdit();
		    filesize.setEditable(isNonZero(files));
		    filesize.setEnabled(isNonZero(files));
		}
		catch (Exception ex)
		{
		}
	    }
	}
    }

    public void actionPerformed(ActionEvent e)
    {
    }

    public JComponent createPane()
    {
	JPanel gb = new JPanel();
	GridBagConstraints gc = new GridBagConstraints();
	gb.setLayout(new GridBagLayout());
	gc.gridx=0;
	gc.gridy=0;
	gc.anchor = gc.EAST;

	addToGridBagLine(gb, gc, "", new JLabel(""));
	addToGridBag(gb, gc, "In Flight time", inflight);
	addToGridBag(gb, gc, "Send delay", delay);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Stored Files", files);
	addToGridBag(gb, gc, "File Size", filesize);
	addGridBagLine(gb, gc);
	addToGridBag(gb, gc, "Expiration time", news_expire);
	addToGridBagLine(gb, gc, "", new JLabel(""));

	JPanel p = new JPanel();
	p.setLayout(new BorderLayout());
	p.add(gb, BorderLayout.NORTH);
	JScrollPane rv = new JScrollPane(p);
	return rv;
    }	

    public String createCfg()
    {
	String rv = new String();

	rv += "[MemoServ]\n";
	rv += "NEWS_EXPIRE = " + news_expire.getText() + "\n";
	rv += "INFLIGHT = " + inflight.getText() + "\n";
	rv += "DELAY = " + delay.getText() + "\n";
	rv += "FILES = " + files.getText() + "\n";
	rv += "FILESIZE = " + filesize.getText() + "\n";

	return rv;
    }

    public void parseCfg(String data)
    {
    }
}
