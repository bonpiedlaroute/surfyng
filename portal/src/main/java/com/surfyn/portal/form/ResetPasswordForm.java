package com.surfyn.portal.form;

import javax.validation.constraints.NotEmpty;

public class ResetPasswordForm {

    private static final String NOT_BLANK_MESSAGE = "{notBlank.message}";
    private static final String EMAIL_MESSAGE = "{email.message}";

    @NotEmpty(message = ResetPasswordForm.NOT_BLANK_MESSAGE)
    private String email;

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

}
