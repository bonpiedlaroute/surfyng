package com.surfyn.portal.web;

public class AjaxUtils {

    private AjaxUtils() {
    }

    public static boolean isAjaxRequest(String requestedWith) {
        return requestedWith != null ? "XMLHttpRequest".equals(requestedWith) : false;
    }
}
