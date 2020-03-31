package gui;

import entity.CuckooHashing;
import entity.Tuple;
import global.GlobalVar;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class CuckooPane extends Container {
    private static final String[] tableHeader = {"Index", "H1(x) = x % S", "H2(x) = (x/S) % S"};
    private static final int TABLEMAX = 8;
    private InputPane inputPane;
    private JPanel contentPane;
    private JTable cuckooTable;
    private CuckooHashing cuckooHashing;
    private Object[][] tableData;

    public CuckooPane() {
        inputPane = new InputPane();
        contentPane = new JPanel(new BorderLayout());

        cuckooHashing = new CuckooHashing();

        tableData = new Object[TABLEMAX][tableHeader.length];
        initTableData();
        cuckooTable = new JTable(tableData, tableHeader);
        cuckooTable.setRowHeight(30);
        GlobalVar.setTableColumnCenter(cuckooTable);


        contentPane.setBorder(BorderFactory.createTitledBorder("Cuckoo Hash Table"));
        contentPane.add(new JScrollPane(cuckooTable), BorderLayout.CENTER);

        setLayout(new BorderLayout());
        add(inputPane, BorderLayout.NORTH);
        add(contentPane, BorderLayout.CENTER);

        initActionListener();
    }

    private void initTableData() {
        if (tableData == null) {
            tableData = new Object[TABLEMAX][tableHeader.length];
        }
        for (int i = 0; i < TABLEMAX; i++) {
            tableData[i][0] = i;
        }
    }

    private void repaintTable() {
        Tuple[] table1 = cuckooHashing.getTable1();
        Tuple[] table2 = cuckooHashing.getTable2();
        for (int i = 0; i < TABLEMAX; i++) {
            if (table1[i] != null) {
                tableData[i][1] = String.format("(%d, %d)", table1[i].getKey(), table1[i].getVal());
            } else {
                tableData[i][1] = null;
            }
            if (table2[i] != null) {
                tableData[i][2] = String.format("(%d, %d)", table2[i].getKey(), table2[i].getVal());
            } else {
                tableData[i][2] = null;
            }
        }
        cuckooTable.repaint();
    }

    private void initActionListener() {
        inputPane.getInsertButton().addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                long key = Long.parseLong(inputPane.getKeyText());
                long val = Long.parseLong(inputPane.getValText());
                cuckooHashing.set(key, val);
                GlobalVar.log(key, val);
                repaintTable();
            }
        });

        inputPane.getDeleteButton().addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                long key = Long.parseLong(inputPane.getKeyText());
                cuckooHashing.delete(key);
                repaintTable();
            }
        });
    }

}
