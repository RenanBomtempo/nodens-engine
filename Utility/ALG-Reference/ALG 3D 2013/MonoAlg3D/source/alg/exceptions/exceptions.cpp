/**
 * @file exceptions.cpp
 * File where the class Exception is implemented.
 */

#include <iostream>
#include "exceptions.h"

using std::cout;
using std::cin;
using std::endl;

/**
 * Constructor of class Exception.
 *
 * @param msg String that should contain a message explaining the cause of this exception.
 */
Exception::Exception( const string msg ) {
	this->msg = msg;
	this->stackTrace = list<string>();
}

/**
 * Destructor of class Exception.
 */
Exception::~Exception() {
    delete &msg;
    delete &stackTrace;
}

/**
 * Returns a string with information about an exception.
 *
 * @return message of an exception.
 */
string Exception::getMessage() {
    return msg;
}

/**
 * Sets the message of an Exception.
 *
 * @param msg New message to set.
 */
void Exception::setMessage( string msg ) {
	 this->msg = msg;
}

/**
 * Prints an informative message of the cause of an exception on standard output.
 */
void Exception::printMessage() {
	 cout << msg << "\n";
}

/**
 * Storages on a stack the name of a method where an exception was thrown.
 *
 * @param methodName Name of the method that where this exception was thrown.
 */
void Exception::pushIntoStackTrace( const string methodName ) {
	 stackTrace.push_back( methodName );
}

/**
 * Prints the stack trace of an exception on standard output.
 */
void Exception::printStackTrace() {
	 string method;
	 int stackSize = stackTrace.size();
	 cout << "============================= EXCEPTION =============================\n";
	 cout << "Procedure Stack Calls:\n\n";
	 for( int i = 0; i < stackSize; i++ ) {
	 	  method = stackTrace.back();
	 	  stackTrace.pop_back();
	 	  cout << i << " " << method << "\n";
  	 }
  	 printMessage();
  	 cout << "\nPress a key to terminate ...";
     cin.get();
}
