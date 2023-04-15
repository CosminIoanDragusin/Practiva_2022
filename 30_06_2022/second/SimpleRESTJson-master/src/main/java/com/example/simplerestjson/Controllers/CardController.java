package com.example.simplerestjson.Controllers;

import com.example.simplerestjson.Entitites.Card;
import com.example.simplerestjson.Services.CardServices;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.ArrayList;

@Controller
@RestController
@CrossOrigin(origins = "*", allowedHeaders = "*")
public class CardController {

    @GetMapping("/getTestCard")
    Card getTestCard(){
        return (new CardServices()).getTestCard();
    }
    @GetMapping("/getAllCards")
    ArrayList<Card> getAllCards(){
        return (new CardServices()).getTestCards();
    }
}
