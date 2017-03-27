package ija.ija2016.homework2.model.board;

public class workingStack extends ija.ija2016.homework2.cardpack.CardStack{

    public workingStack(){
        super(13, true);
    }

    @Override
    public boolean put(ija.ija2016.homework2.model.cards.Card card) {
        if(stack_cards.size() >= size)
            return false;

        if(stack_cards.isEmpty()){
            if(card.value() == 13) {
                stack_cards.push(card);
                return true;
            }
        }
        else if((card.value() - stack_cards.peek().value()) == -1
                && !card.similarColorTo(stack_cards.peek())) {
            stack_cards.push(card);
            return true;
        }
        else
            return false;
        return false;
    }

}
