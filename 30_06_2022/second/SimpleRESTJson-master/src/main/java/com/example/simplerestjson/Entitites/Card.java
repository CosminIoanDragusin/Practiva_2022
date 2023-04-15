package com.example.simplerestjson.Entitites;

public class Card {
    String type;
    String value;

    public Card(String type, String value) {
        this.type = type;
        this.value = value;
    }

    public String getType() {
        return type;
    }

    public void setType(String name) {
        this.type = name;
    }

    public String getValue() {
        return value;
    }

    public void setValue(String email) {
        this.value = email;
    }



}
