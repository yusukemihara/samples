import java.awt.Color;
import java.awt.Graphics;
import java.awt.GraphicsDevice;
import java.awt.GraphicsEnvironment;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JFrame;

public class AllScreen extends JFrame implements ActionListener {

private JButton drawButton = new JButton("Draw");
private JButton exitButton = new JButton("Exit");
private Graphics g;

public AllScreen(GraphicsDevice device) {
super(device.getDefaultConfiguration());
setDefaultCloseOperation(EXIT_ON_CLOSE);

setLayout(null);
drawButton.addActionListener(this);
drawButton.setBounds(20, 40, 100, 20);
add(drawButton);

exitButton.addActionListener(this);
exitButton.setBounds(140, 40, 100, 20);
add(exitButton);
setUndecorated(true);
if (device.isFullScreenSupported()) {
device.setFullScreenWindow(this);
}
this.g = getGraphics();
}

public void actionPerformed(ActionEvent e) {
Object src = e.getSource();
if (src.equals(drawButton)) {
g.setColor(Color.RED);
g.fillRect(20, 100, 220, 100);
} else if (src.equals(exitButton)) {
System.exit(0);
}
}

public static void main(String[] args) {
GraphicsEnvironment env = GraphicsEnvironment.getLocalGraphicsEnvironment();
GraphicsDevice device = env.getDefaultScreenDevice();
AllScreen test = new AllScreen(device);
}
}
