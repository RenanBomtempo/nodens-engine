#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include <sstream>   // Used for string streams.
#include <string>

using namespace std;

class Table {
private:
	  int numberOfPoints; // Number of points in this table.	
	  double *xPoint;	  // Vector to storage the X coordinates.
	  double *yPoint;     // Vector to storage the Y coordinates.
	  char symbol;           // Simbol used to draw the points in the screen. It can be '.', 'o', or 'x'
	  char type;
	  
public:
       
	  Table( char symbol, char typeOfPoints ); // Constructor.
	  ~Table();	    // Destructor.
	  
	  // Creates the vectors of X and Y coordinates.
	  void creates( int N );

      // Set the simbol used to draw the points in the screen. It can be '.', 'o', or 'x'
      void setSymbol( char symbolOfPoint );

      // Returns the numberOfPoints of the Table.
      int getNumberOfPoints();

      // Fills the vectors of X and Y coordinates with the values from xTable and yTable.
      void fills( double* xTable, double* yTable );
	  
	  // Returns the vector of X coordinates
	  double* getsXPoint();
	  
	  // Returns the vector of Y coordinates
	  double* getsYPoint();
	  
	  // Returns the number of points existent in an area described by a square.
	  int getOccurrences( double, double, double );
	  
	  // Returns the position on the table of the point that's in the gridCell.
	  long int* searchPoints(double, double, double, int);
      	  
      // Sets the value of each point of this table. (Used to aleatories points)
	  void fillTable( double (*Fx)(), double (*Fy)());
	  
	  void copyTable( Table *auxTable);
	  
	  // Creates a file with all the points' coordinates
	  void createFile (char *nome);
	  
	  // Change the points' coordinates making a diminutive zoom.
	  void zoomTable ( double w );
	  
	  // Shifts the x or/and y coordinates 
	  void coordinatesShift (int desloc, double w);
	  
 	  // Rotates the coordinates x and y 
	  void coordinatesRotacao( int graus );

      // Turn back the position of points (before the rotation have been done)
      void turnBackPointsPosition( int graus );
	  	  
 	  void drawNPoints( double x, double y );
	  	  
	  // Draws all points of this table.
	  void draw( int, double );
};

#endif
