package com.example.projectweb;

import com.example.projectweb.user.Student;
import com.example.projectweb.user.StudentRepository;
import org.assertj.core.api.Assertions;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.shadow.com.univocity.parsers.annotations.Replace;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.jdbc.AutoConfigureTestDatabase;
import org.springframework.boot.test.autoconfigure.orm.jpa.DataJpaTest;
import org.springframework.test.annotation.Rollback;

import java.util.Optional;

@DataJpaTest
@AutoConfigureTestDatabase (replace = AutoConfigureTestDatabase.Replace.NONE)
@Rollback(false)
public class StudentRepositoryTests {
    @Autowired private StudentRepository repo;

    @Test
    public void testAddNew(){
        Student student=new Student();
        student.setFirst_name("Ana");
        student.setSecond_name("Catavencu");
        student.setEmail("anca@yahoo.com");
        student.setPassword("floricele99");

        Student savedStudent=repo.save(student);
        Assertions.assertThat(savedStudent).isNotNull();
        Assertions.assertThat(savedStudent.getId()).isGreaterThan(0);
    }
    @Test
    public void testListAll(){
    Iterable<Student> students=repo.findAll();
    Assertions.assertThat(students).hasSizeGreaterThan(0);

    for(Student student:students){
    System.out.println(student);
    }
    }
    @Test
    public void testUpdate(){
        Integer studentId=1;
        Optional<Student> optionalStudent= repo.findById(studentId);
        Student student=optionalStudent.get();
        student.setPassword("hello1234");
        repo.save(student);

        Student updateStudent=repo.findById(studentId).get();
        Assertions.assertThat(updateStudent.getPassword()).isEqualTo("hello1234");
    }
    @Test
    public void testGet(){
        Integer studentId=2;
        Optional<Student> optionalStudent= repo.findById(studentId);
        Assertions.assertThat(optionalStudent.get());
        System.out.println(optionalStudent.get());
    }

    @Test
    public void testDelete(){
        Integer studentId=4;
        repo.deleteById(studentId);

        Optional<Student> optionalStudent=repo.findById(studentId);
        Assertions.assertThat(optionalStudent).isNotPresent();
    }
}
