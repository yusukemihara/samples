import java.util.*;

public class Unicode {
	public static void main(String args[]) {
		String[] a1 = {"\u301c","\u2212","\u2014","\u2016","\u00a2","\u00a3"};
		String[] a2 = {"\uff5e","\uff0d","\uff0d","\u2225","\uffe0","\uffe1"};
		int i = 0;
		for (String c1:a1) {
			String c2 = a2[i];
			System.out.println("----");
			System.out.println(c1 + " -> " + c2);
			byte[] b1 = c1.getBytes();
			byte[] b2 = c2.getBytes();
			for (byte h:b1)System.out.printf("%02X",h);
			System.out.printf(" -> ");
			for (byte h:b2)System.out.printf("%02X",h);
			System.out.println();
			i++;
		}
	}
}
