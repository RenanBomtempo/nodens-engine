#include "Boids.h"
#include <time.h>


glm::vec3 RandomVec3(float min, float max)
{
	return glm::vec3((float(rand()) / float((RAND_MAX)) * (max + glm::abs(min))) + min,
		(float(rand()) / float((RAND_MAX)) * (max + glm::abs(min))),
		(float(rand()) / float((RAND_MAX)) * (max + glm::abs(min))) + min);
}

glm::vec3 RandomOrientation()
{
	return glm::vec3((float(rand()) / float((RAND_MAX)) * (2 * glm::pi<float>())),
		(float(rand()) / float((RAND_MAX)) * (2 * glm::pi<float>())),
		(float(rand()) / float((RAND_MAX)) * (2 * glm::pi<float>())));
}

Boid::Boid(uint32_t id) : ID(id)
{
	m_Position = RandomVec3(-DOMAIN_SIZE, DOMAIN_SIZE);
	m_Orientation = RandomOrientation();
	m_Velocity = glm::normalize(RandomVec3(-DOMAIN_SIZE, DOMAIN_SIZE)) * m_Speed;
}

glm::mat4 Boid::GetModelMatrix() {
	glm::mat4 transform =
		glm::translate(glm::mat4(1.0f), m_Position)
		* m_Rotate
		* glm::rotate(glm::mat4(1.0f), glm::radians(m_Orientation.x), glm::vec3(1, 0, 0))
		* glm::rotate(glm::mat4(1.0f), glm::radians(m_Orientation.y), glm::vec3(0, 1, 0))
		* glm::rotate(glm::mat4(1.0f), glm::radians(m_Orientation.z), glm::vec3(0, 0, 1))
		* glm::scale(glm::mat4(1.0f), m_Scale);
	return transform;
}

void Boid::UpdateVelocity(glm::vec3 direction, float speed)
{
	//MX_ASSERT(glm::length2(direction) > 0 && speed > 0, "Direction is zero!");
	if (glm::length2(direction) > 0)
	{
		m_Speed = speed;
		m_Velocity = glm::normalize(direction) * speed;
		m_Rotate = glm::rotate(glm::mat4(1.0f), glm::angle(m_Front, direction), glm::cross(m_Front, direction));
	}
	else
		m_Rotate = glm::mat4(1.0f);
}

Boids::Boids()
{
	m_TargetBoid = Boid(0);
	m_TargetBoid.m_Position = glm::vec3(2, 3, 2);

	for (int i = 1; i <= NUM_BOIDS; i++)
		m_Flock.push_back(Boid(i));
		//m_Flock[i] = Boid(i);
	
}

void Boids::OnUpdate(Moxxi::TimeStep ts)
{
	//m_ElapsedTime += ts;
	
	auto& begin = m_Flock.begin();
	auto& end = m_Flock.end();

	glm::vec3 avgFlockPosition = glm::vec3(0);

	for (auto boid = begin; boid != end; boid++)
	//for (int i = 0; i < NUM_BOIDS; i++)
	{
		//Boid* boid = &m_Flock[i];
		uint32_t othersInRange = 0;

		glm::vec3 newDirection = boid->m_Velocity / boid->m_Speed;
		glm::vec3 directionToOther(0.0f);
		glm::vec3 avgDirectionInRange(0.0f);
		glm::vec3 avgPositionInRange(0.0f);
		glm::vec3 avgDirectionToOthers(0.0f);

		float distanceToOther = 0;
		float avgDistanceToOthers = 0;
		float speed = boid->m_Speed;

		avgFlockPosition += boid->m_Position;

		for (auto other = begin; other != end; other++)
		//for (int j = 0; j < NUM_BOIDS; j++)
		{
			if (other->ID == boid->ID)
			//if (i == j)
			{
				//other++;
				continue;
			}
			
			//Boid* other = &m_Flock[j];

			directionToOther = other->m_Position - boid->m_Position;
			distanceToOther = 
				directionToOther.x * directionToOther.x +
				directionToOther.y * directionToOther.y +
				directionToOther.z * directionToOther.z;
			//distanceToOther = glm::length(directionToOther);

			//MX_TRACE("distance{0}", distanceToOther);
			if (distanceToOther <= m_SearchRadius * m_SearchRadius && glm::angle(boid->m_Velocity, directionToOther) <= m_FOV)
			{
				avgDirectionToOthers += directionToOther*1.0f / glm::length(directionToOther);
				avgDistanceToOthers += distanceToOther;
				avgDirectionInRange += glm::normalize(other->m_Velocity);
				avgPositionInRange += other->m_Position;
				othersInRange++;
			}
		}

		if (othersInRange > 0)
		{
			avgDirectionToOthers /= (float)othersInRange;
			MX_ASSERT(glm::length(avgDirectionToOthers) < 1000000, "Position too large");
			avgDirectionInRange /= (float)othersInRange;
			avgDistanceToOthers /= (float)othersInRange;
			avgPositionInRange /= (float)othersInRange;

			// Separation
			if (glm::length2(avgDirectionToOthers) > 0)
				newDirection -= m_SeparationFactor * glm::normalize(avgDirectionToOthers);

			// Cohesion
			newDirection += m_CohesionFactor * (avgPositionInRange - boid->m_Position) ;

			// Alignment
			newDirection += m_AlignementFactor * (avgDirectionInRange);	


			speed = m_SpeedMultiplier * avgDistanceToOthers / m_SearchRadius + m_MinSpeed;

		}
		else
			speed = m_MinSpeed;

		// Follow
		newDirection += m_FollowFactor * (m_TargetBoid.m_Position - boid->m_Position);

		MX_ASSERT(glm::length2(newDirection) > 0, "newDirection too small");
		boid->UpdateVelocity(newDirection, speed);
		boid->m_Position += boid->m_Velocity * ts.GetSeconds();

		MX_ASSERT(glm::length(boid->m_Position) > 0, "Assert");
		
		if (glm::abs(boid->m_Position.x) > DOMAIN_SIZE)
			boid->m_Position.x *= -0.99;
		if (glm::abs(boid->m_Position.z) > DOMAIN_SIZE)
			boid->m_Position.z *= -0.99;
		if (boid->m_Position.y > 2*DOMAIN_SIZE)
			boid->m_Position.y = 0.01;
		else if (boid->m_Position.y < 0)
			boid->m_Position.y = 2 * DOMAIN_SIZE;
	}


	glm::vec3 tmp = m_ControllerPosition - m_TargetBoid.m_Position;
	if (glm::length2(tmp) > 0)
	{
		float speed = m_SpeedMultiplier * glm::length(tmp);
		m_TargetBoid.UpdateVelocity(glm::normalize(tmp), speed);
		m_TargetBoid.m_Position += m_TargetBoid.m_Velocity * ts.GetSeconds();
	}

	if (glm::abs(m_TargetBoid.m_Position.x) > DOMAIN_SIZE)
		m_TargetBoid.m_Position.x *= -0.99;
	if (glm::abs(m_TargetBoid.m_Position.z) > DOMAIN_SIZE)
		m_TargetBoid.m_Position.z *= -0.99;
	if (m_TargetBoid.m_Position.y > 2 * DOMAIN_SIZE)
		m_TargetBoid.m_Position.y = 0.01;
	else if (m_TargetBoid.m_Position.y < 0)
		m_TargetBoid.m_Position.y = 2 * DOMAIN_SIZE;

	m_AveragePosition = avgFlockPosition /= m_Flock.size();
}
