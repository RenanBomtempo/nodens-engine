#include "ndpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Nodens {
	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GeIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::DrawNonIndexed(const Ref<VertexArray>& vertexArray)
	{
		glDrawArrays(GL_TRIANGLES, 0, vertexArray->GetVertexBuffers()[0]->GetVertexCount());
	}

	void OpenGLRendererAPI::DrawLine(const Ref<VertexArray>& vertexArray)
	{
		glDrawArrays(GL_LINE, 0, vertexArray->GetVertexBuffers()[0]->GetVertexCount());
	}

	void OpenGLRendererAPI::DrawQuads(const Ref<VertexArray>& vertexArray)
	{
		glDrawArrays(GL_QUADS, 0, vertexArray->GetVertexBuffers()[0]->GetVertexCount());
	}
}

