#pragma once
#include "Core.h"
#include "Log.h"

namespace alg {
	enum class Direction {
		North = 0, East, South, West
	};

	Direction Opposite(Direction direction);
}