#pragma once
#include "Nodens/Renderer/RendererAPI.h"

namespace Nodens {
	class OpenGLRendererAPI : public RendererAPI
	{
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
		virtual void DrawNonIndexed(const Ref<VertexArray>& vertexArray) override;
		virtual void DrawLine(const Ref<VertexArray>& vertexArray) override;
		virtual void DrawQuads(const Ref<VertexArray>& vertexArray) override;

	};

}

