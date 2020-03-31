package gui;

import entity.LinearHashing;
import entity.Tuple;
import global.GlobalVar;

import javax.swing.*;
import javax.swing.border.Border;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class LinearPane extends Container {
    private static final String[] tableHeader = {"Index", "H(x) = x % S"};
    private static final int TABLEMAX = 8;
    private Object[][] tableData;
    private InputPane inputPane;
    private JTable linearTable;
    private LinearHashing linearHashing;
    private JPanel contentPane;

    public LinearPane() {
        linearHashing = new LinearHashing();

        inputPane = new InputPane();
        setLayout(new BorderLayout());
        add(inputPane, BorderLayout.NORTH);

        tableData = new Object[TABLEMAX][tableHeader.length];
        initTableData();

        linearTable = new JTable(tableData, tableHeader);
        linearTable.setRowHeight(30);
        GlobalVar.setTableColumnCenter(linearTable);

        contentPane = new JPanel(new BorderLayout());
        contentPane.setBorder(BorderFactory.createTitledBorder("Linear Hash Table"));
        contentPane.add(new JScrollPane(linearTable), BorderLayout.CENTER);
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
        Tuple[] tuples = linearHashing.getTuples();
        for (int i = 0; i < TABLEMAX; i++) {
            if (tuples[i] != null){
                tableData[i][1] = String.format("(%d, %d)", tuples[i].getKey(), tuples[i].getVal());
            } else {
                tableData[i][1] = null;
            }
        }
        linearTable.repaint();
    }

    private void initActionListener() {
        inputPane.getInsertButton().addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                long key = Long.parseLong(inputPane.getKeyText());
                long val = Long.parseLong(inputPane.getValText());
                linearHashing.set(key, val);
                repaintTable();
                GlobalVar.log(key, val);
            }
        });

        inputPane.getDeleteButton().addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                long key = Long.parseLong(inputPane.getKeyText());
                linearHashing.delete(key);
                repaintTable();
            }
        });
    }
}
