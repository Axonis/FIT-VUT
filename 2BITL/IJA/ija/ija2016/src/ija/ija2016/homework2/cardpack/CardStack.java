package ija.ija2016.homework2.cardpack;

import ija.ija2016.homework2.model.cards.Card;

public class CardStack extends CardDeck implements ija.ija2016.homework2.model.cards.CardStack {


    public CardStack(int size) {
        super(size);
    }

    // Inserts card on the top of the stack
    public boolean put(ija.ija2016.homework2.model.cards.CardStack stack) {
        // If new stack is going to have proper size
        if (stack.size() + stack_cards.size() <= size) {
            for (int i = 0; i < stack.size(); i++) {
                stack_cards.push(stack.get(i));
            }
            while (!stack.isEmpty())
                stack.pop();

            return true;
            // Otherwise return false
        } else
            return false;
    }

    public CardStack pop(Card card) {
        return takeFrom(card);
    }

    public boolean isEmpty() {
        return stack_cards.isEmpty();
    }

    public int size() {
        return stack_cards.size();
    }

    public CardStack takeFrom(Card card) {
        // grabbed_card_position of grabbed card
        int grabbed_card_position = 0;

        if ((grabbed_card_position = stack_cards.search(card)) == -1)
            return null;

        // Creates new stack beginning from grabbed_card_position
        CardStack tmp_stack = new CardStack(grabbed_card_position);

        // Puts cards from original stack to new stack
        for (int i = stack_cards.size() - grabbed_card_position; i < stack_cards.size(); i++)
            tmp_stack.put(stack_cards.get(i));

        int stack_size = stack_cards.size();

        // Pops card used to create tmp_stack
        for (int i = stack_cards.size() - grabbed_card_position; i < stack_size; i++)
            stack_cards.pop();

        return tmp_stack;
    }

    @Override
    public boolean equals(Object object) {
        //If they point to the same object return true
        if (this == object) return true;
        //Check for nulls
        if (this == null || object == null) return false;
        //If the stacks are not the same length, then they won't be equal, easy first test case
        if (stack_cards.size() != ((CardStack) object).stack_cards.size()) return false;

        for (int i = 0; i < stack_cards.size(); i++) {
            //Step through each item in both stacks, if any don't match return false
            if (!stack_cards.get(i).equals(((CardStack) object).stack_cards.get(i))) {
                return false;
            }
        }
        //Haven't returned yet, they must be equal
        return true;
    }
}