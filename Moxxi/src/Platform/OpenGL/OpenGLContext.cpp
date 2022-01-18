#include "mxpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Moxxi {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		MX_CORE_ASSERT(windowHandle, "Window hadnle is null!")
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);

		// Initialize GLAD
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		MX_CORE_ASSERT(status, "Failed to initialize glad!");
	}
	void OpenGLContext::SwapBuffers()
	{
		glBegin(GL_TRIANGLES);
		
		
		glEnd();

		glfwSwapBuffers(m_WindowHandle);
	}
}