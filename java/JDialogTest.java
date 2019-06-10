import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

class JDialogTest extends JFrame {
	JDialogTest() {
		getContentPane().setLayout(new FlowLayout());

		MyDialog dlg = new MyDialog(this);
		dlg.show();
System.out.println("after dlg.show");

		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setTitle("JDialogTest");
		setSize(200, 100);
		getContentPane().add(new JButton("hoge") {
			public void actionPerformed(ActionEvent e) {
				System.out.println("action!!");
			}
});
		setVisible(true);
	}
	public static void main(String[] args) {
		new JDialogTest();
	}
}

class MyDialog extends JDialog implements ActionListener {
	MyDialog(Frame owner) {
		super(owner);
		getContentPane().setLayout(new FlowLayout());

		JButton btn = new JButton("OK");
		btn.addActionListener(this);
		getContentPane().add(btn);

		setTitle("MyDialog");
		setSize(100, 70);
		setModal(true);
		setVisible(true);
	}
	public void actionPerformed(ActionEvent e) {
		dispose();
	}
}

