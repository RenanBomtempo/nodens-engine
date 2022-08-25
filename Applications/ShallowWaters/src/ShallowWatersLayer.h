#pragma once
#include "Moxxi.h"
#include "ShallowWaters.h"

class ShallowWatersLayer : public Moxxi::Layer
{
public:
public:
	ShallowWatersLayer();

	void InitFlatShader();
	void InitSquareMesh();

	void ProcessInputs(Moxxi::TimeStep ts);

	void OnUpdate(Moxxi::TimeStep ts) override;
	void OnImGuiRender(Moxxi::TimeStep ts) override;
	void OnEvent(Moxxi::Event& event) override;

	bool OnWindowResizeEvent(Moxxi::WindowResizeEvent& event);
private:
	ShallowWaters m_ShallowWaters;

	float m_ElapsedTime = 0;

	Moxxi::Ref<Moxxi::Shader> m_FlatShader;
	Moxxi::Ref<Moxxi::VertexArray> m_SquareVA;

	Moxxi::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraFOV = 50;

	Moxxi::DirectionalLight m_Light;
	glm::vec3 m_LightDirection;
	glm::vec3 m_LightOrientation;
	glm::vec3 m_LightColor;
	
	glm::vec4 m_ClearColor;
};
