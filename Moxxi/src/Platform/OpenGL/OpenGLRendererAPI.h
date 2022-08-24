#pragma once

#include "Moxxi/Renderer/RendererAPI.h"

namespace Moxxi {
	class OpenGLRendererAPI : public RendererAPI
	{
		virtual void SetFrontFaceOrientation(const RendererProps::FrontFaceOrientation orientation) override;
		virtual void SetPolygonMode(const RendererProps::PolygonMode mode) override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
		virtual void DrawArrays(const Ref<VertexArray>& vertexArray) override;
	};

}

