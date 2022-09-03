#pragma once
#include <cmath>

namespace alg {
	// Double precision 2-dimensional vector.
	class Vector2D
	{
	public:
		Vector2D();
		Vector2D(float xy);
		Vector2D(float _x, float _y);
		~Vector2D() = default;

		float x;
		float y;

		inline const float Length() const { return std::sqrt(x*x + y*y); }
	};
}

