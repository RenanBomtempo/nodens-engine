#pragma once
#include "Moxxi.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <vector>

#define DOMAIN_SIZE 15

#define NUM_BOIDS 200

class Boid {
	friend class Boids;
	friend class BoidsLayer;
public:
	Boid() {}
	Boid(uint32_t id);
	glm::mat4 GetModelMatrix();
	void UpdateVelocity(glm::vec3 direction, float speed);
private:
	// Local
	glm::vec3 m_Position;
	glm::vec3 m_Orientation;
	glm::mat4 m_Rotate = glm::mat4(1.0f);
	glm::vec3 m_Scale = { 0.1, 0.1, 0.1 };
	glm::vec3 m_Up = { 0, 1, 0 };
	glm::vec3 m_Front = { 0, 0, -1 };
	glm::vec3 m_Right = { 1, 0, 0 };

	glm::vec3 m_Velocity = { 0, 0, -0.1f };
	float m_Speed = 0.1f;
	uint32_t ID;
};

class Boids
{
public:
	friend class BoidsLayer;
	Boids();

	void OnUpdate(Moxxi::TimeStep ts);

	inline const std::vector<Boid> GetFlock() const { return m_Flock; }
	//inline const Boid* GetFlock() const { return m_Flock; }
	inline Boid& GetTargetBoid() { return m_TargetBoid; }
	inline glm::vec3 GetAveragePosition() { return m_AveragePosition; }
private:
	glm::vec3 m_ControllerPosition = glm::vec3(0, 8, -3);
	Boid m_TargetBoid;
	std::vector<Boid> m_Flock;
	//Boid m_Flock[NUM_BOIDS];
	float m_SeparationFactor = 0.24;
	float m_CohesionFactor = 0.30;
	float m_AlignementFactor = 0.02;
	float m_FollowFactor = 0.1;

	float m_FOV = 125;
	float m_SearchRadius = 2;
	float m_AvoidRadius = 0.5;
	float m_ElapsedTime = 0;
	float m_SpeedMultiplier = 3;
	float m_MinSpeed = 1.5f;

	glm::vec3 m_AveragePosition = {0, 0, 0};
};

