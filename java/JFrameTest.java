import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

class JFrameTest extends JFrame {
	JFrameTest() {
		final JPanel panel1 = new JPanel();
		final JPanel panel2 = new JPanel();

		JButton button1 = new JButton(new AbstractAction("button1") {
			public void actionPerformed(ActionEvent ev) {
System.out.println("push button1");
				getContentPane().removeAll();
				getContentPane().add(panel2);
				panel2.revalidate();
				panel2.repaint();
			}
		});
		panel1.add(button1);

		JButton button2 = new JButton(new AbstractAction("button2") {
			public void actionPerformed(ActionEvent ev) {
System.out.println("push button2");
				getContentPane().removeAll();
				getContentPane().add(panel1);
				panel1.revalidate();
				panel1.repaint();
			}
		});
		panel2.add(button2);

		getContentPane().setLayout(new FlowLayout());
		getContentPane().add(panel1);

		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setSize(400, 300);
		setVisible(true);

		Timer timer1 = new Timer(5 * 1000, new ActionListener(){
			public void actionPerformed(ActionEvent e) {
				JDialog dialog = new JDialog(JFrameTest.this, "tiemr1", true);
				dialog.setSize(200,200);
				dialog.setVisible(true);
			}
		});
		timer1.start();

		Timer timer2 = new Timer(7 * 1000, new ActionListener(){
			public void actionPerformed(ActionEvent e) {
				JDialog dialog = new JDialog(JFrameTest.this, "tiemr2", true);
				dialog.setSize(200,200);
				dialog.setVisible(true);
			}
		});
		timer2.start();

	}
	public static void main(String[] args) {
		new JFrameTest();
	}
}
