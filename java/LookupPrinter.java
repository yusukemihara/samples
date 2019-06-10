import java.util.Date;
import javax.print.*;
import javax.print.attribute.*;
import javax.print.attribute.standard.*;
import javax.print.event.*;

public class LookupPrinter {
	public static void main(String args[]) {
		System.out.println("1:"+ (new Date()));
		PrintService[] pss = PrintServiceLookup.lookupPrintServices(null,null);
		System.out.println("2:"+ (new Date()));
		for(PrintService ps: pss) {
			System.out.println(ps.getName());
/*
			PrintServiceAttributeSet myAset = ps.getAttributes();
			Attribute[] attr = myAset.toArray();
			int loop = attr.length;
			System.out.println("My Attributes set:");
			for (int i = 0; i < attr.length; i++) {
    			System.out.println("   " + attr[i]);
			}

			DocFlavor[] myFlavors = ps.getSupportedDocFlavors();
 			if (myFlavors == null) {
				continue;
			}
			loop = myFlavors.length;
			for (int i = 0; i < myFlavors.length; i++) {
			    DocFlavor flavor = myFlavors[i];
			  System.out.println("MediaSubtype: " + flavor.getMediaSubtype());
			  System.out.println("MediaType:    " + flavor.getMediaType());
			  System.out.println("MimeType:     " + flavor.getMimeType());
			    System.out.println("toString:     " + flavor.toString());
			}
*/
			DocFlavor flavor = DocFlavor.SERVICE_FORMATTED.PRINTABLE;
			
		 	Object o = ps.getSupportedAttributeValues(Media.class, flavor, null);
            if (o != null && o.getClass().isArray()) {
                for (Media media : (Media[]) o) {
                    // we collect the MediaTray available
                    if (media instanceof MediaTray) {
                        System.out.println(media.getValue() + " : " + media + " - " + media.getClass().getName());
                    }
                }
            }
			System.out.println("3:"+ (new Date()));
		}

		System.out.println("4:"+ (new Date()));
		pss = PrintServiceLookup.lookupPrintServices(null,null);
		System.out.println("5:"+ (new Date()));
		PrintRequestAttributeSet attrSet = new HashPrintRequestAttributeSet();
		attrSet.add(MediaSizeName.ISO_A5);
		pss = PrintServiceLookup.lookupPrintServices(null,attrSet);
		System.out.println("6:"+ (new Date()));
		for(PrintService ps: pss) {
			System.out.println(ps.getName());
/*
			PrintServiceAttributeSet myAset = ps.getAttributes();
			Attribute[] attr = myAset.toArray();
			int loop = attr.length;
			System.out.println("My Attributes set:");
			for (int i = 0; i < attr.length; i++) {
    			System.out.println("   " + attr[i]);
			}

			DocFlavor[] myFlavors = ps.getSupportedDocFlavors();
 			if (myFlavors == null) {
				continue;
			}
			loop = myFlavors.length;
			for (int i = 0; i < myFlavors.length; i++) {
			    DocFlavor flavor = myFlavors[i];
			  System.out.println("MediaSubtype: " + flavor.getMediaSubtype());
			  System.out.println("MediaType:    " + flavor.getMediaType());
			  System.out.println("MimeType:     " + flavor.getMimeType());
			    System.out.println("toString:     " + flavor.toString());
			}
*/
			DocFlavor flavor = DocFlavor.SERVICE_FORMATTED.PRINTABLE;
			
		 	Object o = ps.getSupportedAttributeValues(Media.class, flavor, null);
            if (o != null && o.getClass().isArray()) {
                for (Media media : (Media[]) o) {
                    // we collect the MediaTray available
                    if (media instanceof MediaTray) {
                        System.out.println(media.getValue() + " : " + media + " - " + media.getClass().getName());
                    }
                }
            }
			System.out.println("3:"+ (new Date()));
		}
	}
}
