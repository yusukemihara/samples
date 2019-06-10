import java.awt.*;
import java.awt.event.*;
import java.awt.geom.*;
import java.awt.print.*;
import javax.swing.*;
import java.nio.channels.FileChannel;
import java.io.IOException;
import java.io.FileInputStream;
import java.nio.ByteBuffer;
import java.io.File;
import java.awt.image.*;
import java.lang.Math;

import com.sun.pdfview.PDFRenderer;
import com.sun.pdfview.PDFFile;
import com.sun.pdfview.PDFPage;

public class PrintDialogTest {
    public void print() throws java.io.IOException {
        JFileChooser chooser = new JFileChooser();
        chooser.showOpenDialog(null);
        File file = new File(chooser.getSelectedFile().getAbsolutePath());
        FileInputStream fis = new FileInputStream(file);
        FileChannel fc = fis.getChannel();
        ByteBuffer bb = fc.map(FileChannel.MapMode.READ_ONLY, 0, fc.size());
        PDFFile pdfFile = new PDFFile(bb); // Create PDF Print Page

        PDFPrintPage pages = new PDFPrintPage(pdfFile);

        PrinterJob pjob = PrinterJob.getPrinterJob();
        
        pjob.setJobName(file.getName());
        if (pjob.printDialog()) {
            // validate the page against the chosen printer to correct
            // paper settings and margins
            PageFormat pfDefault = pjob.validatePage(pjob.defaultPage());
            Paper paper = pfDefault.getPaper();
            paper.setImageableArea(0, 0, paper.getWidth() , paper.getHeight());
            pfDefault.setPaper(paper);
            Book book = new Book();

            book.append(pages, pfDefault, pdfFile.getNumPages());
            pjob.setPageable(book);

            try {
                pjob.print();
            } catch (PrinterException exc) {
                System.out.println(exc);
            }
        }
    }

    public static void main(String args[]) throws Exception {
      PrintDialogTest test = new PrintDialogTest();
      test.print();
    }

    public class PDFPrintPage implements Printable {
    
        /** The PDFFile to be printed */
        private PDFFile file;
        /** The PrinterJob for this print job */
        private PrinterJob pjob;
        /** A dialog box indicating printing status, with cancel button */
        private JDialog pd;
        /** The text in the progress dialog indicating the current page */
        private JLabel pagenumlabel;
        /** The cancel button in the progress dialog */
        private JButton cancel;
    
        /**
         * Create a new PDFPrintPage object for a particular PDFFile.
         * @param file the PDFFile to be printed.
         */
        public PDFPrintPage(PDFFile file) {
            PDFPrintPage.this.file = file;
        }
    
        // from Printable interface:  prints a single page, given a Graphics
        // to draw into, the page format, and the page number.
        public int print(Graphics g, PageFormat format, int index)
                throws PrinterException {
            int pagenum = index + 1;
    
            // don't bother if the page number is out of range.
            if ((pagenum >= 1) && (pagenum <= file.getNumPages())) {
    
                // update the page number in the progress dialog
                if (pagenumlabel != null) {
                    pagenumlabel.setText(String.valueOf(pagenum));
                }
    
                // fit the PDFPage into the printing area
                Graphics2D g2 = (Graphics2D) g;
                PDFPage page = file.getPage(pagenum);
                int width = (int)page.getWidth();
                int height = (int)page.getHeight();
 
                if (width > height) {
                    g2.transform(new AffineTransform(0f,-1f,1f,0f,0,width));
                }
    
                // render the page
                PDFRenderer pgs = new PDFRenderer(page, g2, new Rectangle(0,0,width,height), null, null);
                try {
                    page.waitForFinish();
                    pgs.run();
                } catch (InterruptedException ie) {
                }
                return PAGE_EXISTS;
            } else {
                return NO_SUCH_PAGE;
            }
        }
    }
    public class PrintObserver implements ImageObserver {
        public boolean imageUpdate(Image img, int infoflags, int x, int y, int width, int height) {
            if ((infoflags & ImageObserver.ALLBITS) == ImageObserver.ALLBITS) {
                return false;
            }
            return true;
        }
    }
}
