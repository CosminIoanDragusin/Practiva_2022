package com.example.rest_demo;


import org.springframework.web.bind.annotation.*;

import java.util.ArrayList;

@RestController
@RequestMapping("/student")
@CrossOrigin(origins = "*", allowedHeaders = "*")
public class StudentController {
    private StudentService studentService;

    public StudentController(StudentService studentService) {
        this.studentService = studentService;
    }

    @GetMapping("/all")
    ArrayList<Student> getAllStudents(){
        return studentService.getAllStudents();
    }

    @PostMapping("/add")
    public void add(@RequestBody Student student){
        studentService.add(student);
    }

    @PostMapping("/addMockData")
    public void addMockData(){
        studentService.addMockData();
    }

    @GetMapping("/{id}")
    public Student getStudent(@PathVariable int id){
        return studentService.getStudentById(id);
    }

    @DeleteMapping("/delete/{id}")
    public void deleteStudent(@PathVariable int id){
        studentService.deleteStudentById(id);
    }
    @PutMapping("/update")
    public Student updateStudent(@RequestBody Student student){
        return studentService.update(student);
    }
}
