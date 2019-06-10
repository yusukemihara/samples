import javax.swing.*;
import java.awt.BorderLayout;

public class JTabbedPaneTest extends JFrame{

  public static void main(String[] args){
    JTabbedPaneTest frame = new JTabbedPaneTest();

    frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    frame.setBounds(10, 10, 300, 200);
    frame.setTitle("タイトル");
    frame.setVisible(true);
  }

  JTabbedPaneTest(){
    JTabbedPane tabbedpane = new JTabbedPane();

    JPanel tabPanel1 = new JPanel();
    tabPanel1.add(new JButton("button1"));

    JPanel tabPanel2 = new JPanel();
    tabPanel2.add(new JLabel("Name:"));
    tabPanel2.add(new JTextField("", 10));

    JPanel tabPanel3 = new JPanel();
    tabPanel3.add(new JButton("button2"));

/*
    tabbedpane.addTab(null,tabPanel1);
    tabbedpane.addTab(null,tabPanel2);
    tabbedpane.addTab(null,tabPanel3);
*/

    tabbedpane.add(tabPanel1);
    tabbedpane.add(tabPanel2);
    tabbedpane.add(tabPanel3);

    tabbedpane.setTabPlacement(JTabbedPane.BOTTOM);
	tabbedpane.setTabLayoutPolicy(JTabbedPane.SCROLL_TAB_LAYOUT);
	tabbedpane.setDisabledIconAt(0, null);

    getContentPane().add(tabbedpane, BorderLayout.CENTER);
  }
}

