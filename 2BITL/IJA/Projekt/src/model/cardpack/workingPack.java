/**
 * @file CardDeck.java.
 *
 * @author xuhlia03
 *
 * Implements the card deck class.
 */

package model.cardpack;

import java.util.Collections;
import java.util.LinkedList;

public class workingPack extends CardStack{
    /**
     * Get a stack of card from working pack with pulling away from pack.
     *
     * @param  cardFromWorkingPack A card from pack pop.
     *
     * @return  A stack of cards that was pulled away from pack.
     */
    public CardStack getByCard(Card cardFromWorkingPack){
        int pos = this.deckStack.search(cardFromWorkingPack);
        if (pos == -1)
            return null;
        CardStack returnStack = new CardStack(13);
        int size = deckStack.size();
        for(int i = size - pos; i < size; i++){
           returnStack.put(this.deckStack.get(i));
        }
        for(int i = size - pos; i < size; i++){
            this.deckStack.pop();
        }
        return returnStack;
    }

    /**
     * Constructor of a working pack.
     *
     * @param cardsToAdd cards to add.
     */
    public workingPack(CardStack cardsToAdd){
        super(19);
        LinkedList<Card> S1 = new LinkedList<>();

        while (!cardsToAdd.isEmpty()) {
            S1.add(cardsToAdd.pop());
        }
        Collections.reverse(S1);
        for (Card actualCard : S1) {
            this.deckStack.push(actualCard);
        }

    }
    /**
     * Put a card to the pack.
     *
     * @param  card A stack of cards to put to the pack.
     *
     * @return  A result of action.
     */
    @Override
    public boolean put(Card card) {
        if (this.deckStack.isEmpty()) {
            if (card.value() == 13) {
                this.deckStack.push(card);
                return true;
            }
            return false;
        }
        else if ((card.value() - this.deckStack.peek().value()) == -1 &&
                !card.similarColorTo(this.deckStack.peek())   ) {

            this.deckStack.push(card);
            return true;
        }
        else {
            return false;
        }

    }
    /**
     * Put a stack of cards to the pack.
     *
     * @param  stack A stack of cards to put to the pack.
     *
     * @return  A result of action.
     */
    @Override
    public boolean put(CardStack stack) {
        if ((this.deckStack.size() + stack.size() > this.size) ||
                stack.size() == 0 ){
            return false;
        }
        else{

            LinkedList<Card> S1 = new LinkedList<>();

            while (!stack.isEmpty()) {
                S1.add(stack.pop());
            }

            Collections.reverse(S1);

            for (Card actualCard : S1) {
                stack.put(actualCard);
            }

            if (this.deckStack.isEmpty() ) {
                if(S1.getFirst().value() == 13) {
                    for (Card actualCard : S1) {
                        this.deckStack.push(actualCard);
                    }
                    return true;
                }
                else {
                    return false;
                }
            }
            else if (((S1.getFirst().value() - this.deckStack.peek().value()) == -1) &&
                    (!S1.getFirst().similarColorTo(this.deckStack.peek()))) {
                for (Card actualCard : S1) {
                    this.deckStack.push(actualCard);
                }
                return true;
            }
            else {

                return false;
            }
        }
    }


}
