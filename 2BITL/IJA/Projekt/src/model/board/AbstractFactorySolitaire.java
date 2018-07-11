/**
 * @file AbstractFactorySolitaire.java.
 *
 * @author xuhlia03
 *
 * Implements the abstract class for factory.
 */
package model.board;

import model.cardpack.Card;
import model.cardpack.CardDeck;
import model.cardpack.CardStack;

public abstract class AbstractFactorySolitaire {
    /**
     * Create a new card.
     *
     * @param color A new card color.
     * @param value A new card value.
     *
     * @return A pointer to the new card.
     */
    public abstract Card createCard(Card.Card_Color color, int value);
    /**
     * Create a new card deck.
     *
     * @return A pointer to the new card deck.
     */
    public abstract CardDeck createCardDeck();
    /**
     * Create new card deck.
     *
     * @param color A new card color.
     *
     * @return A pointer to the new card deck.
     */
    public abstract CardDeck createTargetPack(Card.Card_Color color);
    /**
     * Create a new card stack.
     *
     * @param cardsToAdd Cards to add.
     *
     * @return A pointer to the new card stack.
     */
    public abstract CardStack createWorkingPack(CardStack cardsToAdd);

}
