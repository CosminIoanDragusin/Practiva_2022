package com.example.simplerestjson.Services;

import com.example.simplerestjson.Entitites.Student;
import com.example.simplerestjson.StudentRepository;
import org.springframework.stereotype.Service;

import java.util.ArrayList;

@Service
public class StudentServices {



    StudentRepository studentRepository;
    private final ArrayList<Student> studentList = new ArrayList<>();
    public StudentServices(StudentRepository studentRepository) {
        this.studentRepository = studentRepository;
    }
    public Student getTestStudent() {
        return new Student("Toni", "antoniu.pitic@ulbsibiu.ro");
    }


    public ArrayList<Student> getAllStudents(){
        return (ArrayList<Student>) studentRepository.findAll();
    }

    public void GenerateMockStudents(){
        studentList.clear();
        studentList.add( new Student("Toni", "antoniu.pitic@ulbsibiu.ro"));
        studentList.add( new Student("Ana", "antoniu.ana@ulbsibiu.ro"));
        studentList.add( new Student("Tudor", "tudor.pitic@ulbsibiu.ro"));
        studentList.add( new Student("Constantinescu", "aa.pitic@ulbsibiu.ro"));
        studentList.add( new Student("Popa", "g.pitic@ulbsibiu.ro"));
    }

    public  ArrayList<Student> getTestStudents() {
        GenerateMockStudents();
        return studentList;
    }

}
