
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Point;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.File;
import java.util.ArrayList;
import javax.swing.AbstractCellEditor;
import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.ListSelectionModel;
import javax.swing.WindowConstants;
import javax.swing.table.DefaultTableModel;
import javax.swing.table.TableCellEditor;
import javax.swing.table.TableCellRenderer;
import javax.swing.table.TableColumn;

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
/**
 *
 * @author mihara
 */
public class ProtoPrintList implements Runnable {

    private static final int PREVIEW_COLUMN = 3;
    private static final int SAVE_COLUMN = 4;
    private static final int PRINT_COLUMN = 5;
    private static final int BUTTON_WIDTH = 30;
    private JFrame frame;
    private JTable table;
    private PrintListModel model;

    public ProtoPrintList() {
        frame = new JFrame("ProtoPrintList");
        model = new PrintListModel();
        table = new JTable(model);
        JScrollPane scroll = new JScrollPane(table);
        scroll.setPreferredSize(new Dimension(300, 400));
        frame.add(new JScrollPane(table));
        frame.pack();
        frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);

        table.addMouseListener(new MouseAdapter() {

            @Override
            synchronized public void mousePressed(MouseEvent e) {
                /*Point pt = e.getPoint();
                int mcol = table.convertColumnIndexToModel(table.columnAtPoint(pt));
                int vrow = table.rowAtPoint(e.getPoint());
                int mrow = (vrow>=0)?table.convertRowIndexToModel(vrow):-1;
                if(mrow>=0 && mcol==PREVIEW_COLUMN) {
                targetRow = mrow;
                }*/
            }

            @Override
            synchronized public void mouseReleased(MouseEvent e) {
                Point pt = e.getPoint();
                int mcol = table.convertColumnIndexToModel(table.columnAtPoint(pt));
                int vrow = table.rowAtPoint(e.getPoint());
                int mrow = (vrow >= 0) ? table.convertRowIndexToModel(vrow) : -1;
                if (mrow >= 0) {
                    switch (mcol) {
                        case PREVIEW_COLUMN:
                            System.out.println("preview");
                            break;
                        case SAVE_COLUMN:
                            System.out.println("save");
                            break;
                        case PRINT_COLUMN:
                            System.out.println("print");
                            break;
                    }
                }
            }
        });

        TableColumn tableColumn = table.getColumnModel().getColumn(0);
        tableColumn.setMinWidth(30);
        tableColumn.setMaxWidth(30);

        tableColumn = table.getColumnModel().getColumn(2);
        tableColumn.setMinWidth(50);
        tableColumn.setMaxWidth(50);

        ButtonColumn previewButtonColumn = new ButtonColumn(table, PREVIEW_COLUMN, "V");
        ButtonColumn saveButtonColumn = new ButtonColumn(table, SAVE_COLUMN, "S");
        ButtonColumn printButtonColumn = new ButtonColumn(table, PRINT_COLUMN, "P");
    }

    synchronized public void show() {
        frame.setVisible(true);
    }

    synchronized public void hide() {
        frame.setVisible(false);
    }

    synchronized public void add(String title, File file) {
        model.add(title, file);
    }

    public static void main(String[] args) {
        ProtoPrintList list = new ProtoPrintList();
        list.add("hoge", new File("ProtoPrintList.java"));
        list.add("moge", new File("ProtoPrintList.java"));
        list.add("noge", new File("ProtoPrintList.java"));
        list.show();

        list.run();
    }

    public void run() {
        int i = 0;
        while (true) {
            try {
                Thread.sleep(1000);
                System.out.println("add");
                this.add("add" + i, new File("ProtoPrintList.java"));
                i++;
            } catch (InterruptedException e) {
                System.out.println(e);
            }
        }
    }

    class ButtonColumn extends AbstractCellEditor implements TableCellRenderer, TableCellEditor {

        private String label;
        private final JButton renderButton;
        private final JButton editorButton;
        //public ButtonColumn(final JTable table) {

        public ButtonColumn(final JTable table, int column, String label) {
            super();
            this.label = label;
            table.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
            editorButton = new JButton();
            editorButton.setBorder(BorderFactory.createEmptyBorder());
            editorButton.setFocusPainted(false);
            editorButton.setRolloverEnabled(false);

            renderButton = new JButton(label);
            renderButton.setBorder(BorderFactory.createEmptyBorder());

            TableColumn tableColumn = table.getColumnModel().getColumn(column);
            tableColumn.setCellRenderer(this);
            tableColumn.setCellEditor(this);
            tableColumn.setMinWidth(BUTTON_WIDTH);
            tableColumn.setMaxWidth(BUTTON_WIDTH);
            tableColumn.setResizable(false);
        }

        @Override
        public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
            return renderButton;
        }

        @Override
        public Component getTableCellEditorComponent(JTable table, Object value, boolean isSelected, int row, int column) {
            return editorButton;
        }

        @Override
        public Object getCellEditorValue() {
            return label;
        }
    }

    public class PrintListModel extends DefaultTableModel {

        private ArrayList<PrintEntry> printEntryList;

        public PrintListModel() {
            super();
            printEntryList = new ArrayList<PrintEntry>();
        }
        private final ColumnContext[] columnArray = {
            new ColumnContext("No.", Integer.class, false),
            new ColumnContext("Title", String.class, false),
            new ColumnContext("Pages", Integer.class, false),
            new ColumnContext("", String.class, false),
            new ColumnContext("", String.class, false),
            new ColumnContext("", String.class, false)
        };
        private int number = 0;

        public void add(String title, File file) {
            PrintEntry entry = new PrintEntry(title, file);
            Object[] obj = {number, entry.getTitle(), entry.getNumOfPages(), "", "", ""};
            super.addRow(obj);
            printEntryList.add(entry);
            number++;
        }

        public PrintEntry getPrintEntry(int i) {
            return printEntryList.get(i);
        }

        public boolean isCellEditable(int row, int col) {
            return columnArray[col].isEditable;
        }

        public Class<?> getColumnClass(int modelIndex) {
            return columnArray[modelIndex].columnClass;
        }

        public int getColumnCount() {
            return columnArray.length;
        }

        public String getColumnName(int modelIndex) {
            return columnArray[modelIndex].columnName;
        }

        private class ColumnContext {

            public final String columnName;
            public final Class columnClass;
            public final boolean isEditable;

            public ColumnContext(String columnName, Class columnClass, boolean isEditable) {
                this.columnName = columnName;
                this.columnClass = columnClass;
                this.isEditable = isEditable;
            }
        }

        class PrintEntry {

            private String title;
            private File file;

            public PrintEntry(String title, File file) {
                this.file = file;
                this.title = title;
            }

            public void save() {
                System.out.println("save:" + title);
            }

            public void print() {
                System.out.println("print:" + title);
            }

            public String getTitle() {
                return title;
            }

            public int getNumOfPages() {
                return 1;
            }
        }
    }
}
