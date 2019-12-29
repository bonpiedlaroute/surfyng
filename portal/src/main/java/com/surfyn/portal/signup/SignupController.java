package com.surfyn.portal.signup;

import com.surfyn.portal.dao.UserDAO;
import com.surfyn.portal.entity.AppRole;
import com.surfyn.portal.entity.User;
import com.surfyn.portal.form.UserForm;
import com.surfyn.portal.repository.UserRepository;
import com.surfyn.portal.service.UserDetailsServiceImpl;
import com.surfyn.portal.utils.SecurityUtil;
import org.springframework.beans.factory.annotation.Autowired;
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

import java.util.ArrayList;
import java.util.List;

@Controller
class SignupController {

	private static final String SIGNUP_VIEW_NAME = "signup/signup";

	@Autowired
	private UserRepository userRepository;

	@Autowired
	private UserDAO appUserDAO;

	@Autowired
	private ConnectionFactoryLocator connectionFactoryLocator;

	@Autowired
	private UsersConnectionRepository connectionRepository;

	@Autowired
	private UserDetailsServiceImpl accountService;


	@RequestMapping(value = { "/signup" }, method = RequestMethod.GET)
	public String signupPage(WebRequest request, Model model) {

		ProviderSignInUtils providerSignInUtils //
				= new ProviderSignInUtils(connectionFactoryLocator, connectionRepository);

		// Retrieve social networking information.
		Connection<?> connection = providerSignInUtils.getConnectionFromSession(request);
		//
		UserForm myForm = null;
		//
		if (connection != null) {
			myForm = new UserForm(connection);
		} else {
			myForm = new UserForm();
		}
		model.addAttribute("myForm", myForm);
		return SIGNUP_VIEW_NAME;
	}

	@RequestMapping(value = { "/signup" }, method = RequestMethod.POST)
	public String signupSave(WebRequest request, //
                             Model model, //
                             @ModelAttribute("myForm") @Validated UserForm appUserForm, //
                             BindingResult result, //
                             final RedirectAttributes redirectAttributes) {

		if(userRepository.existsByEmail(appUserForm.getEmail())) {
			result.rejectValue("email", "error.user", "An account already exists for this email.");
		}

		// Validation error.
		if (result.hasErrors()) {
			return SIGNUP_VIEW_NAME;
		}

		List<String> roleNames = new ArrayList<String>();
		roleNames.add(AppRole.ROLE_USER);

		User registered = null;

		try {
			registered = appUserDAO.registerNewUserAccount(appUserForm, roleNames);
		} catch (Exception ex) {
			ex.printStackTrace();
			model.addAttribute("errorMessage", "Error " + ex.getMessage());
			return SIGNUP_VIEW_NAME;
		}

		if (appUserForm.getSignInProvider() != null) {
			ProviderSignInUtils providerSignInUtils //
					= new ProviderSignInUtils(connectionFactoryLocator, connectionRepository);

			// (Spring Social API):
			// If user login by social networking.
			// This method saves social networking information to the UserConnection table.
			providerSignInUtils.doPostSignUp(registered.getUserName(), request);
		}

		// After registration is complete, automatic login.
		SecurityUtil.logInUser(registered, roleNames);

		return "redirect:/";
	}
}
