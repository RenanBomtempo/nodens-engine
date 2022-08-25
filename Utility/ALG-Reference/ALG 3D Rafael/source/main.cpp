/**
 * @file main.cpp
 * File where the main function is implemented.
 *
 * @author Maycon da Costa - mayconufmg@gmail.com
 * @author Denise Burgarelli Duczmall - burgarel@mat.ufmg.br
 * @author Rodney Josué Biezuner - rodney@mat.ufmg.br
 */

// System
#include <iostream>
#include <cstdlib>

using std::cout;
using std::endl;

// Application
#include "alg/grid/grid.h"

/**
 * Function main.
 *
 * @param argc Number of arguments from the command line.
 * @param argv Vector of parameters given in command line.
 */
int main( int argc, char** argv ) {
	try {


		// Creates a new grid and refines it in refinement level five.
		Grid *gridMono = new Grid;
		gridMono->solveMonodomain(argc, argv);
        delete gridMono;


	}
	catch( Exception &error ) {
		error.pushIntoStackTrace( "main()" );
		error.printStackTrace();
		exit(1);
 	}
}
