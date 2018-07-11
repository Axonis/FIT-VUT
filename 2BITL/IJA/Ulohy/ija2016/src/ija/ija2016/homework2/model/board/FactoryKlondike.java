package ija.ija2016.homework2.model.board;

import ija.ija2016.homework2.cardpack.Card;
import ija.ija2016.homework2.cardpack.CardDeck;
import ija.ija2016.homework2.cardpack.CardStack;

public class FactoryKlondike extends AbstractFactorySolitaire {

    public FactoryKlondike() {

    }

    public Card createCard(Card.Color color, int value) {
        if (value > 0 && value < 14)
            return new Card(color, value);
        return null;
    }

    public CardDeck createCardDeck() {
        return CardDeck.createStandardDeck();
    }

    public CardDeck createTargetPack(Card.Color color) {
        return new targetPack(color);
    }

    public CardStack createWorkingPack() {
        return new workingPack();
    }

}
