package gui;

import global.GlobalVar;
import org.junit.Test;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class InputPane extends JPanel {
    private JButton insertButton;
    private JButton deleteButton;
    private JTextField keyTextField;
    private JTextField valTextField;


    public InputPane() {
        initSubcomponents();
        setLayout(new FlowLayout());
        setBorder(BorderFactory.createTitledBorder("Operation"));

        JLabel keyLabel = new JLabel("Key");
        JLabel valLabel = new JLabel("Value");

        add(keyLabel);
        add(keyTextField);
        add(valLabel);
        add(valTextField);
        add(insertButton);
        add(deleteButton);

    }

    private void initSubcomponents() {
        this.insertButton = new JButton("Insert");
        this.deleteButton = new JButton("Delete");
        this.keyTextField = new JTextField(10);
        this.valTextField = new JTextField(10);
    }

    public JButton getInsertButton() {
        return insertButton;
    }

    public JButton getDeleteButton() {
        return deleteButton;
    }

    public String getKeyText() {
        return keyTextField.getText();
    }

    public String getValText() {
        return valTextField.getText();
    }
}
