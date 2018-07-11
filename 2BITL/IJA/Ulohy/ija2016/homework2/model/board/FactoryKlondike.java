package ija.ija2016.homework2.model.board;

import ija.ija2016.homework2.cardpack.Card;
import ija.ija2016.homework2.cardpack.CardDeck;
import ija.ija2016.homework2.cardpack.CardStack;

public class FactoryKlondike extends AbstractFactorySolitaire{

    public FactoryKlondike() {

    }

    public Card createCard(Card.Color color, int value) {
        if(value > 0 && value < 14)
            return new Card(color, value);
        return null;
    }

    public CardDeck createCardDeck() {
        CardDeck tmp_deck = new CardDeck(52);
        for (Card.Color color : Card.Color.values()) {
            for (int i = 1; i <= 13; i++)
                tmp_deck.put(createCard(color,i));
        }
        return tmp_deck;
    }

    public CardDeck createTargetPack(Card.Color color) {
        return new targetStack(color);
    }

    public CardStack createWorkingPack() {
        return new workingStack();
    }

}
