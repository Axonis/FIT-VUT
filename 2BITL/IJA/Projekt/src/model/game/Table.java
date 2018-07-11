/**
 * @file Table.java.
 *
 * @author xuhlia03
 *
 * Implements the main table class that represents backend part.
 */
package model.game;

import model.cardpack.*;
import model.board.FactoryKlondike;

import java.util.Stack;

public class Table implements java.io.Serializable {

    public CardDeck newCardDeck;
    public CardDeck swapCardDeck;

    public CardDeck[] targetDecks;

    public workingPack[] workingPacks;
    public workingPack temporaryStack;

    private transient FactoryKlondike factory = new FactoryKlondike();
    public Stack<UndoElement> undoStack = new Stack<>();

    /**
     * Constructor.
     *
     * Create new table object that represents backend of the application.
     *
     */

    public Table(){
        // Create CardDecks for heap
        this.newCardDeck = factory.createCardDeck();
        this.swapCardDeck = new CardDeck(24);

        // Create Array for target stacks
        this.targetDecks = new CardDeck[4];
        this.targetDecks[0] = new targetDeck(Card.Card_Color.SPADES);
        this.targetDecks[1] = new targetDeck(Card.Card_Color.CLUBS);
        this.targetDecks[2] = new targetDeck(Card.Card_Color.DIAMONDS);
        this.targetDecks[3] = new targetDeck(Card.Card_Color.HEARTS);
    }


    /**
     * Record string representation of the source deck that card that has been moved.
     *
     * @param card   A moved card element.
     * @param source   A move source deck or stack.
     * @param dest   A move destination deck or stack.
     * @param type   A type of undo operation.
     */
    public void recordAction(Card card, String source, String dest, int type){
        UndoElement newAction;
        switch (type) {
            case 0:
                newAction = new UndoElement(null,null, null, type);
                this.undoStack.push(newAction);
                break;
            case 2:
            case 1:
                newAction = new UndoElement(card, source, dest, type);
                this.undoStack.push(newAction);
                break;
        }
    }

    /**
     * Return the undo element from top of the undo element stack.
     *
     * @return An undo element from top of the undo element stack.
     */
    public UndoElement peekLastAction(){
        if(this.undoStack.isEmpty()) {
            return null;
        }
        else {
            return this.undoStack.peek();
        }

    }

    /**
     * Get the last one recoded action and parse its content by the type.
     * Type 0 represents move from newDeck to swap deck.
     * Type 1 represents move from one deck to another without turning face down the another peek.
     * Type 2 represents move from one deck to another with turning face down the another peek.
     */
    public void undoLastAction(){
        if(!this.undoStack.isEmpty()) {
            UndoElement actualAction = this.undoStack.pop();
            switch (actualAction.getType()) {
                case 0:
                    if (this.swapCardDeck.isEmpty()) {
                        while (!this.newCardDeck.isEmpty()) {
                            this.swapCardDeck.put(this.newCardDeck.pop());
                        }
                        this.swapCardDeck.peek().turnFaceUp();
                    } else {
                        this.swapCardDeck.peek().turnFaceDown();
                        this.newCardDeck.put(this.swapCardDeck.pop());
                    }
                    break;
                case 2:
                    CardDeck deckForPut = getStackByCardLocation(actualAction.getSource());
                    deckForPut.peek().turnFaceDown();
                    deckForPut.forcePut(getStackByCardLocation(actualAction.getDestination()).pop());
                    break;
                case 1:
                    Card cardToPut = getStackByCardLocation(actualAction.getDestination()).pop();
                    CardDeck deckToPut = getStackByCardLocation(actualAction.getSource());
                    deckToPut.forcePut(cardToPut);
                    break;
            }
        }
    }

    /**
     * Get a new card from pull deck.
     *
     * @return A result of the action.
     */
    public boolean pullFromDrawStack(){
        if(this.newCardDeck.isEmpty()){
            while (!this.swapCardDeck.isEmpty()){
                this.newCardDeck.put(this.swapCardDeck.pop());
                this.newCardDeck.peek().turnFaceDown();
            }
            return false;
        }
        else {
            this.swapCardDeck.put(this.newCardDeck.pop());
            this.swapCardDeck.peek().turnFaceUp();
            return true;
        }
    }

    /**
     * Create new undo element that is used for undo operations.
     *
     * @param cardToPut A moved card element.
     * @param strDeckToPut   A move destination deck or stack.
     *
     * @return A result of the action.
     */

    public boolean putToTargetStack(Card cardToPut, String strDeckToPut){
        int deckIndex;
        switch (cardToPut.color()){
            case SPADES  : deckIndex = 0;break;
            case CLUBS   : deckIndex = 1;break;
            case DIAMONDS: deckIndex = 2;break;
            case HEARTS  : deckIndex = 3;break;
            default      : return false;
        }

        if( Character.getNumericValue(strDeckToPut.charAt(1)) != deckIndex + 1)
            return false;

        if (targetDecks[deckIndex].isEmpty()){
            if(cardToPut.value() == 1){
                getStackByCardLocation(cardToPut.getCLocation()).pop();
                targetDecks[deckIndex].put(cardToPut);
                if (!getStackByCardLocation(cardToPut.getCLocation()).isEmpty()){
                    if (!getStackByCardLocation(cardToPut.getCLocation()).peek().isTurnedFaceUp()) {
                        getStackByCardLocation(cardToPut.getCLocation()).peek().turnFaceUp();
                    }
                }
                return true;
            }
            else {
                return false;
            }
        }

        if ( targetDecks[deckIndex].peek().value() == (cardToPut.value() - 1)){
            if (targetDecks[deckIndex].peek().color() == cardToPut.color()) {
                targetDecks[deckIndex].put(cardToPut);
                getStackByCardLocation(cardToPut.getCLocation()).pop();
                if (!getStackByCardLocation(cardToPut.getCLocation()).isEmpty()){
                    if (!getStackByCardLocation(cardToPut.getCLocation()).peek().isTurnedFaceUp()) {
                        getStackByCardLocation(cardToPut.getCLocation()).peek().turnFaceUp();
                    }
                }

            }
            return true;
        }
        return false;
    }

    /**
     * Convert string representation of location to pointer to CardDeck.
     *
     * @param location A moved card element.
     *
     * @return A CardDeck pointer.
     */
    public CardDeck getStackByCardLocation(String location){
        switch (location){
            case "W1"   : return workingPacks[0];
            case "W2"   : return workingPacks[1];
            case "W3"   : return workingPacks[2];
            case "W4"   : return workingPacks[3];
            case "W5"   : return workingPacks[4];
            case "W6"   : return workingPacks[5];
            case "W7"   : return workingPacks[6];
            case "T1"   : return targetDecks[0];
            case "T2"   : return targetDecks[1];
            case "T3"   : return targetDecks[2];
            case "T4"   : return targetDecks[3];
            case "ND"   : return newCardDeck;
            case "SD"   : return swapCardDeck;
            default     : return null;
        }
    }

    /**
     * Convert string representation of location to pointer to workingPack.
     *
     * @param location A moved card element.
     *
     * @return A workingPack pointer.
     */
    private workingPack getWorkingPackByCardLocation(String location){
        switch (location){
            case "W1"   : return workingPacks[0];
            case "W2"   : return workingPacks[1];
            case "W3"   : return workingPacks[2];
            case "W4"   : return workingPacks[3];
            case "W5"   : return workingPacks[4];
            case "W6"   : return workingPacks[5];
            case "W7"   : return workingPacks[6];
            default     : return null;
        }
    }

    /**
     * Convert string representation of location to pointer to workingPack.
     *
     * @return If game has been won it returns true, otherwise false.
     */
    public boolean isGameWon(){
        if(this.swapCardDeck.isEmpty()){
            if(this.newCardDeck.isEmpty()) {
                for (CardDeck actualWorkingPack : workingPacks) {
                    if(! actualWorkingPack.isEmpty() )
                        return false;
                }
                return true;
            }
        }
        return false;
    }

    /**
     * Move a card or a stack of cards from working pack to another.
     *
     * @param cardFromWorkingPack A moved card element.
     * @param strDeckToPut A moved card element.
     *
     * @return  A result of the action.
     */
    public boolean putFromWorkingToWorkingPack(Card cardFromWorkingPack, String strDeckToPut) {
        if (strDeckToPut.equals(cardFromWorkingPack.getCLocation()))
            return false;

        workingPack deckToPop = getWorkingPackByCardLocation(cardFromWorkingPack.getCLocation());
        workingPack deckToPut = getWorkingPackByCardLocation(strDeckToPut);

        System.out.println(cardFromWorkingPack.value() + cardFromWorkingPack.color().toString());
        CardStack resultDeck = deckToPop.getByCard(cardFromWorkingPack);

        //noinspection Duplicates
        if (deckToPut.put(resultDeck)) {
            if (!getStackByCardLocation(cardFromWorkingPack.getCLocation()).isEmpty()) {
                if (!getStackByCardLocation(cardFromWorkingPack.getCLocation()).peek().isTurnedFaceUp()) {
                    getStackByCardLocation(cardFromWorkingPack.getCLocation()).peek().turnFaceUp();
                }
            }
            return true;
        }
        while (!resultDeck.isEmpty()){
            deckToPop.forcePut(resultDeck.pop());
        }
        return false;
    }

    /**
     * Move a card from target or swap deck to working pack.
     *
     * @param cardFromTargetSwapPack A moved card element.
     * @param strDeckToPut A moved card element.
     *
     * @return  A result of the action.
     */
    public boolean putFromTargetSwapToWorkingPack(Card cardFromTargetSwapPack, String strDeckToPut) {
        if (cardFromTargetSwapPack.getCLocation().equals(strDeckToPut))
            return false;

        workingPack deckToPut = getWorkingPackByCardLocation(strDeckToPut);

        //noinspection Duplicates
        if (deckToPut.put(getStackByCardLocation(cardFromTargetSwapPack.getCLocation()).pop())) {
            if (!getStackByCardLocation(cardFromTargetSwapPack.getCLocation()).isEmpty()) {
                if (!getStackByCardLocation(cardFromTargetSwapPack.getCLocation()).peek().isTurnedFaceUp()) {
                    getStackByCardLocation(cardFromTargetSwapPack.getCLocation()).peek().turnFaceUp();
                }
            }
            return true;
        }
        return false;
    }

}
