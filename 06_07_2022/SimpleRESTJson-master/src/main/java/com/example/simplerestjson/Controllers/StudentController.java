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

    private StudentServices studentServices;

    public StudentController(StudentServices studentServices) {
        this.studentServices = studentServices;
    }

    @GetMapping("/getTestStudent")
    Student getTestStudent(){
        return null;
    }
    @GetMapping("/getAllStudents")
    ArrayList<Student> getAllStudents(){
        return studentServices.getAllStudents();
    }
}
