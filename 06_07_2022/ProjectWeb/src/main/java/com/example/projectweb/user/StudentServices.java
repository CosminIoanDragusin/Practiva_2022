package com.example.projectweb.user;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

@Service

public class StudentServices {
    @Autowired private StudentRepository repo;

    public List<Student> listAll(){
        return (List<Student>) repo.findAll();
    }
}
