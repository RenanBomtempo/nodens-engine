#include <iostream>
#include "exceptions.h"

using std::cout;
using std::cin;
using std::endl;

/*==============================================================================
                             Exception Constructor
						 
   It receive as parameter a string that should contain a message informing why
   the exception was thrown.
==============================================================================*/
Exception::Exception( const string msg ) {
	this->msg = msg;
	this->stackTrace = list<string>();
}

/*==============================================================================
                             Exception Destructor
==============================================================================*/	
Exception::~Exception() {
    delete &msg;
    delete &stackTrace;
}

/*==============================================================================
                             FUNCTION getMessage()
                             
   Returns a string with information about the exception.     
==============================================================================*/	
string Exception::getMessage() {
    return msg;
}

/*==============================================================================
                             FUNCTION setMessage()
                             
   Sets the message of an Exception to the string received as parameter.
==============================================================================*/	
void Exception::setMessage( string msg ) {
	 this->msg = msg;
}

/*==============================================================================
                        FUNCTION pushIntoStackTrace()
                             
   Storages the name of a method that thrown an exception on a stack of methods
   affected by the exception.
==============================================================================*/	
void Exception::pushIntoStackTrace( const string methodName ) {
	 stackTrace.push_back( methodName );
}

/*==============================================================================
                             FUNCTION printMessage()
                             
   Prints the message of an Exception on the standard output.
==============================================================================*/
void Exception::printMessage() {
	 cout << msg << "\n";
}

/*==============================================================================
                           FUNCTION printStackTrace()
                             
   Prints the stack of methods which thrown the exception.
==============================================================================*/
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
