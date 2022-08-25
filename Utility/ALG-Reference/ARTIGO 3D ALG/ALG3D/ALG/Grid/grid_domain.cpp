#include <iostream>
#include <cmath>

#include "grid.h"

using std::cout;
using std::endl;

/*==============================================================================
                       FUNCTION setSphericalDomain()

     Sets the current domain as a sphere. The parameters are: smallest  ray, and
	 greatest ray.     
==============================================================================*/
void Grid::setSphericalDomain( float insideRay, float outsideRay ) throw (InvalidParameterValue) {
	if( (insideRay > outsideRay) || (insideRay < 0) ) {
		throw InvalidParameterValue
		("Grid::setSphericalDomain(): Invalid value of parameters.\n");		
	}
	
	CellNode *gridCell = firstCell;
	
	// Coordinates of the sphere center 
	const float x_c = 0.5;
	const float y_c = 0.5;
	const float z_c = 0.5;
	
	// Distance between the sphere center and a cell.
	double distance;
	
	double x, y, z;
	while( gridCell != 0 ) {
		x = gridCell->centerX;
		y = gridCell->centerY;
		z = gridCell->centerZ;
		distance = (x-x_c)*(x-x_c) + (y-y_c)*(y-y_c) + (z-z_c)*(z-z_c);
		
		if(( distance <= (outsideRay*outsideRay)) && ( distance >= (insideRay*insideRay)))
			gridCell->active = true;
		else
			gridCell->active = false;
		
		gridCell = gridCell->next;
	}
}

/*==============================================================================
                      FUNCTION setCylindricalDomain()
                      
     Sets the current domain as a cylinder. The parameters are: smallest ray, 
	 and greatest ray.     
==============================================================================*/
void Grid::setCylindricalDomain( float insideRay, float outsideRay ) throw (InvalidParameterValue) {
	if( (insideRay > outsideRay) || (insideRay < 0) ) {
		throw InvalidParameterValue
		("Grid::setSphericalDomain(): Invalid value of parameters.\n");		
	}
	
	CellNode *gridCell = firstCell;
	
	// Coordinates of the cylinder center 
	const float x_c = 0.5;
	const float y_c = 0.5;
	
	// Distance between the central cylinder axis and a cell.
	double distance;
	
	double x, y;
	while( gridCell != 0 ) {
		x = gridCell->centerX;
		y = gridCell->centerY;
		distance = (x - x_c)*(x - x_c) + (y - y_c)*(y - y_c);
		
		if(( distance <= (outsideRay*outsideRay)) && ( distance >= (insideRay*insideRay)))
			gridCell->active = true;
		else
			gridCell->active = false;
		
		gridCell = gridCell->next;
	}
}

/*==============================================================================
                        FUNCTION setConicalDomain()

     Sets the current domain as a Cone.
==============================================================================*/
void Grid::setConicalDomain() {
	CellNode *gridCell = firstCell;
	
	// Equation of the central cone axis.
	const float x_c = 0.5;
	const float y_c = 0.5;
	
	// Distance between the central cone axis and a cell.
	double distance;
	
	double x, y, z;
	while( gridCell != 0 ) {
		x = gridCell->centerX;
		y = gridCell->centerY;
		z = gridCell->centerZ;
		distance = (x - x_c)*(x - x_c) + (y - y_c)*(y - y_c);
		
		if( z >= sqrt( distance ))
			gridCell->active = true;
		else
			gridCell->active = false;
		
		gridCell = gridCell->next;
	}
}

/*==============================================================================
                       FUNCTION setParabolicalDomain()
                       
     Sets the current domain as a Paraboloid.
==============================================================================*/
void Grid::setParabolicalDomain() {
	CellNode *gridCell = firstCell;
	
	// Equation of the central paraboloid axis.
	const float x_c = 0.5;
	const float y_c = 0.5;
	
	// Distance between the central paraboloid axis and a cell.
	double distance;
	
	double x, y, z;
	while( gridCell != 0 ) {
		x = gridCell->centerX;
		y = gridCell->centerY;
		z = gridCell->centerZ;
		distance = (x - x_c)*(x - x_c) + (y - y_c)*(y - y_c);
		
		if( z >= distance )
			gridCell->active = true;
		else
			gridCell->active = false;
		
		gridCell = gridCell->next;
	}
}

/*==============================================================================
                        FUNCTION setCubicalDomain()

    Sets all cells of a grid as active cell.
==============================================================================*/
void Grid::setCubicalDomain() {
	CellNode *gridCell = firstCell;
	while( gridCell != 0 ) {
		gridCell->active = true;
		gridCell = gridCell->next;
	}
}
