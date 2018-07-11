/**
 * @file FactoryKlondike.java.
 *
 * @author xuhlia03
 *
 * Implements the factory class.
 */

package model.board;

import model.cardpack.*;

import java.util.ArrayList;
import java.util.Collections;


public class FactoryKlondike extends AbstractFactorySolitaire {
    /**
     * Create a new card.
     *
     * @param color A new card color.
     * @param value A new card value.
     *
     * @return A pointer to the new card.
     */
    public Card createCard(Card.Card_Color color, int value) {
        if (value > 0 && value < 14) {
            switch (color) {
                case SPADES:
                case CLUBS:
                case HEARTS:
                case DIAMONDS:
                    return new Card(color, value);
                default:
                    return null;
            }
        } else {
            return null;
        }
    }
    /**
     * Create a new card deck.
     *
     * @return A pointer to the new card deck.
     */
    public CardDeck createCardDeck() {
        ArrayList<Card> S1 = new ArrayList<>();
        CardDeck newCardDeck = new CardDeck(52);
        for (Card.Card_Color i : Card.Card_Color.values() ) {
            for (int j = 1; j < 14; j++) {
                S1.add(new model.cardpack.Card(i, j));
            }
        }
        Collections.shuffle(S1);

        for (Card actualCard : S1){
            newCardDeck.put(actualCard);
        }

        return newCardDeck;
    }

    /**
     * Create new card deck.
     *
     * @param color A new card color.
     *
     * @return A pointer to the new card deck.
     */
    public CardDeck createTargetPack(Card.Card_Color color) {
        return new targetDeck(color);
    }
    /**
     * Create a new card stack.
     *
     * @param cardsToAdd Cards to add.
     *
     * @return A pointer to the new card stack.
     */
    public workingPack createWorkingPack(CardStack cardsToAdd) {
        return new workingPack(cardsToAdd);
    }
}