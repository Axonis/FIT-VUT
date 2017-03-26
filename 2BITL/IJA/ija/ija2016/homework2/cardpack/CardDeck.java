package ija.ija2016.homework2.cardpack;

import java.util.Stack;
import ija.ija2016.homework2.model.cards.Card;

public class CardDeck implements ija.ija2016.homework2.model.cards.CardDeck{

    protected int size;
    protected Card.Color color;
    protected Stack<Card> stack_cards = new Stack<>();

    public CardDeck(int size) {
        this.size = size;
    }

    // Creates standard deck of 52 cards
    private static CardDeck createStandardDeck() {
        CardDeck tmp_stack = new CardDeck(52);

        // Iterates over enum values and cast them to string
        for (Card.Color color : Card.Color.values()) {
            for (int i = 1; i <= 13; i++) {
                tmp_stack.put(new ija.ija2016.homework2.cardpack.Card(color, i));
            }
        }
        return tmp_stack;
    }

    // Number of cards in the deck
    public int size() {
        return stack_cards.size();
    }


    // Puts card on top of the deck
    public boolean put(Card card) {
        if (stack_cards.size() < size) {
            stack_cards.push(card);
            return true;
        } else
            return false;
    }

    // Removes and returns card on the top of the deck
    public Card pop() {
        if (stack_cards.size() == 0) {
            return null;
        }
        return stack_cards.pop();
    }

    public Card get(){
        if(!stack_cards.isEmpty())
            return stack_cards.peek();
        else
            return null;
    }

    public Card get(int index) {
        if(!stack_cards.isEmpty())
            return stack_cards.get(index);
        else
            return null;
    }

    public boolean isEmpty() {
        return this.stack_cards.isEmpty();
    }
}