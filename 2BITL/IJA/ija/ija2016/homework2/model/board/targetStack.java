package ija.ija2016.homework2.model.board;

import ija.ija2016.homework2.model.cards.Card;

public class targetStack extends ija.ija2016.homework2.cardpack.CardDeck {

    private Card.Color color;

    public targetStack(Card.Color color){
        super(13);
        this.color = color;
    }

    @Override
    public boolean put(Card card) {
        if(stack_cards.size() >= size)
            return false;

        if(stack_cards.isEmpty()){
            if(card.color() == color) {
                if (card.value() == 1) {
                    stack_cards.push(card);
                    return true;
                }
            }
        }
        else if((card.value() - stack_cards.peek().value() == 1
                && card.color() == color)) {
            stack_cards.push(card);
            return true;
        }
        else
            return false;
        return false;
    }
}
