import java.awt.*;
import java.io.*;
import java.awt.event.*;
import javax.swing.*;

class FontList {

	public static void main(String[] args) {
		String fonts[]= GraphicsEnvironment.getLocalGraphicsEnvironment().getAvailableFontFamilyNames();
			for(int i=0;i<fonts.length;i++){
			System.out.println("'"+fonts[i]+"'");
			}
	}
}
