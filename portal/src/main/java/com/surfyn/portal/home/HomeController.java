package com.surfyn.portal.home;

import com.surfyn.portal.utils.WebUtils;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;

import java.security.Principal;

@Controller
class HomeController {

    @GetMapping("/")
    String index(Model model, Principal principal) {

        // After user login successfully.
        if (principal != null) {
            String userName = principal.getName();

            UserDetails loginedUser = (UserDetails) ((Authentication) principal).getPrincipal();

            String userInfo = WebUtils.toString(loginedUser);
            model.addAttribute("userInfo", userInfo);
        }

        return principal != null ? "home/homeSignedIn" : "home/homeNotSignedIn";
    }

}
