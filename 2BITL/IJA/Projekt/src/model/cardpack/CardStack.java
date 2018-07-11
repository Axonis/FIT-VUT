/**
 * @file CardStack.java.
 *
 * @author xuhlia03
 *
 * Implements the card stack class.
 */

package model.cardpack;

import java.util.*;


public class CardStack extends CardDeck{

    /**
     * Constructor of a card stack.
     *
     * @param size The size of new card stack.
     */
    public CardStack(int size){
        super(size);
    }
    /**
     * Get a card from top the stack.
     *
     * @return  A card from top of the deck with pulling away.
     */
    public Card pop(){
        return this.deckStack.pop();
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
    /**
     * Get a actual size of the deck.
     *
     * @return  A card from top of the deck with pulling away.
     */
    public int size(){
        return deckStack.size();
    }
    /**
     * Get a card from top the stack by card.
     *
     * @param  card A card from the stack.
     *
     * @return  A stack of cards from top of the stack with pulling away.
     */
    public CardStack pop(Card card){
        LinkedList<Card> S1 = new LinkedList<>();


        do{
            S1.add(this.deckStack.pop());
        }while(!card.equals(S1.getLast()));


        Collections.reverse(S1);

        CardStack tmpClassStack = new CardStack(S1.size());

        for (Card actualCard : S1){
            tmpClassStack.put(actualCard);
        }

        return tmpClassStack;
    }
    /**
     * Put a stack of cards to the stack.
     *
     * @param  stack A stack of cards to put to the stack.
     *
     * @return  A result of action.
     */
    public boolean put(CardStack stack){
        if (stack.isEmpty() || (this.deckStack.size() + stack.size() > this.size)){
            return false;
        }
        else {
            LinkedList<Card> S1 = new LinkedList<>();

            while (!stack.isEmpty()) {
                S1.add(stack.pop());
            }

            Collections.reverse(S1);

            for (Card actualCard : S1) {
                stack.put(actualCard);
            }
            return true;
        }
    }



    @Override
    public boolean equals(Object other){
        if (this.deckStack == other) return true;
        if (this.deckStack == null || other == null) return false;
        if (this.deckStack.size() != ((CardStack)other).size()) return false;

        ArrayList<Card> S1 = new ArrayList<>();
        ArrayList<Card> S2 = new ArrayList<>();

        while(!this.deckStack.isEmpty()){
            S1.add(this.deckStack.pop());
            S2.add(((CardStack)other).pop());
        }

        Collections.reverse(S1);
        Collections.reverse(S2);

        for (Card actualCard : S1){
            this.deckStack.push(actualCard);
        }

        for (Card actualCard : S2){
            ((CardStack)other).put(actualCard);
        }

        for (int i = 0; i < S1.size(); i++){
            if (S1.get(i).hashCode() != S2.get(i).hashCode()){
                return false;
            }
        }
        return true;
    }
}
