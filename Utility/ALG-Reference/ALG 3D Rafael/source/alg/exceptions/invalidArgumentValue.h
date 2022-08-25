/**
 * @file invalidArgumentValue.h
 * File where the class InvalidArgumentValue is defined.
 */

#ifndef INVALID_ARGUMENT_H
#define INVALID_ARGUMENT_H

#include "exceptions.h"

/**
 * @class InvalidArgumentValue
 * Class for handling exceptions that are thrown when methods receive invalid
 * arguments values when they are invoked.
 */
class InvalidArgumentValue: public Exception {
public:
	InvalidArgumentValue( const string msg );
    InvalidArgumentValue( const string msg, char );
    InvalidArgumentValue( const string msg, int );
    InvalidArgumentValue( const string msg, float );
    InvalidArgumentValue( const string msg, double );
    InvalidArgumentValue( const string msg, string );
    ~InvalidArgumentValue();
};

#endif
