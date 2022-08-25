#include "algpch.h"
#include "Vector2D.h"

namespace alg {
	Vector2D::Vector2D()
		: x(0.0),
		y(0.0)
	{
	}

	Vector2D::Vector2D(double xy)
		: x(xy),
		y(xy)
	{
	}

	Vector2D::Vector2D(double _x, double _y)
		: x(_x),
		y(_y)
	{
	}
}
