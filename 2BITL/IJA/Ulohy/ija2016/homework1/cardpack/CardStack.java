package ija.ija2016.homework1.cardpack;

import java.util.Stack;

public class CardStack {

    private Stack<Card> stack_cards = new Stack();
    private int size;

    public CardStack(int size) {
        this.size = size;
    }

    // Inserts card on the top of the stack
    public void put(Card card) {
        stack_cards.push(card);
    }

    public void put(CardStack stack) {
        if (!(stack.isEmpty())) {
            stack_cards.push(stack.stack_cards.pop());
        }
    }

    // Doesnt need to be public
    public boolean isEmpty() {
        return stack_cards.isEmpty();
    }

    public int size() {
        return stack_cards.size();
    }

    public CardStack takeFrom(Card card) {
        int position = stack_cards.search(card);

        if (position == -1) {
            return null;
        }

        CardStack tmp_stack = new CardStack(position);

        for (int i = position - 1; i < stack_cards.size(); i++) {
            tmp_stack.put(stack_cards.get(i));
        }

        int stack_size = stack_cards.size();

        for (int i = position - 1; i < stack_size; i++) {
            stack_cards.pop();
        }

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