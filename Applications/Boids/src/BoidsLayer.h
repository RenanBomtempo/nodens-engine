#pragma once
#include "Moxxi.h"
#include "Boids.h"

class BoidsLayer : public Moxxi::Layer
{
public:
	enum class CameraMode {
		Free=0, Tower, FollowFlockBack, FollowFlockSide
	};
public:
	BoidsLayer();

	void InitCameras();
	void InitFlatShader();
	void InitDomainMesh();
	void InitBoidsMesh();
	void InitTowerMesh();
	void ProcessInputs(Moxxi::TimeStep ts);

	void OnUpdate(Moxxi::TimeStep ts) override;
	void OnImGuiRender(Moxxi::TimeStep ts) override;
	void OnEvent(Moxxi::Event& event) override;

	bool OnWindowResizeEvent(Moxxi::WindowResizeEvent& event);
private:
	Boids m_Boids;

	float m_ElapsedTime = 0;

	Moxxi::Ref<Moxxi::Shader> m_FlatShader;
	Moxxi::Ref<Moxxi::VertexArray> m_BoidVA;
	Moxxi::Ref<Moxxi::VertexArray> m_DomainVA;
	Moxxi::Ref<Moxxi::VertexArray> m_TowerVA;

	Moxxi::PerspectiveCamera m_Cameras[4];
	CameraMode m_CameraMode;
	float m_CameraFOV = 50;
	glm::vec3 m_CameraPosition;
	glm::vec3 m_CameraRotation;
	glm::vec3 m_CameraDistance;

	Moxxi::DirectionalLight m_Light;
	glm::vec3 m_LightDirection;
	glm::vec3 m_LightOrientation;
	glm::vec3 m_LightColor;
	
	glm::vec4 m_ClearColor;
	

	float m_CameraMoveSpeed = 1.0f;
	float m_CameraSensitivity = 0.1f;
	bool m_FirstMouse = true;
	bool m_FollowBoid = true;
};
