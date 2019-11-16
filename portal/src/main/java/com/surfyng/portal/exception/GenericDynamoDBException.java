package com.surfyng.portal.exception;

public class GenericDynamoDBException extends RuntimeException {
    public GenericDynamoDBException(Exception e) {
        super(e);
    }
}
