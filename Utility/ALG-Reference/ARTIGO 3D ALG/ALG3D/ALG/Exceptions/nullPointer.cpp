#include "nullPointer.h"

/*==============================================================================
                              NullPointer Constructor
						 
   It receive as parameter a string that should contain a message informing why
   the exception was thrown.
==============================================================================*/
NullPointer::NullPointer( const string msg ): Exception(msg){}

/*==============================================================================
                              NullPointer Destructor
==============================================================================*/
NullPointer::~NullPointer(){}
