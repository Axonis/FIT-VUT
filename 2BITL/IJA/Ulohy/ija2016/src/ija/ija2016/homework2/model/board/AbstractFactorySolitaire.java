package ija.ija2016.homework2.model.board;

import ija.ija2016.homework2.cardpack.Card;
import ija.ija2016.homework2.cardpack.CardDeck;
import ija.ija2016.homework2.cardpack.CardStack;

public abstract class AbstractFactorySolitaire {

    abstract public Card createCard(Card.Color color, int value);
    abstract public CardDeck createCardDeck();
    abstract public CardDeck createTargetPack (Card.Color color);
    abstract public CardStack createWorkingPack();

}
