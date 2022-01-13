#include "mxpch.h"
#include "Application.h"

#include "Log.h"
#include "Events/ApplicationEvent.h"
#include "GLFW/glfw3.h"

namespace Moxxi {

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);
			m_Window->OnUpdate();
		}
	}

}