package com.surfyn.portal.exception;

public class DuplicateTableException extends RuntimeException {
    public DuplicateTableException(Exception e) {
        super(e);
    }
}
