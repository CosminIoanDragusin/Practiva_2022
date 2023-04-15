package com.example.rest_demo;

import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Service;

import java.util.ArrayList;

@Service
public class StudentService {
    StudentRepository studentRepository;

    public StudentService(StudentRepository studentRepository) {
        this.studentRepository = studentRepository;
    }

    public ArrayList<Student> getAllStudents(){
        return (ArrayList<Student>) studentRepository.findAll();
    }

    public void add(Student student) {
        studentRepository.save(student);
    }

    public void addMockData() {
        studentRepository.save(new Student("Toni"));
        studentRepository.save(new Student("Ana"));
        studentRepository.save(new Student("Constatinescu"));
        studentRepository.save(new Student("Dorel"));

    }

    public Student getStudentById(int id) {
        return studentRepository.findById(id).get();
    }

    public void deleteStudentById(int id) {
        studentRepository.deleteById(id);
    }
    // 2, Ana -> 2,Ancutza
    public Student update(Student student) {
        return studentRepository.save(student);
    }
}
