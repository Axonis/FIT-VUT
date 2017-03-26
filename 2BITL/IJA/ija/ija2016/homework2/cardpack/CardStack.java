package ija.ija2016.homework2.cardpack;

import ija.ija2016.homework2.model.cards.Card;

import java.util.Stack;

public class CardStack extends CardDeck implements ija.ija2016.homework2.model.cards.CardStack{

    private boolean working = true;

    public CardStack(int size, boolean working) {
        super(size);
        this.size = size;
        this.working = working;
    }


    // Inserts card on the top of the stack
    public boolean put(ija.ija2016.homework2.model.cards.CardStack stack) {
        if(working){
            if(stack_cards.size() >= size)
                return false;

            if(stack_cards.isEmpty()){
                if(stack.get(0).value() == 13) {
                    for (int i = 0; i < stack.size(); i++)
                        stack_cards.push(stack.get(i));
                    return true;
                }
            }
            else if((stack.get(0).value() - stack_cards.peek().value()) == -1
                    && !stack.get(0).similarColorTo(stack_cards.peek())) {
                for (int i = 0; i < stack.size(); i++)
                    stack_cards.push(stack.get(i));
                return true;
            }
            else
                return false;
            return false;
        }

        if (stack.size()+stack_cards.size() <= size) {
           for (int i = 0; i < stack.size(); i++) {
                stack_cards.push(stack.get(i));
            }
            while (!stack.isEmpty())
                stack.pop();

                return true;

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
        int position = stack_cards.search(card);

        if (position == -1) {
            return null;
        }

        CardStack tmp_stack = new CardStack(position, false);

        for (int i = stack_cards.size() - position; i < stack_cards.size(); i++)
            tmp_stack.put(stack_cards.get(i));

        int stack_size = stack_cards.size();

        for (int i = stack_cards.size() - position; i < stack_size; i++)
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