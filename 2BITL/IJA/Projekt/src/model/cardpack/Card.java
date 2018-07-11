/**
 * @file Card.java.
 *
 * @author xuhlia03
 *
 * Implements the card class.
 */
package model.cardpack;

public class Card extends javax.swing.JLabel implements java.io.Serializable{

    public enum Card_Color {
        HEARTS(),
        CLUBS(),
        SPADES(),
        DIAMONDS();
        @Override
        public String toString(){
            switch(this) {
                case HEARTS: return "H";
                case CLUBS: return "C";
                case DIAMONDS: return "D";
                case SPADES: return "S";
                default: return "";
            }
        }
    }

    private int cardVal;
    private Card_Color cardColor;
    private boolean isFaceUp;
    private String path;
    private String location = "ND";

    /**
     * Constructor of a card.
     *
     * @param color The value of new card.
     * @param value   A Color to assign.
     */
        public Card(Card_Color color, int value){
        this.cardVal = value;
        this.cardColor = color;
        this.isFaceUp = false;
        if(color == Card_Color.HEARTS){
            switch(value) {
                case 1: path = "/img/ace_of_hearts.jpg"; break;
                case 2: path = "/img/2_of_hearts.jpg"; break;
                case 3: path = "/img/3_of_hearts.jpg"; break;
                case 4: path = "/img/4_of_hearts.jpg"; break;
                case 5: path = "/img/5_of_hearts.jpg"; break;
                case 6: path = "/img/6_of_hearts.jpg"; break;
                case 7: path = "/img/7_of_hearts.jpg"; break;
                case 8: path = "/img/8_of_hearts.jpg"; break;
                case 9: path = "/img/9_of_hearts.jpg"; break;
                case 10: path = "/img/10_of_hearts.jpg"; break;
                case 11: path = "/img/jack_of_hearts.jpg"; break;
                case 12: path = "/img/queen_of_hearts.jpg"; break;
                case 13: path = "/img/king_of_hearts.jpg"; break;
            }
        }
        else if (color == Card_Color.CLUBS){
            switch(value) {
                case 1: path = "/img/ace_of_clubs.jpg"; break;
                case 2: path = "/img/2_of_clubs.jpg"; break;
                case 3: path = "/img/3_of_clubs.jpg"; break;
                case 4: path = "/img/4_of_clubs.jpg"; break;
                case 5: path = "/img/5_of_clubs.jpg"; break;
                case 6: path = "/img/6_of_clubs.jpg"; break;
                case 7: path = "/img/7_of_clubs.jpg"; break;
                case 8: path = "/img/8_of_clubs.jpg"; break;
                case 9: path = "/img/9_of_clubs.jpg"; break;
                case 10: path = "/img/10_of_clubs.jpg"; break;
                case 11: path = "/img/jack_of_clubs.jpg"; break;
                case 12: path = "/img/queen_of_clubs.jpg"; break;
                case 13: path = "/img/king_of_clubs.jpg"; break;
            }
        }
        else if (color == Card_Color.DIAMONDS){
            switch(value) {
                case 1: path = "/img/ace_of_diamonds.jpg"; break;
                case 2: path = "/img/2_of_diamonds.jpg"; break;
                case 3: path = "/img/3_of_diamonds.jpg"; break;
                case 4: path = "/img/4_of_diamonds.jpg"; break;
                case 5: path = "/img/5_of_diamonds.jpg"; break;
                case 6: path = "/img/6_of_diamonds.jpg"; break;
                case 7: path = "/img/7_of_diamonds.jpg"; break;
                case 8: path = "/img/8_of_diamonds.jpg"; break;
                case 9: path = "/img/9_of_diamonds.jpg"; break;
                case 10: path = "/img/10_of_diamonds.jpg"; break;
                case 11: path = "/img/jack_of_diamonds.jpg"; break;
                case 12: path = "/img/queen_of_diamonds.jpg"; break;
                case 13: path = "/img/king_of_diamonds.jpg"; break;
            }
        }
        else if (color == Card_Color.SPADES){
            switch(value) {
                case 1: path = "/img/ace_of_spades.jpg"; break;
                case 2: path = "/img/2_of_spades.jpg"; break;
                case 3: path = "/img/3_of_spades.jpg"; break;
                case 4: path = "/img/4_of_spades.jpg"; break;
                case 5: path = "/img/5_of_spades.jpg"; break;
                case 6: path = "/img/6_of_spades.jpg"; break;
                case 7: path = "/img/7_of_spades.jpg"; break;
                case 8: path = "/img/8_of_spades.jpg"; break;
                case 9: path = "/img/9_of_spades.jpg"; break;
                case 10: path = "/img/10_of_spades.jpg"; break;
                case 11: path = "/img/jack_of_spades.jpg"; break;
                case 12: path = "/img/queen_of_spades.jpg"; break;
                case 13: path = "/img/king_of_spades.jpg"; break;
            }
        }
    }

    /**
     * Determine card value.
     *
     * @return A card value in integer.
     */
    public int value(){
        return this.cardVal;
    }

    /**
     * Determine if card face is down or up.
     *
     * @return A actual face status.
     */
    public boolean isTurnedFaceUp(){ return  this.isFaceUp; }

    /**
     * Turn card face down.
     *
     * @return A result of the action.
     */
    public boolean turnFaceUp(){
        if (this.isFaceUp){
            return false;
        }
        else{
            this.isFaceUp = true;
            return true;
        }
    }

    /**
     * Turn card face down.
     *
     * @return A result of the action.
     */
    public boolean turnFaceDown(){
        if (this.isFaceUp){
            this.isFaceUp = false;
            return true;
        }
        else{
            return false;
        }
    }

    /**
     * Get card color.
     *
     * @return A color of this card.
     */
    public Card_Color color(){
        return this.cardColor;
    }

    /**
     * Find out if this card has the same color as card from param.
     *
     * @param c If non-null, a Card to process.
     *
     * @return True if the parameters are considered equivalent.
     */
    public boolean similarColorTo(Card c){
        switch (c.color()) {
            case HEARTS:
            case DIAMONDS:
                switch (this.cardColor){
                    case HEARTS:
                    case DIAMONDS:
                        return true;
                    default:
                        return false;
                }
            case CLUBS:
            case SPADES:
                switch (this.cardColor){
                    case CLUBS:
                    case SPADES:
                        return true;
                    default:
                        return false;
                }
            default:
                return false;
        }
    }

    /**
     * Set card location by deck or stack.
     *
     * @param s A string that represents deck or stack location.
     *
     */
    public void setCLocation(String s){
        this.location = s;
    }

    /**
     * Get card location by deck or stack.
     *
     * @return An difference between cards values
     */
    public String getCLocation(){
        return location;
    }

    /**
     * Compare this card to another.
     *
     * @param c A Card to compare.
     *
     * @return An difference between cards values
     */
    public int compareValue(Card c){
        return Math.abs(this.cardVal - c.value());
    }



    /*@Override
    public boolean equals(Object cardObj1){
        if(((Card)cardObj1).cardVal == this.cardVal &&
                ((Card)cardObj1).cardColor == this.cardColor){
            return true;
        }
        else {
            return false;
        }
    }*/

    @Override
    public int hashCode(){
        switch(this.cardColor){
            case CLUBS:
                return this.cardVal ;
            case DIAMONDS:
                return this.cardVal + 13;
            case HEARTS:
                return this.cardVal + 2*13;
            case SPADES:
                return this.cardVal + 3*13;
            default:
                return 0;
        }
    }

    /**
     * Get card path to the card image.
     *
     * @return A path to the card image.
     */
    public String getPath() {
        return this.path;
    }

}
