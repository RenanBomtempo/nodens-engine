#include "mxpch.h"
#include "Application.h"

#include "Log.h"
#include "Events/ApplicationEvent.h"

namespace Moxxi {

	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		MX_TRACE(e);
		while (true);
	}

}