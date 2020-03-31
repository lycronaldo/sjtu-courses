package global;

import org.junit.Test;

import javax.swing.*;
import javax.swing.table.DefaultTableCellRenderer;

public class GlobalVar {

    public static void log(Object... info) {
        String className = Thread.currentThread().getStackTrace()[2].getClassName();
        String methodName = Thread.currentThread().getStackTrace()[2].getMethodName();
        String title = className + "." + methodName;
        System.out.printf("[%s]\t", title);
        for (int i = 0; i < info.length; i++) {
            System.out.print(info[i] + " ");
        }
        System.out.println();
    }

    /**
     * Set JTable center alignment
     * @param table
     */
    public static void setTableColumnCenter(JTable table) {
        DefaultTableCellRenderer r = new DefaultTableCellRenderer();
        r.setHorizontalAlignment(JLabel.CENTER);
        table.setDefaultRenderer(Object.class, r);
    }
}
