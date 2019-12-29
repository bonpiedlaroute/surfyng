package com.surfyn.portal.password;


import com.surfyn.portal.entity.User;
import com.surfyn.portal.entity.VerificationToken;
import com.surfyn.portal.form.UserForm;
import com.surfyn.portal.service.EmailService;
import com.surfyn.portal.service.UserService;
import com.surfyn.portal.utils.GenericResponse;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.MessageSource;
import org.springframework.core.env.Environment;
import org.springframework.mail.SimpleMailMessage;
import org.springframework.mail.javamail.JavaMailSender;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.validation.Valid;
import java.util.Locale;
import java.util.Optional;
import java.util.UUID;


@Controller
public class PasswordController {

    private static final String PWD_VIEW_NAME = "password/password";

    private static final String PWD_UPDATE_VIEW_NAME = "password/update";

    @Autowired
    private UserService userService;

    @Autowired
    private EmailService emailService;

    @Autowired
    private PasswordEncoder passwordEncoder;


    @Autowired
    private MessageSource messages;

    @Autowired
    private JavaMailSender mailSender;

    @Autowired
    private Environment env;

    // Display forgotPassword page
    @RequestMapping(value = "/forgot", method = RequestMethod.GET)
    public ModelAndView displayForgotPasswordPage() {
        return new ModelAndView(PWD_VIEW_NAME);
    }


    // Reset password
    @RequestMapping(value = "/user/resetPassword", method = RequestMethod.POST)
    @ResponseBody
    public GenericResponse resetPassword(final HttpServletRequest request, @RequestParam("email") final String userEmail) {
        final Optional<User> user = userService.findUserByEmail(userEmail);
        if (user.isPresent()) {
            final String token = UUID.randomUUID().toString();
            userService.createPasswordResetTokenForUser(user.get(), token);
            mailSender.send(constructResetTokenEmail(getAppUrl(request), request.getLocale(), token, user));
        }
        return new GenericResponse(messages.getMessage("message.resetPasswordEmail", null, request.getLocale()));
    }

    @RequestMapping(value = "/user/changePassword", method = RequestMethod.GET)
    public String showChangePasswordPage(final Locale locale, final Model model, @RequestParam("id") final long id, @RequestParam("token") final String token) {
        final String result = userService.validatePasswordResetToken(id, token);
        if (result != null) {
            model.addAttribute("message", messages.getMessage("auth.message." + result, null, locale));
            model.addAttribute("loginForm",new UserForm());
            return "signin/signin";
        }
        return "password/update";
    }

    @RequestMapping(value = "/user/savePassword", method = RequestMethod.POST)
    @ResponseBody
    public GenericResponse savePassword(final Locale locale, @Valid PasswordForm passwordForm) {
        final UserDetails userLogin = (UserDetails) SecurityContextHolder.getContext().getAuthentication().getPrincipal();
        Optional<User> user = userService.findUserByEmail(userLogin.getUsername());
        userService.changeUserPassword(user.get(), passwordForm.getNewPassword());
        return new GenericResponse(messages.getMessage("message.resetPasswordSuc", null, locale));
    }


    // ============== NON-API ============

    private SimpleMailMessage constructResendVerificationTokenEmail(final String contextPath, final Locale locale, final VerificationToken newToken, final User user) {
        final String confirmationUrl = contextPath + "/registrationConfirm.html?token=" + newToken.getToken();
        final String message = messages.getMessage("message.resendToken", null, locale);
        return constructEmail("Resend Registration Token", message + " \r\n" + confirmationUrl, user);
    }

    private SimpleMailMessage constructResetTokenEmail(final String contextPath, final Locale locale, final String token, final Optional<User> user) {
        final String url = contextPath + "/user/changePassword?id=" + user.get().getUserId() + "&token=" + token;
        final String message = messages.getMessage("message.resetPassword", null, locale);
        final String subject = messages.getMessage("message.resetPassword", null, locale);
        return constructEmail(subject, message + " \r\n" + url, user.get());
    }

    private SimpleMailMessage constructEmail(String subject, String body, User user) {
        final SimpleMailMessage email = new SimpleMailMessage();
        email.setSubject(subject);
        email.setText(body);
        email.setTo(user.getEmail());
        email.setFrom(env.getProperty("support.email"));
        return email;
    }

    private String getAppUrl(HttpServletRequest request) {
        String profile = env.getProperty("spring.profiles.active");
        String scheme = profile != null && profile.equals("prod") ? "https://" : "http://";
        String port =  profile != null && profile.equals("prod") ?  "" : ":" + Integer.valueOf(request.getServerPort()) ;
        return scheme + request.getServerName() + port + request.getContextPath();
    }
}
