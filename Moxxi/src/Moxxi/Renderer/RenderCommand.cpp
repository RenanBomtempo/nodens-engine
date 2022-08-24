#include "mxpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Moxxi {
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}