package com.surfyn.portal.exception;

public class GenericDynamoDBException extends RuntimeException {
    public GenericDynamoDBException(Exception e) {
        super(e);
    }
}
