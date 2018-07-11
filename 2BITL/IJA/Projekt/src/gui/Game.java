/**
 * @file Game.java.
 *
 * @author xurban66
 *
 * Implements the main game class that represents frontend of application.
 */

package gui;

import model.cardpack.*;

import java.awt.*;
import java.io.*;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import javax.swing.*;


import model.cardpack.Card;
import model.game.Table;

public class Game extends javax.swing.JPanel {

    private int cord_x = -1;
    private int cord_y = -1;
    private int delta_x = 0;
    private int delta_y = 0;
    private int control = 0; /* 0 - base, 1 - drag and return, 2 - pullfromdrawstack, 3 - puttotargetstack*, 5 - load */
    private Table backend;
    public Card card;
    private ArrayList<Card> Cards_to_move = new ArrayList();
    private final JFileChooser fc = new JFileChooser();

    /**
     * Constructor of class Game, creates new backend and frontend copy
     * */
    public Game() {
        initComponents();
        backend = new Table();
        init_gui_cards();
        create_working_packs();
        create_new_deck();
        setLayout(null);
    }

    /**
     * Initializes GUI components
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        layered = new javax.swing.JLayeredPane();
        undo = new javax.swing.JLabel();
        reset = new javax.swing.JLabel();
        save = new javax.swing.JLabel();
        quit = new javax.swing.JLabel();
        load = new javax.swing.JLabel();
        card_deck = new javax.swing.JLabel();
        swap_deck = new javax.swing.JLabel();
        target_1 = new javax.swing.JLabel();
        target_2 = new javax.swing.JLabel();
        target_3 = new javax.swing.JLabel();
        target_4 = new javax.swing.JLabel();
        working_7 = new javax.swing.JLabel();
        working_6 = new javax.swing.JLabel();
        working_4 = new javax.swing.JLabel();
        working_5 = new javax.swing.JLabel();
        working_3 = new javax.swing.JLabel();
        working_2 = new javax.swing.JLabel();
        working_1 = new javax.swing.JLabel();

        setMaximumSize(new java.awt.Dimension(850, 540));
        setMinimumSize(new java.awt.Dimension(850, 540));
        setPreferredSize(new java.awt.Dimension(850, 540));
        setLayout(null);

        undo.setText("Undo");
        undo.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                undoMouseClicked();
            }
        });
        layered.add(undo);
        undo.setBounds(810, 20, 40, 20);

        reset.setText("Reset");
        reset.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
            }
        });
        layered.add(reset);
        reset.setBounds(810, 40, 40, 20);

        save.setText("Save");
        save.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                saveGame(backend);
            }
        });
        layered.add(save);
        save.setBounds(810, 60, 40, 20);

        quit.setText("Quit");
        quit.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                quitMouseClicked(evt);
            }
        });
        layered.add(quit);
        quit.setBounds(810, 100, 40, 20);

        load.setText("Load");
        load.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                loadMouseClicked();
            }
        });
        layered.add(load);
        load.setBounds(810, 80, 40, 20);

        card_deck.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/empty_card.jpg"))); // NOI18N
        card_deck.setName("ND"); // NOI18N
        card_deck.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseReleased(java.awt.event.MouseEvent evt) {
                mouse_released(evt);
            }
        });
        add(card_deck);
        card_deck.setBounds(32, 22, 100, 145);

        swap_deck.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        swap_deck.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/empty_card.jpg"))); // NOI18N
        swap_deck.setName("SD"); // NOI18N
        add(swap_deck);
        swap_deck.setBounds(144, 22, 100, 145);

        target_1.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/empty_spades.jpg"))); // NOI18N
        target_1.setName("T1"); // NOI18N
        add(target_1);
        target_1.setBounds(367, 22, 100, 145);

        target_2.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/empty_clubs.jpg"))); // NOI18N
        target_2.setName("T2"); // NOI18N
        add(target_2);
        target_2.setBounds(480, 20, 100, 145);

        target_3.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/empty_diamonds.jpg"))); // NOI18N
        target_3.setName("T3"); // NOI18N
        add(target_3);
        target_3.setBounds(591, 22, 100, 145);

        target_4.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/empty_hearts.jpg"))); // NOI18N
        target_4.setName("T4"); // NOI18N
        add(target_4);
        target_4.setBounds(704, 22, 100, 145);

        working_7.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/empty_card.jpg"))); // NOI18N
        working_7.setName("W7"); // NOI18N
        add(working_7);
        working_7.setBounds(704, 185, 100, 145);

        working_6.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/empty_card.jpg"))); // NOI18N
        working_6.setName("W6"); // NOI18N
        add(working_6);
        working_6.setBounds(592, 185, 100, 145);

        working_4.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/empty_card.jpg"))); // NOI18N
        working_4.setName("W4"); // NOI18N
        add(working_4);
        working_4.setBounds(368, 185, 100, 145);

        working_5.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/empty_card.jpg"))); // NOI18N
        working_5.setName("W5"); // NOI18N
        add(working_5);
        working_5.setBounds(480, 185, 100, 145);

        working_3.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/empty_card.jpg"))); // NOI18N
        working_3.setName("W3"); // NOI18N
        add(working_3);
        working_3.setBounds(256, 185, 100, 145);

        working_2.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/empty_card.jpg"))); // NOI18N
        working_2.setName("W2"); // NOI18N
        add(working_2);
        working_2.setBounds(144, 185, 100, 145);

        working_1.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/empty_card.jpg"))); // NOI18N
        working_1.setToolTipText("");
        working_1.setName("W1"); // NOI18N
        add(working_1);
        working_1.setBounds(32, 185, 100, 145);

        add(layered,0);
        layered.setBounds(0, 0, 850, 540);
    }

    /**
     * Quits current game
     */
    private void quit_game() {
        removeAll();
        revalidate();
        repaint();
    }

    /**
     * Operation undo mouse listener, determines last operation and revalidates frontend
     *
     */
    private void undoMouseClicked() {
        try {
            String source = backend.peekLastAction().getSource();
            String dest = backend.peekLastAction().getDestination();
            int type = backend.peekLastAction().getType();

            backend.undoLastAction();

            if (type != 0) {
                switch (source.substring(0, 1)) {
                    case "W":
                        revalidate_work_deck(source); break;
                    case "S":
                        revalidate_swap_deck(); break;
                    case "T":
                        revalidate_target_deck(source); break;
                }

                switch (dest.substring(0, 1)) {
                    case "S":
                        revalidate_swap_deck(); break;
                    case "W":
                        revalidate_work_deck(dest); break;
                    case "T":
                        revalidate_target_deck(dest); break;
                }
            } else {
                revalidate_new_deck();
            }
        }
        catch (java.lang.NullPointerException e){}
    }

    /**
     * Opens a window to choose a file to load from, rewrites backend entry
     *
     */
    private void loadMouseClicked() {
        int returnVal = fc.showOpenDialog(layered);

        if (returnVal == JFileChooser.APPROVE_OPTION) {
            File file = fc.getSelectedFile();
            Table newBackend = load_game(file.getName());
            if (newBackend != null){
                backend = newBackend;
                control = 5;
                revalidate_board();
                control = 0;
            }
            System.out.println("Opening: " + file.getAbsolutePath());
        } else {
            System.out.println("Open command cancelled by user.");
        }


    }

    /**
     * Mouse handler to quit game
     */
    private void quitMouseClicked(java.awt.event.MouseEvent evt) {
        quit_game();
    }


    /**
     * Initializes card instances, assigns mouse listeners and icons
     */
    private void init_gui_cards(){
        for(int i = 0; i < 52; i++) {
            card = backend.newCardDeck.get(i);
            card.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/card_cover.jpg")));
            card.addMouseListener(new java.awt.event.MouseAdapter() {
                public void mousePressed(java.awt.event.MouseEvent evt) {
                    mouse_pressed(evt);
                }
                public void mouseReleased(java.awt.event.MouseEvent evt) {
                    mouse_released(evt);
                }
            });
            card.addMouseMotionListener(new java.awt.event.MouseMotionAdapter() {
                public void mouseDragged(java.awt.event.MouseEvent evt) {
                    mouse_dragged(evt);
                }
            });

        }
    }

    /**
     * Creates new deck to draw from, sets it location and adds it to layered pane
     */
    private void create_new_deck(){
        for(int i = 0; i < 24; i++) {
            card = backend.newCardDeck.get(i);
            card.setBounds(32, 22, 100, 145);
            layered.add(card, 0);
        }
    }

    /**
     * Creates 7 working packs and sets their locations
     */
    private void create_working_packs(){
        backend.workingPacks = new workingPack[7];
        int x;
        int y;

        CardStack tmpstack = new CardStack(7);
        for (int i = 0; i < 7; i++) {
            x = 32 + i * 112;
            for (int j = 0; j < i + 1; j++) {
                y = 185 + j * 25;
                card = backend.newCardDeck.peek();
                card.setCLocation("W"+ (i+1));
                card.setBounds(x, y, 100, 145);
                layered.add(card, 0);
                tmpstack.put(backend.newCardDeck.pop());
            }
            backend.workingPacks[i] = new workingPack(tmpstack);
            backend.workingPacks[i].peek().turnFaceUp();
            backend.workingPacks[i].peek().setIcon(new javax.swing.ImageIcon
                    (getClass().getResource(backend.workingPacks[i].peek().getPath())));
        }
    }

    /**
     * Saves current game to .dat file as a compiled class
     *
     * @param tableToSave - Class Table to be saved
     */
    private boolean saveGame(Table tableToSave){
        try {
            Calendar cal = Calendar.getInstance();
            SimpleDateFormat sdf = new SimpleDateFormat("HH-mm-ss");
            FileOutputStream fileOut = new FileOutputStream("game_from_" + sdf.format(cal.getTime()) +".dat");
            ObjectOutputStream out = new ObjectOutputStream(fileOut);
            out.writeObject(tableToSave);
            out.close();
            fileOut.close();
            JOptionPane.showMessageDialog(this, "Game has been saved to: game_from_" + sdf.format(cal.getTime()) +".dat");
            System.out.printf("Serialized data is saved in game_from_" + sdf.format(cal.getTime()) +".dat");
            return true;

        } catch (IOException i) {

            i.printStackTrace();
            return  false;
        }
    }

    /**
     * Revalidates card after loading new game, sets it mouse listeners and adds it to layered pane
     *
     * @param c - card to be revalidated
     */
    private void revalidate_card(Card c){
            layered.add(c);
            c.addMouseListener(new java.awt.event.MouseAdapter() {
                public void mousePressed(java.awt.event.MouseEvent evt) {
                    mouse_pressed(evt);
                }
                public void mouseReleased(java.awt.event.MouseEvent evt) {
                    mouse_released(evt);
                }
            });
            c.addMouseMotionListener(new java.awt.event.MouseMotionAdapter() {
                public void mouseDragged(java.awt.event.MouseEvent evt) {
                    mouse_dragged(evt);
                }
            });
    }

    /**
     * Loads compiled class from a .dat file
     *
     * @param fileToRead - string representation of file name
     *
     * @return loaded game represented as Table class
     */
    private Table load_game(String fileToRead) {
        try {
            FileInputStream fileIn = new FileInputStream(fileToRead);
            ObjectInputStream in = new ObjectInputStream(fileIn);
            Table loadedGame = (Table) in.readObject();
            in.close();
            fileIn.close();
            return loadedGame;
        } catch (IOException i) {
            JOptionPane.showMessageDialog(this, "Not a game file. Please Choose a valid game file.");
            return null;
        } catch (ClassNotFoundException c) {
            JOptionPane.showMessageDialog(this, "File is not found.");
            return null;
        }
    }

    /**
     * Function that transforms string to backend stack
     *
     * @param aux - String representation of stack
     *
     * @return backend representation of given stack
     */
    private JLabel deck_location(String aux) {
        JLabel tmp = swap_deck;

        if(aux.equals("T1"))
            tmp = target_1;
        if(aux.equals("T2"))
            tmp = target_2;
        if(aux.equals("T3"))
            tmp = target_3;
        if(aux.equals("T4"))
            tmp = target_4;
        if(aux.equals("W1"))
            tmp = working_1;
        if(aux.equals("W2"))
            tmp = working_2;
        if(aux.equals("W3"))
            tmp = working_3;
        if(aux.equals("W4"))
            tmp = working_4;
        if(aux.equals("W5"))
            tmp = working_5;
        if(aux.equals("W6"))
            tmp = working_6;
        if(aux.equals("W7"))
            tmp = working_7;


        if(aux.equals("Mouse")) {
            Point p = getMousePosition();
            int x = p.x;
            int y = p.y;

            if (x >= target_1.getBounds().x && x <= target_1.getBounds().x + 100 &&
                    y >= target_1.getBounds().y && y <= target_1.getBounds().y + 200)
                tmp = target_1;


            if (x >= target_2.getBounds().x && x <= target_2.getBounds().x + 100 &&
                    y >= target_2.getBounds().y && y <= target_2.getBounds().y + 200)
                tmp = target_2;


            if (x >= target_3.getBounds().x && x <= target_3.getBounds().x + 100 &&
                    y >= target_3.getBounds().y && y <= target_3.getBounds().y + 200)
                tmp = target_3;


            if (x >= target_4.getBounds().x && x <= target_4.getBounds().x + 100 &&
                    y >= target_4.getBounds().y && y <= target_4.getBounds().y + 200)
                tmp = target_4;

            if (x >= working_1.getBounds().x && x <= working_1.getBounds().x + 100 &&
                    y >= working_1.getBounds().y && y <= working_1.getBounds().y + 400)
                tmp = working_1;

            if (x >= working_2.getBounds().x && x <= working_2.getBounds().x + 100 &&
                    y >= working_2.getBounds().y && y <= working_2.getBounds().y + 400)
                tmp = working_2;

            if (x >= working_3.getBounds().x && x <= working_3.getBounds().x + 100 &&
                    y >= working_3.getBounds().y && y <= working_3.getBounds().y + 400)
                tmp = working_3;

            if (x >= working_4.getBounds().x && x <= working_4.getBounds().x + 100 &&
                    y >= working_4.getBounds().y && y <= working_4.getBounds().y + 400)
                tmp = working_4;

            if (x >= working_5.getBounds().x && x <= working_5.getBounds().x + 100 &&
                    y >= working_5.getBounds().y && y <= working_5.getBounds().y + 400)
                tmp = working_5;

            if (x >= working_6.getBounds().x && x <= working_6.getBounds().x + 100 &&
                    y >= working_6.getBounds().y && y <= working_6.getBounds().y + 400)
                tmp = working_6;

            if (x >= working_7.getBounds().x && x <= working_7.getBounds().x + 100 &&
                    y >= working_7.getBounds().y && y <= working_7.getBounds().y + 400)
                tmp = working_7;
        }

        return tmp;
    }

    /**
     * Revalidates whole frontend board, removing all cards
     * and revalidating each deck from backend, adding new cards to their locations
     */
    private void revalidate_board() {
        Component[] components = layered.getComponents();
        Component component;
        for (int i = 0; i < components.length; i++) {
            component = components[i];
            if (component instanceof Card) {
                layered.remove(component);
            }
        }
        layered.revalidate();
        layered.repaint();
        for(int i = 1; i <= 7; i++)
            revalidate_work_deck("W"+i);
        for(int j = 1; j <= 4; j++)
            revalidate_target_deck("T"+j);
        revalidate_swap_deck();
        revalidate_new_deck();
    }

    /**
     * Revalidates working deck from backend, setting new icons, position, etc.
     * Control variable indicates that processed card should be revalidated.
     *
     * @param location - String representation of stack location
     */
    private void revalidate_work_deck(String location){
        CardDeck cd = backend.getStackByCardLocation(location);

        int i = 0;
        int y = 0;
        JLabel deck = deck_location(location);

        for (Card c : cd.deckStack){
            y = 185 + i++ * 25;

            System.out.println("Karta "+c.value()+c.color()+ " sa nachadza na "+location+" je otocena hore: "+c.isTurnedFaceUp());

            if(c.isTurnedFaceUp())
                c.setIcon(new javax.swing.ImageIcon(getClass().getResource(c.getPath())));
            else
                c.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/card_cover.jpg")));

            c.setBounds(deck.getX(), y, 100, 145);
            c.setCLocation(location);
            if(control == 5)
                revalidate_card(c);
            layered.moveToFront(c);
        }
    }


    /**
     * Revalidates target deck from backend, setting position and icons
     * Control variable indicates that processed card should be revalidated.
     *
     * @param location - String representation of stack location
     */
    private void revalidate_target_deck(String location){
        CardDeck cd = backend.getStackByCardLocation(location);

        JLabel deck = deck_location(location);

        for (Card c : cd.deckStack){
            c.setBounds(deck.getBounds());
            c.setIcon(new javax.swing.ImageIcon(getClass().getResource(c.getPath())));
            c.setCLocation(location);
            if(control == 5)
                revalidate_card(c);
            layered.moveToFront(c);
        }
    }

    /**
     * Revalidates new deck from backend, setting position and icons
     * Control variable indicates that processed card should be revalidated.
     */
    private void revalidate_new_deck() {

        CardDeck cd = backend.getStackByCardLocation("ND");

        for (Card c : cd.deckStack){
            c.setBounds(card_deck.getBounds());
            c.setCLocation("ND");
            c.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/card_cover.jpg")));
            if(control == 5)
                revalidate_card(c);
            layered.moveToBack(c);
        }
    }

    /**
     * Revalidates swap deck from backend, setting position and icons
     * Control variable indicates that processed card should be revalidated.
     */
    private void revalidate_swap_deck() {

        CardDeck cd = backend.getStackByCardLocation("SD");

        for (Card c : cd.deckStack) {
            c.setBounds(swap_deck.getBounds());
            c.setIcon(new javax.swing.ImageIcon(getClass().getResource(c.getPath())));
            c.setCLocation("SD");
            if(control == 5)
                revalidate_card(c);
            layered.moveToFront(c);
        }
    }

    /**
     * Handles operations when mouse is released from JLabel
     * Determines whether source of mouse event is Card class
     * or JLabel class. If it is card, determines what is source
     * and destination location and calls according backend methods
     * to handle given situation. After each successful backend operation
     * destination and optionally source stacks are revalidated.
     * If source of mouse is JLabel class, evt comes from empty new deck,
     * which tells swap deck to be rotated back to new deck.
     *
     * @param evt - mouse event
     */
    private void mouse_released(java.awt.event.MouseEvent evt) {

        if(evt.getSource().getClass().equals(card.getClass())) { /*If it is card */


            boolean SW_to_T = false;
            boolean ST_to_W = false;
            boolean W_to_W = false;

            int typeRecord = 1;

            Card element = (Card) evt.getSource();
            layered.moveToFront(element);

            String src_letter = element.getCLocation().substring(0,1);
            String dst_string = deck_location("Mouse").getName();
            String dst_letter = deck_location("Mouse").getName().substring(0,1);


            if(!element.getCLocation().equals("ND")) { /* If card is not from New Deck*/

                System.out.println("Miesto na ktore bol release: " + deck_location("Mouse").getName());
                try {
                    if (!backend.getStackByCardLocation(element.getCLocation()).get(1).isTurnedFaceUp())
                        typeRecord = 2;
                }
                catch (java.lang.ArrayIndexOutOfBoundsException e){}

                /*Swap or Working to Target*/
                if ((src_letter.equals("S") || src_letter.equals("W")) && dst_letter.equals("T")) {
                    SW_to_T = backend.putToTargetStack(element, deck_location("Mouse").getName());
                }

                /*Swap or Target to Working*/
                if ((src_letter.equals("S") || src_letter.equals("T")) && dst_letter.equals("W")) {
                    ST_to_W = backend.putFromTargetSwapToWorkingPack(element, deck_location("Mouse").getName());
                }


                /* Working to working*/
                if (src_letter.equals("W") && dst_letter.equals("W")) {
                    W_to_W = backend.putFromWorkingToWorkingPack(element, deck_location("Mouse").getName());
                }

                if (element.getCLocation().substring(0, 1).equals("W")){
                    revalidate_work_deck(element.getCLocation()); /* Card pulled from Work deck */
                }

                if(SW_to_T){
                    backend.recordAction(element, element.getCLocation(), dst_string, typeRecord);
                    revalidate_target_deck(deck_location("Mouse").getName());
                    control = 3;
                }

                if(ST_to_W || W_to_W) {
                    backend.recordAction(element, element.getCLocation(), dst_string, typeRecord);
                    revalidate_work_deck(deck_location("Mouse").getName());
                    control = 3;
                }


            }

            if (element.getCLocation().equals("ND")) { /* If card is located on New Deck*/
                boolean D_to_S = false;

                D_to_S = backend.pullFromDrawStack();
                if(D_to_S){
                    backend.recordAction(null, null, null, 0);
                    revalidate_swap_deck();
                    control = 2;
                }
            }

            if (control != 2 && control != 3)

                if(Cards_to_move.size() > 1) {
                    revalidate_work_deck(element.getCLocation());
                    /*int i = 0;
                    for (Card c : Cards_to_move) {
                        i++;
                        c.setLocation(this.getMousePosition().x - delta_x, this.getMousePosition().y - delta_y + i * 25);
                        layered.setLayer(c, i + 50);
                    }*/
                } else
                element.setLocation(cord_x, cord_y);
            control = 0;


        }
        else if(evt.getSource().getClass().equals(card_deck.getClass())){
            boolean D_to_S = backend.pullFromDrawStack();
            if(!D_to_S){
                revalidate_new_deck();
                control = 2;
            }
        }
        if (backend.isGameWon()){
            JOptionPane.showMessageDialog(this, "You have won the game!");
        }
    }

    /**
     * Handler for dragged Card, determines whether card is facing up or down.
     * If card is facing up, it dynamically moves card along board. If facing-up
     * card has another card above, this stack of card is dragged.
     *
     * @param evt - mouse event
     */
    private void mouse_dragged(java.awt.event.MouseEvent evt) {
        try {
            int i = 0;
            Card element = (Card)evt.getSource();
            layered.moveToFront(element);
            if(element.isTurnedFaceUp()) {
                if (control == 0) {
                    layered.moveToFront(element);
                    delta_x = this.getMousePosition().x - cord_x;
                    delta_y = this.getMousePosition().y - cord_y;
                }
                control = 1;

                if(Cards_to_move.size() > 1) {
                    for (Card c : Cards_to_move) {
                        i++;
                        c.setLocation(this.getMousePosition().x - delta_x, this.getMousePosition().y - delta_y + i * 25);
                        layered.moveToFront(c);
                    }
                } else {
                    element.setLocation(this.getMousePosition().x - delta_x, this.getMousePosition().y - delta_y);
                }

                /*element.setLocation(this.getMousePosition().x - delta_x, this.getMousePosition().y - delta_y);
                if (element.getX() <= 0) // LAVA STENA
                    element.setLocation(0, this.getMousePosition().y - delta_y);

                if (element.getY() <= 0) // HORNA STENA
                    element.setLocation(this.getMousePosition().x - delta_x, 0);

                if (element.getY() <= 0 && element.getX() <= 0) // LAVY HORNY OKRAJ
                    element.setLocation(0, 0);

                if (element.getX() + 118 >= layered.getWidth()) // PRAVA STENA
                    element.setLocation(layered.getWidth() - 118, this.getMousePosition().y - delta_y);

                if (element.getY() + 100 >= layered.getHeight() - 103) // DOLNA STENA
                    element.setLocation(this.getMousePosition().x - delta_x, layered.getHeight() - 205);

                if (element.getX() + 115 >= layered.getWidth() && element.getY() + 100 >= layered.getHeight() - 115) // PRAVY DOLNY OKRAJ
                    element.setLocation(layered.getWidth() - 118, layered.getHeight() - 205);

                if (element.getX() + 118 >= layered.getWidth() && element.getY() <= 0) // PRAVY HORNY OKRAJ
                    element.setLocation(layered.getWidth() - 118, 0);

                if (element.getX() <= 0 && element.getY() + 100 >= layered.getHeight() - 115) // LAVY DOLNY OKRAJ
                    element.setLocation(0, layered.getHeight() - 205);*/
            }
        }

        catch (java.lang.NullPointerException e){

        }


    }

    /**
     * Handler when Card is pressed, remembers location of coordinates
     * and stack from which card comes, if pressed card has facing-up card
     * above it, both are added to ArrayList for later operations.
     *
     * @param evt - mouse event
     */
    private void mouse_pressed(java.awt.event.MouseEvent evt) {

        Cards_to_move.clear();
        Card element = (Card)evt.getSource();
        layered.moveToFront(element);
        String location = element.getCLocation();
        int card_y = element.getBounds().y;
        cord_x = element.getX(); // remember where the element was placed on X
        cord_y = element.getY(); // remember where the element was placed on Y


        if(element.isTurnedFaceUp() && element.getCLocation().substring(0,1).equals("W")) {
            Component[] components = layered.getComponents();
            Component component;
            for (int i = 0; i < components.length; i++) {
                component = components[i];
                if (component instanceof Card) {
                    if (((Card) component).getCLocation().equals(location) && component.getBounds().y >= card_y) {
                        System.out.println("Pressed karta: "+((Card) component).value()+""+((Card) component).color());
                        Cards_to_move.add((Card) component);
                    }
                }
            }
            System.out.println(Cards_to_move);
        }
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JLabel card_deck;
    private javax.swing.JLayeredPane layered;
    private javax.swing.JLabel load;
    private javax.swing.JLabel quit;
    private javax.swing.JLabel reset;
    private javax.swing.JLabel save;
    private javax.swing.JLabel swap_deck;
    private javax.swing.JLabel target_1;
    private javax.swing.JLabel target_2;
    private javax.swing.JLabel target_3;
    private javax.swing.JLabel target_4;
    private javax.swing.JLabel undo;
    private javax.swing.JLabel working_1;
    private javax.swing.JLabel working_2;
    private javax.swing.JLabel working_3;
    private javax.swing.JLabel working_4;
    private javax.swing.JLabel working_5;
    private javax.swing.JLabel working_6;
    private javax.swing.JLabel working_7;
    // End of variables declaration//GEN-END:variables
}
