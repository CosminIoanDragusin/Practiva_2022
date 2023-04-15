package com.example.projectweb.user;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;

import java.util.List;

@Controller
public class StudentController {
@Autowired private StudentServices services;

@GetMapping("/users")
    public String showStudentList(Model model){
    List<Student> listStudents=services.listAll();
    model.addAttribute("listStudents",listStudents);
    return "users";
}
}
