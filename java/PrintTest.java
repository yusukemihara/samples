import java.util.Date;
import javax.print.*;
import javax.print.attribute.*;
import javax.print.attribute.standard.*;
import javax.print.event.*;
import java.net.URL;

public class PrintTest {
	public static void main(String args[]) throws Exception {
		System.out.println("1:"+ (new Date()));

		URL url = new URL("http://www.orca.med.or.jp/images/top/main_banner.jpg");
		DocFlavor flavor = DocFlavor.URL.JPEG;
		Doc doc = new SimpleDoc(url,flavor,null);
		PrintRequestAttributeSet attrSet = new HashPrintRequestAttributeSet();
		attrSet.add(MediaSizeName.ISO_A5);

		PrintService[] pss = PrintServiceLookup.lookupPrintServices(flavor,attrSet);

		System.out.println("2:"+ (new Date()));
		for(PrintService ps: pss) {
			String name = ps.getName();
			System.out.println(name);
			if (name.equals("RICOH_IPSiO_NX85S")) {
				ps.createPrintJob().print(doc, attrSet);
			}
		}
	}
}
