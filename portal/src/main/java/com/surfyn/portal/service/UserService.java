package com.surfyn.portal.service;




import com.surfyn.portal.entity.User;

import java.util.List;
import java.util.Optional;

public interface UserService {
    public Optional<User> findUserByEmail(String email);

    public Optional<User> findOne(Long id);

    public List<User> findAll();

    public Optional<User> findUserByResetToken(String resetToken);

    public void save(User user);

    public void createPasswordResetTokenForUser(User user, String token);

    String validatePasswordResetToken(long id, String token);

    public void changeUserPassword(User user, String password);
}
