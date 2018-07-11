/**
 * @file targetDeck.java.
 *
 * @author xuhlia03
 *
 * Implements the target deck class.
 */

package model.cardpack;

public class targetDeck extends CardDeck{

    public Card.Card_Color targetDeckColor;

    public targetDeck(Card.Card_Color color){
        super(13);
        this.targetDeckColor = color;
    }

    @Override
    public boolean put(Card card){
        if (card.color() == this.targetDeckColor){
            if (this.deckStack.isEmpty()) {
                if (card.value() == 1) {
                    this.deckStack.push(card);
                    return true;
                }
                return false;
            }
            else{
                if ((card.value() - this.deckStack.peek().value()) == 1) {
                    this.deckStack.push(card);
                    return true;
                }
                return false;
            }
        }
        return false;
    }
}
