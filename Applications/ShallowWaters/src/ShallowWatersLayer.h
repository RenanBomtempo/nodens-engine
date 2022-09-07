#pragma once

#include "Moxxi.h"
#include "ALG.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

class ShallowWatersLayer : public Moxxi::Layer
{
public:
public:
	ShallowWatersLayer();

	void OnImGuiRender(Moxxi::TimeStep ts) override;
	void OnUpdate(Moxxi::TimeStep ts) override;
	void OnEvent(Moxxi::Event& event) override;

	void InitFlatShader();
	void InitSquareWireframeMesh();
	void InitSquareFillMesh();
	void UpdateMHCMesh();
	
	void InitSimulation();
	void UpdateSimulation();

	void ProcessInputs(Moxxi::TimeStep ts);
	void DrawGrid();

	bool OnWindowResizeEvent(Moxxi::WindowResizeEvent& event);

private:
	alg::Grid2D m_Grid;

	bool m_Wireframe = true;
	bool m_Fill = false;
	bool m_MHCLines = true;
	bool m_MHCPoints = true;

	float m_Zoom = 1;
	float m_ElapsedTime = 0;
	float m_LineWidth = 2;
	float m_PointSize = 4;

	Moxxi::Ref<Moxxi::Shader> m_FlatShader;
	Moxxi::Ref<Moxxi::VertexArray> m_SquareFillVA;
	Moxxi::Ref<Moxxi::VertexArray> m_MHCVA;
	Moxxi::Ref<Moxxi::VertexArray> m_SquareWireframeVA;

	Moxxi::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;

	glm::vec4 m_ClearColor;
};
