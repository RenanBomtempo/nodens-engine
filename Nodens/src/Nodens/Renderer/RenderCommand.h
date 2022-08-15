#pragma once

#include "RendererAPI.h"

namespace Nodens {
	class RenderCommand
	{
	public:
		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}
		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}
		static void DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}
		static void DrawNonIndexed(const Ref<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawNonIndexed(vertexArray);
		}
		
		// Termporary
		static void DrawLine(const Ref<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawLine(vertexArray);
		}
		static void DrawQuads(const Ref<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawQuads(vertexArray);
		}
	private:

		static RendererAPI* s_RendererAPI;
	};
}