//
//

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
