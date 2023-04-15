package com.example.simplerestjson.Services;

import com.example.simplerestjson.Entitites.Student;

import java.util.ArrayList;

public class StudentServices {

    private ArrayList<Student> studentList = new ArrayList<Student>();
    public Student getTestStudent() {
        Student student = new Student("Toni", "antoniu.pitic@ulbsibiu.ro");
        return  student;
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
