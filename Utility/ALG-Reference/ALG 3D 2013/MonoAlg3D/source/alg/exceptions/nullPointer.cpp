/**
 * @file nullPointer.cpp
 * File where all methods of class NullPointer are implemented.
 */
#include "nullPointer.h"

/**
 * Constructor of class NullPointer.
 *
 * @param msg String that should contain a message explaining the cause of this
 * exception.
 */
NullPointer::NullPointer( const string msg ): Exception(msg){}

/**
 * Destructor of class NullPointer.
 */
NullPointer::~NullPointer(){}
