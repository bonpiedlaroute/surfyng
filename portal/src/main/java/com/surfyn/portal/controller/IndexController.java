package com.surfyn.portal.controller;

import org.springframework.boot.web.servlet.error.ErrorController;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class IndexController implements ErrorController {

    private static final String PATH = "/";

    @GetMapping("/login")
    public String login() {
        return PATH;
    }

    @GetMapping("/signup")
    public String signup() {
        return PATH;
    }

    @Override
    public String getErrorPath() {
        return PATH;
    }
}
