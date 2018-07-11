package ija.ija2016.homework1.cardpack;

import java.util.ArrayList;

public class CardDeck {

    private int size;
    private ArrayList<Card> cards_array = new ArrayList<>();

    private CardDeck(int size) {
        this.size = size;
    }

    // Creates standard deck of 52 cards
    public static CardDeck createStandardDeck() {
        CardDeck tmp_array = new CardDeck(52);

        // Iterates over enum values and cast them to string
        for (Card.Color color : Card.Color.values()) {
            for (int i = 1; i <= 13; i++) {
                tmp_array.cards_array.add(new Card(color, i));
            }
        }
        return tmp_array;
    }

    // Number of card in the deck
    public int size() {
        return cards_array.size();
    }

    // Puts card on top of the deck
    public void put(Card card) {
        cards_array.add(card);
    }

    // Removes and returns card on the top of the deck
    public Card pop() {
        if (cards_array.size() == 0) {
            return null;
        }

        Card tmp_card = cards_array.get(cards_array.size() - 1);
        cards_array.remove(cards_array.size() - 1);
        return tmp_card;
    }
}