#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string>
#include <list>

using std::string;
using std::list;

/*==============================================================================
						     Generic Exception
==============================================================================*/
class Exception {
private:
	string msg;
	list<string> stackTrace;
	
public:
	Exception( const string errorMessage );
	virtual ~Exception();
	virtual string getMessage();
	virtual void setMessage( const string errorMessage );
	virtual void printMessage();
	virtual void pushIntoStackTrace( const string methodName );
	virtual void printStackTrace();
};

#endif
