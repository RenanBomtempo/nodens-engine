#include "mxpch.h"
#include "Application.h"

#include "Log.h"
#include "Events/ApplicationEvent.h"
#include "GLFW/glfw3.h"

namespace Moxxi {
	
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
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

	void Application::OnEvent(Event& e)
	{
		MX_CORE_INFO("{0}", e);
	}

}