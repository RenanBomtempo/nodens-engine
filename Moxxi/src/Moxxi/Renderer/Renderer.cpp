#include "mxpch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Moxxi {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::BeginScene(Camera& camera, Light& light)
	{
		m_SceneData->LightType = light.GetType();
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
		m_SceneData->LightColor = light.GetColor();
		if (light.GetType() == Light::Type::Directional)
			m_SceneData->LightDirection = light.GetDirection();
		else if (light.GetType() == Light::Type::Point)
			m_SceneData->LightPosition = light.GetPosition();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::SubmitIndexed(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4 transform, const glm::vec4 color)
	{
		shader->Use();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("uViewProjection", m_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("uModelMatrix", transform);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("uNormalMatrix", glm::transpose(glm::inverse(transform)));
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat4("uLightColor", color);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("uObjectColor", m_SceneData->LightColor);
		if (m_SceneData->LightType == Light::Type::Directional)
			std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("uLightDirection", m_SceneData->LightDirection);
		else if (m_SceneData->LightType == Light::Type::Point)
			std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("uLightPosition", m_SceneData->LightPosition);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
	void Renderer::SubmitArray(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4 transform, const glm::vec4 color)
	{
		shader->Use();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("uViewProjection", m_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("uModelMatrix", transform);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("uNormalMatrix", glm::transpose(glm::inverse(transform)));
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat4("uObjectColor", color);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("uLightColor", m_SceneData->LightColor);
		if (m_SceneData->LightType == Light::Type::Directional)
			std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("uLightDirection", m_SceneData->LightDirection);
		else if (m_SceneData->LightType == Light::Type::Point)
			std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("uLightPosition", m_SceneData->LightPosition);

		vertexArray->Bind();
		RenderCommand::DrawArrays(vertexArray);
	}

}