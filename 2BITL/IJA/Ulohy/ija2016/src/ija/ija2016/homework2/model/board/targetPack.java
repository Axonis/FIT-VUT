package ija.ija2016.homework2.model.board;

import ija.ija2016.homework2.model.cards.Card;

public class targetPack extends ija.ija2016.homework2.cardpack.CardDeck {

    private Card.Color color;

    public targetPack(Card.Color color) {
        super(13);
        this.color = color;
    }

    @Override
    public boolean put(Card card) {
        // Is there free space in stack and is it the right color ?
        if (stack_cards.size() < size && card.color() == color) {
            // Is stack empty, so only Ace is valid ?
            if (stack_cards.isEmpty() && card.value() == 1) {
                stack_cards.push(card);
                return true;
            }
            // If new card has value higher by
            else if (!stack_cards.isEmpty() && card.value() - stack_cards.peek().value() == 1) {
                stack_cards.push(card);
                return true;
            }
        } else
            return false;

        return false;
    }
}
