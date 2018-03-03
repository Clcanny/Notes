package com.viviansj.loveu;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class LoveuController {

    @GetMapping("/")
    @ResponseBody
    public String sayLoveu() {
        Thread.dumpStack();
        return "Loveu!";
    }
}
