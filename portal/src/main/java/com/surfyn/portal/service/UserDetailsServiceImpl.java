package com.surfyn.portal.service;


import com.surfyn.portal.dao.RoleDAO;
import com.surfyn.portal.dao.UserDAO;
import com.surfyn.portal.entity.User;
import com.surfyn.portal.social.SocialUserDetailsImpl;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Scope;
import org.springframework.context.annotation.ScopedProxyMode;
import org.springframework.security.authentication.UsernamePasswordAuthenticationToken;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.GrantedAuthority;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.core.userdetails.UsernameNotFoundException;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;

@Service
@Scope(proxyMode = ScopedProxyMode.TARGET_CLASS)
public class UserDetailsServiceImpl implements UserDetailsService {

	@Autowired
	private UserDAO appUserDAO;

	@Autowired
	private RoleDAO appRoleDAO;


	@Override
	public UserDetails loadUserByUsername(String userName) throws UsernameNotFoundException {

		User appUser = this.appUserDAO.findAppUserByUserName(userName);

		if (appUser == null) {
			throw new UsernameNotFoundException("User " + userName + " was not found in the database");
		}

		// [ROLE_USER, ROLE_ADMIN,..]
		List<String> roleNames = this.appRoleDAO.getRoleNames(appUser.getUserId());

		List<GrantedAuthority> grantList = new ArrayList<GrantedAuthority>();

		SocialUserDetailsImpl userDetails = new SocialUserDetailsImpl(appUser, roleNames);

		Authentication authentication = new UsernamePasswordAuthenticationToken(userDetails, null,
				userDetails.getAuthorities());
		SecurityContextHolder.getContext().setAuthentication(authentication);

		return userDetails;
	}



}
