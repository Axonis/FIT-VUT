package ija.ija2016.homework2.model.cards;


public interface CardDeck {
    int size();
    boolean put(Card card);
    Card pop();
    Card get();
    Card get(int index);
    boolean isEmpty();

}
