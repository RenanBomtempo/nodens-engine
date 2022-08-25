#include <iostream>
#include <cmath>

#include "grid.h"

using std::cout;
using std::endl;

/**
 * Sets the current domain as a spherical peel centered at the point
 * (0.5, 0.5, 0.50). Those cells that do not belong to the domain become
 * inactive cells.
 *
 * @param internalRadius Length of the internal radius.
 * @param externalRadius Length of the external radius.
 * @throw InvalidArgumentValue If any of the arguments are less than zero or the
 * internal radius is greater than the external radius, an InvalidArgumentValue
 * exception is thrown.
 */
void Grid::setSphericalDomain( double internalRadius, double externalRadius ) throw (InvalidArgumentValue) {
	if( (internalRadius > externalRadius) || (internalRadius < 0) || (externalRadius < 0) ) {
		throw InvalidArgumentValue
		("Grid::setSphericalDomain(): Invalid value of arguments.\n");
	}

	CellNode *gridCell = firstCell;

	// Coordinates of the sphere center
	const double x_c = 0.5;
	const double y_c = 0.5;
	const double z_c = 0.5;

	// Distance between the sphere center and a cell.
	double distance;

	double x, y, z;
	while( gridCell != 0 ) {
		x = gridCell->centerX;
		y = gridCell->centerY;
		z = gridCell->centerZ;
		distance = (x-x_c)*(x-x_c) + (y-y_c)*(y-y_c) + (z-z_c)*(z-z_c);

		if(( distance <= (externalRadius*externalRadius)) && ( distance >= (internalRadius*internalRadius)))
			gridCell->active = true;
		else
			gridCell->active = false;

		gridCell = gridCell->next;
	}
}

/**
 * Sets the current domain as a cylinder whose central axis is parallel to  the  Z
 * axis and intercepts the plane XY at the point (0.5, 0.5, 0.0). Those cells that
 * do not belong to the domain become inactive cells.
 *
 * @param internalRadius Length of the internal radius.
 * @param externalRadius Length of the external radius.
 * @throw InvalidArgumentValue If any of the arguments are less than zero or the
 * internal radius is greater than the external radius, an InvalidArgumentValue
 * exception is thrown.
 */
void Grid::setCylindricalDomain( double internalRadius, double externalRadius ) throw (InvalidArgumentValue) {
	if( (internalRadius > externalRadius) || (internalRadius < 0) || (externalRadius < 0) ) {
		throw InvalidArgumentValue
		("Grid::setSphericalDomain(): Invalid value of parameters.\n");
	}

	CellNode *gridCell = firstCell;

	// Coordinates of the cylinder center
	const double x_c = 0.5;
	const double y_c = 0.5;

	// Distance between the cylinder central  axis and a cell.
	double distance;

	double x, y;
	while( gridCell != 0 ) {
		x = gridCell->centerX;
		y = gridCell->centerY;
		distance = (x - x_c)*(x - x_c) + (y - y_c)*(y - y_c);

		if(( distance <= (externalRadius*externalRadius)) && ( distance >= (internalRadius*internalRadius)))
			gridCell->active = true;
		else
			gridCell->active = false;

		gridCell = gridCell->next;
	}
}

/**
 * Sets the current domain as a Cone with central axis parallel to the Z axis
 * and intercepts the plane XY at the point (0.5, 0.5, 0.0).
 */
void Grid::setConicalDomain() {
	CellNode *gridCell = firstCell;

	// Equation of the cone central axis.
	const double x_c = 0.5;
	const double y_c = 0.5;

	// Distance between the cone central axis and a cell.
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

/**
 * Sets the current domain as a Paraboloid with central axis parallel to the Z
 * axis and intercepts the plane XY at the point (0.5, 0.5, 0.0).
 */
void Grid::setParabolicalDomain() {
	CellNode *gridCell = firstCell;

	// Equation of the paraboloid central axis.
	const double x_c = 0.5;
	const double y_c = 0.5;

	// Distance between the paraboloid  central axis and a cell.
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

/**
 * Sets the current domain as a unitary cube centered at the point
 * (0.5, 0.5, 0.5). As this is the default domain of ALG, all cells
 * are set to active.
 */
void Grid::setCubicalDomain() {
	CellNode *gridCell = firstCell;
	while( gridCell != 0 ) {
		gridCell->active = true;
		gridCell = gridCell->next;
	}
}
