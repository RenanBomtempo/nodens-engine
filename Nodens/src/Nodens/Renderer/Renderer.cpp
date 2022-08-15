#include "ndpch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Nodens {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;


	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::SubmitIndexed(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4 transform)
	{
		shader->Use();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("uViewProjection", m_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("uModelMatrix", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::SubmitNonIndexed(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4 transform)
	{
		shader->Use();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("uViewProjection", m_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("uModelMatrix", transform);

		vertexArray->Bind();
		RenderCommand::DrawNonIndexed(vertexArray);
	}

	void Renderer::SubmitLine(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4 transform, const glm::vec4 color)
	{
		shader->Use();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("uViewProjection", m_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("uModelMatrix", transform);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat4("uColor", color);

		vertexArray->Bind();
		RenderCommand::DrawLine(vertexArray);
	}
	
	void Renderer::SubmitQuads(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4 transform, const glm::vec4 color)
	{
		shader->Use();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("uViewProjection", m_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("uModelMatrix", transform);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat4("uColor", color);

		vertexArray->Bind();
		RenderCommand::DrawQuads(vertexArray);
	}

}