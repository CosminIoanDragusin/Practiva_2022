package com.example.projectweb.user;

import javax.persistence.*;

@Entity
@Table(name="Student")
public class Student {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Integer id;

    @Column(nullable = false,name = "first_name",length = 15)
    private String first_name;

    @Column(nullable = false,name = "second_name",length = 15)
    private String second_name;

    @Column(nullable = false,unique = true,length = 45)
    private String email;

    @Column(nullable = false,length = 15)
    private String password;

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public String getFirst_name() {
        return first_name;
    }

    public void setFirst_name(String first_name) {
        this.first_name = first_name;
    }

    public String getSecond_name() {
        return second_name;
    }

    public void setSecond_name(String second_name) {
        this.second_name = second_name;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    @Override
    public String toString() {
        return "Student{" +
                "id=" + id +
                ", first_name='" + first_name + '\'' +
                ", second_name='" + second_name + '\'' +
                ", email='" + email + '\'' +
                ", password='" + password + '\'' +
                '}';
    }
}
