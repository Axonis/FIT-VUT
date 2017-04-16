package ija.ija2016.homework2.model.cards;

public interface CardStack extends CardDeck {

    boolean put(CardStack stack);

    CardStack pop(Card card);
}
