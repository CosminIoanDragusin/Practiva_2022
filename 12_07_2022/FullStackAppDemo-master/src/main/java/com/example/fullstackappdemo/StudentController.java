package com.example.fullstackappdemo;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.*;

import java.util.ArrayList;

@RestController
@RequestMapping("/student")
@CrossOrigin(origins = "*", allowedHeaders = "*")
public class StudentController {
    private StudentServices studentServices;

    public StudentController(StudentServices studentServices) {
        this.studentServices = studentServices;
    }

    @PostMapping("/addTestData")
    public ArrayList<Student> addTestStudents(){
        return studentServices.addTestStudents();
    }

    @PostMapping("/add")
    public Student addStudent(@RequestBody Student student){
        return studentServices.addStudent(student);
    }
    @GetMapping("/all")
    public ArrayList<Student> getAllStudents(){
        return studentServices.getAllStudents();
    }

    @GetMapping("/{id}")
    public Student getAllStudents(@PathVariable int id){
        return studentServices.getStudentById(id);
    }

    @DeleteMapping("/delete/{id}")
    public void deleteById(@PathVariable int id){
        studentServices.deleteStudentById(id);
    }

    @PutMapping("/modify")
    public Student modifyStudent(@RequestBody Student modifiedStudent){
        return studentServices.modifyStudent(modifiedStudent);
    }

}
