package javafxapplication;

import java.awt.BorderLayout;
import javafx.application.Application;
import javafx.application.Platform;
import javafx.embed.swing.JFXPanel;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.web.WebEngine;
import javafx.scene.web.WebView;
import javafx.stage.Stage;

public class WebFrame extends javax.swing.JFrame {
    WebEngine engine;
    
    /** Creates new form WebFrame */
    public WebFrame() {
        initComponents();

        TmpFxLauncher.launch();

        final JFXPanel panel = new JFXPanel();
        jPanel2.add(panel, BorderLayout.CENTER);

        Platform.runLater(new Runnable() {

            @Override
            public void run() {
                engine = new WebEngine("http://d.hatena.ne.jp/nowokay/");
                WebView view = new WebView(engine);

                Group root = new Group();
                root.getChildren().add(view);
                Scene scene = new Scene(root);

                panel.setScene(scene);
            }
        });
    }

    public static class TmpFxLauncher extends Application {
        @Override public void start(Stage primaryStage) {
        }

        private static void launch() {
            Application.launch(null);
        }
    }    
    
    private void initComponents() {

        jPanel1 = new javax.swing.JPanel();
        jTextField1 = new javax.swing.JTextField();
        jButton1 = new javax.swing.JButton();
        jPanel2 = new javax.swing.JPanel();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("WebView Browser");

        jTextField1.setColumns(30);
        jTextField1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jTextField1ActionPerformed(evt);
            }
        });
        jPanel1.add(jTextField1);

        jButton1.setText("jButton1");
        jButton1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton1ActionPerformed(evt);
            }
        });
        jPanel1.add(jButton1);

        getContentPane().add(jPanel1, java.awt.BorderLayout.NORTH);

        jPanel2.setLayout(new java.awt.BorderLayout());
        getContentPane().add(jPanel2, java.awt.BorderLayout.CENTER);

        java.awt.Dimension screenSize = java.awt.Toolkit.getDefaultToolkit().getScreenSize();
        setBounds((screenSize.width-616)/2, (screenSize.height-488)/2, 616, 488);
    }

    private void jButton1ActionPerformed(java.awt.event.ActionEvent evt) {
        engine.load(jTextField1.getText());
    }

    private void jTextField1ActionPerformed(java.awt.event.ActionEvent evt) {
        jButton1ActionPerformed(evt);
    }

    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        java.awt.EventQueue.invokeLater(new Runnable() {

            public void run() {
                new WebFrame().setVisible(true);
            }
        });
    }
    // Variables declaration - do not modify
    private javax.swing.JButton jButton1;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JPanel jPanel2;
    private javax.swing.JTextField jTextField1;
    // End of variables declaration
}

