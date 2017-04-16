package ija.ija2016.homework2.model.board;

public class workingPack extends ija.ija2016.homework2.cardpack.CardStack {

    public workingPack() {
        super(13);
    }

    // Putting card on top of stack
    @Override
    public boolean put(ija.ija2016.homework2.model.cards.Card card) {
        if (stack_cards.size() < size) {
            // Is stack empty, so only Ace is valid ?
            if (stack_cards.isEmpty() && card.value() == 13) {
                stack_cards.push(card);
                return true;
            }
            // If new card has value higher by, skratkove vyhodnocovanie
            else if (!stack_cards.isEmpty() && (card.value() - stack_cards.peek().value()) == -1 && !card.similarColorTo(stack_cards.peek())) {
                stack_cards.push(card);
                return true;
            }
        } else
            return false;

        return false;
    }

    // Putting stack on top of the stack
    @Override
    public boolean put(ija.ija2016.homework2.model.cards.CardStack stack) {
        if (stack_cards.size() < size) {
            // Is stack empty, so only Ace+ is valid ?
            if (stack_cards.isEmpty() && stack.get(0).value() == 13) {
                for (int i = 0; i < stack.size(); i++)
                    stack_cards.push(stack.get(i));
                return true;
            }
            // If new card has value lower 1, skratkove vyhodnocovanie
            else if (!stack_cards.isEmpty() && (stack.get(0).value() - stack_cards.peek().value()) == -1
                    && !stack.get(0).similarColorTo(stack_cards.peek())) {
                for (int i = 0; i < stack.size(); i++)
                    stack_cards.push(stack.get(i));
                return true;
            }
        } else
            return false;

        return false;
    }

}
