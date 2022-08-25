/**
 * @file nullPointer.h
 * File where the class NullPointer is defined.
 */
#ifndef NULL_POINTER_H
#define NULL_POINTER_H

#include "exceptions.h"

/**
 * @class NullPointer
 * Class for handling exceptions which are thrown when pointers in methods get Null
 * when they are not expected to be Null.
 */
class NullPointer: public Exception {
public:
	   NullPointer( const string msg );
	   ~NullPointer();
};

#endif
