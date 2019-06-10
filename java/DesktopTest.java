import java.awt.*;
import java.io.*;
import java.awt.event.*;
import javax.swing.*;

class DesktopTest extends JFrame implements ActionListener {


	DesktopTest() {
		getContentPane().setLayout(new FlowLayout());
		JButton b1 = new JButton("Open");
		b1.addActionListener(this);
		getContentPane().add(b1);

		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setTitle("DesktopTest");
		setSize(200, 100);
		setVisible(true);
	}
	public void actionPerformed(ActionEvent e) {
		JFileChooser chooser = new JFileChooser();
		if (chooser.showSaveDialog(this) != JFileChooser.APPROVE_OPTION) {
        	return;
        }
		File selected = chooser.getSelectedFile();

System.out.println("AAAction!");
		try {
		Desktop.getDesktop().open(selected);
		} catch (Exception ex) {
			ex.printStackTrace();
		}
	}
	public static void main(String[] args) {
		new DesktopTest();
	}
}
