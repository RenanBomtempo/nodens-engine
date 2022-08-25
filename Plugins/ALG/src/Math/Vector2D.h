#pragma once
#include <cmath>

namespace alg {
	// Double precision 2-dimensional vector.
	class Vector2D
	{
	public:
		Vector2D();
		Vector2D(double xy);
		Vector2D(double _x, double _y);
		~Vector2D() = default;

		double x;
		double y;
	
		inline const double Length() const { return std::sqrt(x*x + y*y); }
	};
}

