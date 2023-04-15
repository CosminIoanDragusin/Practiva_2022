package com.example.FormExampleSpring.StudentController;

import com.example.FormExampleSpring.domain.Adition;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.validation.BindingResult;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PostMapping;

@Controller
public class AdditionController
{
    @GetMapping("/index")
    public String ViewForm(Model model)
    {
        model.addAttribute("Addition", new Adition());
        return "index";
    }

    @PostMapping("/addtion")
    public String addForm(@ModelAttribute Adition addtion, BindingResult result, Model model)
    {
        model.addAttribute("addtion",addtion);
        return "calculate";
    }
}
