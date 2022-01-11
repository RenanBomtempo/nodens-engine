#pragma once
#include "Core.h"

namespace Moxxi {

	class MOXXI_API Application
	{
	public:
		Application();
		~Application();

		void Run();
	};

	Application* CreateApplication();
}