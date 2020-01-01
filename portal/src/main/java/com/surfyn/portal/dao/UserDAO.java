package com.surfyn.portal.dao;

import com.surfyn.portal.entity.AppRole;
import com.surfyn.portal.entity.User;
import com.surfyn.portal.form.UserForm;
import com.surfyn.portal.repository.UserRepository;
import com.surfyn.portal.utils.EncrytedPasswordUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.social.connect.Connection;
import org.springframework.social.connect.ConnectionKey;
import org.springframework.social.connect.UserProfile;
import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import javax.persistence.EntityManager;
import javax.persistence.NoResultException;
import javax.persistence.Query;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.UUID;

@Repository
@Transactional
public class UserDAO {

    @Autowired
    private UserRepository userRepository;

    @Autowired
    private EntityManager entityManager;

    @Autowired
    private RoleDAO appRoleDAO;

    public User findAppUserByUserId(Long userId) {
        try {
            String sql = "Select e from " + User.class.getName() + " e " //
                    + " Where e.userId = :userId ";

            Query query = entityManager.createQuery(sql, User.class);
            query.setParameter("userId", userId);

            return (User) query.getSingleResult();
        } catch (NoResultException e) {
            return null;
        }
    }

    public User findAppUserByUserName(String userName) {
        try {
            String sql = "Select e from " + User.class.getName() + " e " //
                    + " Where e.userName = :userName ";

            Query query = entityManager.createQuery(sql, User.class);
            query.setParameter("userName", userName);

            return (User) query.getSingleResult();
        } catch (NoResultException e) {
            return null;
        }
    }

    public User findByEmail(String email) {
        try {
            String sql = "Select e from " + User.class.getName() + " e " //
                    + " Where e.email = :email ";

            Query query = entityManager.createQuery(sql, User.class);
            query.setParameter("email", email);

            return (User) query.getSingleResult();
        } catch (NoResultException e) {
            return null;
        }
    }

    private String findAvailableUserName(String userName_prefix) {
        User account = this.findAppUserByUserName(userName_prefix);
        if (account == null) {
            return userName_prefix;
        }
        int i = 0;
        while (true) {
            String userName = userName_prefix + "_" + i++;
            account = this.findAppUserByUserName(userName);
            if (account == null) {
                return userName;
            }
        }
    }

    // Auto create App User Account.
    public User createAppUser(Connection<?> connection) {

        ConnectionKey key = connection.getKey();
        // (facebook,12345), (google,123) ...

        UserProfile userProfile = connection.fetchUserProfile();

        String email = userProfile.getEmail();
        User appUser = this.findByEmail(email);
        if (appUser != null) {
            return appUser;
        }
        String userName_prefix = userProfile.getFirstName().trim().toLowerCase()//
                + "_" + userProfile.getLastName().trim().toLowerCase();

        String userName = this.findAvailableUserName(userName_prefix);
        //
        // Random Password! TODO: Need send email to User!
        //
        String randomPassword = UUID.randomUUID().toString().substring(0, 5);
        String encrytedPassword = EncrytedPasswordUtils.encrytePassword(randomPassword);
        //
        appUser = new User();
        appUser.setEnabled(true);
        appUser.setEncrytedPassword(encrytedPassword);
        appUser.setUserName(userName);
        appUser.setEmail(email);
        appUser.setFirstName(userProfile.getFirstName());
        appUser.setLastName(userProfile.getLastName());

        User result  = userRepository.save(appUser);

        // Create default Role
        List<String> roleNames = new ArrayList<String>();
        roleNames.add(AppRole.ROLE_USER);
        this.appRoleDAO.createRoleFor(appUser, roleNames);

        return appUser;
    }

    public User registerNewUserAccount(UserForm appUserForm, List<String> roleNames) {
        User appUser = new User();
        appUser.setUserName(appUserForm.getUserName() != null ? appUserForm.getUserName() : appUserForm.getEmail());
        appUser.setEmail(appUserForm.getEmail());
        appUser.setFirstName(appUserForm.getFirstName());
        appUser.setLastName(appUserForm.getLastName());
        appUser.setEnabled(true);
        appUser.setCreatedOn(new Date());
        appUser.setLastLogin(new Date());
        String encrytedPassword = EncrytedPasswordUtils.encrytePassword(appUserForm.getPassword());
        appUser.setEncrytedPassword(encrytedPassword);

        User result  = userRepository.save(appUser);

        this.appRoleDAO.createRoleFor(result, roleNames);

        return appUser;
    }

}
