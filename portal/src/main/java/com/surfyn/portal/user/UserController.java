package com.surfyn.portal.user;

import com.surfyn.portal.service.UserService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.access.annotation.Secured;
import org.springframework.security.authentication.AnonymousAuthenticationToken;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;

/**
 * Standard Layout System with Fragment Expressions usage example
 */
@Controller
@Secured("ROLE_ADMIN")
class UserController {

    @Autowired
    private UserService userService;


    @ModelAttribute("module")
    public String module() {
        return "users";
    }

    @RequestMapping(value = "/private/admin/users", method = RequestMethod.GET)
    public String tasks(Model model) {
        Authentication authentication = SecurityContextHolder.getContext().getAuthentication();
        if (authentication instanceof AnonymousAuthenticationToken) {
            return "redirect:/signin";
        }
        model.addAttribute("users", userService.findAll());
        return "user/user-list";
    }

    @RequestMapping(value = "private/admin/user/{id}", method = RequestMethod.GET)
    public String task(@PathVariable("id") Long id, Model model) {
        Authentication authentication = SecurityContextHolder.getContext().getAuthentication();
        if (authentication instanceof AnonymousAuthenticationToken) {
            return "redirect:/signin";
        }
        model.addAttribute("user", userService.findOne(id).get());
        return "user/user";
    }
}
