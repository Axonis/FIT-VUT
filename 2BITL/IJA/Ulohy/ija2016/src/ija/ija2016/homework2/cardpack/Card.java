package ija.ija2016.homework2.cardpack;

import static java.lang.Math.abs;

public class Card implements ija.ija2016.homework2.model.cards.Card {

    private Card.Color card_color;
    private int value;
    private boolean card_state = false;

    public Card(Card.Color c, int value) {
        if (value < 1 || value > 13) {
            throw new IllegalArgumentException();
        }

        this.card_color = c;
        this.value = value;
    }

    @Override
    public String toString() {
        switch (value) {
            case 1:
                return "A(" + card_color.toString() + ")";
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
                return "" + value + "(" + card_color.toString() + ")";
            case 11:
                return "J(" + card_color.toString() + ")";
            case 12:
                return "Q(" + card_color.toString() + ")";
            case 13:
                return "K(" + card_color.toString() + ")";
            default:
                return null;
        }
    }

    @Override
    public int hashCode() {
        return value;
    }

    @Override
    public boolean equals(Object o) {
        if (o != null && o instanceof Card) {
            if (((Card) o).toString().equals(this.toString()))
                return true;
        }
        return false;
    }

    public int value() {
        return value;
    }

    public Card.Color color() {
        return card_color;
    }

    public boolean similarColorTo(ija.ija2016.homework2.model.cards.Card c) {
        return this.color().similarColorTo(c.color());
    }

    public boolean isTurnedFaceUp() {
        return this.card_state;
    }

    public boolean turnFaceUp() {
        // skratkove vyhodnocovanie
        return (!card_state && (card_state = true));
    }

    public int compareValue(ija.ija2016.homework2.model.cards.Card c) {
        return abs(c.value() - this.value());
    }
}


