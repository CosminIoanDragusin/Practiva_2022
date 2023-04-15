package com.example.simplerestjson.Services;

import com.example.simplerestjson.Entitites.Card;

import java.util.ArrayList;

public class CardServices {

    private final ArrayList<Card> cardList = new ArrayList<>();
    public Card getTestCard() {
        return new Card("Leaf", "8");
    }

    public void GenerateMockCards(){
        cardList.clear();
        cardList.add( new Card("Heart", "joker"));
        cardList.add( new Card("diamond", "2"));
        cardList.add( new Card("clubs", "4"));
        cardList.add( new Card("leaf", "10"));
        cardList.add( new Card("diamond", "Wait"));
    }

    public  ArrayList<Card> getTestCards() {
        GenerateMockCards();
        return cardList;
    }

}
