package com.example.rest_demo;

import org.springframework.stereotype.Service;

import java.util.ArrayList;

@Service
public class MasinaService {
    MasinaRepository masinaRepository;

    public MasinaService(MasinaRepository masinaRepository) {
        this.masinaRepository = masinaRepository;
    }

    public ArrayList<Masina> getAllMasina(){
        return (ArrayList<Masina>) masinaRepository.findAll();
    }

    public void add(Masina masina) {
        masinaRepository.save(masina);
    }

    public void addMockData() {
        masinaRepository.save(new Masina("Dacia","Verde"));
        masinaRepository.save(new Masina("Volvo","Gri"));
        masinaRepository.save(new Masina("Ford","Rosu"));
        masinaRepository.save(new Masina("Cardillac","Galben"));

    }

    public Masina getMasinaById(int id) {
        return masinaRepository.findById(id).get();
    }

    public void deleteMasinaById(int id) {
        masinaRepository.deleteById(id);
    }
}
