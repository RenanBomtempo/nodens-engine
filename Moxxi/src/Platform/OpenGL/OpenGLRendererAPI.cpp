#include "mxpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Moxxi {
	void OpenGLRendererAPI::SetFrontFaceOrientation(const RendererProps::FrontFaceOrientation orientation)
	{
		if (orientation == RendererProps::FrontFaceOrientation::CounterClockwise)
			glFrontFace(GL_CCW);
		else if (orientation == RendererProps::FrontFaceOrientation::Clockwise)
			glFrontFace(GL_CW);
	}

	void OpenGLRendererAPI::SetPolygonMode(const RendererProps::PolygonMode mode)
	{
		if (mode == RendererProps::PolygonMode::Wireframe)
		{
			glDisable(GL_CULL_FACE);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else if (mode == RendererProps::PolygonMode::Fill)
		{
			glEnable(GL_CULL_FACE);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
		}
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GeIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
	
	void OpenGLRendererAPI::DrawArrays(const Ref<VertexArray>& vertexArray)
	{	
		glDrawArrays(GL_TRIANGLES, 0, vertexArray->GetVertexBuffers()[0]->GetVertexCount());
	}

}

