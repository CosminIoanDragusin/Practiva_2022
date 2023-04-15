package com.example.simplerestjson.Entitites;

import javax.persistence.*;

@Entity
public class Student {

    @Id
    @GeneratedValue(strategy= GenerationType.SEQUENCE)
    private int id;
    String name;
    String email;



    public Student(String name, String email) {
        this.name = name;
        this.email = email;
    }

    public Student() {
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }


    @Override
    public String toString() {
        return "Student{" +
                "name='" + name + '\'' +
                ", email='" + email + '\'' +
                '}';
    }
}
