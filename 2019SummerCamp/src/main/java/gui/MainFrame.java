package gui;

import javax.swing.*;
import java.awt.*;

public class MainFrame extends JFrame {
    private static final int HEIGHT = 600;
    private static final int WIDTH = 500;
    private JTabbedPane tabPane;
    private CuckooPane cuckooPane;
    private LinearPane linearPane;

    public MainFrame() {
        initSubcomponents();

        setSize(WIDTH, HEIGHT);
        setContentPane(tabPane);
        setTitle("2019 SJTU Summer Camp");
        setVisible(true);
        setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
    }

    private void initSubcomponents() {
        this.tabPane = new JTabbedPane();
        this.cuckooPane = new CuckooPane();
        this.linearPane = new LinearPane();
        this.tabPane.add("Cuckoo", cuckooPane);
        this.tabPane.add("Linear", linearPane);
    }

}
