package com.viviansj.loveu;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class LoveuController {

    @GetMapping("/hello")
    @ResponseBody
    public String sayHello() {
        return "Hello!";
    }

    @GetMapping("/hi")
    @ResponseBody
    public String sayHi() {
        return "Hi!";
    }
}
