/**
 * @file Board.java.
 *
 * @author xurban66
 *
 * Implements the main board class.
 */

package gui;

public class Board extends javax.swing.JFrame {

    int num_of_games = 1;

    /***
     * Constructor that creates new JFrame along with 1 game
     */
    public Board() {
        initComponents();
        Game g1 = new Game();
        g1.setSize(850,540);
        getContentPane().add(g1);
    }


    /**
     * Initializes Swing GUI
     */
    private void initComponents() {

        jMenuBar1 = new javax.swing.JMenuBar();
        jMenu1 = new javax.swing.JMenu();
        jMenu2 = new javax.swing.JMenu();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setMaximumSize(new java.awt.Dimension(1920, 1080));
        setMinimumSize(new java.awt.Dimension(900, 600));
        setPreferredSize(new java.awt.Dimension(900, 600));
        getContentPane().setLayout(null);

        jMenu1.setText("New Game");
        jMenu1.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jMenu1MouseClicked(evt);
            }
        });
        jMenuBar1.add(jMenu1);

        setJMenuBar(jMenuBar1);

        pack();
    }

    /**
     * Handler for a new game on click
     *
     * @param evt - mouse event
     */
    private void jMenu1MouseClicked(java.awt.event.MouseEvent evt) {
        System.out.println("Pocet hier: "+num_of_games);
        if(num_of_games == 1) {
            setSize(1800, 1000);
            Game g2 = new Game();
            g2.setBounds(900, 0, 850, 540);
            getContentPane().add(g2);
        }
        if(num_of_games == 2) {
            Game g3 = new Game();
            g3.setBounds(0, 540, 850, 540);
            getContentPane().add(g3);
        }
        if(num_of_games == 3) {
            Game g4 = new Game();
            g4.setBounds(900, 540, 850, 540);
            getContentPane().add(g4);
        }
        num_of_games++;
    }


    /**
     * Initializes Board
     *
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        //<editor-fold defaultstate="collapsed" desc=" Look and feel setting code (optional) ">
        try {
            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (ClassNotFoundException ex) {
            java.util.logging.Logger.getLogger(Board.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(Board.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(Board.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(Board.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>

        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new Board().setVisible(true);
            }
        });
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JMenu jMenu1;
    private javax.swing.JMenu jMenu2;
    private javax.swing.JMenuBar jMenuBar1;
    // End of variables declaration//GEN-END:variables
}
