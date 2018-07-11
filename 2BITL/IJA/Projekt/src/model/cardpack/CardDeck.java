/**
 * @file CardDeck.java.
 *
 * @author xuhlia03
 *
 * Implements the card deck class.
 */

package model.cardpack;

import java.util.Stack;

public class CardDeck implements java.io.Serializable{

    public Stack<Card> deckStack = new Stack<>();
    protected int size;
    /**
     * Constructor of a card deck.
     *
     * @param size The size of new card deck.
     */
    public CardDeck(int size){
        this.size = size;
    }
    /**
     * Get a card from top the deck.
     *
     * @return  A card from top of the deck without pulling away.
     */
    public Card peek(){
        return this.deckStack.peek();
    }
    /**
     * Get a actual size of the deck.
     *
     * @return  A card from top of the deck with pulling away.
     */
    public int size(){
        return this.deckStack.size();
    }
    /**
     * Get a card from top the deck.
     *
     * @return  A card from top of the deck with pulling away.
     */
    public Card pop(){
        return this.deckStack.pop();
    }

    /**
     * Get a card from top the deck by index.
     *
     * @param  index A index from top of the deck.
     *
     * @return  A card from top of the deck with pulling away.
     */
    public Card get(int index){
        if (this.deckStack.isEmpty()){
            return null;
        }
        else if(index < 0 ) {
            return null;
        }
        else {
            return this.deckStack.get(index);

        }
    }
    /**
     * Put a card to the deck.
     *
     * @param  card A card to put to the deck.
     *
     * @return  A result of action.
     */
    public boolean put(Card card){
        if (this.deckStack.size() < this.size){
            this.deckStack.push(card);
            return true;
        }
        else {
            return false;
        }
    }

    /**
     * Put a card to the deck with breaking game rules.
     * This function is used when undo operation is called.
     *
     * @param  card A card to put to the deck.
     *
     */
    public void forcePut(Card card){
        this.deckStack.push(card);
    }
    /**
     * Determine if deck is empty or not.
     *
     * @return  A result of action.
     *
     */
    public boolean isEmpty(){
        return this.deckStack.isEmpty();
    }

}
