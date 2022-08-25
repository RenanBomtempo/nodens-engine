#pragma once
#include "Moxxi.h"
#include "ALG.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <vector>

class ShallowWaters
{
public:
	friend class ShallowWatersLayer;
	ShallowWaters();

	void OnUpdate(Moxxi::TimeStep ts);
private:
	alg::Grid2D m_Grid;
};

