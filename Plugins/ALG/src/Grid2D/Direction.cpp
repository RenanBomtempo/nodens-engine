#include "algpch.h"
#include "Grid2D/Direction.h"

namespace alg {
	Direction Opposite(Direction direction)
	{
		switch (direction)
		{
		case Direction::North:
			return Direction::South;
		case Direction::East:
			return Direction::West;
		case Direction::South:
			return Direction::North;
		case Direction::West:
			return Direction::East;
		default:
			ALG_CORE_ERROR("Invalid direction.");
			throw std::exception("Invalid direction.");
		}
	}
}