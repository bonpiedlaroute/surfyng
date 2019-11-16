package com.surfyng.portal.exception;

public class DuplicateTableException extends RuntimeException {
    public DuplicateTableException(Exception e) {
        super(e);
    }
}
