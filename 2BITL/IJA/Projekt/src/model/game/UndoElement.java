/**
 * @file UndoElement.java.
 *
 * @author xuhlia03
 *
 * Implements the undo element class.
 */

package model.game;

import model.cardpack.*;


public class UndoElement implements java.io.Serializable{
    private Card movedCard;
    private String source;
    private String destination;
    private int type;

    /**
     * Constructor.
     *
     * Create new undo element that is used for undo operations.
     *
     * @param movedCard A moved card element.
     * @param source   A move source deck or stack.
     * @param dest   A move destination deck or stack.
     * @param type   A type of undo operation.
     */
    public UndoElement(Card movedCard, String source, String dest, int type ){
        this.movedCard      = movedCard;
        this.source         = source;
        this.destination    = dest;
        this.type           = type;
    }

    /**
     * Return pointer to card that has been moved.
     *
     * @return  A type of undo operation.
     */
    public Card getThisCard(){
        return this.movedCard;
    }

    /**
     * Return string representation of the source deck that card that has been moved.
     *
     * @return  A string representation of source deck.
     */
    public String getSource(){
        return this.source;
    }

    /**
     * Return string representation of the destination deck that card that has been moved.
     *
     * @return  A string representation of destination deck.
     */
    public String getDestination(){
        return this.destination;
    }

    /**
     * Return type of undo operation.
     *
     * @return  A type of undo operation.
     */
    public int getType(){
        return this.type;
    }
}
