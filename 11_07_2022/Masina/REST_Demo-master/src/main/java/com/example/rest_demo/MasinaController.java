package com.example.rest_demo;


import org.springframework.web.bind.annotation.*;

import java.util.ArrayList;

@RestController
@RequestMapping("/student")
@CrossOrigin(origins = "*", allowedHeaders = "*")
public class MasinaController {
    private MasinaService masinaService;

    public MasinaController(MasinaService masinaService) {
        this.masinaService = masinaService;
    }

    @GetMapping("/all")
    ArrayList<Masina> getAllMasina(){
        return masinaService.getAllMasina();
    }

    @PostMapping("/add")
    public void add(@RequestBody Masina masina){
        masinaService.add(masina);
    }

    @PostMapping("/addMockData")
    public void addMockData(){
        masinaService.addMockData();
    }

    @GetMapping("/{id}")
    public Masina getMasina(@PathVariable int id){
        return masinaService.getMasinaById(id);
    }

    @DeleteMapping("/delete/{id}")
    public void deleteMasina(@PathVariable int id){
        masinaService.deleteMasinaById(id);
    }
}
