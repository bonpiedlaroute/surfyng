package com.surfyn.portal.signin;


import com.surfyn.portal.entity.User;
import com.surfyn.portal.form.UserForm;
import com.surfyn.portal.repository.UserRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.authentication.AuthenticationManager;
import org.springframework.security.authentication.BadCredentialsException;
import org.springframework.security.authentication.UsernamePasswordAuthenticationToken;
import org.springframework.security.core.Authentication;
import org.springframework.social.connect.Connection;
import org.springframework.social.connect.ConnectionFactoryLocator;
import org.springframework.social.connect.UsersConnectionRepository;
import org.springframework.social.connect.web.ProviderSignInUtils;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.validation.BindingResult;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.context.request.WebRequest;
import org.springframework.web.servlet.mvc.support.RedirectAttributes;

import java.util.Date;
import java.util.Optional;

@Controller
public class SigninController {

    private static final String SIGNIN_VIEW_NAME = "signin/signin";

    @Autowired
    private ConnectionFactoryLocator connectionFactoryLocator;

    @Autowired
    private UsersConnectionRepository connectionRepository;

    @Autowired
    private AuthenticationManager authenticationManager;

    private UserRepository userRepository;

    @RequestMapping(value = { "/signin" }, method = RequestMethod.GET)
    public String signupPage(WebRequest request, Model model) {

        ProviderSignInUtils providerSignInUtils //
                = new ProviderSignInUtils(connectionFactoryLocator, connectionRepository);

        // Retrieve social networking information.
        Connection<?> connection = providerSignInUtils.getConnectionFromSession(request);
        //
        UserForm loginForm = null;
        //
        if (connection != null) {
            loginForm = new UserForm(connection);
        } else {
            loginForm = new UserForm();
        }
        model.addAttribute("loginForm", loginForm);
        return SIGNIN_VIEW_NAME;
    }

    @RequestMapping(value = { "/signin" }, method = RequestMethod.POST)
    public String signin(WebRequest request, //
                         Model model, //
                         @ModelAttribute("loginForm") @Validated UserForm appUserForm, //
                         BindingResult result, //
                         final RedirectAttributes redirectAttributes) {

        // Validation error.
        if (result.hasErrors()) {
            return SIGNIN_VIEW_NAME;
        }

        try {

            Authentication authentication = authenticationManager.authenticate(
                    new UsernamePasswordAuthenticationToken(
                            appUserForm.getEmail(),
                            appUserForm.getPassword()
                    ));
            Optional<User> user = userRepository.findByEmail(appUserForm.getEmail());
            user.get().setLastLogin(new Date());
            userRepository.save(user.get());
        } catch (BadCredentialsException bce) {
            result.rejectValue("", "", "Bad credentiels.");
            return SIGNIN_VIEW_NAME;
        } catch (Exception e) {
            result.rejectValue("", "", "Bad credentiels.");
            return SIGNIN_VIEW_NAME;
        }

        return "redirect:/";
    }
}
