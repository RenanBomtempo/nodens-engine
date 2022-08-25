#ifndef NULL_POINTER_H
#define NULL_POINTER_H

#include "exceptions.h"

/*==============================================================================
					         NullPointer Exception
==============================================================================*/
class NullPointer: public Exception {
public:
	   NullPointer( const string msg );
	   ~NullPointer();
};

#endif
