package com.example.simplerestjson.Controllers;

import com.example.simplerestjson.Entitites.Student;
import com.example.simplerestjson.Services.StudentServices;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.ArrayList;

@RestController
@CrossOrigin(origins = "*", allowedHeaders = "*")
public class StudentController {
    @GetMapping("/getTestStudent")
    Student getTestStudent(){
        return (new StudentServices()).getTestStudent();
    }
    @GetMapping("/getAllStudents")
    ArrayList<Student> getAllStudents(){
        return (new StudentServices()).getTestStudents();
    }
}
