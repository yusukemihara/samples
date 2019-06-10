import java.util.Date;
import javax.print.*;
import javax.print.attribute.*;
import javax.print.attribute.standard.*;
import javax.print.event.*;
import java.net.URL;

import com.sun.pdfview.PDFFile;
import com.sun.pdfview.PDFPage;
import com.sun.pdfview.PDFRenderer;

import java.nio.channels.FileChannel;
import java.io.FileInputStream;
import java.nio.ByteBuffer;
import java.io.File;

public class PDFPaperSize {

    public static MediaSizeName getPDFPaperSize(File file) throws Exception {
    	FileInputStream fis = new FileInputStream(file);
    	FileChannel fc = fis.getChannel();
    	ByteBuffer bb = fc.map(FileChannel.MapMode.READ_ONLY, 0, fc.size());
    	PDFFile pdfFile = new PDFFile(bb); // Create PDF Print Page

		PDFPage page = pdfFile.getPage(1);
		float width = page.getWidth();
		float height = page.getHeight();

		if (width > height) {
			float swap = width;
			width = height;
			height = swap;
		}
        System.out.println("width:" + width + " height:" + height  + " bb:" + page.getBBox() + " pageBox:"+page.getPageBox());
		return MediaSize.findMedia(width/72,height/72,Size2DSyntax.INCH);
    }

	public static void main(String args[]) throws Exception {
		System.out.println("size:" + getPDFPaperSize(new File(args[0])));
	}
}
