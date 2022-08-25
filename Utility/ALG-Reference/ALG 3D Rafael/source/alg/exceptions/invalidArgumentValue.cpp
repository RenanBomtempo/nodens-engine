/**
 * @file invalidArgumentValue.cpp
 * File where all methods of class InvalidArgumentValue are implemented.
 */

#include "invalidArgumentValue.h"
#include <stdlib.h>
#include<cstdlib>
#include<cstring>
#include <cstdio>

/**
 * Constructor of class InvalidArgumentValue.
 *
 * @param msg String that should contain a message explaining the cause of this
 * exception.
 * @param value Char variable that caused the exception.
 */
InvalidArgumentValue::InvalidArgumentValue( const string msg, char value ): Exception(msg) {
    char wrongValue[1];
    sprintf( wrongValue, "%c", value );
    string newMessage = Exception::getMessage() + " Value received:  " + wrongValue;
    Exception::setMessage( newMessage );
}

/**
 * Constructor of class InvalidArgumentValue.
 *
 * @param msg String that should contain a message explaining the cause of this
 * exception.
 * @param value Integer variable that caused the exception.
 */
InvalidArgumentValue::InvalidArgumentValue( const string msg, int value ): Exception(msg) {
    char *wrongValue = (char*)malloc(sizeof(int));
    sprintf( wrongValue, "%d", value );
    string newMessage = Exception::getMessage() + " Value received: " + wrongValue;
    Exception::setMessage( newMessage );
}

/**
 * Constructor of class InvalidArgumentValue.
 *
 * @param msg String that should contain a message explaining the cause of this
 * exception.
 * @param value Float variable that caused the exception.
 */
InvalidArgumentValue::InvalidArgumentValue( const string msg, float value ): Exception(msg) {
    char *wrongValue = (char*)malloc(sizeof(float));
    sprintf( wrongValue, "%f", value );
    string newMessage = Exception::getMessage() + " Value received: " + wrongValue;
    Exception::setMessage( newMessage );
}

/**
 * Constructor of class InvalidArgumentValue.
 *
 * @param msg String that should contain a message explaining the cause of this
 * exception.
 * @param value Double variable that caused the exception.
 */
InvalidArgumentValue::InvalidArgumentValue( const string msg, double value ): Exception(msg) {
    char *wrongValue = (char*)malloc(sizeof(double));
    sprintf( wrongValue, "%g", value );
    string newMessage = Exception::getMessage() + " Value received: " + wrongValue;
    Exception::setMessage( newMessage );
}

/**
 * Constructor of class InvalidArgumentValue.
 *
 * @param msg String that should contain a message explaining the cause of this
 * exception.
 * @param value String variable that caused the exception.
 */
InvalidArgumentValue::InvalidArgumentValue( const string msg, string value ): Exception(msg) {
    string newMessage = Exception::getMessage() + " Value received: " + value;
    Exception::setMessage( newMessage );
}

/**
 * Default constructor of class InvalidArgumentValue.
 *
 * @param msg String that should contain a message explaining the cause of this
 * exception.
 */
InvalidArgumentValue::InvalidArgumentValue( const string msg ): Exception(msg) {

}

/**
 * Destructor of class InvalidArgumentValue.
 */
InvalidArgumentValue::~InvalidArgumentValue() {}
