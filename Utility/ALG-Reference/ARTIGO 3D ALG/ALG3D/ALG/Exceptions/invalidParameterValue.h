#ifndef INVALID_PARAMETER_H
#define INVALID_PARAMETER_H

#include "exceptions.h"

/*==============================================================================
				         InvalidParameterValue Exception
==============================================================================*/
class InvalidParameterValue: public Exception {
public:
       InvalidParameterValue( const string msg );
   	   InvalidParameterValue( const string msg, char );
	   InvalidParameterValue( const string msg, int );
   	   InvalidParameterValue( const string msg, float );
   	   InvalidParameterValue( const string msg, double );
   	   InvalidParameterValue( const string msg, string );
   	   
	   ~InvalidParameterValue();
};

#endif
