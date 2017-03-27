package ija.ija2016.homework1.cardpack;

import java.io.Serializable;

public class Card {

    private Card.Color c;
    private int value;

    public Card(Card.Color c, int value) {
        this.c = c;
        this.value = value;
    }

    @Override
    public String toString() {
        switch (value) {
            case 1:
                return "A(" + c.toString() + ")";
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
                return "" + value + "(" + c.toString() + ")";
            case 11:
                return "J(" + c.toString() + ")";
            case 12:
                return "Q(" + c.toString() + ")";
            case 13:
                return "K(" + c.toString() + ")";
            default:
                throw new IllegalArgumentException();
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
        return c;
    }

    public static enum Color implements Serializable, Comparable<Color> {
        CLUBS,
        DIAMONDS,
        HEARTS,
        SPADES;

        @Override
        public String toString() {
            switch (this) {
                case CLUBS:
                    return "C";
                case DIAMONDS:
                    return "D";
                case HEARTS:
                    return "H";
                case SPADES:
                    return "S";
                default:
                    throw new IllegalArgumentException();
            }
        }
    }


}


