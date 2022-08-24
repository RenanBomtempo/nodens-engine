#pragma once

#include "RenderCommand.h"
#include "Camera.h"
#include "Light.h"
#include "Shader.h"

namespace Moxxi {
	class Renderer
	{
	public:
		static void BeginScene(Camera& camera, Light& light);
		static void EndScene();

		static void SubmitIndexed(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4 transform = glm::mat4(1.0f), const glm::vec4 color= glm::vec4(1.0f));
		static void SubmitArray(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4 transform = glm::mat4(1.0f), const glm::vec4 color=glm::vec4(1.0f));

		static inline RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			Light::Type LightType;
			glm::mat4 ViewProjectionMatrix;
			glm::vec3 LightColor;
			glm::vec3 LightPosition;
			glm::vec3 LightDirection;
		};

		static SceneData* m_SceneData;
	};
}