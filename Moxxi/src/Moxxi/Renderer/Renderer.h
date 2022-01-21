#pragma once

namespace Moxxi {
	enum class RendererAPI
	{
		None = 0, OpenGL = 1
	};

	class Renderer
	{
	public:
		static inline RendererAPI GetAPI() { return s_RendererAPI; }
		static RendererAPI s_RendererAPI;
	};
}