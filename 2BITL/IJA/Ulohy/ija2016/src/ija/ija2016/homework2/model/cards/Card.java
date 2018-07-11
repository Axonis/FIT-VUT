package ija.ija2016.homework2.model.cards;

import java.io.Serializable;

public interface Card {

    public static enum Color implements Serializable, Comparable<Color> {
        CLUBS, //black
        DIAMONDS, //red
        HEARTS, //red
        SPADES; //black

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

        public boolean similarColorTo(Card.Color c) {
            return ((this == Color.CLUBS || this == Color.SPADES) && (c == Color.CLUBS || c == Color.SPADES) ||
                    (this == Color.DIAMONDS || this == Color.HEARTS) && (c == Color.DIAMONDS || c == Color.HEARTS));
        }
    }

    Card.Color color();

    boolean similarColorTo(Card c);

    boolean isTurnedFaceUp();

    boolean turnFaceUp();

    int value();

    int compareValue(Card c);

}
