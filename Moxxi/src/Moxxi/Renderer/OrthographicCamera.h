#pragma once

#include "Camera.h"
#include <glm/glm.hpp>

namespace Moxxi {
	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
		
		const float GetRotation() const { return m_Rotation; }
		void SetRotation(const float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

		void SetZoom(float zoom) { m_Zoom = zoom; RecalculateProjectionMatrix(); }

		void SetSides(float left, float right, float bottom, float top) { 
			m_Left = left; 
			m_Right = right;
			m_Bottom = bottom;
			m_Top = top;
			RecalculateProjectionMatrix();
		};

		virtual const glm::mat4 GetProjectionMatrix() const override { return m_ProjectionMatrix; }
		virtual const glm::mat4 GetViewMatrix() const override { return m_ViewMatrix; }
		virtual const glm::mat4 GetViewProjectionMatrix() const override { return m_ViewProjectionMatrix; }
	private:
		void RecalculateViewMatrix();
		void RecalculateProjectionMatrix();
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		float m_Left;
		float m_Right;
		float m_Bottom;
		float m_Top;
		float m_Zoom;

		glm::vec3 m_Position = glm::vec3(0);
		float m_Rotation = 0;
	};

}

