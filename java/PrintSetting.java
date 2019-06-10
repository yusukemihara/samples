/*      PANDA -- a simple transaction monitor

 Copyright (C) 1998-1999 Ogochan.
 2000-2003 Ogochan & JMA (Japan Medical Association).
 2002-2006 OZAWA Sakuro.

 This module is part of PANDA.

 PANDA is distributed in the hope that it will be useful, but
 WITHOUT ANY WARRANTY.  No author or distributor accepts responsibility
 to anyone for the consequences of using it or for whether it serves
 any particular purpose or works at all, unless he says so in writing.
 Refer to the GNU General Public License for full details.

 Everyone is granted permission to copy, modify and redistribute
 PANDA, but only under the conditions described in the GNU General
 Public License.  A copy of this license is supposed to have been given
 to you along with PANDA so you can know your rights and
 responsibilities.  It should be in a file named COPYING.  Among other
 things, the copyright notice and this notice must be preserved on all
 copies.
 */

import java.awt.*;
import java.awt.event.ActionEvent;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import javax.print.PrintService;
import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import javax.swing.table.TableCellEditor;
import javax.swing.table.TableColumn;

/**
 * <p>
 * A class that simulates Gtk+'s ColorButton.</p>
 *
 */
public class PrintSetting {

    private final Map<String, PrintService> printerMap;
    private static final ArrayList<String> printerList;

    static {
        printerList = new ArrayList<>();
        printerList.add("gs1");
        printerList.add("gs2");
        printerList.add("gs3");
    }

    public PrintSetting() {
        printerMap = new HashMap<>();
    }

    public void configPrinter() {
        JFrame frame = new JFrame("configPrinter");
        Container container = frame.getContentPane();
        container.setLayout(new BorderLayout(10, 5));

        final PrintSettingTableModel model = new PrintSettingTableModel();
        final JTable table = new JTable();
        table.setModel(model);
        TableColumn col = table.getColumnModel().getColumn(1);
        col.setCellEditor(new PrintSettingCellEditor());
        table.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        JScrollPane sp = new JScrollPane(table);
        frame.getContentPane().add(sp, BorderLayout.CENTER);

        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new FlowLayout());
        container.add(buttonPanel, BorderLayout.SOUTH);

        JButton button1 = new JButton(new AbstractAction("追加") {
            @Override
            public void actionPerformed(ActionEvent ev) {
                String printer = "";
                for (String p : printerList) {
                    printer = p;
                    break;
                }
                model.addRow(new String[]{"printer", printer});
            }
        });
        buttonPanel.add(button1);

        JButton button2 = new JButton(new AbstractAction("削除") {
            @Override
            public void actionPerformed(ActionEvent ev) {
                int row = table.getSelectedRow();
                if (row >= 0) {
                    model.removeRow(table.getSelectedRow());
                }
            }
        });
        buttonPanel.add(button2);

        JButton button3 = new JButton(new AbstractAction("OK") {
            @Override
            public void actionPerformed(ActionEvent ev) {
                System.out.println("ok");
            }
        });
        buttonPanel.add(button3);

        JButton button4 = new JButton(new AbstractAction("キャンセル") {
            @Override
            public void actionPerformed(ActionEvent ev) {
                System.out.println("cancel");
            }
        });
        buttonPanel.add(button4);

        frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        frame.setLocationRelativeTo(null);
        frame.setSize(600, 800);
        frame.setVisible(true);
    }

    private class PrintSettingCellEditor extends AbstractCellEditor implements TableCellEditor {

        private final JComboBox combo;
        private Object value;

        private Component editor;

        public PrintSettingCellEditor() {
            combo = new JComboBox();
            combo.removeAllItems();
            for (String printer : printerList) {
                combo.addItem(printer);
            }
            combo.setBorder(BorderFactory.createEmptyBorder());
        }

        @Override
        public Object getCellEditorValue() {
            return value;
        }

        @Override
        public boolean stopCellEditing() {
            value = "";
            if (editor instanceof JComboBox) {
                value = ((JComboBox) editor).getSelectedItem();
            }
            return super.stopCellEditing();
        }

        @Override
        public Component getTableCellEditorComponent(JTable table, Object value,
                boolean isSelected, int row, int column) {
            combo.setSelectedItem(value);
            editor = combo;
            return editor;
        }
    }

    private class PrintSettingTableModel extends DefaultTableModel {

        public PrintSettingTableModel() {
            super();
            this.setColumnCount(2);
            this.setRowCount(0);
        }

        @Override
        public boolean isCellEditable(int row, int col) {
            return true;
        }

        @Override
        public String getColumnName(int column) {
            if (column == 0) {
                return "プリンタ名";
            } else {
                return "プリンタ";
            }
        }
    }

    public static void main(String[] args) {
        PrintSetting ps = new PrintSetting();
        ps.configPrinter();
    }
}
