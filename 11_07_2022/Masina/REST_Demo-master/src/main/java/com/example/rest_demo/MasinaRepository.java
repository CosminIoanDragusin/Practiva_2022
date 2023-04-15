package com.example.rest_demo;

import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface MasinaRepository extends CrudRepository<Masina, Integer> {

}
