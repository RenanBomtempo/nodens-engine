#include "invalidParameterValue.h"
#include <stdlib.h>

/*==============================================================================
                      InvalidParameterValue Constructors
                      
   It receive as parameter a string that should contain a message informing why
   the exception was thrown, and the variable that cause the exception.
==============================================================================*/
InvalidParameterValue::InvalidParameterValue( const string msg, char value ): Exception(msg) {
    char wrongValue[1];
    sprintf( wrongValue, "%c", value );
    string newMessage = Exception::getMessage() + " Value received:  " + wrongValue;
    Exception::setMessage( newMessage );
}

InvalidParameterValue::InvalidParameterValue( const string msg, int value ): Exception(msg) {
    char *wrongValue = (char*)malloc(sizeof(int));
    sprintf( wrongValue, "%d", value );
    string newMessage = Exception::getMessage() + " Value received: " + wrongValue;
    Exception::setMessage( newMessage );
}

InvalidParameterValue::InvalidParameterValue( const string msg, float value ): Exception(msg) {
    char *wrongValue = (char*)malloc(sizeof(float));
    sprintf( wrongValue, "%f", value );
    string newMessage = Exception::getMessage() + " Value received: " + wrongValue;
    Exception::setMessage( newMessage );
}

InvalidParameterValue::InvalidParameterValue( const string msg, double value ): Exception(msg) {
    char *wrongValue = (char*)malloc(sizeof(double));
    sprintf( wrongValue, "%g", value );
    string newMessage = Exception::getMessage() + " Value received: " + wrongValue;
    Exception::setMessage( newMessage );
}

InvalidParameterValue::InvalidParameterValue( const string msg, string value ): Exception(msg) {
    string newMessage = Exception::getMessage() + " Value received: " + value;
    Exception::setMessage( newMessage );
}

InvalidParameterValue::InvalidParameterValue( const string msg ): Exception(msg) {

}

/*==============================================================================
                       InvalidParameterValue Destructor
==============================================================================*/
InvalidParameterValue::~InvalidParameterValue() {}
